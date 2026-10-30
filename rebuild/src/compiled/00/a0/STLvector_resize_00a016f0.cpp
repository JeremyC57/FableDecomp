// std::vector::resize(unsigned int) -- STLport `resize(n) { resize(n, _Tp()); }` for a vector<CGraphicInfoFrame>: erase the tail past n, or fill-insert copies of a default CGraphicInfoFrame up to n, then destroy the temporary
// Leaf: ?resize@?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A016F0, 160 bytes, thiscall on the vector, `ret 4`.
// Identity: sole caller CGraphicInfo::InitFrames @ 0x009FF440 (landed; the frame vector at 0x009FF497). The chunk label
//   "Vector_Resize" is right in kind.
// Callees: CBaseClass::CBaseClass() @ 0x0099A2D0, vector<CGraphicInfoFrame>::erase(first, last) @ 0x00A00ED0,
//   vector<CGraphicInfoFrame>::_M_fill_insert(pos, n, x) @ 0x00A00F50, CBaseClass::~CBaseClass() @ 0x0099A2E0; the frame
//   data's virtual slot 1 when the temporary's (null) Data reference count would reach zero.
// Element: CGraphicInfoFrame (Ego_r.pdb, 0x10 bytes; vtable 0x0129C878): FrameNum +4, SmallMipmapLevel +6,
//   Data CIVCountedPointer<CFrameData> +8 (nulled by the default constructor), GraphicInfo +0xC.
// Shape: STLport 4.x `resize(__new_size, __x)` inlined: `if (__new_size < size()) erase(begin() + __new_size, end());
//   else insert(end(), __new_size - size(), __x);` (size() and end() re-read in the insert arm).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

template <class T>
class CIVCountedPointer {
public:
    T* Object;
    CIVCountedPointer() : Object(0) {}
    ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};

class CGraphicInfo;

class CGraphicInfoFrame : public CBaseClass {       // vtable 0x0129C878, 0x10 bytes
public:
    class CFrameData {
    public:
        virtual void v000();
        virtual void DeleteThis();                  // slot 1 (+0x04)
        long RefCount;                              // +0x04
        void Release()
        {
            if (--RefCount == 0)
                DeleteThis();
        }
    };
    unsigned short FrameNum;                        // +0x04
    signed char SmallMipmapLevel;                   // +0x06
    CIVCountedPointer<CFrameData> Data;             // +0x08
    CGraphicInfo* GraphicInfo;                      // +0x0C
    CGraphicInfoFrame() {}
    virtual void v001();
};

namespace std {
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
    typedef _Tp* iterator;
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08

    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);              // 0x00A00ED0
    void _M_fill_insert(iterator __pos, unsigned int __n, const _Tp& __x);   // 0x00A00F50
    void insert(iterator __pos, unsigned int __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};
}

template void std::vector<CGraphicInfoFrame>::resize(unsigned int);