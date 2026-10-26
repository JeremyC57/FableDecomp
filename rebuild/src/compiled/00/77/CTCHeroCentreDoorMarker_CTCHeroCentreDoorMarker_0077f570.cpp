// CTCHeroCentreDoorMarker::CTCHeroCentreDoorMarker(CThing*) -- CTCBase(thing), the door-marker vtable, radius 1.0, door type 0, hero-inside flag set, and the two intelligent pointers (PParticle +0x15, PHeroCentre +0x1d)
// Leaf: ??0CTCHeroCentreDoorMarker@@QAE@PAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077F570, length 77 bytes (+ int3 padding). Manifest label "CTCHero::CTCHero" is
// propagated; vtable 0x0126B044 = CTCHeroCentreDoorMarker (rtti-vtables.tsv).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (twice, each
// followed by the CIntelligentPointer<CThing> vtable 0x01238C6C).
// Shape: Ego_r.pdb layout (0x28 bytes): Radius +0xc, DoorType +0x10, HeroInsideRadius +0x14, PParticle +0x15 and PHeroCentre
// +0x1d (UNALIGNED 8-byte intelligent pointers: pack(1) on the pointer classes). The scalars are immediate stores
// (0x3F800000, 0, 1) scheduled after the vtable store and before the first pointer constructor.
// Local model: no engine header exists for CTCHeroCentreDoorMarker; engine/CTCBase.h is a flattened POD (vptr as a data
// member), so the polymorphic CTCBase is modelled locally.
class CThing;

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase(CThing* thing);                                 // 0x00686800
};

#pragma pack(push, 1)
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {      // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
#pragma pack(pop)

enum EHeroCentreDoorType { HERO_CENTRE_DOOR_TYPE_0 };

struct CTCHeroCentreDoorMarker : CTCBase {                  // vtable 0x0126B044
    float Radius;                                           // +0x0c
    EHeroCentreDoorType DoorType;                           // +0x10
    bool HeroInsideRadius;                                  // +0x14
    CIntelligentPointer<CThing> PParticle;                          // +0x15
    CIntelligentPointer<CThing> PHeroCentre;                        // +0x1d
    virtual void v00();
    CTCHeroCentreDoorMarker(CThing* thing);
};

CTCHeroCentreDoorMarker::CTCHeroCentreDoorMarker(CThing* thing)
    : CTCBase(thing), Radius(1.0f), DoorType(HERO_CENTRE_DOOR_TYPE_0), HeroInsideRadius(true)
{
}