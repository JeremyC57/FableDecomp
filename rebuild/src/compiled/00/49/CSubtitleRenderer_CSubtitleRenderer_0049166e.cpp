// CSubtitleRenderer::CSubtitleRenderer(CGraphicDataBank*) -- subtitle renderer constructor: CBase root, empty strings and text queue, 9 + 3 primitive handles, empty embedded lists, default flags, two fade timers
// Leaf: ??0CSubtitleRenderer
// Retail 0x0049166E, 327 bytes, `ret 4`, no EH frame. Callees: CBase::CBase 0x0099A2F0 (root vtable 0x0129A7C4),
// CWideString::CWideString() 0x0099AED0 (labelled CCharString), queue<CSetTextInfo>::queue 0x00494168 (labelled
// CGameEvent::AddUBYTE), vector<CEnginePrimitiveHandle>::vector(size_type) 0x00485D30 (labelled map::operator[]),
// CFadeInFadeOutBase-derived timer ctor 0x00436560 (labelled CTCHeroStats::SoberUp). Vtable 0x012384BC (rtti-vtables.tsv).
// Layout: engine/CSubtitleRenderer.h is the PDB layout (16-byte vectors, a 0x18-byte queue, no CBase-derived gap); retail
// sits higher: the queue is 0x28 bytes (+0x34..+0x5C) and the vectors are 12 bytes, so a local model is used:
// TextCharactersShowing +0x24, TextGroupHasEntriesRemaining +0x28, TextGroupIndex +0x2C, TextGroupTag +0x30, TextInfoQueue
// +0x34, the seven strings +0x5C..+0x74, alphas +0x78/+0x79, DisplayFlags +0x7C, PFont +0x80, Graphics +0x84,
// MaxDisplayTime +0x88, TextTimer +0x8C, PlayerModeAdded +0x90, the six vectors +0x94..+0xD0, twelve flags +0xDC..+0xE7
// (Button1TextOverridden +0xE3 is left uninitialised by retail), SkipCutsceneTimer +0xE8, ButtonPulseTimer +0x104.
// Shape: a size-optimised TU (`#pragma optimize("s", on)`: `or [esi+2Ch],-1`, `fldz ; fstp` float zeros, immediate byte
// stores); the four empty vectors are the STLport `_Vector_base` + `_STLP_alloc_proxy` model (`lea eax ; mov [eax],ebx` x3).
#pragma optimize("s", on)
struct CGraphicDataBank;
struct CFontBank;
struct CEnginePrimitiveHandle;
struct CEmbeddedGraphic;
struct CEmbeddedHighlight;
struct CWideStringData;

struct CBase {
    virtual void v00();
    CBase();                                                  // 0x0099A2F0
};
struct CWideString {
    CWideStringData* PStringData;
    CWideString();                                            // 0x0099AED0
};
struct CSetTextInfoQueue {
    unsigned char _body[0x28];
    CSetTextInfoQueue();                                      // 0x00494168
};
struct CEnginePrimitiveHandleVector {
    void* m_first; void* m_last; void* m_end;
    CEnginePrimitiveHandleVector(unsigned long count);        // 0x00485D30
};
template <class T> struct CAllocator {                         // STLport allocator<T>: empty
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> {         // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {                         // STLport _Vector_base<T, allocator<T> >
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template <class T> struct CRetailVector : CVectorBase<T> {      // STLport vector<T> (12 bytes)
    CRetailVector() : CVectorBase<T>(CAllocator<T>()) { }
};
struct CFadeTimer {
    unsigned char _body[0x1c];
    CFadeTimer();                                             // 0x00436560
};

struct CSubtitleRenderer : CBase {
    float                                 VWindow[4];                       // +0x04
    float                                 VTelevisionWindow[4];             // +0x14
    long                                  TextCharactersShowing;            // +0x24
    bool                                  TextGroupHasEntriesRemaining;     // +0x28
    long                                  TextGroupIndex;                   // +0x2c
    CWideString                           TextGroupTag;                     // +0x30
    CSetTextInfoQueue                     TextInfoQueue;                    // +0x34
    CWideString                           Text;                             // +0x5c
    CWideString                           NameText;                         // +0x60
    CWideString                           Button1Text;                      // +0x64
    CWideString                           Button2Text;                      // +0x68
    CWideString                           Button3Text;                      // +0x6c
    CWideString                           Button1OverrideText;              // +0x70
    CWideString                           Button2OverrideText;              // +0x74
    unsigned char                         Button1Alpha;                     // +0x78
    unsigned char                         Button2Alpha;                     // +0x79
    unsigned long                         DisplayFlags;                     // +0x7c
    CFontBank*                            PFont;                            // +0x80
    CGraphicDataBank*                     Graphics;                         // +0x84
    float                                 MaxDisplayTime;                   // +0x88
    float                                 TextTimer;                        // +0x8c
    long                                  PlayerModeAdded;                  // +0x90
    CEnginePrimitiveHandleVector          PrimitiveHandles;                 // +0x94
    CEnginePrimitiveHandleVector          ButtonPrimitiveHandles;           // +0xa0
    CRetailVector<CEnginePrimitiveHandle> EmbeddedPrimitiveHandles;         // +0xac
    CRetailVector<CEmbeddedGraphic>       EmbeddedGraphics;                 // +0xb8
    CRetailVector<CEmbeddedHighlight>     EmbeddedHighlights;               // +0xc4
    CRetailVector<CWideString>            EmbeddedHighlightActionNames;     // +0xd0
    bool                                  PausedGame;                       // +0xdc
    bool                                  AllCharactersDisplayed;           // +0xdd
    bool                                  InCutscene;                       // +0xde
    bool                                  CutsceneSkipped;                  // +0xdf
    bool                                  CutsceneSkippable;                // +0xe0
    bool                                  CutsceneSkippableWhilePaused;     // +0xe1
    bool                                  CutsceneActionMode;               // +0xe2
    bool                                  Button1TextOverridden;            // +0xe3
    bool                                  Button2TextOverridden;            // +0xe4
    bool                                  DisplayingText;                   // +0xe5
    bool                                  UpdateWhilePaused;                // +0xe6
    bool                                  UpdateTimer;                      // +0xe7
    CFadeTimer                            SkipCutsceneTimer;                // +0xe8
    CFadeTimer                            ButtonPulseTimer;                 // +0x104
    virtual void v00();
    CSubtitleRenderer(CGraphicDataBank* graphics);
};

CSubtitleRenderer::CSubtitleRenderer(CGraphicDataBank* graphics)
    : TextCharactersShowing(0), TextGroupHasEntriesRemaining(false), TextGroupIndex(-1),
      Button1Alpha(0xff), Button2Alpha(0xff), DisplayFlags(0), PFont(0), Graphics(graphics),
      MaxDisplayTime(0.0f), TextTimer(0.0f), PlayerModeAdded(0),
      PrimitiveHandles(9), ButtonPrimitiveHandles(3),
      PausedGame(false), AllCharactersDisplayed(false), InCutscene(false), CutsceneSkipped(false),
      CutsceneSkippable(true), CutsceneSkippableWhilePaused(false), CutsceneActionMode(false),
      Button2TextOverridden(false), DisplayingText(false), UpdateWhilePaused(false), UpdateTimer(true)
{
}