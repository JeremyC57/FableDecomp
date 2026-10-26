// CMainGameComponent::InitialiseText() -- report progress, allocate and open the text bank.
// Retail00416832 length170. Actual string and game-text headers included below.
// Native data-bank sizeof214 agrees hand header; virtual prefix restored (OpenReadOnly slot1).
// Actual generated CMainGameComponent header absent; native PTextBank+14 matches PDB.
// Calls99EBF0/99EAE0 string;9E9F40 progress;BFEA1A scalar new;9C85A0 ctor;
// 403E40 counted reset;415260 NLocalisation::GetFontBankName;9D56C0 OpenReadOnly.
#pragma optimize("s", on)
#pragma optimize("y", off)
#include <new>
#define CCharString CCharStringDonor
#include "fable_string.h"
#undef CCharString
// Restore the native owning-copy declaration omitted by the hand header.
class CCharString { public: CCharStringData* Data; CCharString(const char*,long); CCharString(const CCharString&); ~CCharString(); };
#define CDataBank CDataBankDonor
#include "fable_game_text_bank.h"
#undef CDataBank
class CCPPointerInfo;
template<class T> class CCountedPointer { public: T* Data;CCPPointerInfo* Info; void Reset(T*); }; //00403e40
namespace NGameText {
class CDataBank {
public:
    virtual ~CDataBank(); //slot0 009cb840
    virtual bool OpenReadOnly(const CCharString&,unsigned long); //slot1 009d56c0
    CDataBank(); //009c85a0
private:
    unsigned char BankFileAndResourceBank[0x210];
};
}
namespace NProgressDisplay { void __fastcall DisplayProgress(const CCharString&,float,bool,bool); } //009e9f40
namespace NLocalisation { CCharString __fastcall GetFontBankName(); } //00415260
class CMainGameComponent { public:
    unsigned char BaseAndSampleBank[0x14];
    CCountedPointer<NGameText::CDataBank> PTextBank;
    void InitialiseText();
};
FABLE_STATIC_ASSERT(sizeof(NGameText::CDataBank)==0x214);
FABLE_STATIC_ASSERT(sizeof(NGameText::CDataBank)==sizeof(NGameText::CDataBankDonor));
FABLE_STATIC_ASSERT(offsetof(CMainGameComponent,PTextBank)==0x14);
void CMainGameComponent::InitialiseText() {
    { CCharString text("Init Text",-1); NProgressDisplay::DisplayProgress(text,-1.0f,false,false); }
    PTextBank.Reset(new NGameText::CDataBank);
    { CCharString text("Opening Text Bank",-1); NProgressDisplay::DisplayProgress(text,-1.0f,false,false); }
    NGameText::CDataBank* bank = PTextBank.Data;
    bank->OpenReadOnly(NLocalisation::GetFontBankName(),0x100);
}