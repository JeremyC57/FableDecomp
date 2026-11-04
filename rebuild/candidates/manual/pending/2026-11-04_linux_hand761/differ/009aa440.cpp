// std::pair<const CCharString, CBankFileManager::CBankInfo>::~pair() -- implicit destructor of a bank-info map value: the four CBankInfo path strings in reverse order, then the key
// Grade: DIFFER(47v47 bytes identical; RELOCATION_MATCH as graded) -- duplicate COMDAT: the byte-identical twin of 0x009A8FE0 in a second TU; the same symbol cannot land at two addresses (coordinator: alias or ICF-style twin record)
// Leaf: ??1?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@QAE@XZ
// Flags: none
// Twin: 0x009AA440, 47 bytes, the second emitted copy of this implicit destructor (graded t1 RELOCATION_MATCH with this exact
//   source); the CBankInfo ctors 0x009A7670 / 0x009AA210 and pair dtors 0x009A9010 / 0x009AA470 are twinned the same way.
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A8FE0, 47 bytes (unreferenced COMDAT in the CBankFileManager TU).
// Identity: FableWin.pdb ??1?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@QAE@XZ; Ego_r layout of CBankInfo (four
//   CWideStrings + RegisteredBank, 20 bytes) and of the 24-byte pair; neighbours CBankInfo ctor 0x009A7670 and pair ctor 0x009A9AD0.
//   A byte-identical twin sits at 0x009AA440 in a second TU (parked: same symbol cannot appear twice).
// Shape: implicit destructor emitted through a qualified call helper.
#include "stlport/fable_containers.h"
#include "fable_string.h"

class CBankFile;
class CBankFileManager {
public:
    class CBankInfo {
    public:
        CWideString DevBankFilePath;                    // +0x00
        CWideString RetailBankFilePath;                 // +0x04
        CWideString DevHeaderFilePath;                  // +0x08
        CWideString RetailHeaderFilePath;               // +0x0c
        CBankFile*  RegisteredBank;                     // +0x10
    };
};

typedef std::pair<const CCharString, CBankFileManager::CBankInfo> CBankInfoMapValue;
void DestroyBankInfoMapValue(CBankInfoMapValue* value)
{
    value->CBankInfoMapValue::~CBankInfoMapValue();
}
