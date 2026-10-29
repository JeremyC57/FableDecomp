// CCombatAction_GenericResponseToDrain::Initialise() -- GENERIC_RESPONSE_DRAIN animation; with a live drainer the creature is turned to face away from it (flat direction, up = Z) and given a CModeDrained(creature, drainer) through its mode manager; then the base Initialise
// Leaf: ?Initialise@CCombatAction_GenericResponseToDrain@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008676A0, 379 bytes (384 with padding), virtual Initialise.
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("GENERIC_RESPONSE_DRAIN" 0x012771D0), SetCombatAnimation 0x008574D0,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPos 0x004C73D0, C3DVector::AccurateNormalise @ 0x00A14440 (catalog
//   label C3DVector::GetScaled), the physics virtual slot 32 (+0x80, orientation from a CRightHandedSet), CVectorMap::LowerBound
//   0x0040F020 (inlined GetInterface 0x31; the absent arm reads the never-set out pointer from its shared frame word),
//   operator new 0x00BFEA1A (0x80 bytes), NCreatureMode::CModeDrained(CThing*, CThing*) 0x00868220 (landed),
//   CCountedPointer<CModeBase>(CModeBase*) 0x00718A00, CTCCreatureModeManager::AddMode 0x0073ACC0, CCombatActionBase::Initialise
//   0x00857520 (called last, on both paths).
// Shape: the landed IceTrollIcicleAssaultInto::Initialise model (scoped animation name, the owner through the intelligent
//   pointer's inline operator CThing*() so it is fetched before the target) plus the CastAssassinRush orientation idiom
//   (physics reference fetched through an inline GetPhysics() before the set, Up member-wise, Forward copied).
// Layout: local model -- retail CCombatActionBase is 0x114 bytes (PCreature +0x08, PTarget +0xa8); CThing PhysicsTC +0x60.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector& AccurateNormalise();                     // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
};
class CRightHandedSet { public: C3DVector Up, Forward; };
class CTCPhysicsBase { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void SetOrientation(const CRightHandedSet& set);   // slot 32 (+0x80)
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;       // 0x004C73D0
    __forceinline CTCPhysicsBase* GetPhysics() const { return PhysicsTC; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    operator CThing*() const { return GetPItem(); }
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
namespace NCreatureMode {
struct CModeBase { };
struct CModeDrained : CModeBase {               // 0x80 bytes
    unsigned char _pad[0x80];
    CModeDrained(CThing* thing, CThing* drainer);   // 0x00868220
};
}
template <class T> class CCountedPointer { public:
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer(T* object);                 // 0x00718A00
    CCountedPointer(const CCountedPointer& other);
};
class CTCCreatureModeManager { public:                 // interface type 0x31
    bool AddMode(CCountedPointer<NCreatureMode::CModeBase> mode);   // 0x0073ACC0
};
class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};
class CCombatAction_GenericResponseToDrain : public CCombatActionBase {
public:
    virtual void Initialise();
};

void CCombatAction_GenericResponseToDrain::Initialise()
{
    {
        CCharString name("GENERIC_RESPONSE_DRAIN");
        SetCombatAnimation(name);
    }
    if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1)) {
        CThing* creature = PCreature.GetPItem();
        const C3DVector& drainerPosition = PTarget.GetPItem()->GetPos();
        const C3DVector& position = creature->GetPos();
        C3DVector away(position.X - drainerPosition.X, position.Y - drainerPosition.Y, 0.0f);
        away.AccurateNormalise();
        CTCPhysicsBase& physics = *PCreature.GetPItem()->GetPhysics();
        CRightHandedSet set;
        set.Up.X = 0; set.Up.Y = 0; set.Up.Z = 1;
        set.Forward = away;
        physics.SetOrientation(set);
        CTCCreatureModeManager* modes;
        PCreature.GetPItem()->GetInterface(&modes, 0x31);
        modes->AddMode(CCountedPointer<NCreatureMode::CModeBase>(new NCreatureMode::CModeDrained(PCreature, PTarget.GetPItem())));
    }
    CCombatActionBase::Initialise();
}