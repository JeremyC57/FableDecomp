// CSpecialAbilitiesForcePushDef::CSpecialAbilitiesForcePushDef() -- CParentDefClassBase base, this vtable, three empty STLport vectors (RadiusOfEffect +0x78, ObjectDamage +0x8c, TimeDelayBetweenCastsSec +0x98)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045767E, length 59 bytes.
// Callees: CParentDefClassBase::CParentDefClassBase @ 0x00430370.
// Vtable 0x012342CC = CSpecialAbilitiesForcePushDef (rtti-vtables.tsv); the manifest label
// "CBossDef" is propagated (the PDB CBossDef is 96 bytes, its vectors at +0x28..+0x48).
// Shape: the vtable store precedes the zero stores because each 12-byte block is a member object
// (first/last/end) with an inline constructor; the size-optimised def module (0x0045xxxx) forms
// each block's address once (`lea eax,[esi+N]`) and stores through short displacements.
#pragma optimize("s", on)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"

namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> >
class vector {                                  // retail STLport 4.6 vector, 12 bytes: start / finish / end of storage
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
}

// Members from Ego_r.pdb CSpecialAbilitiesForcePushDef, its offsets less 4 (the retail base is 0x39 bytes packed,
// the Ego_r one 0x3D); only the members up to the last vector the ctor builds are declared.
class CSpecialAbilitiesForcePushDef : public CParentDefClassBase { public:   // vtable 0x012342CC
    virtual void Transfer(CPersistContext& context);              // ?Transfer@CSpecialAbilitiesForcePushDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase* other);                // ?Copy@CSpecialAbilitiesForcePushDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                       // ?GetSizeofClass@CSpecialAbilitiesForcePushDef@@UAEKXZ
    float              MaxDistanceToPush;       // +0x3C
    float              MinDistanceToPush;       // +0x40
    float              DustBoxWidth;            // +0x44
    long               MaxNumOfExtraEffects;    // +0x48
    long               MinNumOfExtraEffects;    // +0x4C
    float              LevelUpTimings[3];       // +0x50
    float              ReleaseLevel1Delay;      // +0x5C
    float              ReleaseLevel2Delay;      // +0x60
    float              ReleaseLevel3DelayA;     // +0x64
    float              ReleaseLevel3DelayB;     // +0x68
    float              ReleaseLevel4Delay;      // +0x6C
    float              HeightOffGround;         // +0x70
    float              Level4OffsetMagnitude;   // +0x74
    std::vector<float> RadiusOfEffect;          // +0x78
    float              ExtraPush;               // +0x84
    float              PushSpeed;               // +0x88
    std::vector<float> ObjectDamage;            // +0x8C
    std::vector<float> TimeDelayBetweenCastsSec; // +0x98
    CSpecialAbilitiesForcePushDef();
};

CSpecialAbilitiesForcePushDef::CSpecialAbilitiesForcePushDef()
    : CParentDefClassBase()
{
}