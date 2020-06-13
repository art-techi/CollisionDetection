/* Student Name: Margaret (Meg) Coleman
 * Student ID: 
 * Last Updated: October 20, 2017
 * CSC 473 Project
 *
 */

#include <atlas/utils/Application.hpp>
#include <atlas/utils/WindowSettings.hpp>
#include <atlas/gl/ErrorCheck.hpp>
#include <atlas/tools/ModellingScene.hpp>
#include "poolscene.hpp"
#include "staticcollision.hpp"
#include "movingcollision.hpp"

int main(){
    using atlas::utils::WindowSettings;
    using atlas::utils::ContextVersion;
    using atlas::utils::Application;
    using atlas::utils::ScenePointer;
    using atlas::tools::ModellingScene;

    namespace project = personalProject;

    atlas::gl::setGLErrorSeverity(ATLAS_GL_ERROR_SEVERITY_HIGH | ATLAS_GL_ERROR_SEVERITY_MEDIUM);

    WindowSettings settings;
    settings.contextVersion = ContextVersion(4, 1);
    settings.isForwardCompat = true;
    settings.isMaximized = false;
    settings.title = "Animation Personal Project";

    Application::getInstance().createWindow(settings);

    Application::getInstance().addScene(ScenePointer(new project::StaticCollision));
    //Application::getInstance().addScene(ScenePointer(new project::MovingCollision));
    //Application::getInstance().addScene(ScenePointer(new project::PoolScene));
    Application::getInstance().runApplication();

    return 0;
}
