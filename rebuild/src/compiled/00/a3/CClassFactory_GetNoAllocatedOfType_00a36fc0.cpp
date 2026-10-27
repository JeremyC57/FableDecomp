// CClassFactory::GetNoAllocatedOfType(unsigned long) const -- number of live objects of one class type
// Retail: Fable.exe 0x00A36FC0, 17 bytes: `mov ecx,[ecx+4]` (ClassTypeInfo._M_start) ; `lea eax,[eax+eax*4]` ;
// `mov eax,[ecx+eax*4+4]` (element stride 0x14, NoAllocated at +4) ; `ret 4`.
// Identity: Ego_r.pdb ?GetNoAllocatedOfType@CClassFactory@@QBEKK@Z; it sits next to CClassFactory::AddNewClassType
// 0x00A36F90 and GetClassNumberFromName 0x00A37060, and its callers 0x0051E0E0 / 0x0051E0F0 are one-line
// CThingManager wrappers passing type 3 / 4. CClassTypeInfo (Ego_r.pdb: TypeList list, NoAllocated, Used, Local,
// AllocFunc, Name) is 0x14 bytes with the 4-byte STLport list, so NoAllocated is at +4.
// CClassFactory comes from engine/CClassFactory.h (rename pattern); its ClassTypeInfo vector is opaque bytes there,
// so an accessor views them as the STLport 4.6 std::vector<CClassTypeInfo>.
// Leaf: ?GetNoAllocatedOfType@CClassFactory@@QBEKK@Z
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
        typedef const _Tp& const_reference;
        typedef unsigned int size_type;
        const _Tp* begin() const { return this->_M_start; }
        const_reference operator[](size_type __n) const { return *(begin() + __n); }
    };
    template <class _Tp, class _Alloc> class list { public: void* _M_node; };
}

class CClassTypeInfo {
public:
    std::list<CClass*, std::allocator<CClass*> > TypeList;   // +0x00
    unsigned long NoAllocated;                                // +0x04
    bool          Used;                                       // +0x08
    bool          Local;                                      // +0x09
    CClass*     (*AllocFunc)();                               // +0x0C
    CCharString   Name;                                       // +0x10
};

#define CClassFactory DonorCClassFactory
#include "engine/CClassFactory.h"
#undef CClassFactory

class CClassFactory : public DonorCClassFactory {
public:
    unsigned long GetNoAllocatedOfType(unsigned long type) const;
    const std::vector<CClassTypeInfo>& TypeInfos() const { return *reinterpret_cast<const std::vector<CClassTypeInfo>*>(ClassTypeInfo); }
};

unsigned long CClassFactory::GetNoAllocatedOfType(unsigned long type) const
{
    return TypeInfos()[type].NoAllocated;
}