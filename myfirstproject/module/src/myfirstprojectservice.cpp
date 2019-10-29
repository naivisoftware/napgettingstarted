// Local Includes
#include "myfirstprojectservice.h"

// External Includes
#include <nap/core.h>
#include <nap/resourcemanager.h>
#include <nap/logger.h>
#include <iostream>

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::MyFirstProjectService)
	RTTI_CONSTRUCTOR(nap::ServiceConfiguration*)
RTTI_END_CLASS

namespace nap
{
	bool MyFirstProjectService::init(nap::utility::ErrorState& errorState)
	{
		//Logger::info("Initializing MyFirstProjectService");
		return true;
	}


	void MyFirstProjectService::update(double deltaTime)
	{
	}
	

	void MyFirstProjectService::getDependentServices(std::vector<rtti::TypeInfo>& dependencies)
	{
	}
	

	void MyFirstProjectService::shutdown()
	{
	}
}
