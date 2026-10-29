// NUISystem::CMorphingSprite::~CMorphingSprite() -- morphing sprite dtor: delete every engine primitive handle in Handles and empty the list, then the member dtors (GraphicIndices map, Handles list) and ~CSprite
// Leaf: ??1CMorphingSprite@NUISystem@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005541E0, 260 bytes, tail jump to the base destructor.
// Identity: the vtable stores 0x0124A49C / 0x0124A474 / 0x0124A46C carry RTTI `.?AVCMorphingSprite@NUISystem@@`; rtti_bases:
//   CMorphingSprite : CSprite : CChangingStateComponent; Ego_r.pdb has `??1CMorphingSprite@NUISystem@@UAE@XZ`. Label right.
// Callees: free 0x00BFEA14 (list nodes, tree header), std::_Rb_tree<unsigned long, pair<const unsigned long, unsigned long>, ...>
//   ::_M_erase 0x0042FE07 (private; the out-of-line clear 0x00430046 of the same map type calls it too), NUISystem::CSprite::~CSprite
//   0x0041AB40 (tail jump; landed with this spelling); each handle's virtual scalar deleting destructor (slot 0, flag 1).
// Layout: engine/CMorphingSprite.h (retail STLport sizes): RenderCount +0x184, Handles (list<CEnginePrimitiveHandle*>) +0x188,
//   GraphicIndices (map<unsigned long, unsigned long>) +0x18C, SpriteGraphicIndex +0x198. The header is a flattened global struct
//   (vptr as data, no NUISystem namespace, no CSprite base), so the polymorphic class is modelled here (header debt).
// Shape: user body `for (it = Handles.begin(); it != Handles.end(); ++it) delete *it; Handles.clear();` (end() re-read each
//   pass; the STLport clear inlined); then the implicit member dtors: the map's ~_Rb_tree (clear: `_M_erase(root)` out of line
//   and the header reset when node_count != 0) and ~_Rb_tree_base (free the header), the list's ~_List_base (clear again, free
//   the header node), then the out-of-line ~CSprite.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { };
template <class _Pair> struct _Select1st { };

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc> class _List_base { public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, allocator<_Node> > _M_node;   // +0x0: the header node
    ~_List_base()
    {
        clear();
        _M_node.deallocate((_Node*)_M_node._M_data, 1);
    }
    void clear()
    {
        _Node* __cur = (_Node*)this->_M_node._M_data->_M_next;
        while (__cur != this->_M_node._M_data) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            this->_M_node.deallocate(__tmp, 1);
        }
        this->_M_node._M_data->_M_next = this->_M_node._M_data;
        this->_M_node._M_data->_M_prev = this->_M_node._M_data;
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { public:
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
};

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x0
    _Rb_tree_node_base* _M_parent;                          // +0x4
    _Rb_tree_node_base* _M_left;                            // +0x8
    _Rb_tree_node_base* _M_right;                           // +0xC
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
template <class _Tp, class _Alloc> class _Rb_tree_base { public:
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Rb_tree_node_base*, allocator<_Node> > _M_header;   // +0x0
    ~_Rb_tree_base() { _M_header.deallocate((_Node*)_M_header._M_data, 1); }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> { public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    unsigned int _M_node_count;                             // +0x4
    _Compare     _M_key_compare;                            // +0x8
    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
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
private:
    void _M_erase(_Link_type __x);
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map { public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                         // +0x0 (0xC bytes)
};
} // namespace std

class CCPPointerInfo { public:                              // CCountedPointer count block
    unsigned long RefCount;                                 // +0x00
    void (__fastcall* DeleteFunc)(void* data);              // +0x04
    void*         Data;                                     // +0x08
};
template <class T> class CCountedPointer { public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    __forceinline void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    __forceinline ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
};

class CBaseClass { public:
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
class CEnginePrimitiveHandle;
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                            // slot 1 (+0x04)
    mutable unsigned long IVRefCount;                       // +0x04
};
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase {
    friend class CEnginePrimitiveHandle;
protected:
    virtual void CUnprovenSlot2_012a3d60() = 0;             // slot 2: _purecall in the vtable 0x012A3D60 (no PDB name found)
    virtual void NotifyReleased(const CEnginePrimitiveHandle& handle);   // slot 3 (+0x0C)
    virtual bool WasRendered() const;                       // slot 4
};
template <class T> class CIVCountedPointer { public:
    CIVCountedPointeeBase* Object;
    T* operator->() const { return static_cast<T*>(Object); }
    unsigned long GetRefCount() const { return Object ? Object->IVRefCount : 0; }
    __forceinline void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    __forceinline void Reset(T* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->IVRefCount;
    }
    __forceinline ~CIVCountedPointer() { Release(); }
};
class CEnginePrimitiveHandle : public CBaseClass { public:  // vtable 0x0122F598, 8 bytes
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data; // +0x04
    unsigned long GetReferenceCount() const { return Data.GetRefCount(); }
    __forceinline void Clear()
    {
        if (GetReferenceCount() == 1)
            Data->NotifyReleased(*this);
        Data.Reset(0);
    }
    __forceinline virtual ~CEnginePrimitiveHandle() { Clear(); }
};


class CTexture;
enum EEngineSprite2DFlag { };                               // values not needed here

namespace NUISystem {
class CUIInterface { public: virtual ~CUIInterface(); };
class CObserver { public: virtual ~CObserver(); unsigned char _body[0x14 - 4]; };   // EventsToObserve (STLport set) +8, PreventObservation +0x14
class CActionParam { public: virtual ~CActionParam(); long ActionParamType; };       // +0x18, type +0x1C
class CComponent : public CUIInterface, public CObserver, public CActionParam { public:
    virtual ~CComponent();
    unsigned char _body[0x15c - 0x20];                      // +0x20 .. +0x15C: CComponent / CChangingStateComponent state
};
class CChangingStateComponent : public CComponent { public:
    virtual ~CChangingStateComponent();                     // 0x0052CCF0
};
class CSprite : public CChangingStateComponent { public:
    CEnginePrimitiveHandle    Handle;                       // +0x15C
    float                     Angle;                        // +0x164
    unsigned long             Width;                        // +0x168
    unsigned long             Height;                       // +0x16C
    unsigned char             NoRenderNextFrame;            // +0x170
    EEngineSprite2DFlag       Sprite2DFlag;                 // +0x174
    unsigned long             GraphicIndex;                 // +0x178
    CCountedPointer<CTexture> PTexture;                     // +0x17C
    virtual ~CSprite();                                     // 0x0041AB40
};
class CMorphingSprite : public CSprite { public:
    long                                         RenderCount;          // +0x184
    std::list<CEnginePrimitiveHandle*>           Handles;              // +0x188
    std::map<unsigned long, unsigned long>       GraphicIndices;       // +0x18C
    unsigned long                                SpriteGraphicIndex;   // +0x198
    virtual ~CMorphingSprite();
};

CMorphingSprite::~CMorphingSprite()
{
    for (std::list<CEnginePrimitiveHandle*>::iterator it = Handles.begin(); it != Handles.end(); ++it)
        delete *it;
    Handles.clear();
}
} // namespace NUISystem