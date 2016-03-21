/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/graphics/stereo/VRStereoNode.h>

namespace MinVR {

VRStereoNode::VRStereoNode() {
}

VRStereoNode::~VRStereoNode() {
}

void VRStereoNode::render(VRRenderer& renderer) {
	for (int passNum = 0; passNum < getNumPasses(); passNum++)
	{
		preRenderPass(renderer, passNum);
		VRDisplayNode::render(renderer);
		postRenderPass(renderer, passNum);
	}
}

} /* namespace MinVR */
