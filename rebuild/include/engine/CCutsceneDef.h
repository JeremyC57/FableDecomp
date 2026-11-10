#ifndef FABLE_ENGINE_CCUTSCENEDEF_H
#define FABLE_ENGINE_CCUTSCENEDEF_H
// CCutsceneDef (new shared header, chunk 900): the cutscene definition. Ego_r.pdb / FableWin.pdb class (V-keyed), 192 bytes in
// Ego_r (Dinkumware 16-byte vectors) -> retail 0x9c (GetSizeofClass 0x00F29D90 returns 0x9c): the 0x39-byte packed
// CParentDefClassBase, then the eight Ego_r `vector<CCharString>` members as 12-byte STLport 4.6 vectors from +0x3c (4-aligned:
// the class itself is not packed). Offsets proven by the landed Transfer 0x00F2A1D0 (eight TransferVectorBinary calls on
// +0x3c..+0x90), ~CCutsceneDef 0x00F29DC0 (eight ~vector, last first) and operator= 0x00F2A000 (eight vector operator= calls).
// No-RTTI vtable 0x012FB6E0 (25 slots): slot 0 ??_G 0x00F29DA0, slots 10-16 / 21 / 22 / 24 CParentDefClassBase's, slot 18 Transfer
// 0x00F2A1D0, slot 19 Copy 0x00F29FF0, slot 20 GetSizeofClass 0x00F29D90. The destructor is implicit (retail emits the implicit
// ??1 0x00F29DC0; no user declaration), as is operator= (Ego_r ??4CCutsceneDef@@QAEAAV0@ABV0@@Z = 0x00F2A000, the out-of-line
// COMDAT the Copy tail-jumps to).
// Requires FABLE_RETAIL_DEF_CLASS_BASE (the polymorphic CDefClassBase / CParentDefClassBase retail classes) defined before the
// first include; this header turns on FABLE_PDCB_TYPED_MEMBERS (the typed InstantiationName / SubDefInfoMap), as
// engine/NUISystem_CUIMiscThingsDef.h does.
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#ifndef FABLE_PDCB_TYPED_MEMBERS
#define FABLE_PDCB_TYPED_MEMBERS
#endif
#include "engine/CParentDefClassBase.h"
class CPersistContext;
class CCutsceneDef : public CParentDefClassBase {
public:
    CCutsceneDef();                                                      // 0x00F29D00 (??0CCutsceneDef@@QAE@XZ)
    static CDefClassBase* __fastcall Alloc();                            // 0x00F2A0D0 (?Alloc@CCutsceneDef@@SIPAVCDefClassBase@@XZ)
    virtual void Transfer(CPersistContext& context);                     // slot 18 0x00F2A1D0
    virtual void Copy(const CDefClassBase* other);                       // slot 19 0x00F29FF0
    virtual unsigned long GetSizeofClass();                              // slot 20 0x00F29D90 (`mov eax,9Ch ; ret`)
    std::vector<CCharString> Macro;                                      // +0x3c
    std::vector<CCharString> SkipCond;                                   // +0x48
    std::vector<CCharString> Lights;                                     // +0x54
    std::vector<CCharString> LightScene;                                 // +0x60
    std::vector<CCharString> SetupCond;                                  // +0x6c
    std::vector<CCharString> Sound;                                      // +0x78
    std::vector<CCharString> Answer0;                                    // +0x84
    std::vector<CCharString> Answer1;                                    // +0x90
};
FABLE_STATIC_ASSERT(sizeof(CCutsceneDef) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CCutsceneDef, Macro) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CCutsceneDef, Answer1) == 0x90);
#endif // FABLE_ENGINE_CCUTSCENEDEF_H
