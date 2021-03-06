/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Ben Knorlein
 */

#include <ctime>
#include <cctype>

#include "VROpenVRInputDevice.h"

namespace MinVR {
                       
VROpenVRInputDevice::VROpenVRInputDevice(vr::IVRSystem *pHMD, string name):m_pHMD(pHMD), m_name(name){
	//first devices is always HMD
	devices.push_back(m_name + "_HMD");
}

VROpenVRInputDevice::~VROpenVRInputDevice() {
}

void VROpenVRInputDevice::appendNewInputEventsSinceLastCall(VRDataQueue* queue) {
	//process VR Events
	vr::VREvent_t event;
	vr::TrackedDevicePose_t pose;
	while( m_pHMD->PollNextEventWithPose(vr::VRCompositor()->GetTrackingSpace(),  &event, sizeof(event), &pose ) )
	{
		processVREvent( event, &pose );
	}

	//Report current States
	reportStates();

    for (int f = 0; f < _events.size(); f++)
	{
    	queue->push(_events[f]);
    }
    _events.clear();
}

void VROpenVRInputDevice::updatePoses(){
	vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, nullptr, 0);
	for( vr::TrackedDeviceIndex_t unDevice = 0; unDevice < devices.size(); unDevice++ )
	{
		std::string event_name = devices[unDevice];
		_dataIndex.addData(event_name + "/Pose", poseToMatrix4( &m_rTrackedDevicePose[unDevice]));
		_events.push_back(_dataIndex.serialize(event_name));
	}
}

VRMatrix4 VROpenVRInputDevice::getPose(int device_idx){
	return poseToMatrix4( &m_rTrackedDevicePose[device_idx]);
}

bool getButtonState(vr::EVRButtonId id, uint64_t state){
	return vr::ButtonMaskFromId(id) & state;
}

std::string VROpenVRInputDevice::getAxisType(int device, int axis){
	int32_t type = m_pHMD->GetInt32TrackedDeviceProperty( device, ((vr::ETrackedDeviceProperty) (vr::Prop_Axis0Type_Int32 +  axis)));
	switch(type){
		case vr::k_eControllerAxis_None:
			return "None";
		case vr::k_eControllerAxis_TrackPad:
			return "TrackPad";
		case vr::k_eControllerAxis_Joystick:
			return "Joystick";
		case vr::k_eControllerAxis_Trigger:
			return "Trigger";
	}
	return "Invalid";
}

