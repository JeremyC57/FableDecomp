// _STL::vector::push_back() -- STLport anachronism `push_back(_Tp())` for CGraphicDataBank::GraphicList (vector<CGraphicInfo>): append one default-constructed 44-byte CGraphicInfo, through _M_insert_overflow when the storage is full
// Leaf: ?push_back@?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01650, 160 bytes, thiscall on the vector, plain `ret`.
// Identity: sole caller CGraphicDataBank::NotifyEntryAdded @ 0x009FF590 (landed; `while (index >= GraphicList.size())`
//   calls this with ecx = &GraphicList). The chunk label "CGraphicDataBank_AddEntry" is descriptive.
// Layout: Ego_r.pdb CGraphicInfo (0x2C = retail): Header CGraphicHeader +0x00 (Width/Height/Depth/FrameWidth/FrameHeight
//   ushorts, FrameCount ushort +0x0A, PixelFormat +0x0C, TransparencyType +0x10, MipmapLevels +0x11,
//   IsFlatAnimatedGraphic:1 +0x12, FrameDataSize +0x14, CompressedMipmapSizes[1] +0x18), TextureType +0x1C (left alone by
//   the constructor), Bank +0x20, Index +0x24 (-1), Frames +0x28. CPixelFormat() sets FormatIndex -1.
// Callees: vector<CGraphicInfo>::_M_insert_overflow(pos, x, __false_type, 1, true) @ 0x00A01120 (element loops, no memmove).
// Shape: STLport 4.x `push_back(const _Tp& __x)` inlined into `push_back() { push_back(_Tp()); }`: the implicit copy
//   constructor is the `rep movsd` of 11 dwords behind the placement-new null test.
inline void* __cdecl operator new(unsigned int, void* __p) { return __p; }

namespace _STL {
struct __false_type { __false_type() {} };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

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
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08

    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                  // 0x00A01120
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
    void push_back() { push_back(_Tp()); }
};
}

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

template void _STL::vector<CGraphicInfo>::push_back();