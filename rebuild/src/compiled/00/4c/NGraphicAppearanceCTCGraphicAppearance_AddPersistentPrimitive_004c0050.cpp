// NGraphicAppearance::CTCGraphicAppearance::AddPersistentPrimitive(const NGraphicAppearance::CPrimitive& primitive) -- push a copy of the primitive record onto the Primitives list and return the stored copy
// Leaf: AddPersistentPrimitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C0050, length 85 bytes, `ret 4` (thiscall: primitive).
// Callees: malloc @ 0x00BFEA0E (STLport list node, 0x18 bytes); CEnginePrimitiveHandle vtable 0x0122F598.
// Label: the catalog "CArray_InsertAppearanceNode" is behavioural. Ego_r.pdb names
// `?AddPersistentPrimitive@CTCGraphicAppearance@NGraphicAppearance@@QAEPAVCPrimitive@2@ABV32@@Z`; its callers are the
// OnAppearanceDraw / DrawLight / DrawFlare bodies (0x0064150D, 0x007076A1, ...), which pass a CPrimitive temporary
// {Type, CEnginePrimitiveHandle, BankIndex} and store the returned pointer.
// Layout: PDB CPrimitive is 16 bytes (Type +0, PrimitiveHandle +4 = vptr + CIVCountedPointer, BankIndex +0xC). The retail
// CTCGraphicAppearance has 12-byte STLport maps (PDB 16), so Primitives (PDB +0x40) sits at +0x38 as the 4-byte list.
// Shape: `Primitives.push_back(primitive); return &Primitives.back();` -- push_back = insert(end(), x) with the node
// built inline (placement copy: Type, the handle's vtable, the AddRef'ed Data, BankIndex); back() re-reads the sentinel.
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    long RefCount;                                // +0x04
};

class CIVCountedPointer {                         // CIVCountedPointer<CEnginePersistentPrimitiveBase>
public:
    CEnginePersistentPrimitiveBase* Object;
    __forceinline CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
};

class CBaseClass {                                // vtable 0x01231710
public:
    virtual void v000();
};

class CEnginePrimitiveHandle : public CBaseClass {   // vtable 0x0122F598
public:
    CIVCountedPointer Data;                       // +0x04
    virtual void v001();
};

namespace NGraphicAppearance {
class CPrimitive {                                // PDB 16 bytes
public:
    long                   Type;                  // +0x00 EPrimitiveType
    CEnginePrimitiveHandle PrimitiveHandle;       // +0x04
    long                   BankIndex;             // +0x0C
};
}

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> struct allocator {
    __forceinline T* allocate(unsigned int n) { return static_cast<T*>(malloc(n * sizeof(T))); }
};
template <class T> struct _List_iterator {
    _List_node_base* _M_node;
    __forceinline _List_iterator(_List_node_base* x) : _M_node(x) {}
    __forceinline _List_iterator(const _List_iterator& x) : _M_node(x._M_node) {}
};
template <class T> class list {
public:
    typedef _List_node<T> _Node;
    typedef _List_iterator<T> iterator;
    struct : allocator<_Node> { _List_node_base* _M_data; } _M_node;   // _STLP_alloc_proxy: the sentinel
    __forceinline iterator end() { return iterator(_M_node._M_data); }
    __forceinline T& back() { return static_cast<_Node*>(_M_node._M_data->_M_prev)->_M_data; }
    __forceinline _Node* _M_create_node(const T& x)
    {
        _Node* p = _M_node.allocate(1);
        _Construct(&p->_M_data, x);
        return p;
    }
    __forceinline iterator insert(iterator position, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
    __forceinline void push_back(const T& x) { insert(end(), x); }
};
}

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    virtual void v000();
    unsigned char _pad04[0x34];                   // CTCBase + MeshEffects / TintColours maps, PTCAnimation, focal offset, Scale
    std::list<CPrimitive> Primitives;             // +0x38
    CPrimitive* AddPersistentPrimitive(const CPrimitive& primitive);
};

CPrimitive* CTCGraphicAppearance::AddPersistentPrimitive(const CPrimitive& primitive)
{
    Primitives.push_back(primitive);
    return &Primitives.back();
}
}