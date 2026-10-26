// std::_Rb_tree<void*, pair<void* const, NGraphicAppearance::CTCGraphicAppearance::CMeshEffectPriorityEntry>, ...>::_M_create_node(const value_type& x) -- STLport: malloc a 0x20-byte map node and copy-construct the key and mesh-effect entry into it (AddRef on the counted effect)
// Leaf: ?_M_create_node@?$_Rb_tree@PAXU?$pair@QAXVCMeshEffectPriorityEntry@CTCGraphicAppearance@NGraphicAppearance@@@std@@U?$_Select1st@U?$pair@QAXVCMeshEffectPriorityEntry@CTCGraphicAppearance@NGraphicAppearance@@@std@@@2@U?$less@PAX@2@U?$allocator@U?$pair@QAXVCMeshEffectPriorityEntry@CTCGraphicAppearance@NGraphicAppearance@@@std@@@2@@std@@IAEPAU?$_Rb_tree_node@U?$pair@QAXVCMeshEffectPriorityEntry@CTCGraphicAppearance@NGraphicAppearance@@@std@@@2@ABU?$pair@QAXVCMeshEffectPriorityEntry@CTCGraphicAppearance@NGraphicAppearance@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C4A20, length 73 bytes, `ret 4` (thiscall: the value reference).
// Identity: the MeshEffects map (CTCGraphicAppearance +0x0c, map<void*, CMeshEffectPriorityEntry>): its single caller is the
// right-hand site of _M_insert 0x004C4F70 (chunk 372), reached from insert_unique 0x004C5040 / 0x004C5480 and the landed
// SetPMeshEffect 0x004C0240's operator[] 0x004C5B90. The catalog "ListNode_AllocAbilityData" is a propagated label.
// Callees: malloc @ 0x00BFEA0E.
// Shape: STLport 4.x `_M_create_node`: `tmp = allocate(1); _Construct(&tmp->_M_value_field, x); return tmp;` (no EH, so
// no _STLP_TRY). The pair copy is memberwise: the key dword, the CIVCountedPointer copy ctor (`if (Object) AddRef()`,
// RefCount at +4), Priority, then the three bools byte by byte (the entry's implicit copy ctor, made non-trivial by
// the counted pointer).
extern "C" void* __cdecl malloc(unsigned int size);          // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CEngineMeshEffect {
public:
    virtual void v000();
    virtual void Destroy();                                  // slot 1 (+0x04)
    long RefCount;                                           // +0x04
    void AddRef() { ++RefCount; }
};

template <class T> class CIVCountedPointer {
public:
    T* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
};

enum EAppearanceMeshEffectPriority { MESH_EFFECT_PRIORITY_DEFAULT };

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    class CMeshEffectPriorityEntry {                         // Ego_r.pdb, 12 bytes
    public:
        CIVCountedPointer<CEngineMeshEffect> PEffect;        // +0x0
        EAppearanceMeshEffectPriority        Priority;       // +0x4
        bool                                 EffectsCarriedItems;   // +0x8
        bool                                 EffectsWeapons;        // +0x9
        bool                                 InheritedFromCarrier;  // +0xa
    };
};
}

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

struct _Rb_tree_node_base {
    bool                _M_color;                            // +0x00
    _Rb_tree_node_base* _M_parent;                           // +0x04
    _Rb_tree_node_base* _M_left;                             // +0x08
    _Rb_tree_node_base* _M_right;                            // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                        // +0x10
};

template <class T> struct allocator {};
template <class T> struct less {};
template <class P> struct _Select1st {};

template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node<V>* _Link_type;
    struct : allocator<V> { _Rb_tree_node_base* _M_data; } _M_header;   // +0x00
    unsigned int _M_node_count;                                         // +0x04
    C            _M_key_compare;                                        // +0x08

protected:
    _Link_type _M_create_node(const V& x);
};

template <class K, class V, class KoV, class C, class A>
typename _Rb_tree<K, V, KoV, C, A>::_Link_type _Rb_tree<K, V, KoV, C, A>::_M_create_node(const V& x)
{
    _Link_type tmp = static_cast<_Link_type>(malloc(sizeof(_Rb_tree_node<V>)));
    _Construct(&tmp->_M_value_field, x);
    return tmp;
}

typedef pair<void* const, NGraphicAppearance::CTCGraphicAppearance::CMeshEffectPriorityEntry> _MeshEffectValue;
template class _Rb_tree<void*, _MeshEffectValue, _Select1st<_MeshEffectValue>, less<void*>, allocator<_MeshEffectValue> >;
}