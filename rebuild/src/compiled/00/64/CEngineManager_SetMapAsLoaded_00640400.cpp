// CEngineManager::SetMapAsLoaded(long map) -- tell every registered engine (CIEngine::SetMapAsLoaded)
// Leaf: ?SetMapAsLoaded@CEngineManager@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00640400, 43 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?SetMapAsLoaded@CEngineManager@@QAEXJ@Z; it calls CIEngine slot 44 (+0xB0) = ?SetMapAsLoaded@CIEngine@@UAEXJ@Z
//   in the AddMap +0xA8 / RemoveMap +0xAC / SetMapAsLoaded +0xB0 / SetMapAsUnloaded +0xB4 run; its caller is CWorldMap::PostLoadTopology 0x004FF440 (landed: "CEngineManager::SetMapAsLoaded(long) 0x00640400"). The manifest's
//   "CEngineManager::RemoveMap" is propagated across byte-identical bodies.
// Shape (family M174, representative 0x006403D0): `for (it = Engines.begin(); it != Engines.end(); ++it) (*it)->SetMapAsLoaded(map);` --
//   end() re-read every pass (the virtual call may change the list). Speed TU.
class CIEngine {
public:
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
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void SetMapAsLoaded(long map);                    // slot 44 (+0xB0)
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp> struct _List_node { _List_node* _M_next; _List_node* _M_prev; _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    struct iterator {
        _List_node<_Tp>* _M_node;
        iterator(_List_node<_Tp>* __x) : _M_node(__x) { }
        _Tp& operator*() const { return _M_node->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& __x) const { return _M_node != __x._M_node; }
    };
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    _List_node<_Tp>* _M_node;
};
}
class CEngineManager {
public:
    void*                  _vfptr;                        // +0x00 (CTBaseSingleton)
    std::list<CIEngine*>   Engines;                       // +0x04
    void SetMapAsLoaded(long map);
};
void CEngineManager::SetMapAsLoaded(long map)
{
    for (std::list<CIEngine*>::iterator it = Engines.begin(); it != Engines.end(); ++it)
        (*it)->SetMapAsLoaded(map);
}