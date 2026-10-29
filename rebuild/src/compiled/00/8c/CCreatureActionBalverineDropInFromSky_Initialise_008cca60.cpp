// CCreatureAction_BalverineDropInFromSky::Initialise() -- names the animation "GENERATE_DROP", calls the creature's virtual slot 28 with true, attaches the particles of its interface-130 component, marks the creature as not willable, then runs the base Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CCA60, length 151 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetCombatAnimation(const CCharString&) @ 0x008574D0,
// CCharString::~CCharString @ 0x0099EAE0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, three times), the creature's
// virtual slot 28 (+0x70) with one argument, CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 130 = bit 2 of
// the bitset dword at +0x30), CTCParticleAttacher::AttachParticles @ 0x007F8890, SetThingAsNotBloodyWillableAtAll(CThing*, bool) @ 0x007E4040
// (`__fastcall`: ecx thing, dl flag), CCombatActionBase::Initialise @ 0x00857520 (direct base call, last).
// Shape: `this` rides in edi and the PCreature address in ebx; esi is pushed only inside the lookup block; the animation name is
// an implicit conversion temporary re-addressed with `lea`. Retail CCombatActionBase is 0x114 bytes (the engine header is the PDB's
// 0x12c-byte layout), so `this` is modelled locally.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCPhysicsBase;
struct CTCGraphicAppearance;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x14];
    CTCPhysicsBase* m_pPhysics;             // +0x60
    CTCGraphicAppearance* m_pGraphicAppearance;   // +0x64
    CTCPhysicsBase* GetPhysics() const { return m_pPhysics; }                           // inline accessors: the member is
    CTCGraphicAppearance* GetGraphicAppearance() const { return m_pGraphicAppearance; }   //   loaded straight into ecx
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CCharString { public:                        // one pointer; the defaulted length lets the literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};
struct CThingCreatureCalls {                // the creature's own virtuals: slot 28 takes one flag
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetInvisible(bool invisible);   // slot 28 (+0x70)
};
class CTCParticleAttacher { public:                // the interface-130 component
    void AttachParticles();                 // 0x007F8890
};
class CTCWillResponse {
public:
    static void __fastcall SetThingAsNotBloodyWillableAtAll(CThing*, bool);   // 0x007E4040
};   // 0x007E4040

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_BalverineDropInFromSky : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_BalverineDropInFromSky::Initialise()
{
    SetCombatAnimation("GENERATE_DROP");
    ((CThingCreatureCalls*)PCreature.GetPItem())->SetInvisible(true);
    CTCParticleAttacher* pParticles;
    if (PCreature.GetPItem()->GetInterface(&pParticles, 130))
        pParticles->AttachParticles();
    CTCWillResponse::SetThingAsNotBloodyWillableAtAll(PCreature.GetPItem(), true);
    CCombatActionBase::Initialise();
}