// CPhysicalPrimitiveInit::StatRegisterClass() -- register the three hit-location primitive prototypes (sphere, cylinder, mesh): each new primitive is wrapped in a counted pointer and pushed onto CPhysicalPrimitiveInit::RegisteredPrimitives
// Leaf: ?StatRegisterClass@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067F9B0, 520 bytes (520 with no padding), static `__fastcall` with no arguments,
//   plain `ret`. Ego_r.pdb `?StatRegisterClass@CPhysicalPrimitiveInit@@SIXXZ`; called once from 0x0044E911.
// Callees: operator new 0x00BFEA1A (the 0x0c / 0x10 / 0x04 primitives and each 0x0c CCPPointerInfo), the deleter
//   CCountedPointer<CPhysicalPrimitiveBase>::DeleteData 0x0067F650 (stored, not called), vector<CCountedPointer<
//   CPhysicalPrimitiveBase> >::_M_insert_overflow 0x0067FF30 (the inline push_back's growth path), operator delete
//   0x00BFE9BC (the temporary's release); the CCPPointerInfo::DeleteFunc pointer (__fastcall, ecx = Data).
// Global: CPhysicalPrimitiveInit::RegisteredPrimitives 0x013BA888 (start; finish 0x013BA88C, end of storage 0x013BA890;
//   the landed SetPPrimitiveFromType 0x0067F810 models it as `g_PrimitivePrototypes`).
// Vtables: NHitLocations::CPhysicalPrimitiveSphere 0x0125B9FC, CPhysicalPrimitiveCylinder 0x0125BA4C,
//   CPhysicalPrimitiveMesh 0x0125B9B0 (rtti-vtables.tsv); the CPhysicalPrimitiveBase store is dead and dropped.
// Label: catalog "StatRegisterClass" is the PDB name.
// Layout: Ego_r.pdb: CPhysicalPrimitiveSphere {vptr, CDefString BaseName +4, float Radius +8}, CPhysicalPrimitiveCylinder
//   {vptr, BaseName +4, Radius +8, Height +0xc}, CPhysicalPrimitiveMesh {vptr}; CDefString's ctor stores -1.
// Shape: three `push_back(CCountedPointer<CPhysicalPrimitiveBase>(new T))` statements: the counted pointer's raw-pointer
//   ctor allocates a CCPPointerInfo {1, DeleteData, object} only for a non-null object; push_back copies it with an add-ref
//   (placement-new null test) or calls _M_insert_overflow (the empty __false_type tag homed in the frame's spare byte);
//   the temporary's destructor releases through Info held in esi (`dec [esi] ; jne`).
inline void* operator new(unsigned int, void* __p) throw() { return __p; }

class CDefString {
public:
    long TablePos;
    CDefString() : TablePos(-1) { }
};

namespace NHitLocations {
class CPhysicalPrimitiveBase {
public:
    virtual ~CPhysicalPrimitiveBase() { }
    virtual CPhysicalPrimitiveBase* Alloc() const = 0;
};
class CPhysicalPrimitiveSphere : public CPhysicalPrimitiveBase {   // vtable 0x0125B9FC
public:
    CDefString BaseName;                               // +0x04
    float      Radius;                                 // +0x08
    virtual CPhysicalPrimitiveBase* Alloc() const;
};
class CPhysicalPrimitiveCylinder : public CPhysicalPrimitiveBase { // vtable 0x0125BA4C
public:
    CDefString BaseName;                               // +0x04
    float      Radius;                                 // +0x08
    float      Height;                                 // +0x0c
    virtual CPhysicalPrimitiveBase* Alloc() const;
};
class CPhysicalPrimitiveMesh : public CPhysicalPrimitiveBase {     // vtable 0x0125B9B0
public:
    virtual CPhysicalPrimitiveBase* Alloc() const;
};
}

struct CCPPointerInfo {
    unsigned long RefCount;                            // +0x00
    void (__fastcall* DeleteFunc)(void* data);         // +0x04
    void*         Data;                                // +0x08
    CCPPointerInfo(void* data, void (__fastcall* deleteFunc)(void*)) : RefCount(1), DeleteFunc(deleteFunc), Data(data) { }
};

template <class T> class CCountedPointer {
public:
    T*              Data;                              // +0x00
    CCPPointerInfo* Info;                              // +0x04
    static void __fastcall DeleteData(void* data);     // 0x0067F650 for CPhysicalPrimitiveBase
    CCountedPointer(T* data) : Data(data)
    {
        Info = data ? new CCPPointerInfo(data, DeleteData) : 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef __false_type _IsPODType;
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
};
}

// Same-TU stand-in for vector<CCountedPointer<CPhysicalPrimitiveBase> >::_M_insert_overflow 0x0067FF30 (compiled in this
// TU): it only reads through `x` and never touches the tag, so VC7.1 keeps the pushed temporary's words in registers.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void std::vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                                                       unsigned int __fill_len, bool __atend)
{
    if (__atend && __fill_len == 1)
        this->_M_finish = __position + (__x.Data != 0);
}
#pragma auto_inline(on)

class CPhysicalPrimitiveInit {
public:
    CCountedPointer<NHitLocations::CPhysicalPrimitiveBase> PPrimitive;   // +0x00
    static std::vector<CCountedPointer<NHitLocations::CPhysicalPrimitiveBase> > RegisteredPrimitives;   // 0x013BA888
    static void __fastcall StatRegisterClass();
};

void __fastcall CPhysicalPrimitiveInit::StatRegisterClass()
{
    RegisteredPrimitives.push_back(CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>(new NHitLocations::CPhysicalPrimitiveSphere));
    RegisteredPrimitives.push_back(CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>(new NHitLocations::CPhysicalPrimitiveCylinder));
    RegisteredPrimitives.push_back(CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>(new NHitLocations::CPhysicalPrimitiveMesh));
}