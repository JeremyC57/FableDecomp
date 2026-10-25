// _STL::vector::resize(unsigned int new_size) -- STLport vector<CGraphicInfo>::resize(n): resize the graphic list to n entries, filling with a default-constructed CGraphicInfo (one frame, no pixel format, no bank, index -1)
// Leaf: ?resize@?$vector@VCGraphicInfo@@V?$allocator@VCGraphicInfo@@@_STL@@@_STL@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A01820, 107 bytes, thiscall on the vector, `ret 4`.
// Identity: sole caller CGraphicDataBank::NotifyBankSize 0x009FF8B0 (landed; the call on `this + 0x1E0`, the retail
//   GraphicList CArray<CGraphicInfo>); same element model as the landed push_back() 0x00A01650.
// Callees: vector<CGraphicInfo>::resize(n, const CGraphicInfo&) 0x00A01790 (out of line).
// Layout: Ego_r.pdb CGraphicInfo (0x2C): Header CGraphicHeader +0 (Width/Height/Depth/FrameWidth/FrameHeight +0..+8,
//   FrameCount +0xA, PixelFormat +0xC, TransparencyType +0x10, MipmapLevels +0x11, IsFlatAnimatedGraphic:1 +0x12,
//   FrameDataSize +0x14, CompressedMipmapSizes[1] +0x18), TextureType +0x1C (left alone), Bank +0x20, Index +0x24,
//   Frames +0x28; CPixelFormat() sets FormatIndex -1.
// Shape: STLport 4.x `void resize(size_type n) { resize(n, _Tp()); }` with the two-argument resize out of line: the
//   44-byte temporary is built in the frame -- one zero register for the ushort/byte/dword zeroes, the bit-field clear
//   as a read-modify-write of byte +0x12 loaded first (`mov dl,[esp+12h] ; and dl,0FEh`), the FrameCount 1 and the two
//   -1s as immediates after the pushes. The element has no destructor.
// Label correction: catalogued as `vector<EWeaponClass>::_Umove<EWeaponClass*>` (propagated); the body is
//   vector<CGraphicInfo>::resize(n).
class CGraphicDataBank;
class CGraphicInfoFrame;

class CPixelFormat {
public:
    long FormatIndex;
    CPixelFormat() : FormatIndex(-1) {}
};

class CGraphicHeader {                              // 0x1C bytes
public:
    unsigned short Width;                           // +0x00
    unsigned short Height;                          // +0x02
    unsigned short Depth;                           // +0x04
    unsigned short FrameWidth;                      // +0x06
    unsigned short FrameHeight;                     // +0x08
    unsigned short FrameCount;                      // +0x0A
    CPixelFormat PixelFormat;                       // +0x0C
    unsigned char TransparencyType;                 // +0x10
    unsigned char MipmapLevels;                     // +0x11
    unsigned char IsFlatAnimatedGraphic : 1;        // +0x12
    unsigned long FrameDataSize;                    // +0x14
    unsigned long CompressedMipmapSizes[1];         // +0x18
    CGraphicHeader()
        : Width(0), Height(0), Depth(0), FrameWidth(0), FrameHeight(0), FrameCount(1), TransparencyType(0),
          MipmapLevels(0), IsFlatAnimatedGraphic(false), FrameDataSize(0)
    {
        CompressedMipmapSizes[0] = 0;
    }
};

enum ETextureType { TEXTURE_TYPE_NONE };

class CGraphicInfo {                                // 0x2C bytes
public:
    CGraphicHeader Header;                          // +0x00
    ETextureType TextureType;                       // +0x1C
    CGraphicDataBank* Bank;                         // +0x20
    long Index;                                     // +0x24
    CGraphicInfoFrame* Frames;                      // +0x28
    CGraphicInfo() : Bank(0), Index(-1), Frames(0) {}
};

namespace _STL {
template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08

    void resize(size_type __new_size, const _Tp& __x);              // 0x00A01790
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}

template void _STL::vector<CGraphicInfo>::resize(unsigned int);