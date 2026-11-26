// CInputProcessCutScene::ProcessWhilePaused() const -- keep processing cut-scene input while paused when the player GUI's byte flag at +0xF9 is set
// Leaf: ?ProcessWhilePaused@CInputProcessCutScene@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00690AC0, 12 bytes: `mov eax,[0x013B8790] ; mov al,[eax+0F9h] ; ret`. CInputProcessCutScene vftable slot 7 (RTTI; NF477 audit:
// one `.rdata` dword, no call). Identity: Ego_r.pdb + FableWin.pdb ?ProcessWhilePaused@CInputProcessCutScene@@UBE_NXZ (public virtual const).
// NF478 (r467b, NAME_FIXES_QUEUE nf477, 2026-11-26): was the free stand-in extern "C" @ProcessWhilePaused@4 (fastcall, unsigned char); respelt onto
//   engine/CInputProcessCutScene.h FABLE_RETAIL_INPUTPROCESS_VTABLE. The +0xF9 byte is read as the bool the virtual returns (bytes unchanged);
//   its CPlayerGui member is still descriptive (`flag`: the Ego_r.pdb layout does not line up with retail's here).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_obj (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
#pragma pack(push,1)
class CPlayerGui { public: char pad[0xf9]; bool flag; };
#pragma pack(pop)
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessCutScene.h"

bool CInputProcessCutScene::ProcessWhilePaused() const { return CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().flag; }
