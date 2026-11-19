// CCreatureAction_ThrowGameObject::CCreatureAction_ThrowGameObject(CThingCreatureBase&, const C3DVector&) -- base CCreatureActionBase(owner, 0.05, 1.0, the owner's "STANDARD_CAST" animation, zeroed CSoundPair, 0, 0), own vtable, the throw target
// Leaf: ??0CCreatureAction_ThrowGameObject@@QAE@AAVCThingCreatureBase@@ABVC3DVector@@@Z
// NF95 (2026-11-19, q11253): the lookup is the PDB protected static CCreatureActionBase::GetRandomAnimFromName 0x006924B0 (Ego_r.pdb
//   0x00518E00 / FableWin.pdb 0x01DB7950; was the free stand-in GetTableAnimation); the base and own constructors take the PDB
//   references (Ego_r.pdb 0x00671D70 / FableWin.pdb 0x023D70E0 ??0CCreatureAction_ThrowGameObject@@QAE@AAVCThingCreatureBase@@ABVC3DVector@@@Z).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062D890, length 129 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal 0x0125915C "STANDARD_CAST"), GetTableAnimation(CThingCreatureBase*,
// const CCharString&) @ 0x006924B0 (register arguments), CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float, float, CAnimationEntry*,
// const CSoundPair&, long, long) @ 0x00693B30, CCharString::~CCharString @ 0x0099EAE0. Own vtable 0x01259044 (rtti-vtables.tsv).
// Shape: `ret 8`; the animation name is an IMPLICIT conversion temporary (`GetTableAnimation(owner, "STANDARD_CAST")` through the defaulted
// length, as in CCreatureAction_Read 0x007E7FD0): VC builds it first, loads the owner into edi, then the two trailing zeros and the sound pair
// share eax = 0 next to the lookup (an inline helper returning the string by value hoists the sound stores above the string constructor);
// the string dies after the base call, then the vtable store and the target copied word by word through C3DVector's memberwise copy constructor.
// No engine header layout used (the PDB flattens the base); modelled locally (retail CCreatureActionBase is 0xa8 bytes, Target at +0xa8).
// CCharString is modelled locally because fable_string.h declares its (const char*, long) constructor without the default length.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};
class CThingCreatureBase;
class CAnimationEntry;
#include "engine/C3DVector.h"
class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase& owner, float a, float b, const CAnimationEntry& animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
protected:
    // Ego_r.pdb 0x00518E00 / FableWin.pdb 0x01DB7950 protected static __fastcall (engine/CCreatureActionBase.h ANIMCTOR).
    static const CAnimationEntry& __fastcall GetRandomAnimFromName(const CThingCreatureBase& creature, const CCharString& name);   // 0x006924B0
public:
};

class CCreatureAction_ThrowGameObject : public CCreatureActionBase { public:   // vtable 0x01259044
    C3DVector Target;                       // +0xa8
    virtual void v00();
    CCreatureAction_ThrowGameObject(CThingCreatureBase& owner, const C3DVector& target);
};

CCreatureAction_ThrowGameObject::CCreatureAction_ThrowGameObject(CThingCreatureBase& owner, const C3DVector& target)
    : CCreatureActionBase(owner, 0.05f, 1.0f, GetRandomAnimFromName(owner, "STANDARD_CAST"), CSoundPair(), 0, 0)
    , Target(target)
{
}