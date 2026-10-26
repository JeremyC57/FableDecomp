// CTCHeroOnlineScoreboard::AbortWithMessage(const CCharString& textSymbol) -- looks the symbol up in the engine text bank, shows the text, clears the scoreboard data, sets ScoreboardState 0 / TaskState 100 and closes the HTTP connection when one is open
// Leaf: AbortWithMessage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00563410, length 95 bytes (`ret 4`).
// Callees: CDataBank::GetTextBySymbol @ 0x009C95E0 (by value through the hidden result pointer; the
//          text bank is +0x14 of the engine globals @ 0x013B86A0), the message display @ 0x0055EF00
//          (on `this`, takes const CWideString&; manifest label "OnOkClicked"),
//          CTCHeroOnlineScoreboard::ClearScoreboard() @ 0x00562E10, LHNetworkLib::LHHttp2::Close()
//          @ 0x00840070, CWideString::~CWideString() @ 0x0099B510; the safe-bool test names
//          scoped_ptr<LHHttp2>::get @ 0x00564B80 (`mov eax,[ecx] ; ret`).
// Identity: the manifest label "CParticleEffect::~CParticleEffect" is propagated; eight callers in
// the scoreboard's download handlers (0x005645BB..0x0056480B) pass a CCharString symbol and destroy
// it afterwards. The name is descriptive. Layout as 0x00562E70 (PHttp +0x0c, ScoreboardState
// +0x50, TaskState +0x54).
// Shape: the engine-globals pointer is loaded first; the CWideString result is homed in the dead
// `textSymbol` argument slot (`lea ecx,[esp+0Ch]` at the call, `[esp+4]` for its destructor); the
// `if (PHttp)` is boost's safe bool (`neg ; sbb ; test edx,offset get` -- a single-inheritance
// pointer to member), the held pointer is loaded once for both the test and `PHttp->Close()`.

class CCharString { public:
    void* m_pData;
};

class CWideString { public:
    void* PStringData;
    ~CWideString();                                                       // 0x0099B510
};

namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;              // 0x009C95E0
};
}
using NGameText::CDataBank;

struct CEngineGlobals {
    unsigned char _pad[0x14];
    CDataBank*    m_pTextBank;                                            // +0x14
};

extern CEngineGlobals* g_pEngineGlobals_013b86a0;                         // 0x013B86A0

namespace LHNetworkLib {

class LHHttp2 { public:
    void Close();                                                         // 0x00840070
};

} // namespace LHNetworkLib

struct CHttpScopedPtr {                                                   // scoped_ptr<LHNetworkLib::LHHttp2>
    LHNetworkLib::LHHttp2* px;
    typedef LHNetworkLib::LHHttp2* (CHttpScopedPtr::*unspecified_bool_type)() const;
    LHNetworkLib::LHHttp2* get() const { return px; }                    // 0x00564B80
    operator unspecified_bool_type() const { return px == 0 ? 0 : &CHttpScopedPtr::get; }
    LHNetworkLib::LHHttp2* operator->() const { return px; }
};

struct CComponent;

class CTCHeroOnlineScoreboard { public:
    virtual void v00();
    void*          m_pThing;                                              // +0x04
    long           m_8;                                                   // +0x08
    CHttpScopedPtr PHttp;                                                 // +0x0c
    unsigned char  _scoreboardData[0x40];                                 // +0x10 PictureCache .. DownloadedClanScoreboard
    long           ScoreboardState;                                       // +0x50
    long           TaskState;                                             // +0x54
    CComponent*    PScoreboardScreen;                                     // +0x58
    CComponent*    PClanMenuEntry;                                        // +0x5c
    CComponent*    PConnectingMessage;                                    // +0x60
    CComponent*    PConnectingMessage1;                                   // +0x64
    void ClearScoreboard();                                               // 0x00562E10
    void AbortWithMessage(const CCharString& textSymbol);
public:
    void DisplayErrorMessage(const CWideString&);   // 0x0055EF00
};

void CTCHeroOnlineScoreboard::AbortWithMessage(const CCharString& textSymbol)
{
    CWideString text = g_pEngineGlobals_013b86a0->m_pTextBank->GetTextBySymbol(textSymbol);
    DisplayErrorMessage(text);
    ClearScoreboard();
    ScoreboardState = 0;
    TaskState = 100;
    if (PHttp)
        PHttp->Close();
}