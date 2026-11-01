// CThingManager::~CThingManager() -- delete every thing, then destroy the members in reverse order (CurrentSectionName, the NPC name map, the entity runtime persistence, the update lists, the UID-to-thing map, the dead-thing list, the thing type table), then ~CClassFactory
// Leaf: ??1CThingManager
// Retail: Fable.exe (TLC, VC7.1) @ 0x00523440, 250 bytes. Sole caller: the ??_G 0x0052C610 (slot 0 of the CThingManager vtable
//   0x01245C44).
// Identity: first store = vtable 0x01245C44 (RTTI .?AVCThingManager@@ : CClassFactory : CBaseClassNonCopyable : CBaseClass),
//   whose slot 0 ??_G is the only caller; Ego_r.pdb ??1CThingManager@@UAE@XZ (user-declared virtual; the body calls
//   DeleteAllThings, Ego_r.pdb ?DeleteAllThings@CThingManager@@QAEXXZ, retail 0x0051FBA0).
// Callees: CThingManager::DeleteAllThings 0x0051FBA0; CCharString::~CCharString 0x0099EAE0 (CurrentSectionName, and each
//   CThingTypeInfo::DefClassType); STLport _Rb_tree<..>::_M_erase for map<unsigned __int64, pair<CCharString, CWideString> >
//   0x005286D0 and map<unsigned __int64, CThing*> 0x005278A0; NThingManager::CEntityRuntimePersistence::
//   ~CEntityRuntimePersistence 0x0052C4B0 (Ego_r.pdb ??1CEntityRuntimePersistence@NThingManager@@QAE@XZ); free 0x00BFEA14;
//   CClassFactory::~CClassFactory 0x00A37360 (??1CClassFactory@@MAE@XZ, tail jump).
// Layout (Ego_r.pdb with retail STLport sizes: CClassFactory 0x1C): Component +0x1C .. PEnvironment +0x38, ThingTypeInfo
//   vector<CThingTypeInfo> (0xC: DefType, ThingTypeFlags, DefClassType) +0x3C, DeadThingList +0x48, bools +0x54,
//   UIDToThingMap +0x58, CurrentUniqueID +0x68 (8-aligned), CurrentLocalThingUniqueID +0x70, .., RenderList +0x84,
//   UpdateLists vector<ThingLList> +0x8C, bools +0x98, EntityRuntimePersistence (0x28) +0xA0, ErrorFlag +0xC8, NPCNameMap
//   +0xCC, SerialiseVersionNumber +0xD8, LevelBeingLoaded +0xDC, CurrentSectionName +0xE0.
// Shape: body `DeleteAllThings();`, then the members through the STLport 4.6 inline dtors (_Rb_tree::clear + header free;
//   _Destroy range + _Vector_base free). CEntityRuntimePersistence needs 4-byte alignment (a byte-array stand-in put it at
//   +0x9D): its long SaveFormatVersion member gives it.
// Label: manifest "CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip" was a propagated guess.
// Header debt: engine/CClassFactory.h is flattened (no vptr / dtor), and there is no engine/CThingManager.h, so the chain is a
//   local model with the PDB member names (CClassFactory's containers opaque: its out-of-line dtor destroys them).
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
#include "engine/CBaseClassNonCopyable.h"
class CCharString {
public:
    void* PStringData;                                                   // +0x00
    ~CCharString();                                                      // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;                                                   // +0x00
    ~CWideString();
};
namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
// ---- vector (stl/_vector.h)
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
// ---- tree (stl/_tree.h)
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;
    _Base_ptr   _M_parent;
    _Base_ptr   _M_left;
    _Base_ptr   _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    ~_Rb_tree_base() { _M_header.deallocate((_Node*)_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
protected:
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    _Base_ptr& _M_root() const { return (_Base_ptr&)this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);
public:
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
};
} // namespace std
template <class T> class CArray : public std::vector<T> { };

