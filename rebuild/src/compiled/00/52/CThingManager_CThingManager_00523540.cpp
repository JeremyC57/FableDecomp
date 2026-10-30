// CThingManager::CThingManager(const CMainGameComponent&, const CGameDefinitionManager&, CWorld&, CPlayerManager&, CNavigatorManager&, unsigned long*, unsigned long, unsigned long, unsigned long) -- construct the thing manager: class factory for 16 class types, the game-system back pointers, empty type/dead/update containers and UID maps, then register itself as the current manager and build the thing type table
// Leaf: ??0CThingManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00523540, 432 bytes (0x1AA + int3 padding), thiscall `ret 24h`, returns this.
// Identity: Ego_r.pdb / FableWin.pdb ??0CThingManager@@QAE@ABVCMainGameComponent@@ABVCGameDefinitionManager@@AAVCWorld@@
//   AAVCPlayerManager@@AAVCNavigatorManager@@PAKKKK@Z; vtable 0x01245C44 = RTTI CThingManager. The three trailing
//   unsigned longs are unused by the body. Catalog label "CThingManager_Constructor" (descriptive).
// Callees: CClassFactory::CClassFactory(unsigned char) 0x00A373B0 (landed; the base, 0x1C bytes), the world's virtual
//   slot 12 (+0x30, its world map), malloc 0x00BFEA0E (the three STLport map header nodes: 0x20, 0x28, 0x20 bytes),
//   CCharString::CCharString() 0x0099E4B0, SetCurrentThingManager 0x004C74F0 (__fastcall), InitThingTypes 0x00522A20.
// Layout: the PDB member names at the retail offsets (STLport vectors 12 bytes, maps 12 bytes; Ego_r's Dinkumware
//   layout is 0x110 bytes, retail 0xE4): Component +0x1C .. PEnvironment +0x38, ThingTypeInfo +0x3C, DeadThingList +0x48,
//   UIDToThingMap +0x58, CurrentUniqueID +0x68 (8-aligned), RenderList +0x84, UpdateLists +0x8C,
//   EntityRuntimePersistence +0xA0 (two CArrays, the dead-entity map, SaveFormatVersion +0xC4), ErrorFlag +0xC8,
//   NPCNameMap +0xCC, CurrentSectionName +0xE0. No engine header exists for CThingManager; CClassFactory is modelled
//   opaquely (its landed constructor uses a local model too).
// Shape: an initialiser list in declaration order (the base, the seven back pointers with the world map from the
//   virtual call, the STLport `_Vector_base(const allocator&)` zero triples, `_Rb_tree_base` header allocate(1) +
//   `_M_empty_initialize` re-reading the header through the member, the false / 0 / 1 scalars), then the body in
//   source order: RenderList cleared, SetCurrentThingManager(this), the flags, PEnvironment from the world, the UID
//   seed 1, the 60 draw distance, InitThingTypes(), and SaveFormatVersion = 2 last.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

class CCharString {
public:
    void* PStringData;
    CCharString();                                          // 0x0099E4B0
};
class CWideString { public: void* PStringData; };

namespace std {
template <class T> class allocator {
public:
    allocator() {}
    allocator(const allocator&) {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class V, class T, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) {}
};
template <class T, class A> class _Vector_base {
public:
    _Vector_base(const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) {}
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, A> _M_end_of_storage;
};
template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    explicit vector(const A& a = A()) : _Vector_base<T, A>(a) {}
};
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base { V _M_value_field; };

template <class V, class A> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<V> _Node;
    _Rb_tree_base(const A&) : _M_header(allocator<_Node>(), (_Rb_tree_node_base*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Node, allocator<_Node> > _M_header;
};
template <class K, class V, class C, class A> class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    unsigned int _M_node_count;
    C _M_key_compare;
    _Rb_tree(const C& comp, const A& a) : _Rb_tree_base<V, A>(a), _M_node_count(0), _M_key_compare(comp)
    {
        _M_empty_initialize();
    }
    private:
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
    public:
};
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    _Rb_tree<K, pair<const K, T>, C, A> _M_t;
    map() : _M_t(C(), A()) {}
};
}

template <class T> class CArray : public std::vector<T> { };

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { };
class CClassFactory : public CBaseClassNonCopyable {
protected:
    CClassFactory(unsigned char maxClassTypeID);            // 0x00A373B0
public:
    void* ClassTypeInfo[3];                                 // +0x04
    unsigned long MaxClassTypeID;                           // +0x10
    long NoAllocatedClasses;                                // +0x14
    void* ClassList;                                        // +0x18
};

class CThing;
class CMainGameComponent;
class CGameDefinitionManager;
class CWorldMap;
class CEnvironment;
class CPlayerManager;
class CNavigatorManager;
class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();                       // slot 12 (+0x30)
    unsigned char _pad04[0x14];
    CEnvironment* PEnvironment;                             // +0x18
};

