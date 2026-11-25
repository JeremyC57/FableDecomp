// CSalFile::Close() -- close the file handle unless it is INVALID_HANDLE_VALUE (-1), then reset it to -1
// Leaf: ?Close@CSalFile@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5E3D0, 27 bytes: push esi ; mov esi,ecx ; mov eax,[esi+4] ; cmp eax,-1 ; je ; push eax ;
//   call [__imp_CloseHandle] ; mov dword ptr [esi+4],-1 ; pop esi ; ret.
// Name: Vendor naming wave V1 (one address, one name): ?Close@CSalFile@@QAEXXZ, the original GameCODA 2.10 SDK member (CODAApi.lib
//   Release/SalOutputKS.obj); the landed CSalOutputKS::CreateBuffer 0x00C5E650 calls it out of line (table witness).
// NF404 (joins the T3 nf401 0x00C5ECA0 unit): Close is an inline member: retail's ~CSalOutputKS 0x00C5ECA0 expands it in place, and the
//   SDK's SalOutputKS.obj carries ?Close@CSalFile@@QAEXXZ as a 27-byte pick-any (Any) COMDAT, so 0x00C5E3D0 is that out-of-line copy. The unit's ~CSalOutputKS / CSalOutputKS
//   rows emit that same COMDAT from engine/CSalOutputKS.h (FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS); a strong definition here would clash
//   with them (NoDuplicates v Any). Re-rowed in the pick-any form: the header's inline body, emitted through the reference below.
#define FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
#define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
#include "engine/CSalOutputKS.h"
namespace {
// Fidelity: scaffolding, not retail: takes the address of the header's inline CSalFile::Close so this object emits it out of line as a
// pick-any COMDAT (retail's 0x00C5E3D0). Internal to this TU; never called.
class CSalFileCloseReference : public CSalFile {
public:
    static void (CSalFile::*Reference())();
};
// Out of class, so it is an ordinary (not inline) function that VC7.1 compiles even though nothing calls it.
void (CSalFile::*CSalFileCloseReference::Reference())()
{
    return &CSalFileCloseReference::Close;
}
}
