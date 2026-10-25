// CEngineInternalPrimitiveParticleGroup::ClearChildIterator(unsigned long) -- mark a primitive id's iterator slot unused
// Leaf: ?ClearChildIterator@CEngineInternalPrimitiveParticleGroup@@QAEXK@Z
// Retail: Fable.exe @ 0x00BBD600, 34 bytes, `ret 4`. PDB ?ClearChildIterator@CEngineInternalPrimitiveParticleGroup@@QAEXK@Z
// Callees: none.
// Label correction: the manifest row "CWorldMap::GetLevelScriptName" is a propagation; the body writes an 8-byte record
// into the vector at +0x4C, which is CEngineInternalPrimitiveParticleGroup::PrimitiveIDToIterator (Ego_r.pdb
// CPrimitiveIterator {bool Valid; list<CEnginePrimitiveHandle>::iterator Iterator;}); its neighbours 0x00BBD1A0 /
// 0x00BBD200 are the landed AddChildPrimitive / ClearAllChildPrimitives of this class. No caller or vtable reference
// survives in retail (xref: 0), so the PDB name set is the evidence for the name: ?ClearChildIterator@CEngineInternalPrimitiveParticleGroup@@QAEXK@Z (Ego_r.pdb).
// Retail layout (0x14 below the PDB from +0x48, as in the landed 0x00BBD200): Primitives +0x48 (4-byte STLport list),
// PrimitiveIDToIterator +0x4C (12-byte vector of 8-byte records); engine/CEngineInternalPrimitiveParticleGroup.h has the
// PDB offsets, so `this` is a local model.
// Shape: `table[id] = CPrimitiveIterator()`: an 8-byte frame temporary whose constructor stores only Valid = false
// (the STLport iterator's default constructor leaves the node unset), copied bitwise as two dwords (the implicit
// assignment is trivial), the iterator dword read from the uninitialised frame word.
class CEnginePrimitiveHandle;

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base() { }
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
};
template <class T> struct _Nonconst_traits { };
template <class T, class Traits> struct _List_iterator : public _List_iterator_base {
    _List_iterator() { }
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) { }
};
}
typedef std::_List_iterator<CEnginePrimitiveHandle, std::_Nonconst_traits<CEnginePrimitiveHandle> > CHandleListIterator;

template <class T> class CArray {               // retail CArray<T>: STLport vector (12 bytes)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned long i) { return *(_M_start + i); }
};

class CEngineInternalPrimitiveParticleGroup {
public:
    class CPrimitiveIterator {
    public:
        bool                Valid;              // +0x00
        CHandleListIterator Iterator;           // +0x04
        CPrimitiveIterator() : Valid(false) { }
        CPrimitiveIterator(CHandleListIterator it) : Valid(true), Iterator(it) { }
    };
    unsigned char               _pad00[0x48];
    void*                       Primitives;     // +0x48 list<CEnginePrimitiveHandle> (4 bytes in retail; PDB +0x48)
    CArray<CPrimitiveIterator>  PrimitiveIDToIterator;  // +0x4C (PDB +0x54)
    void ClearChildIterator(unsigned long id);
};

void CEngineInternalPrimitiveParticleGroup::ClearChildIterator(unsigned long id)
{
    PrimitiveIDToIterator[id] = CPrimitiveIterator();
}