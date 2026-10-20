// CSubtitleRenderer::SetAsInCutscene(bool inCutscene) -- enter or leave cutscene subtitles: entering starts the skip-prompt fade and resets both button captions to their defaults, leaving notifies the game, closes the skip prompt, clears the text and drops every queued text
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049319C, 227 bytes, `ret 4`. Ego_r.pdb `?SetAsInCutscene@CSubtitleRenderer@@QAEX_N@Z`.
// Leaf: SetAsInCutscene
// Callees: PeekGuiDef @ 0x00438570 (`__fastcall`), CFadeInFadeOutBase::Start(float, float, CRGBColour) @ 0x00434840 (landed name),
//   CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("" @ 0x0122D70E), ~CCharString @ 0x0099EAE0,
//   CSubtitleRenderer::SetButton1OverrideText @ 0x0048F7C0 / SetButton2OverrideText @ 0x0048F881, CSubtitleRenderer::Clear(bool)
//   @ 0x00492BAB, deque<CSetTextInfo>::pop_front @ 0x00493CF5 (manifest label propagated), virtual slot 45 (+0xB4) of the
//   main game component's +0x24 -> +0x38 object.
// Globals: g_pPlayerGui @ 0x013B8790 (its +0x2BC flag follows !inCutscene), g_pMainGameComponent @ 0x013B86A0.
// Shape: frame-pointer size TU (as the landed CSubtitleRenderer rows): one zero register (bl) for the flag stores and tests,
//   the InCutscene member addressed once (`lea eax`) and re-read after the global store; the GUI def is a named local fetched
//   before the skip fade's by-value colour (user ctor R,G,B,A in its argument slot), 0.0f and the def's float; each "" caption
//   is an implicit-conversion temporary in the dead argument slot (re-addressed with `lea` after its ctor); the CRGBColour needs a user copy ctor for the in-slot build; the queue is emptied with a rotated `while (!empty()) pop();`.
// Retail layout (the landed CSubtitleRenderer rows, PDB - 8 for the flags): TextInfoQueue +0x34 (40-byte STLport deque:
//   start cursor +0x34, finish cursor +0x44), InCutscene +0xDE, CutsceneSkipped +0xDF, SkipCutsceneTimer +0xE8 (its Closing/Opening
//   bytes +0xF4 / +0xF5 set by an inline close: Opening = false, Closing = true). CPlayerGuiDef retail +0x2C4 = TextBoxCutsceneSecondsBeforeSkip (PDB +0x2D8).
struct CRGBColour {
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
struct CPlayerGuiDef {
    unsigned char _pad[0x2c4];
    float         TextBoxCutsceneSecondsBeforeSkip;   // +0x2c4 (retail)
};
extern CPlayerGuiDef* __fastcall PeekGuiDef();          // 0x00438570
struct CFadeInFadeOutBase {                            // landed layout (0x00644526)
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
struct CPlayerGui {
    unsigned char _pad[0x2bc];
    bool          NotInCutscene;                        // +0x2bc
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
struct CCutsceneListener {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044();
    virtual void OnCutsceneEnded();                     // slot 45 (+0xb4)
};
struct CMainGameSubsystem {
    unsigned char      _pad[0x38];
    CCutsceneListener* Listener;                        // +0x38
};
struct CMainGameComponent {
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
