// CEngineResourceManager::BuildHWStipplePatterns(const CArray<unsigned char>& stipple) -- build the 256 hardware stipple masks: for every fade level, each of the 32 rows of the 32x32 threshold pattern becomes a 32-bit mask (bit 31 - column set where the threshold is <= level + 1), byte-swapped and stored bottom row first
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3C4B0, length 288 bytes (`ret 4`).
// Callees: CArray<unsigned long>::resize(unsigned long, const unsigned long&) @ 0x00464931 (manifest label "push_back" is wrong;
// as the landed CMirroredTextureHighlightsChunk::ReadFromFile 0x00AC5080).
// Layout: the retail HWInverseStipplePatterns CArray<unsigned long> (begin/end/capacity) sits at +0x2ab4; the PDB header
// (engine/CEngineResourceManager.h) puts it at +0x39ec (its shader-bank arrays are wider), so `this` is modelled locally.
// Shape: `resize(0x2000, 0)` (the zero temporary and the array address in the frame); the column loop, written counting UP over
// `stipple[row * 32 + column]` with `1 << (31 - column)`, is unrolled by four (a pointer walk with a down-counting bit stays rolled)
// by VC7.1 (bit index counting down), the level loop is strength-reduced into the output offset (level*32 + 31) * 4;
// the byte swap is the explicit two-half shift/mask form.
template <class T> class CArray { public:
    T* m_begin;
    T* m_end;
    T* m_capacity;
    T* begin() const { return m_begin; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    const T& operator[](unsigned long n) const { return *(begin() + n); }
    void resize(unsigned long n, const T& value);       // 0x00464931 for T = unsigned long
};

class CEngineResourceManager { public:
    unsigned char         _pad[0x2ab4];
    CArray<unsigned long> HWInverseStipplePatterns;      // +0x2ab4
    void BuildHWStipplePatterns(const CArray<unsigned char>& stipple);
};

void CEngineResourceManager::BuildHWStipplePatterns(const CArray<unsigned char>& stipple)
{
    HWInverseStipplePatterns.resize(0x2000, 0);
    for (long level = 0; level < 256; ++level) {
        for (long row = 0; row < 32; ++row) {
            unsigned long bits = 0;
            for (long column = 0; column < 32; ++column) {
                if (stipple[row * 32 + column] <= level + 1)
                    bits |= 1 << (31 - column);
            }
            HWInverseStipplePatterns[level * 32 + 31 - row] =
                (((bits & 0x00ff0000) | (bits >> 16)) >> 8) | (((bits & 0x0000ff00) | (bits << 16)) << 8);
        }
    }
}