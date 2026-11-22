// CFontDataBankEntry::AcquireFont() -- create the entry's CStaticFontBank (0x2034 bytes, over the words at +0xc and +0x10) into the counted pointer at +4 when it is still empty, then return a copy of that counted pointer (add-reference on its count block)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A634F0, length 96 bytes.
// Callees: operator new @ 0x00BFEA1A (0x2034), CStaticFontBank::CStaticFontBank @ 0x00AB8E10 (on the block, +0xc, +0x10; the null arm yields 0), CCountedPointer::Reset @ 0x0041947A
// (labelled "CCountedPointer<std::vector<CCharString>>::Reset", propagated), the inlined copy constructor into the hidden return slot (`inc dword ptr [count]`).
// Shape: the counted pointer's address in esi, ebx/edi saved only on the create arm (the two constructor arguments are loaded BEFORE operator new: the bank is
// built by an inline helper whose arguments are evaluated first), `ret 4`.
// Leaf: ?AcquireFont@CFontDataBankEntry@@QBE?AV?$CCountedPointer@VCFontBank@@@@XZ
// Name (NF222, family q12402): the self leaf is the Ego_d / Ego_r / FableWin.pdb spelling (const, returns CCountedPointer<CFontBank>);
//   the local CCountedPointer_00a634f0 stand-in is replaced by engine/CCountedPointer.h (member Font, Ego_r.pdb CFontDataBankEntry +0x04
//   CCountedPointer<CFontBank>, `mutable`: the const member re-seats it), Reset = ?Reset@?$CCountedPointer@VCFontBank@@@@QAEXPAVCFontBank@@@Z
//   (0x0041947A, FableWin.pdb; declaration only), the inlined copy constructor is the header's. The local 0x2034-byte
//   `struct CStaticFontBank(long, long)` model is replaced by engine/CStaticFontBank.h (_CTOR: ??0CStaticFontBank@@QAE@PAVCFontDataBank@@K@Z,
//   Ego_r.pdb / FableWin.pdb, the self leaf of the landed row 0x00AB8E10), built from the entry's Bank (+0x0C, CFontDataBank*) and
//   Index (+0x10) as engine/CFontDataBankEntry.h lays them out (were `long Bank` / `long m_b`). Its one landed caller 0x009E2BA0 binds
//   the same const AcquireFont spelling in the same hand-back.
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_BASE
#define FABLE_RETAIL_CSTATICFONTBANK
#define FABLE_RETAIL_CSTATICFONTBANK_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#include "engine/CCountedPointer.h"
#include "engine/CArray.h"
#include "engine/CTexture.h"
#include "engine/CStaticFontBank.h"

class CFontDataBankEntry { public:
    virtual void v00();
    mutable CCountedPointer<CFontBank> Font;           // +0x04
    CFontDataBank* Bank;                            // +0x0c
    unsigned long Index;                            // +0x10
    CCountedPointer<CFontBank> AcquireFont() const;
};

// The HEAD inline helper (no PDB name) as a file-scope internal function: as a static member of the local entry model it would now
//   define an external pick-any ?CreateBank@CFontDataBankEntry@@... with the header argument types (NF222).
static inline CStaticFontBank* CreateBank(CFontDataBank* bank, unsigned long index) { return new CStaticFontBank(bank, index); }

CCountedPointer<CFontBank> CFontDataBankEntry::AcquireFont() const
{
    if (!Font.Data)
        Font.Reset(CreateBank(Bank, Index));
    return Font;
}
