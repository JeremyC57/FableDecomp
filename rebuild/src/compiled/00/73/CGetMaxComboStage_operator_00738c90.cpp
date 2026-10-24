// CGetMaxComboStage::operator()(const std::pair<unsigned long, CAnimationEntry>& entry) -- for_each functor step: when the entry's combo def belongs to the combo being scanned (CurrentID) and its stage is higher than the best so far, keep that stage as MaxStage
// Leaf: ??RCGetMaxComboStage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00738C90, 45 bytes (48 with padding), `ret 4`.
// Label correction: catalog "TCInterface_SetMaxValue" -- the out-of-line copy of the functor body that the landed
//   std::for_each<const pair<unsigned long, CAnimationEntry>*, CGetMaxComboStage> 0x00738F30 inlines (no direct caller
//   survives); Ego_r.pdb CGetMaxComboStage {MaxStage +0, CurrentID +4}.
// Callee: the entry's combo-def getter 0x00738980 (writes the def pointer through its argument; landed model name
//   CAnimationEntry::GetComboDef).
// Layout: CComboDef Stage +0x08, ComboType +0x0C (landed 0x00738F30 model).
// Shape: the def out-pointer local lives in the dead `entry` argument slot (`lea eax,[esp+8]`), re-read after the call.
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}
struct CComboDef {
    unsigned char _pad[8];
    long Stage;                                   // +0x08
    long ComboType;                               // +0x0C
};
class CAnimationEntry {
public:
    long m_words[4];
    void GetComboDef(const CComboDef** out) const;   // 0x00738980
};
class CGetMaxComboStage {
public:
    long MaxStage;                                // +0x00
    long CurrentID;                               // +0x04
    void operator()(const std::pair<unsigned long, CAnimationEntry>& entry);
};

void CGetMaxComboStage::operator()(const std::pair<unsigned long, CAnimationEntry>& entry)
{
    const CComboDef* def;
    entry.second.GetComboDef(&def);
    if (def->ComboType == CurrentID && def->Stage > MaxStage)
        MaxStage = def->Stage;
}