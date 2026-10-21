// CUserProfileManager::ClampResolution(unsigned long*, long) -- reset an out-of-range display-mode index to 0, then step it down until the mode's width fits the given maximum (-1 = no limit)
// Retail: Fable.exe @ 0x00406D90, 84 bytes (+ int3 padding), `ret 8`. No callees.
// Layout: the retail DisplayModes vector<CDisplayModeDesc> sits at +0x80 (first) / +0x84 (last) -- 8 bytes below
// the Ego_r.pdb offset 0x88 because the retail STL vectors are 12 bytes, not 16. CDisplayModeDesc is 16 bytes,
// Dimensions.X (the width) first.
// Shape: size() is `(last - first) >> 4` (exact pointer division = `sar`), compared unsigned with the index; the
// width test is written twice (an `if` around a `while` that re-tests the same element), the index walks down in a
// local and is stored back only when the loop ran. Elements are reached as `*(begin() + n)` (STLport operator[]).
// The while condition is a __forceinline member with early returns (IsModeWiderThan): that keeps the loop UNROTATED
// (`cmp ; jle ; dec ; sub ; jmp` back) like retail; a plain `DisplayModes[index].Dimensions.X > maxWidth` gets rotated.
struct C2DExtentsI {
    long X;
    long Y;
};

struct CDisplayModeDesc {
    C2DExtentsI Dimensions;   // +0x00
    long BitsPerPixel;        // +0x08
    long RefreshRate;         // +0x0c
};

template <class T>
struct CRetailVector {        // STLport vector: three pointers, 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T* begin() { return m_pFirst; }
    unsigned long size() { return m_pLast - m_pFirst; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

struct CUserProfileManager {
    unsigned char _pad00[0x80];
    CRetailVector<CDisplayModeDesc> DisplayModes;   // +0x80
    void ClampResolution(unsigned long* resolution, long maxWidth);
    __forceinline bool IsModeWiderThan(unsigned long index, long width) { if (DisplayModes[index].Dimensions.X > width) return true; return false; }
};

void CUserProfileManager::ClampResolution(unsigned long* resolution, long maxWidth)
{
    if (*resolution >= DisplayModes.size())
        *resolution = 0;
    if (maxWidth != -1) {
        if (DisplayModes[*resolution].Dimensions.X > maxWidth) {
            unsigned long index = *resolution;
            while (IsModeWiderThan(index, maxWidth))
                --index;
            *resolution = index;
        }
    }
}