// CAnimFilterNoGroup::operator()(const CAnimationEntry& entry) const -- the animation filter predicate: accept only entries without a group name (GroupName def-string index == -1)
// Leaf: ??RCAnimFilterNoGroup@@UBE_NABVCAnimationEntry@@@Z
// Retail: Fable.exe @ 0x0041CD13, 16 bytes, virtual __fastcall `ret 4`: `xor eax,eax ; cmp dword [entry+8],-1 ; sete al`.
// Identity (proven): vtable 0x0122FFDC (RTTI CAnimFilterNoGroup) slot 0; the filter vtables' slot 0 is the predicate
// operator() (as the landed CAnimFilterAnyGroup::Clone 0x008350D0 documents for 0x0127261C), and Ego_r.pdb / FableWin.pdb
// hold ??RCAnimFilterNoGroup@@UBE_NABVCAnimationEntry@@@Z.
// Layout: Ego_r.pdb CAnimationEntry {long BankIndex +0, CDefString AnimName +4, CDefString GroupName +8,
// CIVCountedPointer<CAnimComponentMap> Components +0xC}; CDefString is one long TablePos (-1 = no string).
// engine/CAnimFilterNoGroup.h models the vptr as a data member, so the virtual predicate cannot be declared on it: the
// class is modelled locally with the CAnimFilterBase slot order of the landed 0x008350D0 (NOTES.md "Header debt").
// Shape: a size-optimised TU (#pragma optimize("s", on)): /Os compares the member in memory (`cmp dword [ecx+8],-1`);
// the speed build loads it into edx first.
#pragma optimize("s", on)
class CDefString {
public:
    long TablePos;
};

class CAnimationEntry {
public:
    long BankIndex;                                                 // +0x00
    CDefString AnimName;                                            // +0x04
    CDefString GroupName;                                           // +0x08
};

class CAnimFilterBase {
public:
    virtual bool operator()(const CAnimationEntry& entry) const;   // slot 0 (the predicate)
    virtual CAnimFilterBase* Clone() const;                         // slot 1
    virtual ~CAnimFilterBase();                                     // slot 2 (scalar deleting destructor)
};

class CAnimFilterNoGroup : public CAnimFilterBase {                 // vtable 0x0122FFDC, sizeof == 4
public:
    virtual bool operator()(const CAnimationEntry& entry) const;
    virtual CAnimFilterBase* Clone() const;
};

bool CAnimFilterNoGroup::operator()(const CAnimationEntry& entry) const
{
    return entry.GroupName.TablePos == -1;
}