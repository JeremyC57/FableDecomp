// CAnimFilterAnyGroup::Clone() -- a fresh copy of this stateless filter, or null when the allocation fails
// Leaf: ?Clone@CAnimFilterAnyGroup@@UBEPAVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008350D0, length 24 bytes. Vtable slot 1 of
// CAnimFilterAnyGroup (0x0127261C: slot 0 the filter predicate operator() 0x00834CC0 `mov al,1 ; ret 4`, 1 Clone,
// 2 the scalar deleting destructor 0x00834CD0 `test byte ptr [esp+4],1 ...`).
// Callees: operator new @ 0x00BFEA1A (null on failure: the `test eax,eax` is the `new` expression's guard).
// Shape: `return new CAnimFilterAnyGroup;` -- a 4-byte object (vptr only), so the inlined constructor is
// the single vtable store. The filter has no state to copy.
// Slots: FableWin.pdb ??RCAnimFilterAnyGroup@@UBE_NABVCAnimationEntry@@@Z / ?Clone@... / ??_GCAnimFilterAnyGroup@@UAEPAXI@Z;
//   the virtual destructor is declared after Clone so it takes slot 2, as in retail.
class CAnimationEntry;

class CAnimFilterBase {
public:
    virtual bool operator()(const CAnimationEntry& entry) const;   // slot 0 (the predicate)
    virtual CAnimFilterBase* Clone() const;                         // slot 1
    virtual ~CAnimFilterBase();                                     // slot 2 (scalar deleting destructor)
};

class CAnimFilterAnyGroup : public CAnimFilterBase {  // vtable 0x0127261C, sizeof == 4
public:
    CAnimFilterAnyGroup() { }
    virtual bool operator()(const CAnimationEntry& entry) const;
    virtual CAnimFilterBase* Clone() const;
};

CAnimFilterBase* CAnimFilterAnyGroup::Clone() const
{
    return new CAnimFilterAnyGroup;
}
