// CTCBetting::`scalar deleting destructor' @ 0x007E5920 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x1c std::vector<CConversation,std::allocator<CConversation> > (~ @ 0x00440C40), +0x10 std::vector<CConversation,std::allocator<CConversation> > (~ @ 0x00440C40), then the base CTCBase (~ @ 0x00686830), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
// Engine headers for the callee classes (PDB data layouts, renamed out of the way): the local declarations
// below carry the retail identities the relocations bind.
#define CTCBase CTCBasePdb
#include "engine/CTCBase.h"
#undef CTCBase
#pragma optimize("t", on)
struct CTCBase {                            // vtable 0x0125BE0C (.?AVCTCBase@@)
    void* _vptr;
    ~CTCBase();                             // 0x00686830 (stores the vtable, tail-jumps ~CBaseClassNonCopyable)
};
struct CConversation;
namespace std {
template<class T> struct allocator;
template<class T, class A> struct vector {   // {first, last, end of storage}
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    ~vector();                              // vector<CConversation>: 0x00440C40
};
}
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_007e5920 : CTCBase {
    unsigned char _pad10[0xc];
    std::vector<CConversation,std::allocator<CConversation> > Member10;// +0x10
    std::vector<CConversation,std::allocator<CConversation> > Member1c;// +0x1c
};
#pragma pack(pop)

void* __fastcall CTCBetting_CompositeScalarDeletingDestructor(void* betting, int, unsigned int flags)
{
    CObject_007e5920* object = static_cast<CObject_007e5920*>(betting);
    object->Member1c.~vector();
    object->Member10.~vector();
    object->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(object);
    return betting;
}
