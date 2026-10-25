// CEngineInternalPrimitiveParticleGroup::SetChildIterator(unsigned long, list<CEnginePrimitiveHandle>::iterator) -- record a primitive id's list position
// Leaf: ?SetChildIterator@CEngineInternalPrimitiveParticleGroup@@QAEXKU?$_List_iterator@
// Retail: Fable.exe @ 0x00BBD630, 38 bytes, `ret 8`. PDB (Ego_d) ?SetChildIterator@CEngineInternalPrimitiveParticleGroup@@QAEXKV?$_List_iterator@...@Z
// (the Dinkumware iterator of the donor build; retail passes the 4-byte STLport list iterator by value).
// Callees: none.
// Label correction: the manifest row "CWorldMap::GetLevelScriptName" is a propagation; the body writes an 8-byte record
// into the vector at +0x4C, which is CEngineInternalPrimitiveParticleGroup::PrimitiveIDToIterator (Ego_r.pdb
// CPrimitiveIterator {bool Valid; list<CEnginePrimitiveHandle>::iterator Iterator;}); its neighbours 0x00BBD1A0 /
// 0x00BBD200 are the landed AddChildPrimitive / ClearAllChildPrimitives of this class. No caller or vtable reference
// survives in retail (xref: 0), so the PDB name set is the evidence for the name: ?SetChildIterator@CEngineInternalPrimitiveParticleGroup@@QAEXKV?$_List_iterator@... (Ego_d.pdb).
// Retail layout (0x14 below the PDB from +0x48, as in the landed 0x00BBD200): Primitives +0x48 (4-byte STLport list),
// PrimitiveIDToIterator +0x4C (12-byte vector of 8-byte records); engine/CEngineInternalPrimitiveParticleGroup.h has the
// PDB offsets, so `this` is a local model.
// Shape: `table[id] = CPrimitiveIterator(it)`: the temporary's Valid = true byte goes through the frame word (the first
// dword of the bitwise copy, `mov byte ptr [esp+4],1 ; mov esi,[esp+4]`), its iterator dword is forwarded from the
// argument register.
class CEnginePrimitiveHandle;

namespace _STL {
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
typedef _STL::_List_iterator<CEnginePrimitiveHandle, _STL::_Nonconst_traits<CEnginePrimitiveHandle> > CHandleListIterator;

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
    void SetChildIterator(unsigned long id, CHandleListIterator it);
};

void CEngineInternalPrimitiveParticleGroup::SetChildIterator(unsigned long id, CHandleListIterator it)
{
    PrimitiveIDToIterator[id] = CPrimitiveIterator(it);
}