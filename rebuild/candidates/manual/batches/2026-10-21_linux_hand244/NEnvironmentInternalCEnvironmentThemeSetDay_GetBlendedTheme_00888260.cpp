// NEnvironmentInternal::CEnvironmentThemeSetDay::GetBlendedTheme(CBlendedEnvironmentTheme& out, float time) const -- find the first daily theme whose hour lies after time*24, then blend the theme before it (wrapping round the day) towards it by the fraction of the gap already elapsed
// Leaf: GetBlendedTheme
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00888260, length 352 bytes (with int3 padding; body 0x153), `ret 8`.
// PDB: ?GetBlendedTheme@CEnvironmentThemeSetDay@NEnvironmentInternal@@QBEXAAVCBlendedEnvironmentTheme@2@M@Z (Ego_r.pdb names).
// Callees: CBlendedEnvironmentTheme::Blend(CBlendedEnvironmentTheme& out, const CBlendedEnvironmentTheme& other, float t)
//   const @ 0x00887B70 (manifest; called on the earlier theme). Float pool: 0x0123822C = 24.0f, 0x0122DC88 = 1/24,
//   0x0122DED8 = 1.0f.
// Layout: as the landed NEnvironmentInternal::CEnvironmentThemeSetDay::Build 0x00888410: the set is a retail 12-byte
//   CArray of 500-byte CBlendedEnvironmentTheme records whose first member is TimeOfDay (hours).
// Shape: the search loop is unrolled by four by the RTM compiler (signed trip guard, unsigned remainder compares); the
//   theme count is re-derived for the modulo (`/ 500` through the 0x10624DD3 multiply); the wrapped gap adds 1.0.
namespace NEnvironmentInternal {

template <class T> struct CArray {
    T* First;
    T* Last;
    T* Capacity;
    long Size() const { return Last - First; }
    const T& operator[](long i) const { return First[i]; }
};

struct CBlendedEnvironmentTheme {                // 500 bytes in retail
    float TimeOfDay;                             // +0x00 hours
    float Colours[19][4];
    float Scalers[30];
    unsigned char TextureSets[0x48];             // three CIDrawEnvironmentTextureSet (two 12-byte CArrays each)
    void Blend(CBlendedEnvironmentTheme& out, const CBlendedEnvironmentTheme& other, float t) const;   // 0x00887B70
};

struct CEnvironmentThemeSetDay {
    CArray<CBlendedEnvironmentTheme> Themes;     // +0x00
    void GetBlendedTheme(CBlendedEnvironmentTheme& out, float time) const;
};

void CEnvironmentThemeSetDay::GetBlendedTheme(CBlendedEnvironmentTheme& out, float time) const
{
    unsigned long count = Themes.Size();
    long i;
    for (i = 0; i < count; ++i) {
        if (Themes[i].TimeOfDay > time * 24.0f)
            break;
    }
    unsigned long next = i % Themes.Size();
    unsigned long prev = (Themes.Size() + next - 1) % Themes.Size();
    float prevTime = Themes[prev].TimeOfDay * (1.0f / 24.0f);
    float nextTime = Themes[next].TimeOfDay * (1.0f / 24.0f);
    if (prevTime > nextTime)
        nextTime += 1.0f;
    Themes[prev].Blend(out, Themes[next], (time - prevTime) / (nextTime - prevTime));
}

}
