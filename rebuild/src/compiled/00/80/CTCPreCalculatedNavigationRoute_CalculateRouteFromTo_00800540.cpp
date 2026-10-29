// CTCPreCalculatedNavigationRoute::CalculateRouteFromTo(const C3DVector& from, const C3DVector& to, vector<pair<C2DVector,uchar> >& route) -- when the game is running and both ends lie in the same world-map region, spawn the placeholder navigation creature at the owner's position, let its navigation component compute the route, then kill the creature
// Leaf: CalculateRouteFromTo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00800540, 400 bytes (`ret 0xc`).
// Callees: CTCBase accessors 0x00686D60 (`[0x013B8A1C]+0x1c`, its virtual slot 9 gates the body) and 0x00686D20 (world map,
//          `[0x013B8A1C]+0x28`); CWorldMap::GetMapNumberAt (virtual slot 16, +0x40); CWorldMap::GetRegionNumberMapIsIn 0x004FC190;
//          CCharString ctor 0x0099EBF0 / copy 0x0099EC30 / dtor 0x0099EAE0; CGameDefinitionManager::Get 0x0044C6B0;
//          CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410; CThing::GetPos 0x004C73D0;
//          CThingAICreature::Create 0x00833800 (__fastcall); CVectorMap::LowerBound const 0x004365B0 (interface 7); the
//          component's route call 0x006CB9B0 (catalog "CScriptThing::operator==", a byte twin; descriptive name); CThing::Kill 0x004C9B80.
// Layout: CThingAICreatureInit as the PDB (0x1c, landed ctor 0x004FACD0, here inlined); CThing interface bitset +0x20, map +0x44.
// Shape: the def name and the two `""` names are named locals in blocks (brain declared first); the init record is a
//          temporary of the Create call (inline ctor); the navigation lookup is the ternary GetInterface without a test.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
class C2DVector { public: float X, Y; };
struct CRoute;                                          // STLport vector<pair<C2DVector, unsigned char> >
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThingPhysical;
class CThingBuilding;
class CThingAICreatureInit { public:
    CThingPhysical* OriginalThing;                      // +0x00
    CThingPhysical* PreviousThing;                      // +0x04
    CCharString     ScriptName;                         // +0x08
    CCharString     OverridingBrainName;                // +0x0c
    bool            PlayerCharacter;                    // +0x10
    bool            Drawable;                           // +0x11
    bool            PlaceAtEndOfList;                   // +0x12
    bool            EnableCreatureAutoPlacing;          // +0x13
    CThingBuilding* HomeBuilding;                       // +0x14
    CThingBuilding* WorkBuilding;                       // +0x18
    CThingAICreatureInit(CThingPhysical* original, CThingPhysical* previous, bool playerCharacter,
                         const CCharString& scriptName, const CCharString& overridingBrainName, bool drawable)
        : OriginalThing(original), PreviousThing(previous), ScriptName(scriptName), OverridingBrainName(overridingBrainName),
          PlayerCharacter(playerCharacter), Drawable(drawable), PlaceAtEndOfList(true), EnableCreatureAutoPlacing(false),
          HomeBuilding(0), WorkBuilding(0) {}
};
struct CTCNavigationBase {                              // interface 7 (descriptive)
    void CalculateRouteFromTo(const C3DVector& from, const C3DVector& to, CRoute& route);   // 0x006CB9B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->last || interfaceType < it->first ? map->last : it)->second;
            return true;
        }
        return false;
    }
    const C3DVector& GetPos() const;               // 0x004C73D0
    void Kill(bool silent);                             // 0x004C9B80
};
class CThingAICreature : public CThing {
public:
    static CThingAICreature* __fastcall Create(long defIndex, const C3DVector& pos, long flags, const CThingAICreatureInit& init);   // 0x00833800
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();        // 0x0044C6B0
};
struct CGameRunState {                                  // descriptive: [0x013B8A1C]+0x1c
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsRunning();                           // slot 9 (+0x24, descriptive)
};
class CWorldMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAt(const C3DVector& pos) const;   // slot 16 (+0x40)
    long GetRegionNumberMapIsIn(long mapNumber) const;        // 0x004FC190
};
class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    CGameRunState* GetRunState();                       // 0x00686D60
protected:
    const CWorldMap& PeekWorldMap() const;   // 0x00686D20 (Ego_r.pdb CTCBase, protected)
};
class CTCPreCalculatedNavigationRoute : public CTCBase {
public:
    void CalculateRouteFromTo(const C3DVector& from, const C3DVector& to, CRoute& route);
};

void CTCPreCalculatedNavigationRoute::CalculateRouteFromTo(const C3DVector& from, const C3DVector& to, CRoute& route)
{
    if (!GetRunState()->IsRunning())
        return;
    long fromRegion = PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAt(from));
    if (fromRegion != PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAt(to)))
        return;
    long defIndex;
    {
        CCharString name("CREATURE_YOURE_ONLY_INTERESTED_IN_ME_FOR_MY_NAVIGATION_PLEASE_DONT_DELETE_ME");
        defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    }
    CThingAICreature* creature;
    {
        CCharString brain("");
        CCharString script("");
        creature = CThingAICreature::Create(defIndex, ParentThing->GetPos(), 4,
            CThingAICreatureInit(0, 0, false, script, brain, true));
    }
    CTCNavigationBase* navigation;
    creature->GetInterface(&navigation, 7);
    navigation->CalculateRouteFromTo(from, to, route);
    creature->Kill(false);
}