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

// ---- test harness ----
#include <stdio.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)

int main()
{
    static CDisplayModeDesc modes[4] = { { { 640, 480 }, 32, 60 }, { { 800, 600 }, 32, 60 }, { { 1024, 768 }, 32, 60 }, { { 1280, 1024 }, 32, 60 } };
    static CUserProfileManager pm;
    pm.DisplayModes.m_pFirst = modes; pm.DisplayModes.m_pLast = modes + 4; pm.DisplayModes.m_pEnd = modes + 4;
    unsigned long r = 3;
    pm.ClampResolution(&r, -1);
    CHECK(r == 3);
    r = 4;
    pm.ClampResolution(&r, -1);
    CHECK(r == 0);
    r = 3;
    pm.ClampResolution(&r, 1024);
    CHECK(r == 2);
    r = 3;
    pm.ClampResolution(&r, 700);
    CHECK(r == 0);
    r = 1;
    pm.ClampResolution(&r, 1280);
    CHECK(r == 1);
    r = 10;
    pm.ClampResolution(&r, 800);
    CHECK(r == 0);
    if (fails == 0) printf("00406d90_TEST PASS\n");
    return fails;
}