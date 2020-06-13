#ifndef MOVINGCOLLISION_HPP
#define MOVINGCOLLISION_HPP

#include <atlas/tools/ModellingScene.hpp>
#include "coloursphere.hpp"
#include "quad.hpp"
#include "colobject.hpp"
#include "sphere.hpp"

namespace personalProject {
    class MovingCollision : public atlas::tools::ModellingScene {
    public:
        MovingCollision();
        ~MovingCollision() = default;

        void updateScene(double time) override;
        void renderScene() override;

    private:
        std::vector<std::shared_ptr<ColourSphere>> mSpheres;
        std::vector<std::shared_ptr<Quad>> mQuads;
        std::vector<std::shared_ptr<ColObject>> mObjects;
        bool mWireframe = false;
    };
}
#endif // MOVINGCOLLISION_HPP
