#pragma once

// External Includes
#include <nap/resource.h>
#include <nap/resourceptr.h>
#include <texture2d.h>
#include <mesh.h>

namespace nap
{
	/**
	 * modeltexturesresource
	 */
	class NAPAPI ModelTextureResource : public Resource
	{
		RTTI_ENABLE(Resource)
	public:
		virtual ~ModelTextureResource();

		/**
		* Initialize this object after de-serialization
		* @param errorState contains the error message when initialization fails
		*/
		virtual bool init(utility::ErrorState& errorState) override;

		std::vector<ResourcePtr<Texture2D>> mTextures;
		ResourcePtr<IMesh> mMesh;
	};
}
