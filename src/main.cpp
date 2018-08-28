
#include "hcluster.hpp"
#include "vector2.hpp"
#include <vector>
#include <memory>
#include <iostream>

int main()
{
    std::vector<Vector2> points;

    points.emplace_back(1.0, 2.0);
    points.emplace_back(2.0, 1.0);
    points.emplace_back(0.0, 0.0);
    points.emplace_back(4.0, 1.0);
    points.emplace_back(5.0, 0.0);
    points.emplace_back(5.0, 3.0);

    std::unique_ptr<const Cluster<Vector2>> cluster
    (
        Cluster<Vector2>::hierarchical
        (
            points,
            [](const Cluster<Vector2>& A, const Cluster<Vector2>& B)
            {
                const double xdiff = A.centroid().x() - B.centroid().x();
                const double ydiff = A.centroid().y() - B.centroid().y();

                return xdiff * xdiff + ydiff * ydiff;
            }
        )
    );

    cluster->traverse
    (
        [](const Cluster<Vector2>& cluster)
        {
            std::cout << cluster.centroid() << std::endl;
        }
    );

    return 0;
}
