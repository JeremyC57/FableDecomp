// CLevelLoader::Add(const CCountedPointer<CLevelLoader::CObject>& object) -- tell the object it was added to this loader, then append it to the loader's object list
// Leaf: ?Add@CLevelLoader@@QAEXABV?$CCountedPointer@VCObject@CLevelLoader@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C2120, 70 bytes (+ int3 padding), thiscall `ret 4`.
// Callers: four sites in the 0x00500xxx/0x00501xxx level-loading code (0x00500D8A, 0x005010BE, 0x00501329, 0x00501C48).
// Callees: CLevelLoader::CObject::OnAdd(const CLevelLoader&) @ 0x006C20B0 (landed), malloc @ 0x00BFEA0E (the STLport node allocator).
// Layout: Ego_r.pdb CLevelLoader (44 bytes): ObjectList at +0x14 (an STLport list is 4 bytes: the header node pointer);
//   CCountedPointer<T> {Data, Info}, CCPPointerInfo {RefCount, ..}; list node {next, prev, value} = 0x10 bytes.
// Shape: `object->OnAdd(*this); ObjectList.push_back(object);` -- STLport push_back = insert(end(), x) with the node
//   created by allocate(1) + placement copy (null test), the CCountedPointer copy constructor inline (copy both words,
//   bump the count when Info is set), then the four link stores reading the header's prev once.
// Label correction: the catalog `LevelLoader_CreateObjectEntry` is descriptive; Ego_r.pdb names CLevelLoader::Add.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

class CCPPointerInfo {
public:
    long RefCount;                                      // +0
};

template <class T> class CCountedPointer {
public:
    T* Data;                                            // +0
    CCPPointerInfo* Info;                               // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    T* operator->() const { return Data; }
};

namespace std {

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

class _List_node_base {
public:
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> class _List_node : public _List_node_base {
public:
    _Tp _M_data;
};

template <class _Tp> class _List_iterator {
public:
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator end() { return this->_M_node._M_data; }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};

}

class CThingManager;
class CWorldMap;
class CWorld;

class CLevelLoader {
public:
    class CObject {
    public:
        void OnAdd(const CLevelLoader& loader);         // 0x006C20B0
    };
    void Add(const CCountedPointer<CObject>& object);

    unsigned char _pad00[0x14];
    std::list<CCountedPointer<CObject> > ObjectList;    // +0x14
    CThingManager* ThingManager;                        // +0x18
    CWorldMap* WorldMap;                                // +0x1C
    CWorld* World;                                      // +0x20
};

void CLevelLoader::Add(const CCountedPointer<CObject>& object)
{
    object->OnAdd(*this);
    ObjectList.push_back(object);
}