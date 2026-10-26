// CClassFactory::~CClassFactory() -- store the vtable, destroy ClassList, destroy and free the ClassTypeInfo vector, then ~CBaseClassNonCopyable
// Retail 0x00A37360, 60 bytes. Stores the CClassFactory vtable 0x0129CF70 (rtti-vtables: CClassFactory
// slot 0 = 0x00A38080); reached from the deleting destructor at 0x00A175A0 and a jump at 0x00523535
// (ICF-shared bodies); the manifest label "CTCInventoryItem::OnDie" is propagated.
// Members die in reverse order (retail layout, as the landed CClassFactory::DeleteAllClasses
// 0x00A37130): ClassList (std::list<CClass*>, a one-word head at +0x18) through its out-of-line
// destructor 0x00A376A0; ClassTypeInfo (vector<CClassTypeInfo>, {first, last, end} at +0x04) inline:
// the `__fastcall` _Destroy_range 0x00A37AF0 (first in ecx, last in edx, the empty allocator temporary
// by const reference -- homed in the high byte of the `push ecx` frame slot, `lea eax,[esp+7]`), then
// `free` 0x00BFEA14 of the block under a null test; finally CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (manifest label "CBase") (not a tail jump:
// the frame slot is popped after it).
// Leaf: ??1CClassFactory

extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct SScalarTag {
    SScalarTag() { }
};

class CClassTypeInfo;
void __fastcall _Destroy_range(CClassTypeInfo* first, CClassTypeInfo* last, const SScalarTag& tag);   // 0x00A37AF0

struct SClassTypeInfoVector {
    CClassTypeInfo* _M_start;
    CClassTypeInfo* _M_finish;
    CClassTypeInfo* _M_end_of_storage;
    ~SClassTypeInfoVector()
    {
        _Destroy_range(_M_start, _M_finish, SScalarTag());
        if (_M_start)
            free(_M_start);
    }
};

class CClass;
struct SClassList {                                // std::list<CClass*>: retail keeps only the head
    void* head;
    ~SClassList();                                 // 0x00A376A0
};

struct CBaseClassNonCopyable {
    virtual void v00();
    ~CBaseClassNonCopyable();                                      // 0x0099A300
};

class CClassFactory : public CBaseClassNonCopyable { public:                     // vtable 0x0129CF70
    SClassTypeInfoVector ClassTypeInfo;            // +0x04
    unsigned long MaxClassTypeID;                  // +0x10
    unsigned long NoAllocatedClasses;              // +0x14
    SClassList ClassList;                          // +0x18
    virtual void v00();
    ~CClassFactory();
};

CClassFactory::~CClassFactory()
{
}