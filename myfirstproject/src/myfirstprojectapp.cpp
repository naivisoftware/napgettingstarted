#include "myfirstprojectapp.h"

// External Includes
#include <utility/fileutils.h>
#include <nap/logger.h>
#include <inputrouter.h>
#include <renderablemeshcomponent.h>
#include <perspcameracomponent.h>

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::MyFirstProjectApp)
	RTTI_CONSTRUCTOR(nap::Core&)
RTTI_END_CLASS

namespace nap 
{
	/**
	 * Initialize all the resources and instances used for drawing
	 * slowly migrating all functionality to NAP
	 */
	bool MyFirstProjectApp::init(utility::ErrorState& error)
	{
		// Retrieve services
		mRenderService	= getCore().getService<nap::RenderService>();
		mSceneService	= getCore().getService<nap::SceneService>();
		mInputService	= getCore().getService<nap::InputService>();
		mGuiService		= getCore().getService<nap::IMGuiService>();

		// Fetch the resource manager
		mResourceManager = getCore().getResourceManager();

		// Fetch the scene
		mScene = mResourceManager->findObject<Scene>("Scene");

		// Convert our path and load resources from file
		if (!mResourceManager->loadFile("app_structure.json", error))
			return false;

		// Get the render window
		mRenderWindow = mResourceManager->findObject<nap::RenderWindow>("Window");
		if (!error.check(mRenderWindow != nullptr, "unable to find render window with name: %s", "Window"))
			return false;

		// Get the scene that contains our entities and components
		mScene = mResourceManager->findObject<Scene>("Scene");
		if (!error.check(mScene != nullptr, "unable to find scene with name: %s", "Scene"))
			return false;

		mCameraEntity = mScene->findEntity("CameraEntity");
		mModelEntity = mScene->findEntity("ModelEntity");

		// All done!
		return true;
	}
	
	
	// Called when the window is updating
	void MyFirstProjectApp::update(double deltaTime)
	{
		// Use a default input router to forward input events (recursively) to all input components in the default scene
		nap::DefaultInputRouter input_router(true);
		mInputService->processWindowEvents(*mRenderWindow, input_router, { &mScene->getRootEntity() });
	}
	
	
	// Called when the window is going to render
	void MyFirstProjectApp::render()
	{
		// Destroy old GL context related resources scheduled for destruction
		mRenderService->destroyGLContextResources({ mRenderWindow.get() });

		// Prep main window for drawing
		mRenderWindow->makeActive();

		// Clear back-buffer
		mRenderService->clearRenderTarget(mRenderWindow->getBackbuffer());

		std::vector<RenderableComponentInstance*> render_comps;
		RenderableComponentInstance& render_comp = mModelEntity->getComponent<RenderableComponentInstance>();
		render_comps.emplace_back(&render_comp);

		PerspCameraComponentInstance& persp_cam = mCameraEntity->getComponent<PerspCameraComponentInstance>();
		mRenderService->renderObjects(mRenderWindow->getBackbuffer(), persp_cam, render_comps);

		// Draw our gui
		mGuiService->draw();

		// Swap screen buffers
		mRenderWindow->swap();
	}
	

	void MyFirstProjectApp::windowMessageReceived(WindowEventPtr windowEvent)
	{
		mRenderService->addEvent(std::move(windowEvent));
	}
	
	
	void MyFirstProjectApp::inputMessageReceived(InputEventPtr inputEvent)
	{
		KeyPressEvent* key_event = rtti_cast<KeyPressEvent>(inputEvent.get());
		if (key_event != nullptr && key_event->mKey == EKeyCode::KEY_ESCAPE)
		{
			this->quit();
		}

		mInputService->addEvent(std::move(inputEvent));
	}

	
	int MyFirstProjectApp::shutdown()
	{
		return 0;
	}

}
