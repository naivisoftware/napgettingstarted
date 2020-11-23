#include "modeltexturesresource.h"

// nap::modeltexturesresource run time class definition 
RTTI_BEGIN_CLASS(nap::ModelTextureResource)
	RTTI_PROPERTY("Mesh", &nap::ModelTextureResource::mMesh, nap::rtti::EPropertyMetaData::Required)
	RTTI_PROPERTY("Textures", &nap::ModelTextureResource::mTextures, nap::rtti::EPropertyMetaData::Required)
RTTI_END_CLASS

//////////////////////////////////////////////////////////////////////////


namespace nap
{
	ModelTextureResource::~ModelTextureResource()			{ }


	bool ModelTextureResource::init(utility::ErrorState& errorState)
	{
		if (!errorState.check(!mTextures.empty(), "%s: has no textures!", mID.c_str()))
			return false;
		return true;
	}
}