void VROpenVRInputDevice::reportStates(){
	for( vr::TrackedDeviceIndex_t unDevice = 0; unDevice < devices.size(); unDevice++ )
	{
		vr::VRControllerState_t state;
		vr::TrackedDevicePose_t pose;
		if(m_pHMD->GetTrackedDeviceClass(unDevice) == vr::TrackedDeviceClass_Controller &&
			m_pHMD->GetControllerStateWithPose( vr::VRCompositor()->GetTrackingSpace(), unDevice, &state ,&pose)){
			std::string event_name = devices[unDevice];
			_dataIndex.addData(event_name + "/State/Pose", poseToMatrix4(&pose));

			_dataIndex.addData(event_name + "/State/" + "SystemButton" + "_Pressed" , getButtonState(vr::k_EButton_System,state.ulButtonPressed));
			_dataIndex.addData(event_name + "/State/" + "SystemButton" + "_Touched" , getButtonState(vr::k_EButton_System,state.ulButtonPressed));

			_dataIndex.addData(event_name + "/State/" + "ApplicationMenuButton" + "_Pressed" , getButtonState(vr::k_EButton_ApplicationMenu,state.ulButtonPressed));
			_dataIndex.addData(event_name + "/State/" + "ApplicationMenuButton" + "_Touched" , getButtonState(vr::k_EButton_ApplicationMenu,state.ulButtonPressed));

			_dataIndex.addData(event_name + "/State/" + "GripButton" + "_Pressed" , getButtonState(vr::k_EButton_Grip,state.ulButtonPressed));
			_dataIndex.addData(event_name + "/State/" + "GripButton" + "_Touched" , getButtonState(vr::k_EButton_Grip,state.ulButtonPressed));

			_dataIndex.addData(event_name + "/State/" + "DPad_LeftButton" + "_Pressed" , getButtonState(vr::k_EButton_DPad_Left,state.ulButtonPressed));
			_dataIndex.addData(event_name + "/State/" + "DPad_LeftButton" + "_Touched" , getButtonState(vr::k_EButton_DPad_Left,state.ulButtonPressed));

			_dataIndex.addData(event_name + "/State/" + "DPad_UpButton" + "_Pressed" , getButtonState(vr::k_EButton_DPad_Up,state.ulButtonPressed));
			_dataIndex.addData(event_name + "/State/" + "DPad_UpButton" + "_Touched" , getButtonState(vr::k_EButton_DPad_Up,state.ulButtonPressed));

			_dataIndex.addData(event_name + "/State/" + "DPad_RightButton" + "_Pressed" , getButtonState(vr::k_EButton_DPad_Right,state.ulButtonPressed));
			_dataIndex.addData(event_name + "/State/" + "DPad_RightButton" + "_Touched" , getButtonState(vr::k_EButton_DPad_Right,state.ulButtonPressed));

			_dataIndex.addData(event_name + "/State/" + "DPad_DownButton" + "_Pressed" , getButtonState(vr::k_EButton_DPad_Down,state.ulButtonPressed));
			_dataIndex.addData(event_name + "/State/" + "DPad_DownButton" + "_Touched" , getButtonState(vr::k_EButton_DPad_Down,state.ulButtonPressed));

			_dataIndex.addData(event_name + "/State/" + "AButton" + "_Pressed" , getButtonState(vr::k_EButton_A,state.ulButtonPressed));
			_dataIndex.addData(event_name + "/State/" + "AButton" + "_Touched" , getButtonState(vr::k_EButton_A,state.ulButtonPressed));

			_dataIndex.addData(event_name + "/State/" + "Axis0utton" + "_Pressed" , getButtonState(vr::k_EButton_Axis0,state.ulButtonPressed));
			_dataIndex.addData(event_name + "/State/" + "Axis0utton" + "_Touched" , getButtonState(vr::k_EButton_Axis0,state.ulButtonPressed));

			_dataIndex.addData(event_name + "/State/" + "Axis1Button" + "_Pressed" , getButtonState(vr::k_EButton_Axis1,state.ulButtonPressed));
			_dataIndex.addData(event_name + "/State/" + "Axis1Button" + "_Touched" , getButtonState(vr::k_EButton_Axis1,state.ulButtonPressed));

			_dataIndex.addData(event_name + "/State/" + "Axis2Button" + "_Pressed" , getButtonState(vr::k_EButton_Axis2,state.ulButtonPressed));
			_dataIndex.addData(event_name + "/State/" + "Axis2Button" + "_Touched" , getButtonState(vr::k_EButton_Axis2,state.ulButtonPressed));

			_dataIndex.addData(event_name + "/State/" + "Axis3Button" + "_Pressed" , getButtonState(vr::k_EButton_Axis3,state.ulButtonPressed));
			_dataIndex.addData(event_name + "/State/" + "Axis3Button" + "_Touched" , getButtonState(vr::k_EButton_Axis3,state.ulButtonPressed));

			_dataIndex.addData(event_name + "/State/" + "Axis4Button" + "_Pressed" , getButtonState(vr::k_EButton_Axis4,state.ulButtonPressed));
			_dataIndex.addData(event_name + "/State/" + "Axis4Button" + "_Touched" , getButtonState(vr::k_EButton_Axis4,state.ulButtonPressed));

			//for(int axis = 0; axis < vr::k_unControllerStateAxisCount ; axis++){
			for(int axis = 0; axis < 2 ; axis++){
				std::ostringstream stringStream;
				stringStream << event_name << + "/State/Axis" << axis << "/" ;
				std::string axis_name = stringStream.str();
				_dataIndex.addData(axis_name + "Type", getAxisType(unDevice,0));
				_dataIndex.addData(axis_name + "XPos", state.rAxis[axis].x);
				_dataIndex.addData(axis_name + "YPos", state.rAxis[axis].y);
			}
			_events.push_back(_dataIndex.serialize(event_name));
		}
	}
}

std::string VROpenVRInputDevice::getDeviceName(int idx){
	std::string entry;
	if(m_pHMD->GetTrackedDeviceClass(idx) == vr::TrackedDeviceClass_HMD)
	{
		entry = m_name + "_HMD";
	}
	else if (m_pHMD->GetTrackedDeviceClass(idx) == vr::TrackedDeviceClass_Controller){
		entry = m_name + "_Controller";
	}
	else if (m_pHMD->GetTrackedDeviceClass(idx) == vr::TrackedDeviceClass_TrackingReference){
		entry = m_name + "_TrackingReference";
	}
	else{
		entry = m_name + "_UnknownDevice";
	}

	int count = 1;
	std::ostringstream stringStream;
	stringStream << entry << "_" << count;
	std::string newEntry = stringStream.str();

	while(std::find(devices.begin(), devices.end(), newEntry) != devices.end()){
		//entry already present
		count ++;
		std::ostringstream stringStream2;
		stringStream2 << entry << "_" << count;
		newEntry = stringStream2.str();
	}
	return newEntry;
}

