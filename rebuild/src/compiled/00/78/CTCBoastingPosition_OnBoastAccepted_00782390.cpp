// CTCBoastingPosition::OnBoastAccepted(CThingCreatureBase& hero) -- the hero's boast at this position was accepted: if the podium, the position and the hero are all live and the hero can pay the cost, tell the podium, burst the orb effect, play the boast animation, switch the position off and make its thing untargetable
// Leaf: OnBoastAccepted
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00782390, 464 bytes (`ret 4`), no EH frame.
//         Ego_r.pdb: `?OnBoastAccepted@CTCBoastingPosition@@QAEXAAVCThingCreatureBase@@@Z`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020
//          and its const copy @ 0x004365B0, CTCBoastingPodium::OnHeroBoast(long, CThingCreatureBase&) @ 0x00813C50,
//          IS_THING_ALIVE<CIntelligentPointer<...>> __fastcall @ 0x004CC340 (out-of-line copy), CThing::Kill(bool) @
//          0x004C9B80, CTCDParticleEmitter::Create(const CCharString&, const C3DVector&, bool) __fastcall @ 0x006E0970,
//          CCharString @ 0x0099EBF0 / 0x0099EAE0 ("PODIUM_ORB_EXPLODE_01" @ 0x0126B2F0, "ST_BOAST" @ 0x0124DCE8),
//          CCreatureAction_PlayAnimation ctor @ 0x008424F0, CThingCreatureBase::SetCurrentAction @ 0x006644F0,
//          ~CCreatureActionBase @ 0x00693EF0.
// Shape: IS_THING_ALIVE is one inline __fastcall template (pointer test, then the dead bit in a nested `if`); VC7.1
//        expands it for the podium pointer and the hero pointer and calls the out-of-line copy for the particle effect
//        (inline-once band). The affordability test is a bool local (`setge al ; test al,al`) set inside the PeekTC arm;
//        the podium component is a zero-initialised out pointer filled by the GetTC(T**) template (`xor edi,edi` first);
//        both literals are named CCharStrings (built before the other arguments), the CSoundPair a temporary.
// Layout: CTCBoastingPosition (Ego_r.pdb, retail offsets equal): ParentThing +4, PBoastingPodium +0x0c, OrbPosition +0x14,
//         PositionActivated +0x20, BoastIndex +0x24, Cost +0x28, BoastOn +0x2c, HeroInsideRadius +0x2d, PParticleEffect
//         +0x2e (CBaseIntelligentPointer is byte-packed). Retail CThing: interface bitset +0x20, CVectorMap +0x44, dead flag
//         +0x91 bit 0. CTCHeroStats (interface 4) Money retail +0x3c (PDB +0x44: map and vector 4 bytes shorter);
//         CTCBoastingPodium = interface 0x70; CTCTargeted (interface 9) Targetable +0x10.
class CTCBase;
class CBaseObject;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class C3DVector { public: float X, Y, Z; };

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*>: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];               // +0x20
    CInterfaceMap Components;                       // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    void Kill(bool removeFromWorld);                // 0x004C9B80
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || T::INTERFACE_TYPE < it->type)
            it = map->last;
        *out = (T*)it->component;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || T::INTERFACE_TYPE < it->type)
            it = map->last;
        *out = (const T*)it->component;
        return true;
    }
};

class CCreatureActionBase {
public:
    unsigned char _pad[0xb4];                       // retail CCreatureAction_PlayAnimation is 0xb4 bytes
    ~CCreatureActionBase();                         // 0x00693EF0
};

class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CCreatureAction_PlayAnimation : public CCreatureActionBase {
public:
    CCreatureAction_PlayAnimation(CThingCreatureBase& creature, const CCharString& animation, const CSoundPair& sound,
                                  bool b1, bool b2, bool b3, long l1, long l2);   // 0x008424F0
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                           // +0x04
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};
#pragma pack(pop)
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    operator T*() const { return (T*)GetPItem(); }
    T* operator->() const { return (T*)GetPItem(); }
};

template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)
{
    if (thing) {
        if (!(thing->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCHeroStats {                                // interface 4
public:
    enum { INTERFACE_TYPE = 4 };
    unsigned char _pad00[0x3c];
    long Money;                                     // +0x3c
};
class CTCTargeted {                                 // interface 9
public:
    enum { INTERFACE_TYPE = 9 };
    unsigned char _pad00[0x10];
    bool Targetable;                                // +0x10
};
class CTCBoastingPodium {                           // interface 0x70
public:
    enum { INTERFACE_TYPE = 0x70 };
    void OnHeroBoast(long boastIndex, CThingCreatureBase& hero);   // 0x00813C50
};
class CTCDParticleEmitter {
public:
    static CThing* __fastcall Create(const CCharString& name, const C3DVector& position, bool b);   // 0x006E0970
};

class CTCBoastingPosition {
public:
    void* __vftable;                                // +0x00 (CTCBase)
    CThing* ParentThing;                            // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
    unsigned char _pad0a[2];
    CIntelligentPointer<CThing> PBoastingPodium;    // +0x0c
    C3DVector OrbPosition;                          // +0x14
    bool PositionActivated;                         // +0x20
    long BoastIndex;                                // +0x24
    long Cost;                                      // +0x28
    bool BoastOn;                                   // +0x2c
    bool HeroInsideRadius;                          // +0x2d
    CIntelligentPointer<CThing> PParticleEffect;    // +0x2e
    void OnBoastAccepted(CThingCreatureBase& hero);
};

void CTCBoastingPosition::OnBoastAccepted(CThingCreatureBase& hero)
{
    if (!IS_THING_ALIVE(PBoastingPodium) || !PositionActivated || !HeroInsideRadius || !BoastOn)
        return;
    CThingCreatureBase* pHero = &hero;
    if (!IS_THING_ALIVE(pHero))
        return;
    const CTCHeroStats* stats;
    bool canAfford = true;
    if (hero.PeekTC(&stats))
        canAfford = stats->Money >= Cost;
    if (!canAfford)
        return;
    CTCBoastingPodium* podium = 0;
    PBoastingPodium->GetTC(&podium);
    podium->OnHeroBoast(BoastIndex, hero);
    if (IS_THING_ALIVE(PParticleEffect)) {
        PParticleEffect->Kill(true);
        CCharString name("PODIUM_ORB_EXPLODE_01", -1);
        CTCDParticleEmitter::Create(name, OrbPosition, false);
    }
    {
        CCharString animation("ST_BOAST", -1);
        hero.SetCurrentAction(CCreatureAction_PlayAnimation(hero, animation, CSoundPair(), false, true, false, 0, 0));
    }
    BoastOn = false;
    CThing* parent = ParentThing;
    CTCTargeted* targeted;
    if (parent->GetTC(&targeted))
        targeted->Targetable = false;
}