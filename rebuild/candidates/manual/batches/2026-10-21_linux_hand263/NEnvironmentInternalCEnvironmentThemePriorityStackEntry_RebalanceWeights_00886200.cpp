// NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::RebalanceWeights() -- renormalise the stack entry's theme weights so the locked weights keep their share (scaled to 1 when they already exceed it) and the unlocked ones share the rest, then drop near-zero themes, unlock the survivors and give the base the remaining weight
// Leaf: RebalanceWeights
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00886200, length 512 bytes (body 0x1f5 + int3 padding), plain `ret`.
// Layout: PDB CEnvironmentThemePriorityStackEntry: BaseWeight +0, BaseWeightLocked +4, Themes +8 (CArray<CThemeWeight>, the
//   retail 12-byte STLport vector); CThemeWeight {ThemeID +0, Weight +4, LockWeight +8} (12 bytes). No engine header.
// Constants: 0.9990234375f @ 0x01278090 (locked share treated as full), 0.0009765625f @ 0x01278080 (drop threshold),
//   1.0f @ 0x0122DED8, 0.0f @ 0x0122DEDC.
// Shape: the total weight is a memory float, the locked weight stays on the FPU; size() is re-evaluated by every loop
//   test; erase(pos) is forced inline and calls a separate inline copy(first,last,result) (one walking pointer).
struct CThemeWeight {
    long  ThemeID;                                 // +0x00
    float Weight;                                  // +0x04
    bool  LockWeight;                              // +0x08
};

template <class T> inline T* copy_range(T* first, T* last, T* result)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

template <class T> struct CArray {                 // STLport vector
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long index) { return _M_start[index]; }
    __forceinline T* erase(T* position)
    {
        if (position + 1 != _M_finish)
            copy_range(position + 1, _M_finish, position);
        --_M_finish;
        return position;
    }
};

namespace NEnvironmentInternal {
struct CEnvironmentThemePriorityStackEntry {
    float                BaseWeight;               // +0x00
    bool                 BaseWeightLocked;         // +0x04
    CArray<CThemeWeight> Themes;                   // +0x08
    void RebalanceWeights();
};
}

void NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::RebalanceWeights()
{
    float totalWeight = BaseWeight;
    float lockedWeight = BaseWeightLocked ? BaseWeight : 0.0f;
    unsigned long i;
    for (i = 0; i < Themes.size(); ++i) {
        totalWeight += Themes[i].Weight;
        if (Themes[i].LockWeight)
            lockedWeight += Themes[i].Weight;
    }
    if (lockedWeight > 0.9990234375f) {
        for (i = 0; i < Themes.size(); ++i) {
            if (Themes[i].LockWeight)
                Themes[i].Weight /= lockedWeight;
            else
                Themes[i].Weight = 0.0f;
        }
    } else {
        for (i = 0; i < Themes.size(); ++i) {
            if (!Themes[i].LockWeight)
                Themes[i].Weight *= (1.0f - lockedWeight) / (totalWeight - lockedWeight);
        }
    }
    BaseWeight = 1.0f;
    for (i = 0; i < Themes.size();) {
        if (Themes[i].Weight < 0.0009765625f) {
            Themes.erase(&Themes[i]);
        } else {
            Themes[i].LockWeight = false;
            BaseWeight -= Themes[i].Weight;
            ++i;
        }
    }
    BaseWeightLocked = false;
}
