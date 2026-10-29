// CAIStateGroup_VersusScorpionKing::IsRunnable() -- runnable when the creature's live enemy has the scorpion-king creature def type (0x80000): latch the "ARENA_CentrePoint" thing (or the enemy when there is none) as the arena centre, pick a random circling direction (more than 50 of 100) and latch the enemy
// Leaf: ?IsRunnable@CAIStateGroup_VersusScorpionKing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00963640, length 456 bytes (+ int3 padding; plain `ret`, virtual slot 11).
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (inlined GetInterface ternary form, interface 8 = bit 8 of +0x20;
//          the miss path reads the never-written slot), the component's virtual slot 29 (+0x74, the current enemy),
//          CDefClassBase::GetSubDef<...> @ 0x00485815 (CDefPointer out-parameter; manifest "GetSubDef<CAbilityDef>" is a
//          folded instantiation), CCharString(const char*, long) @ 0x0099EBF0 / copy ctor @ 0x0099EC30 / ~CCharString
//          @ 0x0099EAE0 (x2), CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, CThingSearchTools::GetPFirstThing<
//          CThingFilter_HasScriptName, CThing> @ 0x0056D050, CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor
//          @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10 (temporary vtables 0x01238C6C CIntelligentPointer<CThing>,
//          0x0123E7D8 CIntelligentPointer<CThingCreatureBase>), CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (manifest
//          "CPlayer::GetCurrentMode" is wrong), CWorld::GetPRandomSeed @ 0x0049D810, GFROR13 @ 0x00497890; the def's
//          virtual slot 1 on the last release.
// Layout: engine/CAIStateGroup_VersusScorpionKing.h (packed: GoClockwise +0x1c, PArenaCentreThing +0x1d) with the
//          generated header's flattened base modelled as the landed CAIStateGroupBase and PEnemy (+0x14, the base's
//          CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8); CThing PBaseDef +0x70, dead bit +0x91 (retail
//          CThing sits 0x10 below the PDB). The def's +0xAC dword (compared with 0x80000) is not identified.
// Shape: the def is a CDefPointer local (zeroed after the enemy is fetched) filled by a same-TU GetSubDef stand-in (the
//          landed AddWound 0x00757B30 / PickUpGoods 0x00848730 model); the name and filter are named locals in a block
//          and the NEGATED search result is the bool kept in bl across both destructors (`neg ; sbb ; inc`); the target
//          is a ternary `missing ? enemy : centre`; GetPFirstThing gets a same-TU noinline stand-in that does not let
//          the out-pointer's address escape -- with an external declaration VC keeps the out slot alive across the IP
//          temporaries and the frame words shift by 4 (out +0x18 / temps +0x10 instead of retail's shared +0x14).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const; // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CEnemyDef { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0xac - 0x08];
    long CreatureType;                          // +0xac
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
class CDefinitionManager { public:
    CDefPointer<CEnemyDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
class CDefClassBase { public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);  // slot 14 (+0x38)
    unsigned char _pad04[0x1c - 4];
    CDefinitionManager* m_pManager;             // +0x1c
    bool GetSubDef(CDefPointer<CEnemyDef>* out);   // 0x00485815
};
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<CEnemyDef>* out)
{
    long* pIndex;
    {
        CCharString name("CCreatureDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CEnemyDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                      // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {        // CIntelligentPointer<CThing> (vtable 0x01238C6C)
    virtual void v00();
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem(item); return *this; }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {     // CIntelligentPointer<CThingCreatureBase> (vtable 0x0123E7D8)
    virtual void v00();
    __forceinline CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem(item); return *this; }
};
class CThingFilter_HasScriptName { public:
    CCharString ScriptName;
    CThingFilter_HasScriptName(const CCharString& name) : ScriptName(name) { }
};
class CThingSearchTools { public:
    bool GetPFirstThing(const CThingFilter_HasScriptName& filter, CThing** out);   // 0x0056D050
};
long FirstSink_56D050(const void* self, const void* name);
#pragma auto_inline(off)
__declspec(noinline) bool CThingSearchTools::GetPFirstThing(const CThingFilter_HasScriptName& filter, CThing** out)
{
    long first = FirstSink_56D050(this, filter.ScriptName.PStringData);
    *out = (CThing*)first;
    return first != 0;
}
#pragma auto_inline(on)
struct CTCEnemySource {                         // interface 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetEnemy();                 // slot 29 (+0x74)
};
long __fastcall GFROR13(unsigned long value);  // 0x00497890
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range != 0)
        return seed % range;
    return 0;
}
class CWorld { public:
    unsigned long* GetPRandomSeed();            // 0x0049D810
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
};
#pragma pack(push, 1)
struct CAIStateGroup_VersusScorpionKing : CAIStateGroupBase {
    CIntelligentPointerCreature PEnemy;         // +0x14
    bool GoClockwise;                           // +0x1c
    CIntelligentPointerThing PArenaCentreThing; // +0x1d
    virtual bool IsRunnable();
};
#pragma pack(pop)

bool CAIStateGroup_VersusScorpionKing::IsRunnable()
{
    CTCEnemySource* source;
    PBrain->PCreature->GetInterface(&source, 8);
    CThing* enemy = source->GetEnemy();
    CDefPointer<CEnemyDef> def;
    if (enemy != 0 && !(enemy->m_flags91 & 1)) {
        if (enemy->m_pDef->GetSubDef(&def) && def->CreatureType == 0x80000) {
            CThing* centre;
            bool missing;
            {
                CCharString name("ARENA_CentrePoint");
                CThingFilter_HasScriptName filter(name);
                missing = !PeekThingSearchTools()->GetPFirstThing(filter, &centre);
            }
            CThing* target = missing ? enemy : centre;
            PArenaCentreThing = CIntelligentPointerThing(target);
            unsigned long* seed = GetPWorld()->GetPRandomSeed();
            GoClockwise = WorldRandomRange(*seed, 100) > 50;
            PEnemy = CIntelligentPointerCreature(enemy);
            return true;
        }
    }
    return false;
}