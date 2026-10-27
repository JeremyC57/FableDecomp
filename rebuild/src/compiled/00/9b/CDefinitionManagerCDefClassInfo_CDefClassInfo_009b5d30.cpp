// CDefinitionManager::CDefClassInfo::CDefClassInfo() -- the record's default constructor: an empty name, then the empty array
// Leaf: ??0CDefClassInfo@CDefinitionManager@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B5D30, 23 bytes, thiscall, plain `ret`.
// Identity (probable): unreferenced (inlined at its uses); Ego_r.pdb CDefinitionManager::CDefClassInfo is {CCharString Name, AllocFunc, CArray<unsigned long> GlobalIndices} -- the only Ego_r.pdb record with a CCharString at +0, one scalar at +4 and an array at +8 -- and FableWin.pdb has ??0CDefClassInfo@CDefinitionManager@@QAE@XZ; it sits among the definition manager's instantiations (CArray<unsigned char>::DeallocateUnusedMemory 0x009B5DB0, sort_heap<pair<CCharString, unsigned long> >).
// Callees: CCharString::CCharString() 0x0099E4B0.
// Shape (family M109, representative 0x00515620): CCharString's default constructor out of line, then the CArray (an STLport
//   vector, the landed `CArray<T> : public std::vector<T>` model) zeroes its three pointers at +0x8 (one `xor eax,eax`).
class CCharString {
public:
    CCharString();                                        // 0x0099E4B0
    void* PStringData;
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _STLP_alloc_proxy(const _Alloc& __a, _Tp __p) : _Alloc(__a), _M_data(__p) { }
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
};
}
template <class T> class CArray : public std::vector<T> { };
class CDefClassBase;
class CDefinitionManager {
public:
    class CDefClassInfo {                                 // Ego_r.pdb, 24 bytes (Dinkumware CArray)
    public:
        CDefClassInfo() { }
        CCharString Name;                                 // +0x0
        CDefClassBase* (__cdecl* AllocFunc)();            // +0x4 (left uninitialised)
        CArray<unsigned long> GlobalIndices;              // +0x8
    };
};
// the implicit-in-retail constructor, forced out of line for this object (not part of the retail body)
CDefinitionManager::CDefClassInfo* ConstructOne(void* memory);
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
CDefinitionManager::CDefClassInfo* ConstructOne(void* memory) { return new (memory) CDefinitionManager::CDefClassInfo; }