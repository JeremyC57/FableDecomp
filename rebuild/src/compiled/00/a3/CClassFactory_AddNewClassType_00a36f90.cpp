// CClassFactory::AddNewClassType(const CClassTypeInit&) -- register one class type: mark its type-info slot used and copy the local flag, allocation function and name
// Leaf: ?AddNewClassType@CClassFactory@@MAEXABVCClassTypeInit@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A36F90, 47 bytes, thiscall, tail `jmp` to CCharString::operator=.
// Identity: Ego_r.pdb ?AddNewClassType@CClassFactory@@MAEXABVCClassTypeInit@@@Z (protected virtual; CNavigatorManager overrides it,
//   whose landed row 0x00A17590 tail-jumps here); the body writes Used / Local / AllocFunc / Name of ClassTypeInfo[init.TypeID].
// Callees: CCharString::operator=(const CCharString&) 0x0099EFB0 (Ego_r ??4CCharString@@QAEABV0@ABV0@@Z).
// Layout: local model -- engine/CClassFactory.h flattens the vptr into a data member, and the row needs the virtual (MAE) spelling;
//   members as the header (ClassTypeInfo +0x04 as STLport vector, MaxClassTypeID, NoAllocatedClasses, ClassList). CClassTypeInit
//   (Ego_r 0x10): TypeID +0, AllocFunc +4, Local +8, Name +0xC. CClassTypeInfo retail 0x14 (Used +8, Local +9, AllocFunc +0xC, Name +0x10).
// Shape: one element-reference local (four indexed writes recompute the address each time).
class CClass;
class CCharString {
public:
    void* PStringData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
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
class CClassTypeInit {
public:
    unsigned char TypeID;                                     // +0x00
    CClass*     (__fastcall *AllocFunc)();                    // +0x04
    bool          Local;                                      // +0x08
    CCharString   Name;                                       // +0x0C
};
class CClassFactory {
protected:
    virtual void AddNewClassType(const CClassTypeInit& init);
    std::vector<CClassTypeInfo> ClassTypeInfo;                // +0x04
    unsigned long MaxClassTypeID;                             // +0x10
    unsigned long NoAllocatedClasses;                         // +0x14
    std::list<CClass*, std::allocator<CClass*> > ClassList;   // +0x18
};
void CClassFactory::AddNewClassType(const CClassTypeInit& init)
{
    CClassTypeInfo& info = ClassTypeInfo[init.TypeID];
    info.Used = true;
    info.Local = init.Local;
    info.AllocFunc = init.AllocFunc;
    info.Name = init.Name;
}