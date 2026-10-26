// Behaviour test for CSubtitleRenderer::SetAsInCutscene (0x0049319C): entering starts the skip fade and resets both captions; leaving notifies, closes the fade, clears and drains the queue.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }
    CRGBColour(const CRGBColour& other) : B(other.B), G(other.G), R(other.R), A(other.A) {}
};
class CPlayerGuiDef { public:
    unsigned char _pad[0x2c4];
    float         TextBoxCutsceneSecondsBeforeSkip;   // +0x2c4 (retail)
};
extern CPlayerGuiDef* __fastcall PeekGuiDef();          // 0x00438570
class CFadeInFadeOutBase { public:                            // landed layout (0x00644526)
    bool       Active;                                  // +0x00
    float      FadeInTime;                              // +0x04
    float      FadeOutTime;                             // +0x08
    bool       Closing;                                 // +0x0c
    bool       Opening;                                 // +0x0d
    float      OpenTimer;                               // +0x10
    float      CloseTimer;                              // +0x14
    CRGBColour ToColour;                                // +0x18
    void Start(float fadeInTime, float fadeOutTime, CRGBColour toColour);   // 0x00434840
    void BeginClose() { Opening = false; Closing = true; }   // inline: fade back out
};
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CPlayerGui { public:
    unsigned char _pad[0x2bc];
    bool          NotInCutscene;                        // +0x2bc
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
struct CCutsceneListener {
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {}
    virtual void v006() {} virtual void v007() {} virtual void v008() {} virtual void v009() {} virtual void v010() {} virtual void v011() {}
    virtual void v012() {} virtual void v013() {} virtual void v014() {} virtual void v015() {} virtual void v016() {} virtual void v017() {}
    virtual void v018() {} virtual void v019() {} virtual void v020() {} virtual void v021() {} virtual void v022() {} virtual void v023() {}
    virtual void v024() {} virtual void v025() {} virtual void v026() {} virtual void v027() {} virtual void v028() {} virtual void v029() {}
    virtual void v030() {} virtual void v031() {} virtual void v032() {} virtual void v033() {} virtual void v034() {} virtual void v035() {}
    virtual void v036() {} virtual void v037() {} virtual void v038() {} virtual void v039() {} virtual void v040() {} virtual void v041() {}
    virtual void v042() {} virtual void v043() {} virtual void v044() {}
    virtual void OnCutsceneEnded() {}                   // slot 45 (+0xb4)
};
struct CMainGameSubsystem {
    unsigned char      _pad[0x38];
    CCutsceneListener* Listener;                        // +0x38
};
class CMainGameComponent { public:
    unsigned char       _pad[0x24];
    CMainGameSubsystem* Subsystem;                      // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

struct CSetTextInfo;
struct CDequeIterator { CSetTextInfo* _M_cur; CSetTextInfo* _M_first; CSetTextInfo* _M_last; CSetTextInfo** _M_node; };
struct CSetTextInfoQueue {                              // queue<CSetTextInfo, deque<CSetTextInfo> >: 40 bytes in retail
    CDequeIterator  _M_start;                           // +0x00
    CDequeIterator  _M_finish;                          // +0x10
    CSetTextInfo**  _M_map;                             // +0x20
    unsigned long   _M_map_size;                        // +0x24
    bool empty() const { return _M_finish._M_cur == _M_start._M_cur; }
    void pop();                                         // 0x00493CF5 (deque::pop_front)
};

class CSubtitleRenderer {
public:
    void*              __vftable;                       // +0x00
    unsigned char      _pad04[0x34 - 0x04];
    CSetTextInfoQueue  TextInfoQueue;                   // +0x34
    unsigned char      _pad5c[0xde - 0x5c];
    bool               InCutscene;                      // +0xde
    bool               CutsceneSkipped;                 // +0xdf
    unsigned char      _padE0[0xe8 - 0xe0];
    CFadeInFadeOutBase SkipCutsceneTimer;               // +0xe8 (Closing +0xf4, Opening +0xf5)
    void SetButton1OverrideText(const CCharString& text);   // 0x0048F7C0
    void SetButton2OverrideText(const CCharString& text);   // 0x0048F881
    void Clear(bool flag);                              // 0x00492BAB
    void SetAsInCutscene(bool inCutscene);
};

void CSubtitleRenderer::SetAsInCutscene(bool inCutscene)
{
    InCutscene = inCutscene;
    CutsceneSkipped = false;
    g_pPlayerGui->NotInCutscene = !inCutscene;
    if (InCutscene) {
        const CPlayerGuiDef* gui = PeekGuiDef();
        SkipCutsceneTimer.Start(gui->TextBoxCutsceneSecondsBeforeSkip, 0.0f, CRGBColour(0, 0, 0, 255));
        SetButton1OverrideText("");
        SetButton2OverrideText("");
    } else {
        g_pMainGameComponent->Subsystem->Listener->OnCutsceneEnded();
        SkipCutsceneTimer.BeginClose();
        Clear(false);
        while (!TextInfoQueue.empty())
            TextInfoQueue.pop();
    }
}
static CPlayerGuiDef g_def;
CPlayerGuiDef* __fastcall PeekGuiDef() { return &g_def; }
static float g_in, g_out; static CRGBColour g_colour(1, 2, 3, 4); static int g_starts = 0, g_live = 0, g_b1 = 0, g_b2 = 0, g_clears = 0, g_pops = 0, g_notifies = 0;
void CFadeInFadeOutBase::Start(float in, float out, CRGBColour c) { ++g_starts; g_in = in; g_out = out; g_colour = c; }
CCharString::CCharString(const char* text, long) { ++g_live; PStringData = (void*)text; }
CCharString::~CCharString() { --g_live; }
void CSubtitleRenderer::SetButton1OverrideText(const CCharString& t) { if (((const char*)t.PStringData)[0] == 0) ++g_b1; }
void CSubtitleRenderer::SetButton2OverrideText(const CCharString& t) { if (((const char*)t.PStringData)[0] == 0) ++g_b2; }
void CSubtitleRenderer::Clear(bool flag) { if (!flag) ++g_clears; }
void CSetTextInfoQueue::pop() { ++g_pops; _M_start._M_cur = (CSetTextInfo*)((char*)_M_start._M_cur + 4); }
struct TestListener : CCutsceneListener { void OnCutsceneEnded() { ++g_notifies; } };
static CPlayerGui g_gui; CPlayerGui* g_pPlayerGui = &g_gui;
static CMainGameComponent g_mgc; CMainGameComponent* g_pMainGameComponent = &g_mgc;
int main()
{
    TestListener listener; CMainGameSubsystem sub; sub.Listener = &listener; g_mgc.Subsystem = &sub;
    g_def.TextBoxCutsceneSecondsBeforeSkip = 2.5f;
    static unsigned char mem[sizeof(CSubtitleRenderer)]; CSubtitleRenderer& r = *(CSubtitleRenderer*)mem;
    r.CutsceneSkipped = true;
    r.SetAsInCutscene(true);
    bool ok = r.InCutscene && !r.CutsceneSkipped && !g_gui.NotInCutscene && g_starts == 1 && g_in == 2.5f && g_out == 0.0f
        && g_colour.R == 0 && g_colour.G == 0 && g_colour.B == 0 && g_colour.A == 255 && g_b1 == 1 && g_b2 == 1 && g_live == 0
        && g_notifies == 0;
    char items[12];
    r.TextInfoQueue._M_start._M_cur = (CSetTextInfo*)items; r.TextInfoQueue._M_finish._M_cur = (CSetTextInfo*)(items + 12);
    r.SkipCutsceneTimer.Opening = true; r.SkipCutsceneTimer.Closing = false; r.CutsceneSkipped = true;
    r.SetAsInCutscene(false);
    ok = ok && !r.InCutscene && !r.CutsceneSkipped && g_gui.NotInCutscene && g_notifies == 1 && g_clears == 1 && g_pops == 3
        && !r.SkipCutsceneTimer.Opening && r.SkipCutsceneTimer.Closing && g_starts == 1 && g_b1 == 1;
    if (!ok) { std::printf("0049319c_TEST FAIL\n"); return 1; }
    std::printf("0049319c_TEST PASS\n");
    return 0;
}