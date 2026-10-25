// _STL::vector<CGraphicInfo>::erase(CGraphicInfo* first, CGraphicInfo* last) -- remove the graphic records [first, last): move the tail down over them, destroy the vacated tail records, return first
// Leaf: ?erase@?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A00E60, 109 bytes (+ int3 padding), `__fastcall` (this -> ebp, first and last
//   on the stack), `ret 8`, returns first.
// Identity: CGraphicDataBank::Close (0x009FEB97) calls it as `GraphicList.erase(First, Last)` on the bank's
//   vector<CGraphicInfo> (+0x1E0), next to the CGraphicInfoFrame vector's erase 0x00A00ED0 (the landed
//   ~CGraphicDataBank 0x009FEBC0 models both lists). Element: Ego_r.pdb CGraphicInfo (44 bytes = retail: CGraphicHeader
//   Header +0, ETextureType TextureType +0x1c, CGraphicDataBank* Bank +0x20, long Index +0x24, CGraphicInfoFrame* Frames
//   +0x28) whose destructor only clears Bank and Frames (the landed ~CGraphicDataBank model). Other callers: 0x009FEBE3,
//   0x009FECF7, 0x00A01107, 0x00A017BC.
// Shape: STLport 4.6 `pointer __i = __copy_ptrs(__last, _M_finish, __first, __false_type()); _Destroy(__i, _M_finish);
//   _M_finish = __i; return __first;` with the random-access __copy inlined (signed count by the 0x2E8BA2E9 reciprocal,
//   one `rep movsd` of 11 dwords per implicit assignment) and __destroy_aux inlined (two zero stores per record).
struct CGraphicHeader {                                 // 28 bytes
    unsigned short Width, Height, Depth, FrameWidth, FrameHeight, FrameCount;   // +0x00
    long           PixelFormat;                         // +0x0c
    unsigned char  TransparencyType, MipmapLevels, Flags, Padding;              // +0x10
    unsigned long  FrameDataSize;                       // +0x14
    unsigned long  CompressedMipmapSizes;               // +0x18
};

enum ETextureType { TEXTURE_TYPE_NONE = 0 };
class CGraphicDataBank;
class CGraphicInfoFrame;

class CGraphicInfo {                                    // Ego_r.pdb, 44 bytes
public:
    CGraphicHeader     Header;                          // +0x00
    ETextureType       TextureType;                     // +0x1c
    CGraphicDataBank*  Bank;                            // +0x20
    long               Index;                           // +0x24
    CGraphicInfoFrame* Frames;                          // +0x28
    ~CGraphicInfo() { Bank = 0; Frames = 0; }
};

namespace _STL {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                     const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _InputIter, class _OutputIter>
inline _OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> inline void __fastcall _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
inline void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator>
inline void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp>
class vector {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    _Tp* erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};

template class vector<CGraphicInfo>;
}