// CBankInfo_009a9900::CBankInfo_009a9900(long unused) -- default constructor of the bank-file record: four empty CWideString paths (dev / retail bank file, dev / retail header file) and a null registered-bank pointer; the one stack argument is never read (as the landed CTargetAndThing_0078e700(long unused) constructor)
// Leaf: CBankInfo_009a9900
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A9900, 45 bytes (+ int3 padding), __fastcall, `ret 4`.
// Callees: CWideString::CWideString() 0x0099AED0 four times (+0, +4, +8, +0xc).
// Layout: Ego_r.pdb struct CBankInfo (0x14) { CWideString DevBankFilePath, RetailBankFilePath, DevHeaderFilePath,
//   RetailHeaderFilePath; CBankFile* RegisteredBank; }, used by the CBankFileManager functions at 0x009A7720.. .
// Evidence: no direct callers; a second CBankInfo-shaped constructor of the bank module (one argument, unread), so it is suffixed by address (one retail address = one symbol). Manifest label "SExpSpendUndo" is propagated and wrong.
// Shape: the four member constructors in declaration order, then the in-body `RegisteredBank = 0`.
#include "fable_string.h"

class CBankFile;

struct CBankInfo_009a9900 {
    CWideString DevBankFilePath;             // +0x00
    CWideString RetailBankFilePath;          // +0x04
    CWideString DevHeaderFilePath;           // +0x08
    CWideString RetailHeaderFilePath;        // +0x0c
    CBankFile* RegisteredBank;               // +0x10
    CBankInfo_009a9900(long unused);
};

CBankInfo_009a9900::CBankInfo_009a9900(long unused)
{
    RegisteredBank = 0;
}