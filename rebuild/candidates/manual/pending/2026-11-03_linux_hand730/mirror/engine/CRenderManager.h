#ifndef FABLE_ENGINE_CRENDERMANAGER_H
#define FABLE_ENGINE_CRENDERMANAGER_H
// Retail CRenderManager (movie row 0x006286F0): RTTI (vtable 0x0129B400) CRenderManager : CRenderManager2D, offset 0.
// CDisplayManager's creator 0x009C0FD0 allocates 0x3EA4 bytes and the inline constructor is the CRenderManager2D
// constructor 0x009D9A30 plus the vtable store: no own data member. Constructor: Ego_r.pdb
// ??0CRenderManager@@QAE@AAVCDisplayManager@@ABVCWideString@@K@Z (declaration only; not defined by any retail row yet).
#include "engine/CRenderManager2D.h"
class CDisplayManager; class CWideString;
class CRenderManager : public CRenderManager2D {
public:
    CRenderManager(CDisplayManager&,const CWideString&,unsigned long);
};
FABLE_STATIC_ASSERT(sizeof(CRenderManager) == 0x3ea4);
#endif // FABLE_ENGINE_CRENDERMANAGER_H
