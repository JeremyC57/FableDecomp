// Grade: DIFFER(727v727) -- one register pair left in the blended-array teardown (as 0x008884D0): retail loads First into esi and copies it to ecx for the __fastcall range destroy, VC7.1 here loads ecx and copies to esi; same-TU stand-ins fixed the weights caching
// CEnvironmentThemePriorityStackEntry::BlendInLevel(CBlendedEnvironmentTheme& out, const CBlendedEnvironmentTheme& base, float time, const CArray<CEnvironmentThemeSetDay>& sets) const -- a base weight near 1 copies base; near 0 with one theme blends that day set straight into out; otherwise blend base (when weighted) and every theme's day set into a local list with their weights and make out their weighted blend
// Leaf: BlendInLevel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008886E0, length 0x2D7 bytes (+ int3 padding), `ret 0x10`, no EH frame.
// Callees: CEnvironmentThemeSetDay::GetBlendedTheme 0x00888260; CBlendedEnvironmentTheme ctor 0x008864A0, dtor 0x006B5D80
//   (manifest "CEngineWaterBackgroundSubPatch::OnPostRelocation" is propagated); CBlendedEnvironmentTheme::SetAsWeightedBlendOfThemes
//   0x008873C0; vector<CBlendedEnvironmentTheme>::reserve 0x00889CB0 / resize 0x0088A520; vector<float>::reserve 0x00724E30 and
//   the scalar InsertGrow 0x004408A0; the out-of-line range destroy 0x006B67A0 (`__fastcall`, first/last + tag); CRT free 0x00BFEA14.
// Layout: retail Themes is a 12-byte STLport vector at +8 (the PDB's 16-byte CArray is the debug view); CThemeWeight is 12 bytes
//   {ThemeID, Weight, LockWeight}; a CBlendedEnvironmentTheme is 500 bytes in retail.
#include <new>
extern "C" void __cdecl free(void*);
extern "C" void* __cdecl malloc(unsigned int);

struct SScalarTag { SScalarTag() { } };
struct SClassTag { SClassTag() { } };

