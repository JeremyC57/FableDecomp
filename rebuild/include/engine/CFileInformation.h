#ifndef FABLE_ENGINE_CFILEINFORMATION_H
#define FABLE_ENGINE_CFILEINFORMATION_H
#include "rebuild_abi.h"
#include "engine/CDateAndTime.h"
// Original PDB complete class: three CDateAndTime values at0/28/56, bool84, size88.
class CFileInformation { public:
 CFileInformation();
 CDateAndTime CreationTime,LastAccessTime,LastWriteTime;
 bool ReadOnly;
};
FABLE_STATIC_ASSERT(sizeof(CFileInformation)==88);
#endif
