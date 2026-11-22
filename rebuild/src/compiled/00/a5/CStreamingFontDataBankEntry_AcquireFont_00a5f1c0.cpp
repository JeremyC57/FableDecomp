// CStreamingFontDataBankEntry::AcquireFont() -- create the entry's CStreamingFontBank (0x3034 bytes, over the words at +0xc and +0x10) into the counted pointer at +4 when it is still empty, then return a copy of that counted pointer (add-reference on its count block)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5F1C0, length 96 bytes.
// Callees: operator new @ 0x00BFEA1A (0x3034), CStreamingFontBank::CStreamingFontBank @ 0x009A3C20 (on the block, +0xc, +0x10; the null arm yields 0), CCountedPointer::Reset @ 0x0041947A
// (labelled "CCountedPointer<std::vector<CCharString>>::Reset", propagated), the inlined copy constructor into the hidden return slot (`inc dword ptr [count]`).
// Shape: the counted pointer's address in esi, ebx/edi saved only on the create arm (the two constructor arguments are loaded BEFORE operator new: the bank is
// built by an inline helper whose arguments are evaluated first), `ret 4`.
// Name (round 7, NF53): the font constructor 0x009A3C20 is ??0CStreamingFontBank@@QAE@PAVCStreamingFontDataBank@@K@Z (Ego_r.pdb / FableWin.pdb;
//   engine/CStreamingFontBank.h _CTOR), built from the entry's Bank (+0x0C, CStreamingFontDataBank*) and Index (+0x10) as
//   engine/CStreamingFontDataBankEntry.h lays them out; was a local 0x3034-byte CStreamingFontBank(long, long) model.
// Leaf: ?AcquireFont@CStreamingFontDataBankEntry@@QBE?AV?$CCountedPointer@VCFontBank@@@@XZ
// Name (NF222, family q12402): the self leaf is the Ego_d / Ego_r / FableWin.pdb spelling (const, returns CCountedPointer<CFontBank>);
//   the local CCountedPointer_00a5f1c0 stand-in is replaced by engine/CCountedPointer.h (member Font, Ego_r.pdb CStreamingFontDataBankEntry
//   +0x04 CCountedPointer<CFontBank>, `mutable`: the const member re-seats it), Reset = ?Reset@?$CCountedPointer@VCFontBank@@@@QAEXPAVCFontBank@@@Z
//   (0x0041947A, FableWin.pdb; declaration only), the inlined copy constructor is the header's. Its one landed caller 0x009E2C10 binds
//   the same const spelling in the same hand-back.
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_BASE
#define FABLE_RETAIL_CSTREAMINGFONTBANK
#define FABLE_RETAIL_CSTREAMINGFONTBANK_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#include "engine/CCountedPointer.h"
#include "engine/CArray.h"
#include "engine/CTexture.h"
#include "engine/CStreamingFontBank.h"

class CStreamingFontDataBankEntry { public:
    virtual void v00();
    mutable CCountedPointer<CFontBank> Font;           // +0x04
    CStreamingFontDataBank* Bank;                   // +0x0c
    unsigned long Index;                            // +0x10
    static CStreamingFontBank* CreateBank(CStreamingFontDataBank* bank, unsigned long index) { return new CStreamingFontBank(bank, index); }
    CCountedPointer<CFontBank> AcquireFont() const;
};

CCountedPointer<CFontBank> CStreamingFontDataBankEntry::AcquireFont() const
{
    if (!Font.Data)
        Font.Reset(CreateBank(Bank, Index));
    return Font;
}