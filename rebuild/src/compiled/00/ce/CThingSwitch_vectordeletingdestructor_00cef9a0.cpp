// CThingSwitch::`vector_deleting_destructor' @ 0x00CEF9A0 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// Leaf: `vector_deleting_destructor'
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x58 CCharStringVector (~ @ 0x00414EA0), +0x4c CCharStringVector (~ @ 0x00414EA0), then the base CScriptBase (~CScriptBase @ 0x00CBD510, ??1CScriptBase@@UAE@XZ; naming wave 35d: the comment named the stand-in CRecordSet_00cbd510), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
#pragma optimize("t", on)
class CScriptBase { public:                // CScriptBase (Ego_r.pdb ??1CScriptBase@@UAE@XZ; was stand-in CRecordSet_00cbd510, naming wave 15)
    virtual ~CScriptBase();                 // 0x00CBD510
};
class CCharString;
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc = allocator<_Tp> > class vector;
template <> class vector<CCharString, allocator<CCharString> > { public:                  // std::vector<CCharString>: {first, last, end of storage}
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    ~vector();                   // 0x00414EA0 (std::vector<CCharString>::~vector, as landed rows name it)
};
}
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_00cef9a0 : CScriptBase {
    unsigned char _pad4c[0x48];
    std::vector<CCharString> Member4c;                 // +0x4c
    std::vector<CCharString> Member58;                 // +0x58
};
#pragma pack(pop)

void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    CObject_00cef9a0* object = static_cast<CObject_00cef9a0*>(self);
    object->Member58.~vector();
    object->Member4c.~vector();
    object->CScriptBase::~CScriptBase();
    if (flags & 1)
        operator delete(object);
    return self;
}
