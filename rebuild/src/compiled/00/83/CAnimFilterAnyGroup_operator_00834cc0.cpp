// CAnimFilterAnyGroup::operator()(const CAnimationEntry& entry) const -- the any-group filter accepts every animation entry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00834CC0, 5 bytes: `mov al,1 ; ret 4` (+ int3 padding).
// Leaf: ??RCAnimFilterAnyGroup@@UBE_NABVCAnimationEntry@@@Z
// Naming: FableWin.pdb ??RCAnimFilterAnyGroup@@UBE_NABVCAnimationEntry@@@Z (the CAnimFilterBase predicate, as
//   ??RCAnimFilterNoGroup@@ / ??RCAnimFilterOfGroup@@UBE_NABVCAnimationEntry@@@Z in Ego_r.pdb). Slot 0 of the RTTI
//   vtable 0x0127261C (.?AVCAnimFilterAnyGroup@@: slot 1 Clone 0x008350D0, slot 2 the scalar deleting destructor
//   0x00834CD0). Was landed as the gap-scan stand-in "T::Get(int)" (catalog "CAnimFilterAnyGroup::vfunc_0").
//   FINDINGS chunk 402 follow-up (the 0x008350D0 slot comment).
class CAnimationEntry;

class CAnimFilterBase {
public:
    virtual bool operator()(const CAnimationEntry& entry) const;   // slot 0 (the predicate)
    virtual CAnimFilterBase* Clone() const;                         // slot 1
    virtual ~CAnimFilterBase();                                     // slot 2 (scalar deleting destructor)
};

class CAnimFilterAnyGroup : public CAnimFilterBase {
public:
    virtual bool operator()(const CAnimationEntry& entry) const;
    virtual CAnimFilterBase* Clone() const;
};

bool CAnimFilterAnyGroup::operator()(const CAnimationEntry& entry) const
{
    return true;
}
