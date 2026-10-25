// Behaviour test for 0x00676740: CPersistContext::Transfer(CRandomAppearanceMorph& value) -- persist a random-appearance morph by binary stream only: CRC-check the empty key, build the default value, then load or save the morph by the context's binary mode
// A copy of the source model and logic (compiled separately from src/00676740.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif

class CDataOutputStream;
class CMemoryDataInputStream;

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}

template <class T> class CArray : public std::vector<T> { };   // retail CArray<T>: an STLport vector (12 bytes)

template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class Compare> class CVectorMap {   // retail 16 bytes
public:
    CArray<std::pair<K, V> > m_vector;                 // +0x00
    Compare                  m_compare;                // +0x0c (empty)
    bool                     m_sorted;                 // +0x0d
};

class CRandomAppearanceMorph {                         // retail 0x3c bytes
public:
    class CBodyPartMesh;
    class CTextureMorph {
    public:
        unsigned char _body[0x10];
    };
    class CSkeletalMorph {
    public:
        unsigned char _body[0x20];
    };
    typedef CVectorMap<unsigned long, CTextureMorph, CKeyPairCompareLess<unsigned long, CTextureMorph> > CTextureMorphMap;
    CArray<CArray<CBodyPartMesh> >                                                     BodyParts;              // +0x00
    CArray<CTextureMorphMap>                                                           BodyPartTextureMorphs;  // +0x0c
    CVectorMap<long, CSkeletalMorph, CKeyPairCompareLess<long, CSkeletalMorph> >       SkeletalMorphs;         // +0x18
    CTextureMorphMap                                                                   GeneralTextureMorphs;   // +0x28
    unsigned long                                                                      MaxTextureGroupID;      // +0x38
    CRandomAppearanceMorph();                                                          // 0x0086C4D0
    ~CRandomAppearanceMorph();
    void TransferBinaryIn(CMemoryDataInputStream& stream);                             // 0x0086BFB0
    void TransferBinaryOut(CDataOutputStream& stream) const;                           // 0x0086B6D0
};

namespace std {
template <>
void __fastcall __destroy_aux<CArray<CRandomAppearanceMorph::CBodyPartMesh>*>(CArray<CRandomAppearanceMorph::CBodyPartMesh>* __first,
    CArray<CRandomAppearanceMorph::CBodyPartMesh>* __last, const __false_type&);   // 0x00675850
template <> vector<CRandomAppearanceMorph::CTextureMorphMap, allocator<CRandomAppearanceMorph::CTextureMorphMap> >::~vector();   // 0x006753B0
}

__forceinline CRandomAppearanceMorph::~CRandomAppearanceMorph() { }

class CPersistContext {
public:
    unsigned char           _pad00[0x18];
    long                    Mode;                      // +0x18: 2 binary load, 3 binary save
    unsigned char           _pad1c[8];
    CMemoryDataInputStream* PLoadStream;               // +0x24
    CDataOutputStream*      PSaveStream;               // +0x28
    void CheckCRC(const char* name);                   // 0x00404500
    template <class T> static T GetDefaultVal() { return T(); }
    template <class T> void Transfer(T& value);
};

template <class T>
void CPersistContext::Transfer(T& value)
{
    CheckCRC("");
    T defaultValue = GetDefaultVal<T>();
    switch (Mode) {
    case 3:
        value.TransferBinaryOut(*PSaveStream);
        return;
    case 2:
        value.TransferBinaryIn(*PLoadStream);
        return;
    }
}

template void CPersistContext::Transfer<CRandomAppearanceMorph>(CRandomAppearanceMorph& value);

// ---- test stand-ins ----
class CRandomAppearanceMorph::CBodyPartMesh { public: long MeshIndex; };
class CDataOutputStream { public: int Writes; };
class CMemoryDataInputStream { public: int Reads; };
static int g_ctors = 0, g_bodyPartDestroys = 0, g_textureMorphDtors = 0, g_crcChecks = 0;
static const char* g_lastCRC = 0;
static const CRandomAppearanceMorph* g_lastOut = 0;
static CRandomAppearanceMorph* g_lastIn = 0;
CRandomAppearanceMorph::CRandomAppearanceMorph()
{
    ++g_ctors;
    BodyParts._M_start = BodyParts._M_finish = BodyParts._M_end_of_storage._M_data = 0;
    BodyPartTextureMorphs._M_start = BodyPartTextureMorphs._M_finish = BodyPartTextureMorphs._M_end_of_storage._M_data = 0;
    SkeletalMorphs.m_vector._M_start = SkeletalMorphs.m_vector._M_finish = SkeletalMorphs.m_vector._M_end_of_storage._M_data = 0;
    GeneralTextureMorphs.m_vector._M_start = GeneralTextureMorphs.m_vector._M_finish = GeneralTextureMorphs.m_vector._M_end_of_storage._M_data = 0;
    SkeletalMorphs.m_sorted = GeneralTextureMorphs.m_sorted = true;
    MaxTextureGroupID = 0;
}
void CRandomAppearanceMorph::TransferBinaryIn(CMemoryDataInputStream& stream) { ++stream.Reads; g_lastIn = this; }
void CRandomAppearanceMorph::TransferBinaryOut(CDataOutputStream& stream) const { ++stream.Writes; g_lastOut = this; }
void CPersistContext::CheckCRC(const char* name) { ++g_crcChecks; g_lastCRC = name; }

namespace std {
template <>
void __fastcall __destroy_aux<CArray<CRandomAppearanceMorph::CBodyPartMesh>*>(CArray<CRandomAppearanceMorph::CBodyPartMesh>* __first,
    CArray<CRandomAppearanceMorph::CBodyPartMesh>* __last, const __false_type&)
{
    ++g_bodyPartDestroys;
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <> vector<CRandomAppearanceMorph::CTextureMorphMap, allocator<CRandomAppearanceMorph::CTextureMorphMap> >::~vector()
{
    ++g_textureMorphDtors;
    _Destroy(this->_M_start, this->_M_finish);
}
}

static int Fail(int line) { std::printf("00676740_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) if (!(c)) return Fail(__LINE__)

int main()
{
    CDataOutputStream out; out.Writes = 0;
    CMemoryDataInputStream in; in.Reads = 0;
    CPersistContext context; context.PLoadStream = &in; context.PSaveStream = &out;
    CRandomAppearanceMorph morph;
    CHECK(g_ctors == 1);
    context.Mode = 3;                                          // binary save
    context.Transfer(morph);
    CHECK(out.Writes == 1 && in.Reads == 0 && g_lastOut == &morph && g_crcChecks == 1 && g_lastCRC && g_lastCRC[0] == 0);
    CHECK(g_ctors == 2 && g_bodyPartDestroys == 1 && g_textureMorphDtors == 1);   // the default value is built and destroyed
    context.Mode = 2;                                          // binary load
    context.Transfer(morph);
    CHECK(in.Reads == 1 && out.Writes == 1 && g_lastIn == &morph && g_crcChecks == 2 && g_ctors == 3 && g_textureMorphDtors == 2);
    context.Mode = 1;                                          // text modes: nothing transferred
    context.Transfer(morph);
    CHECK(in.Reads == 1 && out.Writes == 1 && g_crcChecks == 3 && g_ctors == 4 && g_bodyPartDestroys == 3);
    std::printf("00676740_TEST PASS\n");
    return 0;
}