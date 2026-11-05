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
#ifdef FABLE_RETAIL_CRENDERMANAGER_ONVIEWPORTCHANGED
    // Opt-in (asm round 2b, row 0x009BEF80): the empty viewport-change hook 0x00A0AAC0 (landed row, descriptive name; no PDB
    // symbol), called by CDisplayManager::SetViewport through PRenderManager.
    void OnViewportChanged();                                            // 0x00A0AAC0
#endif
};
FABLE_STATIC_ASSERT(sizeof(CRenderManager) == 0x3ea4);
#endif // FABLE_ENGINE_CRENDERMANAGER_H
