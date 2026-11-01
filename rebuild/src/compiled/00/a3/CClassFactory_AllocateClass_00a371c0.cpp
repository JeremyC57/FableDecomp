// CClassFactory::AllocateClass(const CCharString&) -- look the class type up by name and call its allocation function (0 when the type has none)
// Leaf: ?AllocateClass@CClassFactory@@QAEPAVCClass@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A371C0, 42 bytes, thiscall `ret 4`.
// Identity: Ego_r.pdb ?AllocateClass@CClassFactory@@QAEPAVCClass@@ABVCCharString@@@Z (donor order: GetClassNumberFromName,
//   AllocateClass, next to AddNewClassType 0x00A36F90 / GetNoAllocatedOfType 0x00A36FC0 landed); the body is exactly "type =
//   GetClassNumberFromName(name); ClassTypeInfo[type].AllocFunc ? AllocFunc() : 0" with the 0x14-byte retail CClassTypeInfo.
// Callees: CClassFactory::GetClassNumberFromName 0x00A37060 (Ego_r ?GetClassNumberFromName@CClassFactory@@IBEJABVCCharString@@@Z).
// Layout: engine/CClassFactory.h (rename pattern; ClassTypeInfo vector viewed as STLport std::vector<CClassTypeInfo>, as the
//   landed 0x00A36FC0); AllocFunc is `CClass* (__fastcall*)()` per FableWin ??0CClassTypeInit@@QAE@JP6IPAVCClass@@XZ_NABVCCharString@@@Z.
// Shape: the indexed member read twice (`ClassTypeInfo[type].AllocFunc` in the test and the call) gives retail's shl/[eax+esi+0xc]
//   load; a named function-pointer or element-reference local gives lea-scaled addressing.
class CClass;
class CCharString { public: void* PStringData; };
namespace std {
    template <class _Tp> class allocator { public: };
    template <class _Value, class _Tp, class _MaybeReboundAlloc>
    class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
    template <class _Tp, class _Alloc> class _Vector_base {
    public:
        _Tp* _M_start;
        _Tp* _M_finish;
        _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    };
    template <class _Tp, class _Alloc = allocator<_Tp> >
    class vector : protected _Vector_base<_Tp, _Alloc> {
    public:
        typedef _Tp& reference;
        typedef unsigned int size_type;
        _Tp* begin() { return this->_M_start; }
        reference operator[](size_type __n) { return *(begin() + __n); }
    };
    template <class _Tp, class _Alloc> class list { public: void* _M_node; };
}
class CClassTypeInfo {
public:
    std::list<CClass*, std::allocator<CClass*> > TypeList;   // +0x00
    unsigned long NoAllocated;                                // +0x04
    bool          Used;                                       // +0x08
    bool          Local;                                      // +0x09
    CClass*     (__fastcall *AllocFunc)();                    // +0x0C
    CCharString   Name;                                       // +0x10
};
#define CClassFactory DonorCClassFactory
#include "engine/CClassFactory.h"
#undef CClassFactory
class CClassFactory : public DonorCClassFactory {
protected:
    long GetClassNumberFromName(const CCharString& name) const;   // 0x00A37060
public:
    CClass* AllocateClass(const CCharString& name);
    std::vector<CClassTypeInfo>& TypeInfos() { return *reinterpret_cast<std::vector<CClassTypeInfo>*>(ClassTypeInfo); }
};
CClass* CClassFactory::AllocateClass(const CCharString& name)
{
    long type = GetClassNumberFromName(name);
    if (TypeInfos()[type].AllocFunc)
        return TypeInfos()[type].AllocFunc();
    return 0;
}