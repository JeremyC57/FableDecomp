// CCreatureAction_Die::CCreatureAction_Die(CThingCreatureBase*, bool) -- base CCreatureActionBase(owner, -1.0, 0.0, the owner's "STANDARD_DIE" animation, the owner's "DEATH" sounds, 100, 250), the flag byte, own vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008ED650, length 142 bytes (+ int3 padding). Label correction: the chunk/manifest name
// "CCreatureAction_StabbedToDeath" is propagated from the byte-similar 0x0087E7A0; the vtable stored here (0x01287FC4) is CCreatureAction_Die's
// (rtti-vtables.tsv, slot 0 = 0x008EDB90).
// Leaf: ??0CCreatureAction_Die
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literals 0x01277C9C "DEATH", 0x01280D14 "STANDARD_DIE"), the owner's
// vtable slot 89 (`call [eax+164h]`: the sound pair for a name, returned by value through a hidden pointer), GetTableAnimation(CThingCreatureBase*,
// const CCharString&) @ 0x006924B0 (register arguments), CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float, float, CAnimationEntry*,
// const CSoundPair&, long, long) @ 0x00693B30, CCharString::~CCharString @ 0x0099EAE0 (twice). Own vtable 0x01287FC4.
// Shape: `ret 8`; the shape of CCreatureAction_StabbedToDeath's constructor 0x0087E7A0 plus the flag byte at +0xa8, an initialiser stored before the
// vtable store; a 0x10-byte frame: both names come from inline helpers returning a CCharString by value, so they are built before any argument
// is pushed (the sound name first: arguments are evaluated right to left), the sound pair is the hidden-pointer result slot, the two strings are
// destroyed after the base call in reverse order and the vtable store follows them.
// No engine header used (the PDB headers flatten the base); modelled locally (retail CCreatureActionBase is 0xa8 bytes).
#include "fable_string.h"
class CAnimationEntry;
class CSoundPair { public:                         // two sound ids (user constructor: returned through a hidden pointer)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
class CThingCreatureBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87();
    virtual void v88();
    virtual CSoundPair GetSoundPair(const CCharString& name);   // slot 89 (+0x164)
};
CAnimationEntry* __fastcall GetTableAnimation(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};

static inline CCharString AnimationName() { return CCharString("STANDARD_DIE", -1); }
static inline CCharString SoundName() { return CCharString("DEATH", -1); }
class CCreatureAction_Die : public CCreatureActionBase { public:             // vtable 0x01287FC4
    bool Violent;                           // +0xa8
    virtual void v00();
    CCreatureAction_Die(CThingCreatureBase* owner, bool violent);
};

CCreatureAction_Die::CCreatureAction_Die(CThingCreatureBase* owner, bool violent)
    : CCreatureActionBase(owner, -1.0f, 0.0f, GetTableAnimation(owner, AnimationName()), owner->GetSoundPair(SoundName()), 100, 250)
    , Violent(violent)
{
}