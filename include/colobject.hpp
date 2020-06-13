#ifndef COLOBJECT_H
#define COLOBJECT_H

#include <atlas/utils/Geometry.hpp>
#include <atlas/math/Math.hpp>
#include <memory>

namespace personalProject {


    class ColObject : public atlas::utils::Geometry
    {
    public:
        ColObject();
        ~ColObject() = default;

        virtual float getDistance(std::shared_ptr<ColObject> object, float dT) = 0;
        virtual atlas::math::Vector getNormal(std::shared_ptr<ColObject> object) = 0;
        bool isDynamic;
    };
}
#endif // COLOBJECT_H
