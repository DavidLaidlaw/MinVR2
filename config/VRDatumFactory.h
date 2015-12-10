#ifndef VRDATUMFACTORY_H
#define VRDATUMFACTORY_H

#include <map>
#include <iostream>
#include <stdexcept>
#include "VRDatum.h"

// This factory architecture was suggested by Andrei Alexandrescu in
// his book "Modern C++ Design" --- read it to get the full
// explanation (and a more generic implementation); this is just an
// example

// This class is where we parse the serialized data and create the
// appropriate VRDatum to hold that information.

class VRDatumFactory {
public:
  // This typedef allows us to "name" arbitrary functions which take no
  // arguments and return a pointer to a VRDatum instance
  typedef VRDatumPtr (*CreateVRDatumCallback)(void *pData);

  VRDatumPtr CreateVRDatum(int VRDatumId, void *pData);

  bool RegisterVRDatum(int VRDatumId,
                          CreateVRDatumCallback Creator);

  bool UnRegisterVRDatum(int VRDatumId);

private:
  // the typedef simplifies the implementation
  typedef std::map<int, CreateVRDatumCallback> CallbackMap;
  // the callbacks are stored in a map int->callback (see typedef
  // above)
  CallbackMap m_callbacks;
};

#endif
