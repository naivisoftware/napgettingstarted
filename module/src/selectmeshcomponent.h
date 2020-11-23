#pragma once

#include "modeltexturesresource.h"
#include <component.h>
#include <componentptr.h>
#include <renderablemeshcomponent.h>

namespace nap
{
	class SelectMeshComponentInstance;

	/**
	 * selectmeshcomponent
	 */
	class NAPAPI SelectMeshComponent : public Component
	{
		RTTI_ENABLE(Component)
		DECLARE_COMPONENT(SelectMeshComponent, SelectMeshComponentInstance)
	public:

		/**
		* Get a list of all component types that this component is dependent on (i.e. must be initialized before this one)
		* @param components the components this object depends on
		*/
		virtual void getDependentComponents(std::vector<rtti::TypeInfo>& components) const override;

		std::vector<ResourcePtr<ModelTextureResource>> mModels;
		int mModelIndex = 0;
		int mTextureIndex = 0;
		ComponentPtr<RenderableMeshComponent> mRenderableMeshComponent = nullptr;
	};


	/**
	 * selectmeshcomponentInstance	
	 */
	class NAPAPI SelectMeshComponentInstance : public ComponentInstance
	{
		RTTI_ENABLE(ComponentInstance)
	public:
		SelectMeshComponentInstance(EntityInstance& entity, Component& resource) :
			ComponentInstance(entity, resource)									{ }

		/**
		 * Initialize selectmeshcomponentInstance based on the selectmeshcomponent resource
		 * @param entityCreationParams when dynamically creating entities on initialization, add them to this this list.
		 * @param errorState should hold the error message when initialization fails
		 * @return if the selectmeshcomponentInstance is initialized successfully
		 */
		virtual bool init(utility::ErrorState& errorState) override;

		/**
		 * update selectmeshcomponentInstance. This is called by NAP core automatically
		 * @param deltaTime time in between frames in seconds
		 */
		virtual void update(double deltaTime) override;

		void selectModel(int index);
		int getModelIndex() const						{ return mCurrentModelIndex; }
		int getModelCount() const						{ return static_cast<int>(mModels.size()); }

		void selectTexture(int index);
		int getTextureIndex() const						{ return mCurrentTextureIndex; }
		int getTextureCount(int modelIndex) const;
		
		const Texture2D& getCurrentTexture() const		{ assert(mCurrentTexture != nullptr);  return *mCurrentTexture; }
		
	private:
		
		// Members
		ComponentInstancePtr<RenderableMeshComponent> mRenderableMeshComponent = { this, &SelectMeshComponent::mRenderableMeshComponent };
		int mCurrentModelIndex = 0;
		int mCurrentTextureIndex = 0;
		std::vector<ModelTextureResource*> mModels;
		std::vector<RenderableMesh> mMeshes;
		Texture2D* mCurrentTexture = nullptr;
	};
}
