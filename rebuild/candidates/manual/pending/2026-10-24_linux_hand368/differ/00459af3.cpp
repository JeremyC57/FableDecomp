// CPlayerInventoryDef::CPlayerInventoryDef() -- default constructor: def base, own vtable, three vector<long>(6, 0) slot tables, the vector<bool>(6, false) IsUsable table, InitialSelectedSlot = 0
// Grade: DIFFER(96v96) -- the 4th call's empty allocator temporary at [ebp-2] where retail overlays it on the live `false` temporary at [ebp-1] (empty-temporary frame placement; default-argument form tried)
// Leaf: ??0CPlayerInventoryDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00459AF3, length 96 bytes, plain `ret` (returns this).
// Callees: CParentDefClassBase::CParentDefClassBase @ 0x00430370, vector<long>::vector<int>(int, int, const allocator&)
// @ 0x00453659 (STLport's iterator-range template ctor; with two ints it dispatches to the fill path), vector<bool>::vector
// (unsigned int, const bool&, const allocator&) @ 0x00451583 (this chunk).
// Label: the catalog "CParentDefClassBase_ConstructorWithVectors" is behavioural. The body is reached from the factory's
// `jmp` at 0x00462F8B and stores vtable 0x01235274 = CPlayerInventoryDef (rtti-vtables.tsv). PDB CPlayerInventoryDef: Slot,
// MaxInSlot, InitialAmount (vector<long>), IsUsable (vector<bool>), InitialSelectedSlot; retail puts them at
// +0x3C/+0x48/+0x54/+0x60/+0x6C (0x3C-byte def base, 12-byte STLport vectors), the layout of the landed 0x00462E2D operator=.
// The PDB header (engine/CParentDefClassBase.h) describes the Ego layout, so the retail base is modelled locally.
// Shape: frame pointer with one frame byte = size TU with /Oy off (#pragma optimize("y", off) + ("s", on)). The default
// allocator temporaries (a user-declared empty ctor, so no zero store) and the `false` temporary share the one frame byte;
// `Slot(6, 0)` binds STLport's template range ctor (two ints), `IsUsable(6, false)` the (n, value) ctor.
#pragma optimize("y", off)
#pragma optimize("s", on)

namespace std {
template <class T> struct allocator {
    allocator() { }
};

template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector(unsigned int n, const T& value, const A& a = A());                       // 0x00451583 (vector<bool>)
    template <class InputIter> vector(InputIter first, InputIter last, const A& a = A());   // 0x00453659 (vector<long>, int)
};
}

class CParentDefClassBase {                        // 0x3C bytes in retail
public:
    virtual void v000();
    unsigned long _body[0x0E];
    CParentDefClassBase();                         // 0x00430370
};

class CPlayerInventoryDef : public CParentDefClassBase {   // vtable 0x01235274
public:
    virtual void v000();
    std::vector<long> Slot;                        // +0x3C
    std::vector<long> MaxInSlot;                   // +0x48
    std::vector<long> InitialAmount;               // +0x54
    std::vector<bool> IsUsable;                    // +0x60
    long              InitialSelectedSlot;         // +0x6C
    CPlayerInventoryDef();
};

CPlayerInventoryDef::CPlayerInventoryDef()
    : Slot(6, 0), MaxInSlot(6, 0), InitialAmount(6, 0), IsUsable(6, false), InitialSelectedSlot(0)
{
}
