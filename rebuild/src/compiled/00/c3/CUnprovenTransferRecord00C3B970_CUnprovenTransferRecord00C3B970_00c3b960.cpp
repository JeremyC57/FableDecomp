// CUnprovenTransferRecord_00C3B970::~CUnprovenTransferRecord_00C3B970() -- libpk transfer-record destructor: destroy its CString member
// Leaf: ??1CUnprovenTransferRecord_00C3B970@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3B960, 8 bytes: `add ecx,8h ; jmp 0x00C0BA40` (the only member with a destructor is the
//   CString at +8; the compiler-generated body tail-calls ~CString).
// Identity: libpk (no PDB, no RTTI). The spelling is the one landed callers bind to this address (rebuild/oracles/reloc_bindings.tsv:
//   the record list's Transfer 0x00C2FA20, PKCAutoDelete<CUnprovenTransferRecord_00C3B970>::~PKCAutoDelete and the scalar deleting destructor
//   call it as ??1CUnprovenTransferRecord_00C3B970@@QAE@XZ; one retail address, one name). The original record name is unknown: the
//   placeholder is the landed one, not a new claim (NAME_FIXES.tsv carries no proposal). Catalog "_Cons_val<..>" was propagated.
// Callees: ??1CString@@QAE@XZ 0x00C0BA40 (landed, the libpk MFC-style CString).
// Header: engine/CString.h (FABLE_RETAIL_CSTRING_BUILDERS declares the out-of-line destructor). The record class has no shared
//   header (every landed user models it locally, 48 bytes); only the member the destructor touches is typed here.
#define FABLE_RETAIL_CSTRING_BUILDERS
#include "engine/CString.h"
class CUnprovenTransferRecord_00C3B970 {
public:
    ~CUnprovenTransferRecord_00C3B970();
private:
    unsigned char Unmodeled_0x00[8];                    // +0x00
    CString Text;                                         // +0x8 (name unknown)
    unsigned char Unmodeled_tail[48-8-4];
};
CUnprovenTransferRecord_00C3B970::~CUnprovenTransferRecord_00C3B970()
{
}