class CClass;
class CClassTypeInfo;
class CClassFactory : public CBaseClassNonCopyable {
protected:
    virtual ~CClassFactory();                                            // 0x00A37360
public:
    unsigned char ClassTypeInfo[0xc];                                    // +0x04 vector<CClassTypeInfo> (opaque: destroyed by ~CClassFactory)
    unsigned long MaxClassTypeID;                                        // +0x10
    unsigned long NoAllocatedClasses;                                    // +0x14
    unsigned char ClassList[0x4];                                        // +0x18 list<CClass*> (opaque, as above)
};
class CThing;
class CMainGameComponent; class CGameDefinitionManager; class CWorld; class CWorldMap; class CPlayerManager;
class CNavigatorManager; class CEnvironment;
class CThingTypeInfo {
public:
    long DefType;                                                        // +0x00
    unsigned long ThingTypeFlags;                                        // +0x04
    CCharString DefClassType;                                            // +0x08
};
struct ThingLList {
    CThing* Head;
    CThing* Tail;
};
namespace NThingManager {
class CEntityRuntimePersistence {                                        // retail 0x28 bytes
public:
    ~CEntityRuntimePersistence();                                        // 0x0052C4B0
    unsigned char LevelPersistentEntityScripts[0xc];                     // +0x00 CArray<CEntityScriptInfo>
    unsigned char GamePersistentEntityScripts[0xc];                      // +0x0c CArray<CEntityScriptInfo>
    unsigned char PersistedDeadEntities[0xc];                            // +0x18 map<unsigned __int64, CPersistedDeadEntity>
    long SaveFormatVersion;                                              // +0x24
};
}
class CThingManager : public CClassFactory {
public:
    virtual ~CThingManager();
    void DeleteAllThings();                                              // 0x0051FBA0
    CMainGameComponent* Component;                                       // +0x1c
    CGameDefinitionManager* DefinitionManager;                           // +0x20
    CWorld* World;                                                       // +0x24
    CWorldMap* WorldMap;                                                 // +0x28
    unsigned long* WorldSeed;                                            // +0x2c
    CPlayerManager* PlayerManager;                                       // +0x30
    CNavigatorManager* NavigationManager;                                // +0x34
    CEnvironment* PEnvironment;                                          // +0x38
    std::vector<CThingTypeInfo> ThingTypeInfo;                           // +0x3c
    std::vector<CThing*> DeadThingList;                                  // +0x48
    bool NonScriptedEntitesPauseMode;                                    // +0x54
    bool AllEntitesPauseMode;                                            // +0x55
    bool FirstRenderOfFrame;                                             // +0x56
    std::map<unsigned __int64, CThing*> UIDToThingMap;                   // +0x58
    unsigned __int64 CurrentUniqueID;                                    // +0x68
    unsigned __int64 CurrentLocalThingUniqueID;                          // +0x70
    bool DrawCombatCollisionDebug;                                       // +0x78
    bool DrawAttitudeDebug;                                              // +0x79
    long MaxThingDrawDist;                                               // +0x7c
    long GlobalThingLoadingBehaviour;                                    // +0x80 EGlobalThingLoadingBehaviour
    ThingLList RenderList;                                               // +0x84
    std::vector<ThingLList> UpdateLists;                                 // +0x8c
    bool UseMapSpecificUIDs;                                             // +0x98
    bool UpdateAndRenderAllFlag;                                         // +0x99
    bool SerialisingGameState;                                           // +0x9a
    bool CurrentlyLoading;                                               // +0x9b
    bool LoadingEntitiesFromSaveGame;                                    // +0x9c
    NThingManager::CEntityRuntimePersistence EntityRuntimePersistence;   // +0xa0
    bool ErrorFlag;                                                      // +0xc8
    std::map<unsigned __int64, std::pair<CCharString, CWideString> > NPCNameMap;   // +0xcc
    long SerialiseVersionNumber;                                         // +0xd8
    long LevelBeingLoaded;                                               // +0xdc
    CCharString CurrentSectionName;                                      // +0xe0
};
CThingManager::~CThingManager()
{
    DeleteAllThings();
}