#include "myfirstprojectapp.h"
#include "selectmeshcomponent.h"

// External Includes
#include <utility/fileutils.h>
#include <nap/logger.h>
#include <inputrouter.h>
#include <renderablemeshcomponent.h>
#include <perspcameracomponent.h>
#include <imgui/imgui.h>

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

		SelectMeshComponentInstance& select_mesh_comp = mModelEntity->getComponent<SelectMeshComponentInstance>();
		ImGui::Begin("Controls");

		int current_model_idx = select_mesh_comp.getModelIndex();
		if (ImGui::SliderInt("Select Model", &current_model_idx, 0, select_mesh_comp.getModelCount() - 1))
			select_mesh_comp.selectModel(current_model_idx);

		int current_text_idx = select_mesh_comp.getTextureIndex();
		if (ImGui::SliderInt("Select Texture", &current_text_idx, 0, select_mesh_comp.getTextureCount(current_model_idx) - 1))
			select_mesh_comp.selectTexture(current_text_idx);

		ImGui::End();
	}
	
	
	// Called when the window is going to render
	void MyFirstProjectApp::render()
	{
		// Signal the beginning of a new frame, allowing it to be recorded.
		// The system might wait until all commands that were previously associated with the new frame have been processed on the GPU.
		// Multiple frames are in flight at the same time, but if the graphics load is heavy the system might wait here to ensure resources are available.
		mRenderService->beginFrame();

		// Begin recording the render commands for the main render window
		if (mRenderService->beginRecording(*mRenderWindow))
		{
			// Begin the render pass
			mRenderWindow->beginRendering();

			// Get the component that renders the model to screen
			std::vector<RenderableComponentInstance*> render_comps;
			RenderableComponentInstance& render_comp = mModelEntity->getComponent<RenderableComponentInstance>();
			render_comps.emplace_back(&render_comp);

			// Get the camera to render with
			PerspCameraComponentInstance& persp_cam = mCameraEntity->getComponent<PerspCameraComponentInstance>();
			
			// Render the model using the camera to the window
			mRenderService->renderObjects(*mRenderWindow, persp_cam, render_comps);

			// Draw the GUI on top of the rest
			mGuiService->draw();

			// End the render pass
			mRenderWindow->endRendering();

			// We're done recording
			mRenderService->endRecording();
		}

		// Tell the render service we finished rendering into the frame.
		mRenderService->endFrame();
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
