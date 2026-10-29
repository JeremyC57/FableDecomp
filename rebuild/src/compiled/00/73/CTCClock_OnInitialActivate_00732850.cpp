// CTCClock::OnInitialActivate() -- reset the last-updated time to 0.5 and start the clock's "CLOCKHAND_TURN" appearance animation as the main animation (type 0, looping, mode 1, percent 100)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00732850, 247 bytes (+ int3 padding), plain `ret`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined lookup of CTCAnimationComplex,
//   interface 0x5a), CTCAnimationComplex::GetAnimation @ 0x0070B460, CDefClassBase::GetSubDef<CAppearanceDef> @ 0x0042B0A2
//   (byte-twin label GetSubDef<CAbilityDef>), CAnimationSet::GetAnimation(const CCharString&, long, const CAnimFilterBase&)
//   @ 0x005DC340, CAnimInit::Set @ 0x0070C050, CAnimateThing::SetMainAnim(const CAnimInit&) @ 0x0070D920 (the one-argument
//   overload forwarding (init, 1, 0) to 0x0070D580; manifest "operator_delete" is propagated), the defs' virtual slot 1.
// Globals: the static CCharString "CLOCKHAND_TURN" @ 0x013BAD3C (its dynamic initializer is landed at 0x0121ED40).
// Filter temporary: CAnimFilterNoGroup (vtable 0x0122FFDC; base CAnimFilterBase 0x0122FFCC).
// Layout: Ego_r.pdb CTCClock (0x14): PDef +0x0c, LastTimeUpdated +0x10; retail CThing PBaseDef +0x70; CAppearanceDef
//   Animation +0x34.
// Shape: the landed CTCAnimationComplex::OnCreate 0x0070B4D0 block: the base def through a by-value counted temporary read
//   through .Object (the add-ref and the release share one null test, both count stores kept), the raw pointer then asks
//   for the appearance sub-def (def in esi for the release at the end); the animation component through the landed
//   CTCCupboard 0x00749B40 GetTCIfPresent accessor, fetched first.
class CCharString { public:
    void* PStringData;
};
extern CCharString g_ClockHandTurnName;         // 0x013BAD3C ("CLOCKHAND_TURN")
class CAnimationEntry { public:
    long AnimIndex;                             // +0x00
};
class CAnimFilterBase { public:                        // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;   // 0x0041CCE4
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {   // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;    // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                     // 0x00429A2F
};
class CAnimationSet { public:
    const CAnimationEntry* GetAnimation(const CCharString& name, long index, const CAnimFilterBase& filter) const;   // 0x005DC340
};
class CAnimInit;
class CAnimateThing { public:
    void SetMainAnim(const CAnimInit& init);    // 0x0070D920
};
struct CBlendPair {
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};
class CAnimInit { public:                              // 0x2c bytes
    long          Mode;                         // +0x00
    unsigned char AnimType;                     // +0x04
    bool          Flag05;                       // +0x05
    long          AnimIndex;                    // +0x08
    float         Scales[3];                    // +0x0c
    CBlendPair    Blend;                        // +0x18
    unsigned long Flags;                        // +0x20
    long          Arg24;                        // +0x24
    long          Percent;                      // +0x28
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};
class CDefPointeeBase { public:
    virtual void Destroy0();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CCountedDefPointer {  // the by-value CDefPointer<T> copy: add-ref on copy, release on destruction
    T* Object;
    CCountedDefPointer(T* object) : Object(object) { if (Object) Object->AddRef(); }
    ~CCountedDefPointer() { if (Object) Object->Release(); }
};
class CAppearanceDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                    // +0x34
};
class CThingBaseDef : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CAppearanceDef>* out) const;    // 0x0042B0A2
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CThingBaseDef* PBaseDef;                    // +0x70
    CCountedDefPointer<CThingBaseDef> GetBaseDef() const { return CCountedDefPointer<CThingBaseDef>(PBaseDef); }
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (it == map->last || interfaceType < it->first ? map->last : it)->second;
    }
    __forceinline CTCBase* GetTCIfPresent(long interfaceType)
    {
        CTCBase* tc;                            // deliberately unset when absent (retail reads a dead slot)
        if (HasTC(interfaceType))
            tc = GetTC(interfaceType);
        return tc;
    }
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();              // 0x0070B460
};
class CClockDef;
class CTCClock { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    const CClockDef* PDef;                      // +0x0c CDefPointer<CClockDef const>
    float   LastTimeUpdated;                    // +0x10
    void OnInitialActivate();
};

void CTCClock::OnInitialActivate()
{
    LastTimeUpdated = 0.5f;
    CAnimateThing& animation = ((CTCAnimationComplex*)ParentThing->GetTCIfPresent(0x5a))->GetAnimation();
    const CThingBaseDef* baseDef = ParentThing->GetBaseDef().Object;
    CDefPointer<CAppearanceDef> out;
    baseDef->GetSubDef(&out);
    CAppearanceDef* def = out.Object;
    const CAnimationEntry* entry = def->Animation.GetAnimation(g_ClockHandTurnName, 0, CAnimFilterNoGroup());
    CAnimInit init(0, 0, entry->AnimIndex, 1, true, 0, 100);
    animation.SetMainAnim(init);
    if (def) {
        if (--def->RefCount == 0)
            def->Destroy();
    }
}