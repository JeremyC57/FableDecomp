// NFire::CFirePoint::~CFirePoint() -- implicit destructor of a fire point: the singe and fire effect pointers, then the helper name
// Leaf: ??1CFirePoint@NFire@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007803D0, 27 bytes, thiscall, `jmp` tail; no call or address reference (an
//   unreferenced COMDAT: VC7.1 emits an implicit destructor's `??1` when the TU destroys the object by name).
// Identity (proven): (1) layout: Ego_r.pdb NFire::CFirePoint (44 bytes) is the only PDB class with CCharString at +0 and
//   CIntelligentPointer<CThing> at +0x1C and +0x24 (HelperName, PEffect, PSingeEffect; the members between are PODs) --
//   exactly the members this body destroys. (2) TU and placement: it opens the CTCFlammable TU (CTCFlammable 0x007803F0 /
//   IsOnFire 0x00780400, CFirePoint::SetAsBurning 0x00780540); the same TU holds the other two COMDATs VC7.1 emits for a
//   destroyed-by-name class with an STLport container of it (lane experiment, chunk 661): the `??_G` 0x007811D0 (same
//   member sequence + flag test) and STLport `_Destroy<CFirePoint>` 0x00781540 among the vector<CFirePoint> helpers
//   (for_each<CFirePoint*> 0x00781370, __uninitialized_copy 0x007816C0) -- the ??1 / ??_G / _Destroy triple seen for
//   pair<CCharString,CSymbolInfo> (0x006FB460 / 0x006FB730 / 0x006FB8D0). The manifest label is propagated.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (x2), CCharString::~CCharString 0x0099EAE0.
// Shape: no user destructor (compiler-generated: members destroyed in reverse order, the first by tail jump); the
//   helper below (not a retail function) destroys a CFirePoint by name so VC7.1 emits the `??1` COMDAT.
#include "fable_string.h"      // CCharString::~CCharString @ 0x0099EAE0

class CThing;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                          // Ego_r.pdb, 8 bytes (vptr +0, PData +4)
public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10 ??1CBaseIntelligentPointer@@UAE@XZ
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {   // no destructor of its own: members call ~CBaseIntelligentPointer
public:
};

namespace NFire {
class CFirePoint {                                       // Ego_r.pdb, 44 bytes
public:
    CCharString                 HelperName;              // +0x00
    long                        HelperIndex;             // +0x04
    long                        EffectIndex;             // +0x08
    long                        Resistance;              // +0x0c
    bool                        Burning;                 // +0x10
    long                        FireDamageAfterFrame;    // +0x14
    CThing*                     PParentThing;            // +0x18
    CIntelligentPointer<CThing> PEffect;                 // +0x1c
    CIntelligentPointer<CThing> PSingeEffect;            // +0x24
};
}

void TUHarness_DestroyCFirePoint(NFire::CFirePoint* point)   // emission helper, not a retail function
{
    point->NFire::CFirePoint::~CFirePoint();
}