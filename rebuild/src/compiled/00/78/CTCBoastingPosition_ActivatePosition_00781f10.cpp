// CTCBoastingPosition::ActivatePosition(long boastIndex, long cost) -- turn the boasting position on: store the boast and its cost, spawn the idle orb effect at the orb position, make the thing drawable and set the graphic-appearance flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00781F10, length 207 bytes (+ int3 padding), `ret 8`
// (?ActivatePosition@CTCBoastingPosition@@QAEXJJ@Z).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, the __fastcall effect-thing factory
// Create(const CCharString&, const C3DVector&, bool) @ 0x006E0970, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 /
// GetPItem 0x00A01B50 / dtor 0x00A01C10 (the temporary's vtable 0x01238C6C is CIntelligentPointer<CThing>), CThing::SetAsDrawable
// (virtual slot 28, +0x70), the CThing graphic-appearance forwarder @ 0x004C7590, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (inlined GetInterface for the interface-9 component). Literal "PODIUM_ORB_IDLE_02" 0x0126B2DC.
// Layout = Ego_r.pdb CTCBoastingPosition (56 bytes): OrbPosition +0x14, PositionActivated +0x20, BoastIndex +0x24, Cost +0x28,
// BoastOn +0x2C, HeroInsideRadius +0x2D, PParticleEffect +0x2E (unaligned: the intelligent pointer is declared packed).
// Shape: the effect name is a named local in its own block (destroyed after the intelligent-pointer temporary); the pointer
// assignment reads the temporary's item before forming the member address; the parent thing is re-read for each call.
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CTCBaseInterface;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCGraphicAppearance {                   // interface type 9
    unsigned char _pad00[0x10];
    bool m_flag10;                              // +0x10
};
class CThing {
public:
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15)
    VF(16) VF(17) VF(18) VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27)
    virtual void SetAsDrawable(bool drawable);  // slot 28 (+0x70)
    unsigned char _pad04[0x20 - 4];
    unsigned long m_presentBits[9];             // +0x20 UsingComponents
    CInterfaceMap m_interfaces;                 // +0x44 Components
    void ForwardToGraphicAppearance(long request);   // 0x004C7590
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
CThing* __fastcall Create(const CCharString& defName, const C3DVector& position, bool flag);   // 0x006E0970

struct CBaseObject;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointed-to item
    CBaseObject* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
#pragma pack(push, 1)
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C for <CThing>
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        T* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
#pragma pack(pop)

class CTCBoastingPosition {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char PBoastingPodium[8];           // +0x0c CIntelligentPointer<CThing>
    C3DVector OrbPosition;                      // +0x14
    bool    PositionActivated;                  // +0x20
    long    BoastIndex;                         // +0x24
    long    Cost;                               // +0x28
    bool    BoastOn;                            // +0x2c
    bool    HeroInsideRadius;                   // +0x2d
    CIntelligentPointer<CThing> PParticleEffect;   // +0x2e
    void ActivatePosition(long boastIndex, long cost);
};

void CTCBoastingPosition::ActivatePosition(long boastIndex, long cost)
{
    PositionActivated = true;
    BoastIndex = boastIndex;
    Cost = cost;
    BoastOn = true;
    {
        CCharString name("PODIUM_ORB_IDLE_02");
        PParticleEffect = CIntelligentPointer<CThing>(Create(name, OrbPosition, false));
    }
    ParentThing->SetAsDrawable(true);
    ParentThing->ForwardToGraphicAppearance(4);
    CTCGraphicAppearance* graphic;
    if (ParentThing->GetInterface(&graphic, 9))
        graphic->m_flag10 = true;
}