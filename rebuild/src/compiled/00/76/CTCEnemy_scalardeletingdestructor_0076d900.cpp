// CTCEnemy::`scalar deleting destructor' @ 0x0076D900 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the
// stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the members last-to-first,
// +0x14 CList_004406a0 (~ @ 0x004406A0), +0x10 CList_004406a0 (~ @ 0x004406A0), then the base CTCBase (~ @ 0x00686830), and frees the block with operator delete
// (MSVCR71 thunk 0x00BFE9BC) when flags & 1. Each callee is declared under its own identity (vtable RTTI /
// retail labels); the class itself is not attributed, so it is named by address.
// Engine headers for the callee classes (PDB data layouts, renamed out of the way): the local declarations
// below carry the retail identities the relocations bind.
#define CTCBase CTCBasePdb
#include "engine/CTCBase.h"
#undef CTCBase
#pragma optimize("t", on)
class CTCBase { public:                            // vtable 0x0125BE0C (.?AVCTCBase@@)
    virtual ~CTCBase();                             // 0x00686830 (stores the vtable, tail-jumps ~CBaseClassNonCopyable)
};
struct CList_004406a0 {                     // unattributed STLport list (one word, the sentinel node): nodes cleared @ 0x0043F230, then free(sentinel)
    void* _M_node;
    ~CList_004406a0();                    // 0x004406A0 (catalog label propagated from another body; class not identified)
};
void __cdecl operator delete(void* block);  // MSVCR71 import thunk 0x00BFE9BC

#pragma pack(push, 1)
struct CObject_0076d900 : CTCBase {
    unsigned char _pad10[0xc];
    CList_004406a0 Member10;                    // +0x10
    CList_004406a0 Member14;                    // +0x14
};
#pragma pack(pop)

void* __fastcall CTCEnemy_CompositeScalarDeletingDestructor(void* enemy, int, unsigned int flags)
{
    CObject_0076d900* object = static_cast<CObject_0076d900*>(enemy);
    object->Member14.~CList_004406a0();
    object->Member10.~CList_004406a0();
    object->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(object);
    return enemy;
}
