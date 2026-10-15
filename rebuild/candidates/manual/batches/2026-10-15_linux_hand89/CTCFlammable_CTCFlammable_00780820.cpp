// CTCFlammable::CTCFlammable(CThing*) -- CTCBase(thing), the flammable vtable, a null def pointer, an empty fire-point vector, not burning, fire damage frame / resistance / strength / damage caused zeroed and both per-point flags cleared
// Leaf: ??0CTCFlammable@@QAE@PAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00780820, length 60 bytes. Manifest label "CTCHeroCentre::CTCHeroCentre" is
// propagated; vtable 0x0126B1A4 = CTCFlammable (rtti-vtables.tsv).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800.
// Shape: Ego_r.pdb order (0x38 bytes in its debug-STL build): PDef +0xc, FirePoints (vector<NFire::CFirePoint>), Burning,
// FireDamageAfterFrame, FireResistance, FireStrength, DamageCaused, DamagePerPoint, HasFirePoint. With the retail 0xc-byte
// vector the members land at +0x10 / +0x1c / +0x20 / +0x24 / +0x28 / +0x2c / +0x30 / +0x31, which is exactly the store
// pattern. The def pointer and the vector have inline null constructors, the rest are initialisers; one zero register
// (`xor eax,eax`), every store after the vtable store, in declaration order (the float 0.0 is a dword store of eax).
// Local model: no engine header exists for CTCFlammable; engine/CTCBase.h is a flattened POD, so the polymorphic CTCBase is
// modelled locally.
struct CThing;

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase(CThing* thing);                                 // 0x00686800
};

struct CDefPointeeBase;
struct CDefPointer {                                        // CDefPointer<CFlammableDef const>
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
};

namespace NFire { struct CFirePoint; }

template<class T> struct CRetailVector {                    // std::vector<T>, VC7.1 release: first / last / end
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    CRetailVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CTCFlammable : CTCBase {                             // vtable 0x0126B1A4
    CDefPointer PDef;                                       // +0x0c
    CRetailVector<NFire::CFirePoint> FirePoints;            // +0x10
    bool Burning;                                           // +0x1c
    long FireDamageAfterFrame;                              // +0x20
    long FireResistance;                                    // +0x24
    long FireStrength;                                      // +0x28
    float DamageCaused;                                     // +0x2c
    bool DamagePerPoint;                                    // +0x30
    bool HasFirePoint;                                      // +0x31
    virtual void v00();
    CTCFlammable(CThing* thing);
};

CTCFlammable::CTCFlammable(CThing* thing)
    : CTCBase(thing), Burning(false), FireDamageAfterFrame(0), FireResistance(0), FireStrength(0),
      DamageCaused(0.0f), DamagePerPoint(false), HasFirePoint(false)
{
}
