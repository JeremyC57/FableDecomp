#ifndef FABLE_ENGINE_CENGINETREEFILER_H
#define FABLE_ENGINE_CENGINETREEFILER_H
// Original PDB types and native layout; see 00bca6b0/NAMING_EVIDENCE.md.
#include "rebuild_abi.h"
class CEngineTreeFilerBase {
 class CNodeHeader;
 CNodeHeader* BaseNode;
 long KeySize;
};
template<class T>class CEngineTreeFiler:public CEngineTreeFilerBase {};
FABLE_STATIC_ASSERT(sizeof(CEngineTreeFilerBase)==8);
#endif
