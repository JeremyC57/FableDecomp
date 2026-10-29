// CThingDeadCreature::SetupDeadAnimation() -- start the corpse's "STANDARD_DEAD" animation: a random one passing the creature's animation filter (else of any group) from its appearance def, played as the main animation and immediately finished, then a zero-time frame update, a frame-updated mark and a graphic appearance update
// Leaf: ?SetupDeadAnimation@CThingDeadCreature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00835E30, length 392 bytes, plain `ret`.
//   Name from Ego_r.pdb `?SetupDeadAnimation@CThingDeadCreature@@IAEXXZ`.
// Callees: LowerBound @ 0x0040F020 (component 0x5A CTCAnimationComplex, read uninitialised when absent, as the landed
//   CTCBed GetAnimationComplex model), CTCAnimationComplex::GetAnimation @ 0x0070B460, CDefClassBase::GetSubDef
//   <CAppearanceDef> @ 0x0042B0A2, the animation set's has-animation test @ 0x005DC2E0 and random pick @ 0x005DCC00,
//   CThing::GetRandomSeed @ 0x004C7AE0, the animate thing's reset @ 0x0070EC30 (name unproven),
//   CAnimationEntry::GetAnimateThingFlags @ 0x005DC390, CAnimInit::Set @ 0x0070C050, CAnimateThing::SetMainAnim
//   @ 0x0070D580, CAnimateThing::SetAsFinished @ 0x0070BE80, CAnimateThing::UpdateFrameLogic(const CAnimFrameTime&)
//   @ 0x0070E8B0, CThing::SetAsFrameUpdated @ 0x004C7C40, CThing::UpdateGraphicAppearance(long) @ 0x004C7590, the def's
//   virtual slot 1 on the last release. The name is the static CCharString @ 0x013BAF40 "STANDARD_DEAD".
// Identity: CAnimFilterNoGroup (vtable 0x0122FFDC) destroyed through the CAnimFilterBase store 0x0122FFCC.
// Layout (retail): CThing bitset +0x20 (0x5A = bit 26 of +0x28), map +0x44, PBaseDef +0x70; CThingDeadCreature
//   PAnimFilter +0xd0 (Ego_r.pdb +0xe8, CThingPhysical 0x18 lower; CCountedPointer object word); CAppearanceDef
//   Animation +0x34; CAnimInit 0x2c bytes (the landed CTCBed model).
// Shape: the filter is reached through the counted pointer's inline operator* (a raw pointer member rotates eax/edx);
//   the zero frame time is three floats (immediate stores after the push); the entry's index is read before the reset.
class CTCBase;
class CCharString { public: void* PStringData; };
extern CCharString g_CCharString_013baf40;              // "STANDARD_DEAD"

class CAnimationEntry { public:
    long AnimIndex;                                     // +0x00
    unsigned long GetAnimateThingFlags() const;         // 0x005DC390
};
class CAnimFilterBase { public:                                // vtable 0x0122FFCC
    virtual bool Accept(const CAnimationEntry* entry) const;   // 0x0041CCE4
    virtual CAnimFilterBase* Clone() const;
    ~CAnimFilterBase() {}
};
struct CAnimFilterNoGroup : CAnimFilterBase {           // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;    // 0x0041CD13
    virtual CAnimFilterBase* Clone() const;                     // 0x00429A2F
};
class CAnimationSet { public:
    bool HasAnimation(const CCharString& name, const CAnimFilterBase& filter) const;     // 0x005DC2E0
    CAnimationEntry* GetRandomAnimation(const CCharString& name, unsigned long* seed, const CAnimFilterBase& filter,
                                        long exclude) const;                             // 0x005DCC00
};

