// std::_List_base<NGraphicAppearance::CPrimitive, allocator<NGraphicAppearance::CPrimitive> >::clear() -- STLport: destroy every primitive node of the list (release the engine primitive handle, free the node) and relink the empty sentinel
// Leaf: clear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C5690, length 144 bytes (136 + int3 padding), plain `ret` (thiscall, no arguments).
// Identity: CTCGraphicAppearance::Primitives (+0x38, list<CPrimitive>): the callers 0x004BF914 (jmp), 0x004BF986, 0x004BF9C2,
// 0x004C0022 are CTCGraphicAppearance members and 0x004C5993 / 0x004C5C63 / 0x004C5DB2 are the list destructors
// (`clear()` then free the sentinel). The catalog "LinkedList_ClearAndDestroy" is behavioural.
// Callees: CBaseClass::~CBaseClass @ 0x0099A2E0, free @ 0x00BFEA14; the primitive's virtual slot 3 (+0x0C, last handle
// released) and slot 1 (+0x04, Destroy).
// Shape: STLport 4.x `_List_base::clear()` verbatim (cur = sentinel->next; while (cur != sentinel) { tmp = cur; cur = next;
// _Destroy(&tmp->_M_data); deallocate(tmp); } sentinel->next = sentinel->prev = sentinel). The node value is CPrimitive
// (Ego_r.pdb, 16 bytes: Type, CEnginePrimitiveHandle, BankIndex) whose implicit destructor inlines the handle's destructor
// exactly as the landed out-of-line copy 0x004C4160 (IsUnique -> slot 3, `Data = 0` = release plus a second zero store,
// the counted pointer's member destructor, then the out-of-line CBaseClass destructor). The node allocator frees
// without a null test (as in the landed RemovePrimitive 0x004BF920).
extern "C" void __cdecl free(void* block);       // 0x00BFEA14

class CEnginePrimitiveHandle;

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};

class CIVCountedPointer {                        // CIVCountedPointer<CEnginePersistentPrimitiveBase>
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    __forceinline CIVCountedPointer& operator=(int)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
};

class CBaseClass {                                 // vtable 0x01231710
public:
    virtual void v000();
    virtual ~CBaseClass();                                 // 0x0099A2E0
};

class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598
public:
    CIVCountedPointer Data;                        // +0x04
    virtual void v001();
    __forceinline ~CEnginePrimitiveHandle()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

namespace NGraphicAppearance {
class CPrimitive {                                 // Ego_r.pdb, 16 bytes
public:
    long                   Type;                   // +0x00 EPrimitiveType
    CEnginePrimitiveHandle PrimitiveHandle;        // +0x04
    long                   BankIndex;              // +0x0c
};
}

namespace std {
template <class T> struct allocator {
    __forceinline void deallocate(void* p, unsigned int) { free(p); }
};

struct _List_node_base {
    _List_node_base* _M_next;                      // +0x00
    _List_node_base* _M_prev;                      // +0x04
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;                                     // +0x08
};

template <class T> __forceinline void _Destroy(T* p) { p->~T(); }

template <class T, class A> class _List_base {
public:
    struct : allocator<_List_node<T> > { _List_node_base* _M_data; } _M_node;   // +0x00 (sentinel)
    void clear();
};

template <class T, class A> void _List_base<T, A>::clear()
{
    _List_node<T>* cur = static_cast<_List_node<T>*>(_M_node._M_data->_M_next);
    while (cur != _M_node._M_data) {
        _List_node<T>* tmp = cur;
        cur = static_cast<_List_node<T>*>(cur->_M_next);
        _Destroy(&tmp->_M_data);
        _M_node.deallocate(tmp, 1);
    }
    _M_node._M_data->_M_next = _M_node._M_data;
    _M_node._M_data->_M_prev = _M_node._M_data;
}

template class _List_base<NGraphicAppearance::CPrimitive, allocator<NGraphicAppearance::CPrimitive> >;
}