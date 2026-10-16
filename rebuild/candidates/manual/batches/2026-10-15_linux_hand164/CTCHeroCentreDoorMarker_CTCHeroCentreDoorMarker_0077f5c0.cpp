// CTCHeroCentreDoorMarker::~CTCHeroCentreDoorMarker() -- kill the door marker's live particle and clear the particle pointer, then destroy both intelligent pointers and the CTCBase part
// Leaf: ??1CTCHeroCentreDoorMarker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077F5C0, length 141 bytes (+3 int3 padding). Vtable 0x0126B044.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times: the inline IsThingAlive pair, then Kill), CThing::Kill(bool)
// @ 0x004C9B80, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 / SetPItem @ 0x00A01B90 / ~CBaseIntelligentPointer
// @ 0x00A01C10, CTCBase::~CTCBase @ 0x00686830.
// Shape: `PParticle = CIntelligentPointer<CThing>(0);` builds a frame temporary (out-of-line base ctor, the CIntelligentPointer<CThing>
// vtable 0x01238C6C, SetPItem(0)), assigns it through the inline operator= (GetPItem, SetPItem) and destroys it (CIntelligentPointer has NO
// user dtor: an empty inline one would store the vtable before each base-dtor call); then the members PHeroCentre and PParticle are destroyed in reverse order.
// Layout (packed, Ego_r.pdb CTCHeroCentreDoorMarker 0x28): Radius +0x0c, DoorType +0x10, HeroInsideRadius +0x14,
// PParticle +0x15, PHeroCentre +0x1d (CIntelligentPointer<CThing>: vptr + item); CThing dead bit = bit 0 of +0x91.
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                     // +0x91 bit 0
    void Kill(bool b);                          // 0x004C9B80
};
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    virtual void v00();
    void* PData;
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { SetPItem(o.GetPItem()); return *this; }
    virtual void v00();
};
template <class P> inline bool IS_THING_ALIVE(const P& p) { return p.GetPItem() && !p.GetPItem()->Dead; }

class CTCBase {
public:
    virtual ~CTCBase();                         // 0x00686830
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
#pragma pack(push, 1)
class CTCHeroCentreDoorMarker : public CTCBase {    // vtable 0x0126B044
public:
    float Radius;                               // +0x0c
    long  DoorType;                             // +0x10
    bool  HeroInsideRadius;                     // +0x14
    CIntelligentPointer<CThing> PParticle;      // +0x15
    CIntelligentPointer<CThing> PHeroCentre;    // +0x1d
    virtual ~CTCHeroCentreDoorMarker();
};
#pragma pack(pop)

CTCHeroCentreDoorMarker::~CTCHeroCentreDoorMarker()
{
    if (IS_THING_ALIVE(PParticle)) {
        PParticle.GetPItem()->Kill(true);
        PParticle = CIntelligentPointer<CThing>(0);
    }
}
