// CAnimFilterAnyGroup::Clone() -- a fresh copy of this stateless filter, or null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008350D0, length 24 bytes. Vtable slot 1 of
// CAnimFilterAnyGroup (0x0127261C: slot 0 destructor, 1 Clone, 2 the filter predicate).
// Callees: operator new @ 0x00BFEA1A (null on failure: the `test eax,eax` is the `new` expression's guard).
// Shape: `return new CAnimFilterAnyGroup;` -- a 4-byte object (vptr only), so the inlined constructor is
// the single vtable store. The filter has no state to copy.
struct CAnimFilterBase {
    virtual void v0();                          // slot 0 (destructor)
    virtual CAnimFilterBase* Clone() const;     // slot 1
    virtual void v2();                          // slot 2 (the predicate)
};

struct CAnimFilterAnyGroup : CAnimFilterBase {  // vtable 0x0127261C, sizeof == 4
    CAnimFilterAnyGroup() { }
    virtual CAnimFilterBase* Clone() const;
};

CAnimFilterBase* CAnimFilterAnyGroup::Clone() const
{
    return new CAnimFilterAnyGroup;
}