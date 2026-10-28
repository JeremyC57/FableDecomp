// CPersistContext::Transfer(CRandomAppearanceMorph& value) -- persist a random-appearance morph by binary stream only: CRC-check the empty key, build the default value, then load or save the morph by the context's binary mode
// Leaf: ??$Transfer@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00676740, 256 bytes (247 without padding), thiscall `ret 4`.
// Callers: CCreatureDef::Transfer 0x00675E20 (PRandomAppearanceMorph's object, `call` at 0x00675E61) and the two-argument
//   forwarder 0x006768B0 (`push [esp+8] ; call ; ret 8`).
// Callees: CPersistContext::CheckCRC(const char*) 0x00404500 with the pooled empty string 0x0122D70E, CRandomAppearanceMorph
//   ctor 0x0086C4D0 (the default value: GetDefaultVal<T>() returned in place), TransferBinaryIn(CMemoryDataInputStream&)
//   0x0086BFB0, TransferBinaryOut(CDataOutputStream&) const 0x0086B6D0, the default's inline destructor: free 0x00BFEA14
//   (GeneralTextureMorphs, SkeletalMorphs), vector<CVectorMap<unsigned long, CTextureMorph> >::~vector 0x006753B0 (out of
//   line in this TU), __destroy_aux(CArray<CBodyPartMesh>*, .., const __false_type&) 0x00675850 (`__fastcall`; the tag
//   homed in the dead `value` argument slot).
// Label: catalog "CRandomAppearanceMorph_Serialize" is descriptive; Ego_r.pdb names CPersistContext::Transfer<
//   CRandomAppearanceMorph>(const char*, T&[, const T&]) and GetDefaultVal<CRandomAppearanceMorph>(); this one-argument
//   body has no name parameter, so the name here is descriptive.
// Layout: CPersistContext (Ego_r.pdb) Mode +0x18, PLoadStream +0x24, PSaveStream +0x28; CRandomAppearanceMorph as the
//   landed operator= 0x00675B60 (retail 12-byte CArray, 16-byte CVectorMap): BodyParts +0, BodyPartTextureMorphs +0xc,
//   SkeletalMorphs +0x18, GeneralTextureMorphs +0x28, MaxTextureGroupID +0x38.
// Shape: the binary modes are a two-case switch (`sub eax,2 ; je ; dec eax ; je`) whose cases return, so VC7.1 emits the
//   default value's destructor at each of the three exits (cross-jumped from the __destroy_aux call on); the save case
//   comes first in the source (retail lays out default, case 3, case 2). The default value's destructor is inlined at each
//   exit: the lane VC7.1 keeps the implicit (or plain inline) ~CRandomAppearanceMorph out of line (a `call` per exit), so
//   it is declared and then defined `__forceinline` after the explicit specializations of the two out-of-line callees.
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14

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
    protected: void CheckCRC(const char* name); public:                   // 0x00404500
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