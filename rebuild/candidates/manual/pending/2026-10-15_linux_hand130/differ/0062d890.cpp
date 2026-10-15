// CCreatureAction_ThrowGameObject::CCreatureAction_ThrowGameObject(CThingCreatureBase*, const C3DVector&) -- base CCreatureActionBase(owner, 0.05, 1.0, the owner's "STANDARD_CAST" animation, zeroed CSoundPair, 0, 0), own vtable, the throw target
// Grade: DIFFER(129v129) -- scheduling only: retail builds the name string, loads the owner into edi and then zeroes the sound pair and the two trailing arguments from eax after the string constructor; the lane hoists the zero (in edi) above the string constructor call and loads the owner after the pushes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062D890, length 129 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal 0x0125915C "STANDARD_CAST"), GetTableAnimation(CThingCreatureBase*,
// const CCharString&) @ 0x006924B0 (register arguments), CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float, float, CAnimationEntry*,
// const CSoundPair&, long, long) @ 0x00693B30, CCharString::~CCharString @ 0x0099EAE0. Own vtable 0x01259044 (rtti-vtables.tsv).
// Shape: `ret 8`; the animation name comes from an inline helper returning a CCharString by value, so it is built before the zero arguments are
// pushed (GOTCHAS 24a); the two trailing zeros and the sound pair share eax = 0; the string dies after the base call, then the vtable store and the
// target copied word by word at fixed offsets through C3DVector's memberwise copy constructor.
// No engine header layout used (the PDB flattens the base); modelled locally (retail CCreatureActionBase is 0xa8 bytes, Target at +0xa8).
#include "fable_string.h"
struct CThingCreatureBase;
struct CAnimationEntry;
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise copy constructor: fixed-offset word moves
};
struct CSoundPair {                         // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
CAnimationEntry* __fastcall GetTableAnimation(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};

static inline CCharString AnimationName() { return CCharString("STANDARD_CAST", -1); }
struct CCreatureAction_ThrowGameObject : CCreatureActionBase {   // vtable 0x01259044
    C3DVector Target;                       // +0xa8
    virtual void v00();
    CCreatureAction_ThrowGameObject(CThingCreatureBase* owner, const C3DVector& target);
};

CCreatureAction_ThrowGameObject::CCreatureAction_ThrowGameObject(CThingCreatureBase* owner, const C3DVector& target)
    : CCreatureActionBase(owner, 0.05f, 1.0f, GetTableAnimation(owner, AnimationName()), CSoundPair(), 0, 0)
    , Target(target)
{
}
