// std::__uninitialized_fill_n(CVertexShaderInput::CStreamInfo* first, unsigned int n, const CVertexShaderInput::CStreamInfo& x, const __false_type&) -- copy-construct n copies of a vertex stream description into raw storage and return the end
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB4490, length 57 bytes (+7 int3), __fastcall (first in ecx -> esi,
//   n in edx -> ebx, the value and the empty tag reference on the stack, `ret 8`), returns the end in eax.
// Callees: CArray<CVertexStreamFormat::CDataElement>::CArray(const CArray&) @ 0x00A5E0A0 (out-of-line array copy).
// Identity: the only caller (0x00AB45DA) is the landed vector<CVertexShaderInput::CStreamInfo>::_M_insert_overflow
//   0x00AB4540 (its fill_len != 1 arm). Vtable 0x0129D578 = CVertexStreamFormat (rtti-vtables.tsv). The manifest's
//   "_Fill_n<unsigned_long*,unsigned_int,unsigned_long>" is propagated and wrong.
// Layout: CStreamInfo (20 bytes) = StreamIndex +0, StreamFormat +4; CVertexStreamFormat (16) = CBaseClass vptr +0,
//   DataElements (12-byte retail CArray) +4 -- the model of the landed sibling __uninitialized_copy 0x00AB4450.
// Shape: STLport 4.x `for (; n > 0; --n, ++cur) _Construct(&*cur, x)`: unsigned count (`jbe`), placement-new null
//   test, the inline copies store the index, the derived vtable (the base store is dead), then call the array copy.
inline void* operator new(unsigned int, void* p) { return p; }

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

template <class T> class CArray {                  // retail 12-byte array: start, finish, end of storage
public:
    T* m_first;
    T* m_last;
    T* m_end;
    CArray(const CArray& other);                   // 0x00A5E0A0 (CDataElement instance)
};

class CVertexStreamFormat : public CBaseClass {    // vtable 0x0129D578, 16 bytes
public:
    class CDataElement { public: unsigned long Usage; long Type; };
    CArray<CDataElement> DataElements;             // +0x04
    CVertexStreamFormat(const CVertexStreamFormat& other) : CBaseClass(other), DataElements(other.DataElements) { }
    virtual ~CVertexStreamFormat() { }
};

class CVertexShaderInput {
public:
    class CStreamInfo {                            // 20 bytes
    public:
        unsigned long       StreamIndex;           // +0x00
        CVertexStreamFormat StreamFormat;          // +0x04
        CStreamInfo(const CStreamInfo& other) : StreamIndex(other.StreamIndex), StreamFormat(other.StreamFormat) { }
    };
};

namespace std {
struct __false_type { __false_type() {} };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template CVertexShaderInput::CStreamInfo* __fastcall __uninitialized_fill_n(CVertexShaderInput::CStreamInfo*, unsigned int,
                                                                           const CVertexShaderInput::CStreamInfo&,
                                                                           const __false_type&);
}