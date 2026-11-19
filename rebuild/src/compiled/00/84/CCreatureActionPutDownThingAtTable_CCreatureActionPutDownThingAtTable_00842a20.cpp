// CCreatureAction_PutDownThingAtTable::CCreatureAction_PutDownThingAtTable(CThingCreatureBase&, const CCharString&, NCreatureMode::EMode) -- base CCreatureActionBase(owner, -1.0, 0.5, the owner's random animation for the name, zeroed CSoundPair, 0, 0), own vtable (0x01273C4C), the mode at +0xa8
// Leaf: ??0CCreatureAction_PutDownThingAtTable@@QAE@AAVCThingCreatureBase@@ABVCCharString@@W4EMode@NCreatureMode@@@Z
// NF95 (2026-11-19, q11253): the lookup is the PDB protected static CCreatureActionBase::GetRandomAnimFromName 0x006924B0 (Ego_r.pdb
//   0x00518E00 / FableWin.pdb 0x01DB7950; was the free stand-in GetTableAnimation(CThingCreatureBase*, long)); the base constructor takes
//   the PDB references, and the own constructor is the PDB one (Ego_r.pdb 0x0051CF60 / FableWin.pdb 0x01DE2520: creature&, the animation
//   name, NCreatureMode::EMode; landed callers 0x00922F50 (MODE_NULL) and 0x00967A20 (MODE_HOLD_JUG 0x29) pass those).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00842A20, length 82 bytes.
// Callees: CCreatureActionBase::GetRandomAnimFromName @ 0x006924B0 (register arguments: ecx owner, edx name), CCreatureActionBase::CCreatureActionBase(CThingCreatureBase&, float, float, const CAnimationEntry&, const CSoundPair&, long, long) @ 0x00693B30.
// Shape: `ret 0Ch`; arguments evaluated right to left -- the two zeros, the sound-pair temporary (register-zeroed frame stores), the lookup, 0.5f, -1.0f, the owner.
class CThingCreatureBase;
class CCharString;
#include "engine/NCreatureMode.h"
class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (register-zeroed stack stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
class CAnimationEntry;
class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase& owner, float a, float b, const CAnimationEntry& animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
protected:
    // Ego_r.pdb 0x00518E00 / FableWin.pdb 0x01DB7950 protected static __fastcall (engine/CCreatureActionBase.h ANIMCTOR).
    static const CAnimationEntry& __fastcall GetRandomAnimFromName(const CThingCreatureBase& creature, const CCharString& name);   // 0x006924B0
public:
};

class CCreatureAction_PutDownThingAtTable : public CCreatureActionBase { public:   // vtable 0x01273C4C
    NCreatureMode::EMode Mode;              // +0xa8 (PDB +0xb4)
    virtual void v00();
    CCreatureAction_PutDownThingAtTable(CThingCreatureBase& owner, const CCharString& name, NCreatureMode::EMode mode);
};

CCreatureAction_PutDownThingAtTable::CCreatureAction_PutDownThingAtTable(CThingCreatureBase& owner, const CCharString& name, NCreatureMode::EMode mode)
    : CCreatureActionBase(owner, -1.0f, 0.5f, GetRandomAnimFromName(owner, name), CSoundPair(), 0, 0)
    , Mode(mode)
{
}