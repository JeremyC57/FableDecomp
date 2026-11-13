#ifndef FABLE_ENGINE_CENGINELOCALDETAILGENERATOR_H
#define FABLE_ENGINE_CENGINELOCALDETAILGENERATOR_H
// New header (chunk 731, 2026-11-03; one model, no class macro: no generated default exists): RTTI .?AVCEngineLocalDetailGenerator@@ (vtable 0x012A2588) : CEngineComponent (+0x0); sizeof 0x7C from
// CEngine::Initialise 0x00B26360 (`new` of 0x7C bytes before the constructor call). Constructor ??0CEngineLocalDetailGenerator@@QAE@_N@Z 0x00B59B60 and
// the PeekSceneFilterFlags override (?PeekSceneFilterFlags@CEngineLocalDetailGenerator@@UBEKXZ): Ego_r.pdb / FableWin.pdb. Own members are
// not modelled yet (one opaque block; the generated Ego_r model below lists their names).
#ifndef FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECOMPONENT
#endif
#include "rebuild_abi.h"
#include "engine/CEngineComponent.h"
class CEngineInit;
class CEngineLocalDetailGenerator : public CEngineComponent {
public:
    CEngineLocalDetailGenerator(bool);         // 0x00B59B60 ??0CEngineLocalDetailGenerator@@QAE@_N@Z
    virtual unsigned long PeekSceneFilterFlags() const;
#ifdef FABLE_RETAIL_CENGINELOCALDETAILGENERATOR_ZSPRITE
    // Overlay opt-in (row 0x00BAA870): FableWin.pdb inline ?PeekZSpriteGenerationEnabled@CEngineLocalDetailGenerator@@QBE_NXZ; Ego_r.pdb
    // ZSpriteGenerationEnabled at retail +0x24 (0x00BAA944 reads [g_pLocalDetailGenerator]+24h; the landed 0x00B59B60 ctor stores it there).
    bool PeekZSpriteGenerationEnabled() const { return ZSpriteGenerationEnabled; }
private:
    unsigned char _pad_0xc[0x24 - 0xc];
    bool ZSpriteGenerationEnabled;                                       // +0x24
    unsigned char _pad_0x25[0x7c - 0x25];
#else
private:
    unsigned char _pad_0xc[0x70];
#endif               // +0xc members not modelled yet (Ego_r.pdb names; retail STLport sizes)
};
FABLE_STATIC_ASSERT(sizeof(CEngineLocalDetailGenerator) == 0x7c);
#endif // FABLE_ENGINE_CENGINELOCALDETAILGENERATOR_H
