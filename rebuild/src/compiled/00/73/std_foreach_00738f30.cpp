// std::for_each(const std::pair<unsigned long, CAnimationEntry>* first, const std::pair<unsigned long, CAnimationEntry>* last, CGetMaxComboStage f) -- finds the highest combo stage among the animation entries of one combo type
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x00738F30, 81 bytes, `ret 0Ch`. Callers: CTCCombat::GetComboMaxStage 0x0073393B (functor built
// from -777 and the combo type), CTCCombat::HasAppropriateFollowingCombo, 0x00734529. Callee: the entry's def getter @ 0x00738980
// (writes the def pointer through its argument; manifest "GetTCInterface_LowerBound" is wrong).
// SGI/STLport `for_each` (`for (; first != last; ++first) f(*first); return f;`), compiled /Gr in retail so it is `__fastcall` here: the functor is returned through the hidden pointer (ecx), a pointer `first` rides in edx, the rest on the stack.
// Shape: the def out-pointer local lands in the dead `last` argument slot (`lea eax,[esp+14h]`), the running max in edi.
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
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
    long m_maxStage;                              // +0x00
    long m_comboType;                             // +0x04
    CGetMaxComboStage(long initialStage, long comboType) : m_maxStage(initialStage), m_comboType(comboType) {}
    void operator()(const std::pair<unsigned long, CAnimationEntry>& entry)
    {
        const CComboDef* def;
        entry.second.GetComboDef(&def);
        if (def->ComboType == m_comboType && def->Stage > m_maxStage)
            m_maxStage = def->Stage;
    }
};
template CGetMaxComboStage __fastcall std::for_each(const std::pair<unsigned long, CAnimationEntry>*, const std::pair<unsigned long, CAnimationEntry>*, CGetMaxComboStage);