std::string  VROpenVRInputDevice::getButtonName(vr::EVRButtonId id){
	switch( id )
	{
		case vr::k_EButton_System:
			return "SystemButton";
		case vr::k_EButton_ApplicationMenu:
			return "ApplicationMenuButton";
		case vr::k_EButton_Grip:
			return "GripButton";
		case vr::k_EButton_DPad_Left:
			return "DPad_LeftButton";
		case vr::k_EButton_DPad_Up:
			return "DPad_UpButton";
		case vr::k_EButton_DPad_Right:
			return "DPad_RightButton";
		case vr::k_EButton_DPad_Down:
			return "DPad_DownButton";
		case vr::k_EButton_A:
			return "AButton";
		case vr::k_EButton_Axis0:
			return "Axis0utton";
		case vr::k_EButton_Axis1:
			return "Axis1Button";
		case vr::k_EButton_Axis2:
			return "Axis2Button";
		case vr::k_EButton_Axis3:
			return "Axis3Button";
		case vr::k_EButton_Axis4:
			return "Axis4Button";
	}
	return "Invalid";
}

void VROpenVRInputDevice::processVREvent( const vr::VREvent_t & event ,vr::TrackedDevicePose_t *pose)
{	
	
	switch( event.eventType )
	{
		case vr::VREvent_TrackedDeviceActivated:
		{
			devices.push_back(getDeviceName( event.trackedDeviceIndex) );
			std::cerr << "Device " << devices[devices.size()-1] << " idx : "<< event.trackedDeviceIndex << " attached.\n" << std::endl;
		}
		break;
	case vr::VREvent_TrackedDeviceDeactivated:
		{
			std::cerr << "Device " << devices[event.trackedDeviceIndex] << " detached.\n" << std::endl;
		}
		break;
	case vr::VREvent_TrackedDeviceUpdated:
		{
			std::cerr << "Device " << devices[event.trackedDeviceIndex] << " updated.\n" << std::endl;
		}
		break;
	case vr::VREvent_ButtonPress :
		{
			
			std::string event_name = devices[event.trackedDeviceIndex] + "_" + getButtonName((vr::EVRButtonId) event.data.controller.button) + "_Pressed";
			_dataIndex.addData(event_name + "/Pose", poseToMatrix4(pose));
			_events.push_back(_dataIndex.serialize(event_name));
		}
		break;
	case vr::VREvent_ButtonUnpress:
		{
			std::string event_name = devices[event.trackedDeviceIndex] + "_" + getButtonName((vr::EVRButtonId) event.data.controller.button) + "_Released";
			_dataIndex.addData(event_name + "/Pose", poseToMatrix4(pose));
			_events.push_back(_dataIndex.serialize(event_name));
		}
		break;
	case vr::VREvent_ButtonTouch:
		{
			std::string event_name = devices[event.trackedDeviceIndex] + "_" + getButtonName((vr::EVRButtonId) event.data.controller.button) + "_Touched";
			_dataIndex.addData(event_name + "/Pose", poseToMatrix4(pose));
			_events.push_back(_dataIndex.serialize(event_name));
		}
		break;
	case vr::VREvent_ButtonUntouch:
		{
			std::string event_name = devices[event.trackedDeviceIndex] + "_" + getButtonName((vr::EVRButtonId) event.data.controller.button) + "_Untouched";
			_dataIndex.addData(event_name + "/Pose", poseToMatrix4(pose));
			_events.push_back(_dataIndex.serialize(event_name));
		}
		break;
	}
}

VRMatrix4 VROpenVRInputDevice::poseToMatrix4(vr::TrackedDevicePose_t *pose)
{
	if(!pose->bPoseIsValid)
		return VRMatrix4();

	
	VRMatrix4 mat(
		pose->mDeviceToAbsoluteTracking.m[0][0], pose->mDeviceToAbsoluteTracking.m[1][0], pose->mDeviceToAbsoluteTracking.m[2][0], 0.0, 
		pose->mDeviceToAbsoluteTracking.m[0][1], pose->mDeviceToAbsoluteTracking.m[1][1], pose->mDeviceToAbsoluteTracking.m[2][1], 0.0,
		pose->mDeviceToAbsoluteTracking.m[0][2], pose->mDeviceToAbsoluteTracking.m[1][2], pose->mDeviceToAbsoluteTracking.m[2][2], 0.0,
		pose->mDeviceToAbsoluteTracking.m[0][3], pose->mDeviceToAbsoluteTracking.m[1][3], pose->mDeviceToAbsoluteTracking.m[2][3], 1.0f
		);

	return mat.transpose();
}

} /* namespace MinVR */


