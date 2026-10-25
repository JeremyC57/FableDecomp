// GFUpdateFontTextureCache() -- copy every dirty 32x32 glyph of the streaming-font cache memory into the font texture through the palette for the texture's pixel size (16-bit, 8-bit or any other size by memcpy), clear the glyphs' dirty flags and the cache's pending flag
// Grade: DIFFER(473v495) -- same blocks and the x4-unrolled pixel loops, but register/slot choice differs: retail keeps the destination in ebp and the pixel size in edx (unroll counter edi, so the palette index goes through dl), ours swaps them (index through edi, 22 bytes shorter); tried y as a pure counter with ++row, char cache memory, dest before the size local
// Leaf: ?GFUpdateFontTextureCache@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A23E0, 496 bytes (incl. int3 padding). FableWin.pdb
//   `?GFUpdateFontTextureCache@?A0xfe46a155@@YIXXZ`: an anonymous-namespace `__fastcall` with no arguments, plain `ret`
//   (the text TU of ReadTextFileIntoArray 0x009A3460). Caller: the font draw routine 0x009A25D0 (when the pending flag
//   0x013C5A70 is set).
// Callees: CTexture::Lock(level, flags) 0x009FA450 (by-value CLockedRect {Width, Height, Pitch +8, Bits +0xC}, as the
//   landed CTextureRenderer::DoRenderSample 0x00A3B730), CTexture::Unlock(level) 0x009F9DE0.
// Globals (names descriptive): the font texture 0x013C5A80 (FableWin.pdb's anonymous GFontTextureCache), its STLport
//   byte palette vector 0x013C5A88, the cache memory pointer 0x013BDA24 (512 glyphs of 32x32 bytes), the per-glyph dirty
//   flags 0x013BD808, the texture's bytes per pixel 0x0138E070, the pending flag 0x013C5A70.
// Shape: glyph i sits at column i % 32, row (i / 32) * 32 of the texture; the 16- and 8-bit pixel loops (32 constant
//   iterations) are unrolled x4 by VC7.1; the palette is read through the "data or the vector itself when empty" idiom
//   in those two arms and straight from _M_start in the memcpy arm.
#include <string.h>
#pragma intrinsic(memcpy)

class CLockedRect
{
public:
    long           Width;                               // +0x00
    long           Height;                              // +0x04
    long           Pitch;                               // +0x08
    unsigned char* Bits;                                // +0x0C
};
class CTexture
{
public:
    CLockedRect Lock(long level, unsigned long flags);  // 0x009FA450
    void Unlock(long level);                            // 0x009F9DE0
};

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector
{
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    typedef unsigned int size_type;
    size_type size() const { return size_type(_M_finish - _M_start); }
    _Tp& operator[](size_type __n) { return *(_M_start + __n); }
};
} // namespace std

extern CTexture                    GFontTextureCache;          // 0x013C5A80
extern std::vector<unsigned char>  GFontTexturePalette;        // 0x013C5A88
extern char*                       GFontCacheMemory;           // 0x013BDA24
extern bool                        GFontCacheEntryDirty[512];  // 0x013BD808
extern long                        GFontTextureBytesPerPixel;  // 0x0138E070
extern bool                        GFontTextureCacheDirty;     // 0x013C5A70

static inline unsigned char* GetData(std::vector<unsigned char>& v) { return v.size() ? &v[0] : (unsigned char*)&v; }

namespace {
void __fastcall GFUpdateFontTextureCache()
{
    CLockedRect rect = GFontTextureCache.Lock(0, 0);
    for (unsigned long i = 0; i < 512; ++i) {
        if (GFontCacheEntryDirty[i]) {
            const char* src = GFontCacheMemory + i * 1024;
            unsigned long column = i % 32;
            unsigned long row = (i / 32) * 32;
            for (long y = 0; y < 32; ++y, ++row) {
                long bytesPerPixel = GFontTextureBytesPerPixel;
                unsigned char* dest = rect.Bits + row * rect.Pitch + column * bytesPerPixel * 32;
                if (bytesPerPixel == 2) {
                    const unsigned short* palette = (const unsigned short*)GetData(GFontTexturePalette);
                    unsigned short* dest16 = (unsigned short*)dest;
                    for (long x = 0; x < 32; ++x)
                        dest16[x] = palette[(unsigned char)*src++];
                } else if (bytesPerPixel == 1) {
                    const unsigned char* palette = GetData(GFontTexturePalette);
                    for (long x = 0; x < 32; ++x)
                        dest[x] = palette[(unsigned char)*src++];
                } else {
                    for (long x = 0; x < 32; ++x) {
                        memcpy(dest, &GFontTexturePalette[(unsigned char)*src * bytesPerPixel], bytesPerPixel);
                        ++src;
                        dest += bytesPerPixel;
                    }
                }
            }
            GFontCacheEntryDirty[i] = false;
        }
    }
    GFontTextureCache.Unlock(0);
    GFontTextureCacheDirty = false;
}
}

void (__fastcall* g_keepGFUpdateFontTextureCache)() = GFUpdateFontTextureCache;
