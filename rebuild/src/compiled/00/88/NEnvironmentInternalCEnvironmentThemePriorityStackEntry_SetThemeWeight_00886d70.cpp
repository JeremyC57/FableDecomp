// NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::SetThemeWeight(long themeID, float weight, bool rebalance) -- clamp the weight to [0, 1], store and lock it on the matching theme entry or append a new locked entry when it is noticeable, then optionally rebalance
// Leaf: SetThemeWeight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00886D70, length 263 bytes (+ int3 padding), `ret 0Ch`.
// Callees: STLport vector<CThemeWeight>::_M_insert_overflow(pos, x, __false_type, 1, true) @ 0x00889010,
// CEnvironmentThemePriorityStackEntry::RebalanceWeights() @ 0x00886200.
// Float pool: 0x0122DEDC = 0.0f, 0x0122DED8 = 1.0f, 0x01278080 = 1/1024 (the append threshold).
// Layout: the landed GetThemeWeight 0x00886160 model (retail 48 bytes, Ego_r.pdb names): BaseWeight +0,
// BaseWeightLocked +4, Themes +8 (12-byte STLport vector of CThemeWeight {ThemeID, Weight, LockWeight}).
// Shape: the clamp stays on the x87 stack (if / else-if); the search walks a strength-reduced pointer while the count
// is re-derived through the 12-byte reciprocal multiply each pass; the hit arm reaches the entry twice through
// `Themes[i]` (begin re-read); the append builds a CThemeWeight temporary and inlines push_back (null-tested
// placement copy, finish += 12, else the overflow call with the empty tag temporary).
// The hit arm runs its own `if (rebalance) RebalanceWeights(); return;`, which VC7.1 tail-merges into the one call.
// A `goto done` or break leaves the search/append region single-exit, and VC7.1 then shrink-wraps ebx/esi/edi
// (pushed after the clamp and popped before the rebalance). Retail pushes all four registers up front.
// _M_insert_overflow is a same-TU noinline stand-in forwarding to 0x00889010: the empty __false_type tag escapes into
// it by const reference, and VC7.1 homes it over the live `rebalance` slot (+0xc) as retail does, not over the dead weight slot.
struct CThemeWeight {                                   // 12 bytes
    long  ThemeID;                                      // +0
    float Weight;                                       // +4
    bool  LockWeight;                                   // +8
    CThemeWeight(long id, float weight, bool lock) : ThemeID(id), Weight(weight), LockWeight(lock) {}
};
struct __false_type { __false_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) { return _M_start[i]; }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long fill_len, bool atend);
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

void InsertOverflow_00889010(void* v, void* position, const void* x, unsigned long fill_len, bool atend);
#pragma auto_inline(off)
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long fill_len,
                                                        bool atend)   // 0x00889010 stand-in
{
    InsertOverflow_00889010(this, position, &x, fill_len, atend);
}
#pragma auto_inline(on)

namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry { public:
    float                BaseWeight;                    // +0x00
    bool                 BaseWeightLocked;              // +0x04
    vector<CThemeWeight> Themes;                        // +0x08
    unsigned char        _transition[0x30 - 0x14];      // +0x14 transition state
    void SetThemeWeight(long themeID, float weight, bool rebalance);
    void RebalanceWeights();                            // 0x00886200
};

void CEnvironmentThemePriorityStackEntry::SetThemeWeight(long themeID, float weight, bool rebalance)
{
    if (weight < 0.0f)
        weight = 0.0f;
    else if (weight > 1.0f)
        weight = 1.0f;
    for (unsigned long i = 0; i < Themes.size(); ++i) {
        if (Themes[i].ThemeID == themeID) {
            Themes[i].Weight = weight;
            Themes[i].LockWeight = true;
            if (rebalance)
                RebalanceWeights();
            return;
        }
    }
    if (weight > 0.0009765625f)
        Themes.push_back(CThemeWeight(themeID, weight, true));
    if (rebalance)
        RebalanceWeights();
}
}