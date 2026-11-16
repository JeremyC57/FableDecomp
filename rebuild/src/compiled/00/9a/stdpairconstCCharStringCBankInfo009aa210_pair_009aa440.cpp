// std::pair<const CCharString, CBankInfo_009aa210>::~pair() -- implicit destructor of a bank-info map value in the second copy of the bank module: the four CBankInfo path strings in reverse order, then the key
// Leaf: ??1?$pair@$$CBVCCharString@@UCBankInfo_009aa210@@@std@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AA440, 47 bytes (unreferenced COMDAT; no call, vtable or data reference).
// Identity: byte twin of the landed 0x009A8FE0 ??1?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@QAE@XZ in a second
//   TU of the bank module: its CBankInfo constructor is the landed CBankInfo_009aa210::CBankInfo_009aa210 0x009AA210 (the twin of
//   CBankInfo::CBankInfo 0x009A7670), so the value type here is that copy's record, spelled as the landed row spells it (address
//   suffix, one retail address = one symbol; the original names of the second copy are not recoverable: libpk-style second build,
//   no PDB entry distinguishes it). DESCRIPTIVE class name, recorded in NOTES.
// Shape: implicit destructor emitted through a qualified call helper (as 0x009A8FE0).
#include "stlport/fable_containers.h"
#include "fable_string.h"

class CBankFile;
struct CBankInfo_009aa210 {
    CWideString DevBankFilePath;             // +0x00
    CWideString RetailBankFilePath;          // +0x04
    CWideString DevHeaderFilePath;           // +0x08
    CWideString RetailHeaderFilePath;        // +0x0c
    CBankFile* RegisteredBank;               // +0x10
};

typedef std::pair<const CCharString, CBankInfo_009aa210> CBankInfoMapValue_009aa210;
void DestroyBankInfoMapValue_009aa210(CBankInfoMapValue_009aa210* value)
{
    value->CBankInfoMapValue_009aa210::~CBankInfoMapValue_009aa210();
}