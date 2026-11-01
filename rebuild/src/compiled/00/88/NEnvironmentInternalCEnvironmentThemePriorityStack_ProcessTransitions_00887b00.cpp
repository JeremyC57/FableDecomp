// NEnvironmentInternal::CEnvironmentThemePriorityStack::ProcessTransitions() -- advance the theme transitions of every priority-stack entry, then give the bottom entry a locked zero base weight and rebalance it
// Leaf: ?ProcessTransitions@CEnvironmentThemePriorityStack@NEnvironmentInternal@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00887B00, 99 bytes, tail jump to RebalanceWeights.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?ProcessTransitions@CEnvironmentThemePriorityStack@NEnvironmentInternal@@QAEXXZ;
//   the sole caller 0x006B4045 walks CEnvironment::PriorityStackChannels (+0x48, 12-byte stride) calling it per stack; the body
//   calls CEnvironmentThemePriorityStackEntry::ProcessTransitions 0x00887A30 per 0x30-byte entry. The catalog label is right.
// Callees: CEnvironmentThemePriorityStackEntry::ProcessTransitions 0x00887A30, RebalanceWeights 0x00886200 (tail jump).
// Layout: Ego_r.pdb CEnvironmentThemePriorityStack Ents (CArray<CEnvironmentThemePriorityStackEntry>, a 12-byte STLport vector in
//   retail) +0x00; the entry (retail 0x30) BaseWeight +0x00, BaseWeightLocked +0x04, Themes +0x08 .. ThemeTransitionDuration +0x28.
// Shape: `for (i < Ents.size()) Ents[i].ProcessTransitions(); Ents[0].SetDefaultThemeWeight(0.0f, true);` -- size() re-read every
//   pass (signed /0x30, unsigned compare); SetDefaultThemeWeight (out of line at 0x00886430, landed) is a plain member defined
//   above, which VC7.1 expands with the clamp folded to `BaseWeight = 0; BaseWeightLocked = true; jmp RebalanceWeights`.
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp& reference;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    reference operator[](size_type __n) { return *(this->_M_start + __n); }
};
} // namespace std
template <class T> class CArray : public std::vector<T> { };

namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry {                 // retail 0x30 bytes (Ego_r 0x38 less the 16 -> 12 byte CArray)
public:
    struct CThemeWeight {
        long  ThemeID;                                      // +0x00
        float Weight;                                       // +0x04
        bool  LockWeight;                                   // +0x08
    };
    void RebalanceWeights();                                // 0x00886200
    void SetDefaultThemeWeight(float weight, bool rebalance);   // 0x00886430
    void ProcessTransitions();                              // 0x00887A30
    float                   BaseWeight;                     // +0x00
    bool                    BaseWeightLocked;               // +0x04
    CArray<CThemeWeight>    Themes;                         // +0x08
    long                    ThemeTransitionTargetID;        // +0x14
    float                   ThemeTransitionInitialWeight;   // +0x18
    bool                    ThemeTransitionActive;          // +0x1C
    double                  ThemeTransitionStartTime;       // +0x20
    float                   ThemeTransitionDuration;        // +0x28
};

void CEnvironmentThemePriorityStackEntry::SetDefaultThemeWeight(float weight, bool rebalance)
{
    if (weight < 0.0f)
        weight = 0.0f;
    else if (weight > 1.0f)
        weight = 1.0f;
    BaseWeight = weight;
    BaseWeightLocked = true;
    if (rebalance)
        RebalanceWeights();
}

class CEnvironmentThemePriorityStack {
public:
    void ProcessTransitions();
    CArray<CEnvironmentThemePriorityStackEntry> Ents;       // +0x00
};

void CEnvironmentThemePriorityStack::ProcessTransitions()
{
    for (unsigned long i = 0; i < Ents.size(); ++i)
        Ents[i].ProcessTransitions();
    Ents[0].SetDefaultThemeWeight(0.0f, true);
}
}