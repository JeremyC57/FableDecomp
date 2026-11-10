// 00be1e20 CEnginePrimitive2DViewportManager::CEnginePrimitive2DViewportManager
// Leaf: ??0CEnginePrimitive2DViewportManager@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z
// Evidence: both PDBs exact ctor; donor02e901f0; retail RTTI vtable012a7ed8.
#define FABLE_RETAIL_CENGINEPRIMITIVE2DVIEWPORTMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_C2DALPHAMASKINFO_RESET
#include "engine/CEnginePrimitive2DViewportManager.h"
CEnginePrimitive2DViewportManager::CEnginePrimitive2DViewportManager(CArray<CEngineScene*>& scenes)
    : CEnginePrimitiveManager(scenes)
{
}