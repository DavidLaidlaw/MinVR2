/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRVIEWPORTNODE_H_
#define VRVIEWPORTNODE_H_

#include "display/graphics/VRGraphicsWindowChild.h"
#include "VRViewport.h"
#include "VRViewportCalculator.h"

namespace MinVR {

class VRViewportNode : public VRGraphicsWindowChildNode {
public:
	VRViewportNode(const VRViewport& viewport);
	virtual ~VRViewportNode();

	void render(VRRenderer& renderer);

private:
	VRViewportCalculator m_viewportCalculator;
	VRViewport m_viewport;
};

} /* namespace MinVR */

#endif /* VRVIEWPORTNODE_H_ */
