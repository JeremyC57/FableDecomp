// CEnginePrimitiveManagerParticleLineGroup::GetSupportedTypes(CEnginePrimitiveTypeList& types) const -- report the primitive types this manager creates: append 0x1A and 0x15 to the caller's type list
// Leaf: ?GetSupportedTypes@CEnginePrimitiveManagerParticleLineGroup@@UBEXAAVCEnginePrimitiveTypeList@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC4C50, 122 bytes (+ int3 padding to 128), thiscall (`this` unused), `ret 4`.
// Identity (proven): CEnginePrimitiveManagerParticleLineGroup::GetSupportedTypes(CEnginePrimitiveTypeList&) const; evidence: the address is slot 2 of vtable 0x012A6590 (RTTI .?AVCEnginePrimitiveManagerParticleLineGroup@@, base array CEnginePrimitiveManagerParticleLineGroup <- CEnginePrimitiveManager <- CEnginePrimitiveRenderUtil <- CBaseClassNonCopyable <- CBaseClass), the GetSupportedTypes slot (const, one CEnginePrimitiveTypeList& argument: PDB ?GetSupportedTypes@CEnginePrimitiveManagerParticleLineGroup@@UBEXAAVCEnginePrimitiveTypeList@@@Z); its CreateInternalPrimitive (slot 1, 0x00BC4CD0) compares the requested type with 0x1A and 0x15; the body appends 0x1A then 0x15
// Label: the manifest's "CEnginePrimitiveManagerParticleLineGroup::GetSupportedTypes" names the right class (family J027: six two-type GetSupportedTypes overrides of one shape).
// Callees: STLport vector<long>::_M_insert_overflow 0x00440990 (the PDB-format STLport binding
//   ?_M_insert_overflow@?$vector@JV?$allocator@J@std@@@std@@IAEXPAJABJABU__true_type@2@I_N@Z, protected; same-TU body kept out of
//   line, as in the landed 0x00B29E30 model).
// Layout: CEnginePrimitiveTypeList (PDB class: ctor, dtor, Add(EEnginePrimitiveType), Add(EEngineInternalPrimitiveType); no
//   layout record) holds one 12-byte STLport vector<long> at +0 (the body reads finish +4 / end of storage +8 of the argument);
//   the member name `Types` is descriptive. EEnginePrimitiveType's enumerator names are not in the PDBs: the values are written
//   as casts. Class chain from RTTI: CEnginePrimitiveManagerParticleLineGroup <- CEnginePrimitiveManager <- CEnginePrimitiveRenderUtil <- CBaseClassNonCopyable <- CBaseClass.
// Shape: two inline CEnginePrimitiveTypeList::Add calls (PDB ?Add@CEnginePrimitiveTypeList@@QAEXW4EEnginePrimitiveType@@@Z):
//   each push_back(const long&) binds the enum-to-long temporary (the first homed in the `push ecx` word, the second in the
//   dead `types` slot); STLport's inline push_back with the placement-new null test and the __true_type tag.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E (in the same-TU overflow body)
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}

namespace std {

struct __true_type { __true_type() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
                               : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
                                     + ((const char*)__last - (const char*)__first);
}

template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __value;
    return __first;
}

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
protected:
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    // Same-TU body: retail's 0x00B292B0 (vector<CEngineComponent*>, family F007) is a COMDAT of this TU; VC7.1 keeps it
    // out of line, as retail does.
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false)
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);

        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
        __new_finish = fill_n(__new_finish, __fill_len, __x);
        if (!__atend)
            __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
};

} // namespace std

template <class T> class CArray : public std::vector<T> { };

enum EEnginePrimitiveType { };
class CEngineInternalPrimitiveBase;

class CEnginePrimitiveTypeList {
public:
    CArray<long> Types;                                     // +0x00 (descriptive member name)
    void Add(EEnginePrimitiveType type) { Types.push_back(type); }
};

class CBaseClass {
public:
    CBaseClass();                                           // 0x0099A2D0
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                                // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                       // 0x0099A300
};
class CEnginePrimitiveRenderUtil : public CBaseClassNonCopyable {
};
class CEngineScene;
class CEnginePrimitiveManager : public CEnginePrimitiveRenderUtil {   // Ego_r.pdb 0xC bytes
public:
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType type) = 0;   // slot 1
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const = 0;                       // slot 2
    CArray<CEngineScene*>* SceneList;                       // +0x04
    bool                   Enable;                          // +0x08
};
class CEnginePrimitiveManagerParticleLineGroup : public CEnginePrimitiveManager {   // RTTI .?AVCEnginePrimitiveManagerParticleLineGroup@@, vtable 0x012A6590
public:
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const;   // slot 2
};

void CEnginePrimitiveManagerParticleLineGroup::GetSupportedTypes(CEnginePrimitiveTypeList& types) const
{
    types.Add((EEnginePrimitiveType)0x1A);
    types.Add((EEnginePrimitiveType)0x15);
}