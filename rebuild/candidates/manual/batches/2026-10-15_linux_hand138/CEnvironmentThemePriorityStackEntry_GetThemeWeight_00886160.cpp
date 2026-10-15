// CEnvironmentThemePriorityStackEntry::GetThemeWeight(long themeID) -- the base weight for a negative theme ID, else the weight of the matching theme entry, else 0
// Retail: Fable.exe (TLC, VC7.1) @ 0x00886160, 111 bytes (+ int3 padding).
// Callees: none. Float pool: 0x0122DEDC = 0.0f.
// Layout (Ego_r.pdb NEnvironmentInternal::CEnvironmentThemePriorityStackEntry, retail 48 bytes with a 12-byte CArray):
//   BaseWeight +0, BaseWeightLocked +4, Themes (CArray<CThemeWeight>: begin +8, end +0xc) of 12-byte
//   CThemeWeight { long ThemeID; float Weight; bool LockWeight; }.
// Shape: the theme count `(end - begin) / 12` is re-derived each pass (unsigned compare) while the compare walks a
//   strength-reduced pointer; the found arm returns through the begin pointer captured at loop entry
//   (`fld [ebx+ecx*4+4]`, ecx = 3*i): an indexed `Themes[i]` whose base VC keeps from the first load.
struct CThemeWeight {                            // 12 bytes
    long  ThemeID;                               // +0
    float Weight;                                // +4
    bool  LockWeight;                            // +8
};

template <class T> struct CArray {               // retail CArray: begin, end, capacity
    T* m_pBegin;
    T* m_pEnd;
    T* m_pCapacity;
    unsigned long Size() const { return m_pEnd - m_pBegin; }
    const T& operator[](unsigned long i) const { return m_pBegin[i]; }
};

struct CEnvironmentThemePriorityStackEntry {
    float                BaseWeight;             // +0x00
    bool                 BaseWeightLocked;       // +0x04
    CArray<CThemeWeight> Themes;                 // +0x08
    unsigned char        _transition[0x30 - 0x14];
    float GetThemeWeight(long themeID) const;
};

float CEnvironmentThemePriorityStackEntry::GetThemeWeight(long themeID) const
{
    if (themeID < 0)
        return BaseWeight;
    for (unsigned long i = 0; i < Themes.Size(); ++i)
        if (Themes[i].ThemeID == themeID)
            return Themes[i].Weight;
    return 0.0f;
}