class CThingTypeInfo { public: long DefType; unsigned long ThingTypeFlags; CCharString DefClassType; };
struct ThingLList { CThing* Head; CThing* Tail; };
enum EGlobalThingLoadingBehaviour { GLOBAL_THING_LOADING_BEHAVIOUR_0, GLOBAL_THING_LOADING_BEHAVIOUR_1 };

namespace NThingManager {
class CEntityRuntimePersistence {
public:
    class CEntityScriptInfo { public: long _a, _b, _c; };
    class CPersistedDeadEntity { public: long _a, _b, _c, _d; };
    CArray<CEntityScriptInfo> LevelPersistentEntityScripts;                    // +0x00
    CArray<CEntityScriptInfo> GamePersistentEntityScripts;                     // +0x0c
    std::map<unsigned __int64, CPersistedDeadEntity> PersistedDeadEntities;   // +0x18
    long SaveFormatVersion;                                                    // +0x24
};
}

class CThingManager : public CClassFactory {
public:
    const CMainGameComponent* Component;                    // +0x1c
    const CGameDefinitionManager* DefinitionManager;        // +0x20
    CWorld* World;                                          // +0x24
    CWorldMap* WorldMap;                                    // +0x28
    unsigned long* WorldSeed;                               // +0x2c
    CPlayerManager* PlayerManager;                          // +0x30
    CNavigatorManager* NavigationManager;                   // +0x34
    CEnvironment* PEnvironment;                             // +0x38
    std::vector<CThingTypeInfo> ThingTypeInfo;             // +0x3c
    std::vector<CThing*> DeadThingList;                    // +0x48
    bool NonScriptedEntitesPauseMode;                       // +0x54
    bool AllEntitesPauseMode;                               // +0x55
    bool FirstRenderOfFrame;                                // +0x56
    std::map<unsigned __int64, CThing*> UIDToThingMap;     // +0x58
    unsigned __int64 CurrentUniqueID;                       // +0x68
    unsigned __int64 CurrentLocalThingUniqueID;             // +0x70
    bool DrawCombatCollisionDebug;                          // +0x78
    bool DrawAttitudeDebug;                                 // +0x79
    long MaxThingDrawDist;                                  // +0x7c
    EGlobalThingLoadingBehaviour GlobalThingLoadingBehaviour;   // +0x80
    ThingLList RenderList;                                  // +0x84
    std::vector<ThingLList> UpdateLists;                   // +0x8c
    bool UseMapSpecificUIDs;                                // +0x98
    bool UpdateAndRenderAllFlag;                            // +0x99
    bool SerialisingGameState;                              // +0x9a
    bool CurrentlyLoading;                                  // +0x9b
    bool LoadingEntitiesFromSaveGame;                       // +0x9c
    NThingManager::CEntityRuntimePersistence EntityRuntimePersistence;   // +0xa0
    bool ErrorFlag;                                         // +0xc8
    std::map<unsigned __int64, std::pair<CCharString, CWideString> > NPCNameMap;   // +0xcc
    long SerialiseVersionNumber;                            // +0xd8
    long LevelBeingLoaded;                                  // +0xdc
    CCharString CurrentSectionName;                         // +0xe0

    CThingManager(const CMainGameComponent& component, const CGameDefinitionManager& definitionManager, CWorld& world,
                  CPlayerManager& playerManager, CNavigatorManager& navigatorManager, unsigned long* worldSeed,
                  unsigned long, unsigned long, unsigned long);
    virtual ~CThingManager();
    void InitThingTypes();                                  // 0x00522A20
};

void __fastcall SetCurrentThingManager(CThingManager* manager);   // 0x004C74F0

CThingManager::CThingManager(const CMainGameComponent& component, const CGameDefinitionManager& definitionManager,
                             CWorld& world, CPlayerManager& playerManager, CNavigatorManager& navigatorManager,
                             unsigned long* worldSeed, unsigned long, unsigned long, unsigned long)
    : CClassFactory(0x10),
      Component(&component),
      DefinitionManager(&definitionManager),
      World(&world),
      WorldMap(world.GetWorldMap()),
      WorldSeed(worldSeed),
      PlayerManager(&playerManager),
      NavigationManager(&navigatorManager),
      NonScriptedEntitesPauseMode(false),
      AllEntitesPauseMode(false),
      CurrentLocalThingUniqueID(0),
      DrawCombatCollisionDebug(false),
      DrawAttitudeDebug(false),
      GlobalThingLoadingBehaviour(GLOBAL_THING_LOADING_BEHAVIOUR_1),
      LoadingEntitiesFromSaveGame(false),
      ErrorFlag(false)
{
    RenderList.Head = 0;
    RenderList.Tail = 0;
    SetCurrentThingManager(this);
    CurrentlyLoading = false;
    UseMapSpecificUIDs = false;
    PEnvironment = World->PEnvironment;
    CurrentUniqueID = 1;
    MaxThingDrawDist = 60;
    UpdateAndRenderAllFlag = false;
    InitThingTypes();
    EntityRuntimePersistence.SaveFormatVersion = 2;
}