namespace NEnvironmentInternal {

struct CBlendedEnvironmentTheme {
    unsigned char _data[500];
    CBlendedEnvironmentTheme();                                  // 0x008864A0
    CBlendedEnvironmentTheme& operator=(const CBlendedEnvironmentTheme& other);   // 0x006B5DF0
    ~CBlendedEnvironmentTheme();                                 // 0x006B5D80
};

void __fastcall DestroyRange(CBlendedEnvironmentTheme* first, CBlendedEnvironmentTheme* last, const SClassTag& tag);   // 0x006B67A0

struct CBlendedThemeArray {
    CBlendedEnvironmentTheme* First;
    CBlendedEnvironmentTheme* Last;
    CBlendedEnvironmentTheme* Capacity;
    CBlendedThemeArray() : First(0), Last(0), Capacity(0) { }
    ~CBlendedThemeArray() { CBlendedEnvironmentTheme* first = First; DestroyRange(first, Last, SClassTag()); if (first) free(first); }
    long Size() const { return Last - First; }
    void reserve(unsigned long n);                               // 0x00889CB0
    void resize(unsigned long n, const CBlendedEnvironmentTheme& value);   // 0x0088A520
    void push_back(const CBlendedEnvironmentTheme& value);       // 0x0088A350
};

struct CFloatArray {
    float* First;
    float* Last;
    float* Capacity;
    CFloatArray() : First(0), Last(0), Capacity(0) { }
    ~CFloatArray() { if (First) free(First); }
    void reserve(unsigned long n);                               // 0x00724E30
    void InsertGrow(float* where, const float& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x004408A0
    void push_back(const float& value)
    {
        if (Last != Capacity) {
            new (Last) float(value);
            ++Last;
        }
        else
            InsertGrow(Last, value, SScalarTag(), 1, true);
    }
};

#pragma auto_inline(off)
__declspec(noinline) void CFloatArray::InsertGrow(float* where, const float& value, const SScalarTag&, unsigned long count, bool noTail)
{
    float* first = First;
    float* last = Last;
    unsigned long size = last - first;
    unsigned long len = size + (size > count ? size : count);
    float* buffer = (float*)malloc(len * sizeof(float));
    float* cur = buffer;
    for (float* p = first; p != where; ++p, ++cur)
        *cur = *p;
    for (unsigned long i = 0; i < count; ++i, ++cur)
        *cur = value;
    if (!noTail)
        for (float* p = where; p != last; ++p, ++cur)
            *cur = *p;
    if (first)
        free(first);
    First = buffer;
    Last = cur;
    Capacity = buffer + len;
}
__declspec(noinline) void CFloatArray::reserve(unsigned long n)
{
    float* first = First;
    float* last = Last;
    if ((unsigned long)(Capacity - first) < n) {
        float* buffer = (float*)malloc(n * sizeof(float));
        float* cur = buffer;
        for (float* p = first; p != last; ++p, ++cur)
            *cur = *p;
        if (first)
            free(first);
        First = buffer;
        Last = cur;
        Capacity = buffer + n;
    }
}
#pragma auto_inline(on)

struct CBlendedEnvironmentThemeOut {
};

struct CEnvironmentThemeSetDay {
    unsigned char _data[12];
    void GetBlendedTheme(CBlendedEnvironmentTheme& out, float time) const;   // 0x00888260
};

struct CEnvironmentThemeSetDayArray {
    CEnvironmentThemeSetDay* First;
    CEnvironmentThemeSetDay* Last;
    CEnvironmentThemeSetDay* Capacity;
    const CEnvironmentThemeSetDay& operator[](long i) const { return First[i]; }
};

}
using namespace NEnvironmentInternal;

struct CBlendedEnvironmentThemeTarget : CBlendedEnvironmentTheme {
};

struct CThemeWeight {
    long  ThemeID;                                               // +0x00
    float Weight;                                                // +0x04
    bool  LockWeight;                                            // +0x08
};

struct CThemeWeightArray {
    CThemeWeight* First;
    CThemeWeight* Last;
    CThemeWeight* Capacity;
    long Size() const { return Last - First; }
    const CThemeWeight& operator[](long i) const { return First[i]; }
};

void SetAsWeightedBlendOfThemes(CBlendedEnvironmentTheme& out, const CBlendedThemeArray& themes, const CFloatArray& weights);

struct CEnvironmentThemePriorityStackEntry {
    float             BaseWeight;                                // +0x00
    bool              BaseWeightLocked;                          // +0x04
    CThemeWeightArray Themes;                                    // +0x08
    void BlendInLevel(CBlendedEnvironmentTheme& out, const CBlendedEnvironmentTheme& base, float time, const CEnvironmentThemeSetDayArray& sets) const;
};

namespace NEnvironmentInternal {
struct CBlendedEnvironmentThemeX;
}

struct CBlended : CBlendedEnvironmentTheme {
    void SetAsWeightedBlendOfThemes(const CBlendedThemeArray& themes, const CFloatArray& weights);   // 0x008873C0
};


#pragma auto_inline(off)
__declspec(noinline) void CBlended::SetAsWeightedBlendOfThemes(const CBlendedThemeArray& themes, const CFloatArray& weights)
{
    float total = 0.0f;
    for (float* w = weights.First; w != weights.Last; ++w)
        total += *w;
    _data[0] = (unsigned char)(themes.Size() + (long)total);
}
#pragma auto_inline(on)

void CEnvironmentThemePriorityStackEntry::BlendInLevel(CBlendedEnvironmentTheme& out, const CBlendedEnvironmentTheme& base, float time, const CEnvironmentThemeSetDayArray& sets) const
{
    if (BaseWeight > 0.9990234375f) {
        out = base;
        return;
    }
    if (BaseWeight < 0.0009765625f && Themes.Size() == 1) {
        sets[Themes[0].ThemeID].GetBlendedTheme(out, time);
        return;
    }
    CBlendedThemeArray blended;
    CFloatArray weights;
    if (BaseWeight > 0.0009765625f) {
        blended.reserve(Themes.Size() + 1);
        weights.reserve(Themes.Size() + 1);
        blended.push_back(base);
        weights.push_back(BaseWeight);
    }
    else {
        blended.reserve(Themes.Size());
        weights.reserve(Themes.Size());
    }
    for (unsigned long i = 0; i < Themes.Size(); ++i) {
        long n = blended.Size();
        blended.resize(n + 1, CBlendedEnvironmentTheme());
        sets[Themes[i].ThemeID].GetBlendedTheme(blended.First[n], time);
        weights.push_back(Themes[i].Weight);
    }
    static_cast<CBlended&>(out).SetAsWeightedBlendOfThemes(blended, weights);
}
