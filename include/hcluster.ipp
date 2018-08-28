
#pragma once

#include "heap.hpp"
#include <vector>           // std::vector
#include <functional>       // std::function
#include <unordered_map>    // std::unordered_map
#include <algorithm>        // std::remove_if, std::sort
#include <utility>          // std::pair

// Cluster Base class:

template <typename T>
const Cluster<T> * Cluster<T>::hierarchical
(
    const std::vector<T>& elements,
    const std::function<double(const Cluster&, const Cluster&)>& cost
)
{
    // Pseudocode@: http://www.saedsayad.com/clustering_hierarchical.htm

    DMatrix matrix;

    auto costO1 = [&](const Cluster& A, const Cluster& B)
    {
        typename DMatrix::const_iterator mit;
        typename DVector::const_iterator vit;
    
        if ((mit = matrix.find(&A)) != matrix.end())
            if ((vit = mit->second.find(&B)) != mit->second.end())
                return vit->second;

        return (matrix[&A][&B] = cost(A, B));
    };

    // Create the initial "trivial" (one element per cluster) clusters
    std::vector<Cluster *> clusters;
    for (const auto& element : elements)
    {
        clusters.push_back(new Cluster(element)); clusters.back()->_size = 1UL;
    }

    while (clusters.size() > 1)
    {
        ICluster<T> * bmatch = new ICluster<T>();
        
        double bcost = std::numeric_limits<double>().max();

        // Step 1:
        // In each iteration, determine the "best-matching" cluster
        // to the current cluster, while at the same time checking
        // if mit rocks the highest score so far and thus making mit
        // the new "best-match"
        for (const auto& current : clusters)
        {
            heap<const Cluster *> matches
            (
                clusters.size(),
                [&](const Cluster * A, const Cluster * B)
                {
                    return costO1(*A, *current) < costO1(*B, *current);
                }
            );

            for (const auto& other : clusters)
                if (current != other)
                    matches.push(other);

            const Cluster * top; matches.pop(top);

            const double ccost = costO1(*current, *top);
            if (ccost < bcost)
            {
                bmatch->_left  = current;
                bmatch->_right = top;
                
                bcost = ccost;
            }
        }

        bmatch->_size = bmatch->_left->_size + bmatch->_right->_size;

        bmatch->_centroid =
        (
              bmatch->_left->_centroid
            + bmatch->_right->_centroid
        ) / 2.0;
        
        matrix.erase(bmatch->_left); matrix.erase(bmatch->_right);

        // Step 2:
        // Delete the child clusters of the newly created cluster from the vector
        clusters.erase
        (
            std::remove_if
            (
                clusters.begin(),
                clusters.end(),
                [&bmatch](const Cluster * cluster)
                {
                    return cluster == bmatch->_left || cluster == bmatch->_right;
                }
            ),
            clusters.end()
        );

        // Step 3:
        // Add the newly created cluster to the vector
        clusters.push_back(bmatch);
    }

    return clusters.front();
}

template <typename T>
void Cluster<T>::traverse(const std::function<void(const Cluster<T>&)>& f) const
{
    f(*this);
}

#ifdef  __VERBOSE__

#include <iostream>

#endif

// ICluster Derived class:
template <typename T>
void ICluster<T>::traverse(const std::function<void(const Cluster<T>&)>& f) const
{
    #ifdef __VERBOSE__

    std::cout << "<- " << std::endl;

    #endif

    if (_left)
        _left->traverse(f);

    #ifdef __VERBOSE__

    std::cout << "-> " << std::endl;
    
    #endif

    if (_right)
        _right->traverse(f);
}
