// CTCCarriedActionUseRead::~CTCCarriedActionUseRead() -- carried readable's use component dtor (compiler-generated): release the readable def pointer, then the base chain (~CTCActionUseBase)
// Leaf: ??1CTCCarriedActionUseRead@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DCDA4, 32 bytes, thiscall; size-optimised TU (unaligned, `and [esi+14h],0`), ends
//   `jmp 0x004DA967` (~CTCActionUseBase: the action-use def's release at +0x0C, then `jmp ~CTCBase`).
// Identity (agentFAM06): the only caller is 0x004DCD88, slot 0 (scalar deleting destructor) of the CTCCarriedActionUseRead RTTI
//   vtable 0x01240834; Ego_r.pdb / FableWin.pdb ??1CTCCarriedActionUseRead@@UAE@XZ; Ego_r.pdb CTCCarriedActionUseRead is 0x1C bytes,
//   PDef (CDefPointer<const CCarriedReadableDef>) +0x14 and AlreadyRead +0x18 over CTCCarriedActionUseBase (no members of its own,
//   over CTCActionUseBase, 0x14 bytes). Catalog CIVCountedPointer_Release and the landed leaf ?dtor_C3DAnimationBlendState@@... are
//   placeholders.
// Body: the shared engine/CTCActionUseBase.h (retail CTCBase, CDefPointer<T>). No shared header exists for CTCCarriedActionUseBase /
//   CTCCarriedActionUseRead: declared here with Ego_r.pdb's members (NOTES.md "Header debt"). The helper below only makes VC7.1
//   emit the implicit ??1 COMDAT; it is not a retail function.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCActionUseBase.h"
class CCarriedReadableDef;

class CTCCarriedActionUseBase : public CTCActionUseBase {     // Ego_r.pdb, 0x14 bytes (no members of its own)
};
class CTCCarriedActionUseRead : public CTCCarriedActionUseBase {   // Ego_r.pdb, 0x1C bytes
public:
    CDefPointer<const CCarriedReadableDef> PDef;              // +0x14
    bool                                   AlreadyRead;       // +0x18
};
FABLE_STATIC_ASSERT(sizeof(CTCCarriedActionUseRead) == 0x1C);

void EmitCarriedActionUseReadDestructor(CTCCarriedActionUseRead* p)   // emits the implicit ??1; not a retail function
{
    p->CTCCarriedActionUseRead::~CTCCarriedActionUseRead();
}
