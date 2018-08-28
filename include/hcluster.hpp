
#pragma once

#include <vector>           // std::vector
#include <functional>       // std::function
#include <unordered_map>    // std::unordered_map

template <typename T>
class Cluster
{
private:

    using DVector = std::unordered_map<const Cluster<T> *, double>;
    using DMatrix = std::unordered_map<const Cluster<T> *, DVector>;

protected:

    std::size_t _size;

    T _centroid;

public:

    Cluster() : _size(0UL) {}

    Cluster(const T& _centroid) : _size(0UL), _centroid(_centroid) {}

    Cluster(const Cluster& other) : _size(other._size), _centroid(other._centroid) {}

    virtual ~Cluster() {};

    Cluster& operator=(const Cluster& other)
    {
        this->_size     = other.size;
        this->_centroid = other._centroid;

        return *this;
    }

    const T& centroid() const { return _centroid; }
    
    std::size_t size() const { return _size; }

    virtual const Cluster<T> * left()  const { return nullptr; }
    virtual const Cluster<T> * right() const { return nullptr; }

    static const Cluster * hierarchical
    (
        const std::vector<T>&,
        const std::function<double(const Cluster&, const Cluster&)>&
    );

    virtual void traverse(const std::function<void(const Cluster<T>&)>& f) const;
};

template <typename T>
class ICluster : public Cluster<T>
{
    friend class Cluster<T>;

    const Cluster<T> * _left, * _right;

public:

    ICluster() : _left(nullptr), _right(nullptr) {}

    ICluster(const ICluster& other) : Cluster<T>(other), _left(other._left), _right(other._right) {}

    ~ICluster()
    {
        if (_left)
            delete _left;

        if (_right)
            delete _right;
    }

    ICluster& operator=(const ICluster& other)
    {
        Cluster<T>::operator=(other);

        this->_left  = other._left;
        this->_right = other._right;

        return *this;
    }

    const Cluster<T> * left()  const { return _left;  }
    const Cluster<T> * right() const { return _right; }

    void traverse(const std::function<void(const Cluster<T>&)>& f) const;
};

#include "hcluster.ipp"
