#include "selectmeshcomponent.h"

// External Includes
#include <entity.h>
#include <mathutils.h>

// nap::selectmeshcomponent run time class definition 
RTTI_BEGIN_CLASS(nap::SelectMeshComponent)
	RTTI_PROPERTY("Models", &nap::SelectMeshComponent::mModels, nap::rtti::EPropertyMetaData::Required)
	RTTI_PROPERTY("ModelIndex", &nap::SelectMeshComponent::mModelIndex, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("TextureIndex", &nap::SelectMeshComponent::mTextureIndex, nap::rtti::EPropertyMetaData::Default)
	RTTI_PROPERTY("RenderableMeshComponent", &nap::SelectMeshComponent::mRenderableMeshComponent, nap::rtti::EPropertyMetaData::Required)
RTTI_END_CLASS

// nap::selectmeshcomponentInstance run time class definition 
RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::SelectMeshComponentInstance)
	RTTI_CONSTRUCTOR(nap::EntityInstance&, nap::Component&)
RTTI_END_CLASS

//////////////////////////////////////////////////////////////////////////



namespace nap
{
	void SelectMeshComponent::getDependentComponents(std::vector<rtti::TypeInfo>& components) const
	{

	}


	bool SelectMeshComponentInstance::init(utility::ErrorState& errorState)
	{
		SelectMeshComponent* resource = this->getComponent<SelectMeshComponent>();
		if (!errorState.check(!resource->mModels.empty(), "Has no models!"))
			return false;

		for (auto& model : resource->mModels)
		{
			RenderableMesh render_mesh = mRenderableMeshComponent->createRenderableMesh(*(model->mMesh), errorState);
			if (!render_mesh.isValid())
				return false;
			mMeshes.emplace_back(render_mesh);
			mModels.emplace_back(model.get());
		}

		mCurrentTextureIndex = resource->mTextureIndex;
		selectModel(resource->mModelIndex);

		return true;
	}


	void SelectMeshComponentInstance::update(double deltaTime)
	{

	}


	void SelectMeshComponentInstance::selectModel(int index)
	{
		mCurrentModelIndex = math::clamp<int>(index, 0, mMeshes.size() - 1);
		mRenderableMeshComponent->setMesh(mMeshes[mCurrentModelIndex]);
		selectTexture(mCurrentTextureIndex);
	}


	void SelectMeshComponentInstance::selectTexture(int index)
	{
		int tex_count = getTextureCount(mCurrentModelIndex);
		mCurrentTextureIndex = math::clamp<int>(index, 0, tex_count - 1);

		mCurrentTexture = mModels[mCurrentModelIndex]->mTextures[mCurrentTextureIndex].get();
		MaterialInstance& material = mRenderableMeshComponent->getMaterialInstance();
		UniformTexture2D& tex_uniform = material.getOrCreateUniform<UniformTexture2D>("inModelTexture");
		tex_uniform.setTexture(*mCurrentTexture);
	}


	int SelectMeshComponentInstance::getTextureCount(int modelIndex) const
	{
		assert(modelIndex < mModels.size());
		return mModels[modelIndex]->mTextures.size();
	}

}