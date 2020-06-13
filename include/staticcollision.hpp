#ifndef STATICCOLLISION_HPP
#define STATICCOLLISION_HPP

#include <atlas/tools/ModellingScene.hpp>
#include "coloursphere.hpp"
#include "quad.hpp"
#include <vector>
#include <memory>

namespace personalProject {
    class StaticCollision : public atlas::tools::ModellingScene {
    public:
        StaticCollision();
        ~StaticCollision() = default;

        void updateScene(double time) override;
        void renderScene() override;
        float doPlaneCollisions(std::shared_ptr<ColourSphere> sphere, float fLeft);

    private:
        std::vector<std::shared_ptr<ColourSphere>> mSpheres;
        std::vector<std::shared_ptr<Quad>> mQuads;
    };
}
#endif // STATICCOLLISION_HPP
