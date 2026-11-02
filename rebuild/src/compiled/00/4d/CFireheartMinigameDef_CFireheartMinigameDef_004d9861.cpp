// CFireheartMinigameDef::~CFireheartMinigameDef() -- implicit destructor: destroy SunFX and MoonFX, then the inline CSubDefClassBase / CDefClassBase / CDefPointeeBase destructors and ~CResource
// Leaf: ??1CFireheartMinigameDef@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D9861, length 33 bytes, thiscall: ~CCharString(+0x2C), ~CCharString(+0x28), store the
//   CDefPointeeBase vtable 0x01230BA0, `jmp ~CResource` 0x009FC550. Size-optimised def TU (no alignment padding in the region).
// Identity: the only caller is the scalar deleting destructor 0x004D9845, slot 0 of vtable 0x0123E584 (RTTI CFireheartMinigameDef :
//   CSubDefClassBase : CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase). The same vtable is stored by the
//   constructor 0x004D6638 (CSubDefClassBase() 0x0044C0C0, then CCharString() at +0x28 / +0x2C) and GetSizeofClass 0x004D665A
//   returns 0x3C. Ego_r + FableWin ??1CFireheartMinigameDef@@UAE@XZ (Ego_r donor 0x004F2F19 right after ??0 0x004F2EF7); Ego_r
//   layout (no containers): CDefClassBase 0x25 packed, CCharString MoonFX +0x28, SunFX +0x2C, QuitText +0x30, YesText +0x34,
//   NoText +0x38 (0x3C bytes). reloc_bindings already binds ?CFireheartMinigameDef_Destruct@@YIXPAX@Z to this address (from the
//   caller 0x004D9845): a stand-in spelling.
// Shape: compiler-generated destructor (no CFireheartMinigameDef vtable store); the inline base destructors' vtable stores are
//   dead behind the last one, ~CDefPointeeBase's 0x01230BA0, which retail keeps (engine/CDefClassBase.h: 0x0042DAB0 does the
//   same). The out-of-line constructor is retail's own (0x004D6638): it emits the vtable, hence ??_G and this ??1. Under
//   `#pragma optimize("s", on)` the same TU also reproduces the retail ??_G 0x004D9845 (`pop ecx`) and ctor 0x004D6638 bytes.
// Repair (chunk 705): the landed source modelled an invented `__declspec(novtable) CDefPointeeBase_Derived_004d9861 :
//   CDefPointeeBase` whose user-declared ~... named no retail symbol (DIFFER 36v33: the leaf matched nothing and the verifier
//   graded the largest function of the TU). This is the real class on the shared def-chain headers.
#pragma optimize("s", on)
#include "fable_string.h"                               // CCharString() 0x0099E4B0, ~CCharString 0x0099EAE0
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CSubDefClassBase.h"                    // : CDefClassBase : CDefPointeeBase (vtable 0x01230BA0) : CResource (~CResource 0x009FC550)

class CFireheartMinigameDef : public CSubDefClassBase {        // vtable 0x0123E584; slot 0 0x004D9845
public:
    CCharString   MoonFX;                                       // +0x28
    CCharString   SunFX;                                        // +0x2c
    unsigned long QuitText;                                     // +0x30
    unsigned long YesText;                                      // +0x34
    unsigned long NoText;                                       // +0x38
    CFireheartMinigameDef();                                    // 0x004D6638
    virtual void Transfer(CPersistContext&);                    // slot 18
    virtual void Copy(const CDefClassBase*);                    // slot 19
    virtual unsigned long GetSizeofClass();                     // slot 20 0x004D665A
};

CFireheartMinigameDef::CFireheartMinigameDef()
{
}