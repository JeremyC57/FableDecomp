// CTCFlammable::CTCFlammable(CThing&) -- CTCBase(thing), the flammable vtable, a null def pointer, an empty fire-point vector, not burning, fire damage frame / resistance / strength / damage caused zeroed and both per-point flags cleared
// Leaf: ??0CTCFlammable@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase (which bound ??0CTCBase@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCFlammable@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00780820, length 60 bytes. Manifest label "CTCHeroCentre::CTCHeroCentre" is
// propagated; vtable 0x0126B1A4 = CTCFlammable (rtti-vtables.tsv).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800.
// Shape: Ego_r.pdb order (0x38 bytes in its debug-STL build): PDef +0xc, FirePoints (vector<NFire::CFirePoint>), Burning,
// FireDamageAfterFrame, FireResistance, FireStrength, DamageCaused, DamagePerPoint, HasFirePoint. With the retail 0xc-byte
// vector the members land at +0x10 / +0x1c / +0x20 / +0x24 / +0x28 / +0x2c / +0x30 / +0x31, which is exactly the store
// pattern. The def pointer and the vector have inline null constructors, the rest are initialisers; one zero register
// (`xor eax,eax`), every store after the vtable store, in declaration order (the float 0.0 is a dword store of eax).
// Local model: no engine header exists for CTCFlammable; engine/CTCBase.h is a flattened POD, so the polymorphic CTCBase is [superseded 2026-11-16: the header's FABLE_RETAIL_CTCBASE branch is polymorphic; see the Ported note]
// modelled locally.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

class CDefPointeeBase;
struct CDefPointer {                                        // CDefPointer<CFlammableDef const>
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
};

namespace NFire { class CFirePoint; }

template<class T> struct CRetailVector {                    // std::vector<T>, VC7.1 release: first / last / end
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    CRetailVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

class CTCFlammable : public CTCBase { public:                             // vtable 0x0126B1A4
    CDefPointer PDef;                                       // +0x0c
    CRetailVector<NFire::CFirePoint> FirePoints;            // +0x10
    bool Burning;                                           // +0x1c
    long FireDamageAfterFrame;                              // +0x20
    long FireResistance;                                    // +0x24
    long FireStrength;                                      // +0x28
    float DamageCaused;                                     // +0x2c
    bool DamagePerPoint;                                    // +0x30
    bool HasFirePoint;                                      // +0x31
    CTCFlammable(CThing& thing);
};

CTCFlammable::CTCFlammable(CThing& thing)
    : CTCBase(thing), Burning(false), FireDamageAfterFrame(0), FireResistance(0), FireStrength(0),
      DamageCaused(0.0f), DamagePerPoint(false), HasFirePoint(false)
{
}