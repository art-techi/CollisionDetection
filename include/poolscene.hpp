#ifndef POOLSCENE_HPP
#define POOLSCENE_HPP

#include <atlas/tools/ModellingScene.hpp>
#include "coloursphere.hpp"
#include "quad.hpp"
#include "colobject.hpp"
#include "sphere.hpp"

namespace personalProject {
    class PoolScene : public atlas::tools::ModellingScene {
    public:
        PoolScene();
        ~PoolScene() = default;

        void updateScene(double time) override;
        void renderScene() override;

    private:
        std::vector<std::shared_ptr<ColourSphere>> mSpheres;
        std::vector<std::shared_ptr<Quad>> mQuads;
        std::vector<std::shared_ptr<ColObject>> mObjects;
        bool mWireframe = false;
    };
}
#endif // POOLSCENE_HPP
