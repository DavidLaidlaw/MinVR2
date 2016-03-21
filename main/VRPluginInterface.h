/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRPLUGININTERFACE_H_
#define VRPLUGININTERFACE_H_

#include "display/factories/VRDisplayFactory.h"
#include "plugin/PluginInterface.h"
#include "main/VRTimer.h"
#include "event/VRInputDevice.h"
#include "VRSystem.h"

namespace MinVR {

class VRPluginInterface : public PluginInterface {
public:
	virtual ~VRPluginInterface() {}

	virtual void addVRDisplayFactory(MinVR::VRDisplayFactory* factory) = 0;
	virtual MinVR::VRDisplayFactory* getBaseDisplayFactory() = 0;
	virtual void setBaseDisplayFactory(MinVR::VRDisplayFactory* displayFactory) = 0;
	virtual void addVRInputDeviceFactory(VRInputDeviceFactory* factory) = 0;
	virtual void addVRTimer(VRTimer* timer) = 0;
	virtual VRSystem* getVRSystem() = 0;

	std::string getName() { return "VRPluginInterface"; }
	const std::type_info& getType() { return typeid(VRPluginInterface); }
	int getMinVersion() { return getVersion(); }
	static int getVersion() { return 0; }
};

} /* namespace MinVR */

#endif /* VRPLUGININTERFACE_H_ */
