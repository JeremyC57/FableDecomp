// CTCWife::GetGreetConfirmationText(CWideString&) -- the greet prompt: "go to bed" for a married wife offering sex, "marry me" for a willing one
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D0590, 148 bytes (+12 int3). __fastcall, `ret 4`, returns bool.
// Callees: CWorld::GetWorldFrame() 0x0049D870 (static); CTCWife::IsOfferingSex() 0x006CEEA0; CTCWife::IsWillingToMarry()
//          0x006CFEC0; CCharString(const char*, long) 0x0099EBF0; ~CCharString 0x0099EAE0; CDataBank::GetTextBySymbol(const
//          CCharString&) 0x009C95E0 (CWideString by value); CWideString::operator= 0x0099B7D0; ~CWideString 0x0099B510.
//          Globals: g_pMainGameComponent 0x013B86A0 (text data bank at +0x14); strings 0x0125EEEC "TEXT_GUI_WIFE_GO_TO_BED",
//          0x0125EED4 "TEXT_GUI_WIFE_MARRY_ME".
// Shape: married (FrameGotMarriedToThePlayer <= world frame) arm first; each arm builds a named CCharString symbol
//        (`lea edx ; push edx`) and reads the bank into a named local AFTER it, which keeps the ctor call in each arm
//        (with the bank inline VC7.1 merges the two ctor calls too); the GetTextBySymbol / operator= / dtor tails merge.
// Layout: CTCWife FrameGotMarriedToThePlayer +0x20; CMainGameComponent text CDataBank* +0x14.
class CWorld { public: static long GetWorldFrame(); };  // 0x0049D870
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);  // 0x0099EBF0
    ~CCharString();                              // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    ~CWideString();                              // 0x0099B510
};
namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
using NGameText::CDataBank;
class CMainGameComponent { public:
    unsigned char _pad_0x00[0x14];
    CDataBank* TextBank;                         // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;  // 0x013B86A0
class CTCWife { public:
    virtual void v00();
    unsigned char _pad_0x04[0x20 - 0x04];
    long FrameGotMarriedToThePlayer;             // +0x20
    bool IsOfferingSex() const;                  // 0x006CEEA0
    bool IsWillingToMarry() const;               // 0x006CFEC0
    bool GetGreetConfirmationText(CWideString& text) const;
};
bool CTCWife::GetGreetConfirmationText(CWideString& text) const
{
    if (FrameGotMarriedToThePlayer <= CWorld::GetWorldFrame()) {
        if (IsOfferingSex()) {
            CCharString symbol("TEXT_GUI_WIFE_GO_TO_BED", -1);
            CDataBank* bank = g_pMainGameComponent->TextBank;
            text = bank->GetTextBySymbol(symbol);
            return true;
        }
    } else if (IsWillingToMarry()) {
        CCharString symbol("TEXT_GUI_WIFE_MARRY_ME", -1);
        CDataBank* bank = g_pMainGameComponent->TextBank;
        text = bank->GetTextBySymbol(symbol);
        return true;
    }
    return false;
}