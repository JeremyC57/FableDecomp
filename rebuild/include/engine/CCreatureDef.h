#ifndef FABLE_ENGINE_CCREATUREDEF_H
#define FABLE_ENGINE_CCREATUREDEF_H
#include "engine/CSubDefClassBase.h"
// Raw TPI0x14be3 public CSubDefClassBase base. Native00667df1 reads FlashOnHit+c9.
// Original PWoundMorphs through RespawnWaitInFramesMax and RadiusWeight/Spread tail
// remain opaque at complete retail offsets, after the four STLport container reductions.
#ifdef FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES
#include "engine/CDefString.h"
#include "stlport/fable_containers.h"
#endif
class CCreatureDef : public CSubDefClassBase {
public:
#ifdef FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES
    // Nested opt-in (chunk 760, row 0x006E2C00 CTCTalk::OnCreate): Ego_r.pdb CCreatureDef DialogueVoices
    // (vector<CDefString>, Ego_r +0x78 behind InitialActions / InitialAppearanceModifiers / PhonemeAnim, each 4 bytes wider
    // in Ego_r's STLport) at retail +0x6C (`mov eax,[edi+70h]; sub eax,[edi+6Ch]; sar eax,2` at 0x006E2C3B); the inline
    // ?StatGetTypeName@CCreatureDef@@SI?AVCCharString@@XZ (GetSubDef<CCreatureDef> 0x00485815 builds "CCreatureDef" in place).
    static CCharString __fastcall StatGetTypeName() { return CCharString("CCreatureDef", -1); }
#ifdef FABLE_RETAIL_CCREATUREDEF_INITIALAPPEARANCEMODIFIERS
    // Nested opt-in (chunk 1003, row 0x005B3AEF CTCInventoryClothing::OnInitialActivate): Ego_r.pdb InitialActions / InitialAppearanceModifiers
    // (vector<long>, Ego_r +72 / +88 with the 16-byte debug vectors) at retail +0x48 / +0x54 (the 12-byte STLport vector; OnInitialActivate
    // walks `mov eax,[edi+58h] ; sub eax,[edi+54h] ; sar eax,2` at 0x005B3B43), then PhonemeAnim (map<CCharString,long>, 12 bytes) at +0x60,
    // which puts DialogueVoices at the +0x6C above. No layout change.
    unsigned char _pad_0x25[0x48 - 0x25];
    std::vector<long> InitialActions;                                    // +0x48
    std::vector<long> InitialAppearanceModifiers;                        // +0x54
    unsigned char PhonemeAnim[0xc];                                      // +0x60 map<CCharString,long>
#else
    unsigned char _pad_0x25[0x6c - 0x25];
#endif
    std::vector<CDefString> DialogueVoices;                              // +0x6c
    unsigned char _pad_0x78[0xc8 - 0x78];
#else
    unsigned char _pad_0x25[0xa3];
#endif
    bool Short,FlashOnHit,PickPocketable,ChildThatCanRegionFollow;
    bool AllowedInProtectedTowns,UseActualHitPosForHitEffects;
    unsigned char _pad_0xce[0x1a];
};
FABLE_STATIC_ASSERT(sizeof(CCreatureDef)==0xe8);
FABLE_STATIC_ASSERT(offsetof(CCreatureDef,FlashOnHit)==0xc9);

#endif