class CDefPointeeBase { public:
    virtual void v000();
    virtual void DestroyDef();                          // slot 1 (+4)
    long m_refCount;                                    // +4
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};
template<class T> class CDefPointer { public:
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
    ~CDefPointer() { if (m_pObject) m_pObject->ReleaseDef(); }
};
class CAppearanceDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x2c];
    CAnimationSet Animation;                            // +0x34
};
class CDefClassBase { public:
    CAppearanceDef* FindAppearanceDef();                // stand-in callee of the stand-in body below
    bool GetSubDef(CDefPointer<CAppearanceDef>& out);   // 0x0042B0A2
};
// Same-TU stand-in for GetSubDef<CAppearanceDef> (the landed 0x00835BD0 idiom): it only writes the out slot, so VC7.1
// knows the def pointer does not escape and reloads it from the slot after a clobber.
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAppearanceDef>& out)
{
    CAppearanceDef* def = FindAppearanceDef();
    if (!def)
        return false;
    ++def->m_refCount;
    out.m_pObject = def;
    return true;
}
#pragma auto_inline(on)

struct CBlendPair {
    float m_first;
    float m_second;
    CBlendPair() : m_first(0.0f), m_second(0.0f) { }
};
class CAnimInit { public:                                      // 0x2c bytes
    long          Mode;                                 // +0x00
    unsigned char AnimType;                             // +0x04
    bool          Flag05;                               // +0x05
    long          AnimIndex;                            // +0x08
    float         Scales[3];                            // +0x0c
    CBlendPair    Blend;                                // +0x18
    unsigned long Flags;                                // +0x20
    long          Arg24;                                // +0x24
    long          Percent;                              // +0x28
    void Set(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent);   // 0x0070C050
    CAnimInit(long animType, long arg24, long animIndex, long mode, bool flag05, unsigned long flags, long percent)
    {
        Set(animType, arg24, animIndex, mode, flag05, flags, percent);
    }
};
struct CAnimFrameTime {
    float Frame;
    float SubFrame;
    float Extra;
    CAnimFrameTime() : Frame(0.0f), SubFrame(0.0f), Extra(0.0f) { }
};
class CAnimateThing { public:
    void SetAsFinished();                               // 0x0070BE80
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
    void UpdateFrameLogic(const CAnimFrameTime& time);  // 0x0070E8B0
    void Reset();                                       // 0x0070EC30
};
class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();                      // 0x0070B460
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefClassBase* PBaseDef;                            // +0x70
    void UpdateGraphicAppearance(long reason);          // 0x004C7590
    unsigned long* GetRandomSeed();                     // 0x004C7AE0
    void SetAsFrameUpdated();                           // 0x004C7C40
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        return (T*)((it == map->last || type < it->first) ? map->last : it)->second;
    }
    __forceinline CTCAnimationComplex* GetAnimationComplex()
    {
        CTCAnimationComplex* complex;
        if (HasTC(0x5a))
            complex = GetTC<CTCAnimationComplex>(0x5a);
        return complex;
    }
};

template<class T> class CCountedPointer { public:
    T*    m_pObject;
    void* m_pCount;
    T& operator*() const { return *m_pObject; }
};
class CThingDeadCreature : public CThing { public:
    unsigned char    _pad74[0xd0 - 0x74];
    CCountedPointer<CAnimFilterBase> PAnimFilter;       // +0xd0
    void SetupDeadAnimation();
};

void CThingDeadCreature::SetupDeadAnimation()
{
    CAnimateThing& animate = GetAnimationComplex()->GetAnimation();
    CDefPointer<CAppearanceDef> def;
    PBaseDef->GetSubDef(def);
    const CAnimationSet& animations = def.m_pObject->Animation;
    const CAnimationEntry* entry;
    if (animations.HasAnimation(g_CCharString_013baf40, *PAnimFilter))
        entry = animations.GetRandomAnimation(g_CCharString_013baf40, GetRandomSeed(), *PAnimFilter, -1);
    else if (animations.HasAnimation(g_CCharString_013baf40, CAnimFilterNoGroup()))
        entry = animations.GetRandomAnimation(g_CCharString_013baf40, GetRandomSeed(), CAnimFilterNoGroup(), -1);
    else
        entry = 0;
    if (entry) {
        long animIndex = entry->AnimIndex;
        animate.Reset();
        CAnimInit init(2, 0, animIndex, 1, true, entry->GetAnimateThingFlags(), 100);
        animate.SetMainAnim(init, 5, 0);
        animate.SetAsFinished();
    }
    animate.UpdateFrameLogic(CAnimFrameTime());
    SetAsFrameUpdated();
    UpdateGraphicAppearance(2);
}