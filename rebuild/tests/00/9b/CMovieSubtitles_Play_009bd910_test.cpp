// Behaviour test for 0x009BD910 CMovieSubtitles::Play: when the first subtitle's start time is reached it formats the text and plays
// its sound sample from the entry's bank; a sound name the bank does not know is retried as "SND_" (retail 0x01239ED4) + the name.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); the callees are defined
// under the row's own spellings, and the sound manager / 2-D emitter are vtable fixtures (slot 9 Get2DEmitter, slot 3 PlaySample).
// The test asserts the symbols the row looks up (the "SND_" retry included); a mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
// ---- row declarations (copied from the row source; its function definition left out) ----
// CMovieSubtitles::Play(float time, bool display) -- per-frame movie subtitle update: lay the subtitle window out in the bottom of the render target (10% side margins, 300..30 pixels up) in virtual pixels; when the front subtitle's start time is reached format its text into the window and play its "SND_" sample, when its end time is reached drop it; draw the current text centred when asked; count the frame
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BD910, 496 bytes, thiscall `ret 8`. PDB ?Play@CMovieSubtitles@@QAEXM_N@Z;
//   single caller 0x00628C56 (the movie player, subtitles at +0x30). Catalog label
//   "CEngineInternalPrimitiveBase_UpdateSubtitleDisplay" is descriptive.
// Sound manager (agentSND2, 2026-11-10): the member PSoundManager is a CASoundManager* of engine/CASoundManager.h -- GetPSoundBank(const
//   CCharString&) 0x009917E0 (FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP), slot 9 Get2DEmitter and the emitter's slot 3 PlaySample;
//   engine/CASoundBank.h GetSoundIndexFromSymbol and engine/CSoundInit.h (inline ctor opt-in); was a local CASoundManager /
//   CASoundPlayer / CSoundInit / CASoundBank model.
// Callees: GFGetSystemManager 0x009A4EC0, CDisplayManager::GetRenderTargetDimensions 0x009BEDC0 (hidden C2DExtentsI),
//   GFABoxToVBox 0x009E2020, NGameText::CTextEntry::GetText 0x009C7A40, FormatTextForVWindow 0x009BC9D0, ~CWideString
//   0x0099B510, the sound-bank lookup 0x009917E0 on PSoundManager (ICF-shared with CSymbolMap::FindFirstSymbolWithVal),
//   CCharString copy 0x0099EC30 / operator+ 0x0099F690 ("SND_" 0x01239ED4) / dtor 0x0099EAE0,
//   CASoundBank::GetSoundIndexFromSymbol 0x00A01920, PSoundManager virtual +0x24 then that object's virtual +0x0C
//   (bank, sample, 1, init, 0), list<CSubTitle>::pop_front 0x009BE100, DisplayCenteredText 0x009BCA20.
// Globals: the subtitle font bank pointer 0x013CA7F8; 0.1f 0x0122EE58.
// Layout: CMovieSubtitles / CSubTitle as the landed constructor 0x009BDBA0 (retail STLport list = the sentinel pointer);
//   NGameText::CTextEntry BankName +0x1c / SoundName +0x24 (landed 0x0066BA80); CSoundInit = Ego_r.pdb.
// Shape: GFGetSystemManager() is a named local (retail calls it before pushing the extents' hidden pointer); the screen box
//   is a named C2DBoxF built from two C2DVector temporaries (top-left, bottom-right) -- the 4-float constructor keeps the
//   width on the x87 stack where retail spills it and stores the bottom edge through a float temporary; the font reference
//   is a named local read before GetText(); CSoundInit's inline default constructor assigns in the 0x0041C930 order
//   (PlaybackType last) and GainFaderBits = 8 overrides its zero; size() is the STLport counting walk.
typedef unsigned short wchar_t;

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                         // 0x0099B510
};

class C2DExtentsI { public:
    long Width, Height;
    C2DExtentsI() { }
};
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF() { }
    C2DBoxF(const C2DVector& tl, const C2DVector& br) : TLX(tl.X), TLY(tl.Y), BRX(br.X), BRY(br.Y) { }
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};

class CDisplayManager {
public:
    const C2DExtentsI GetRenderTargetDimensions();          // 0x009BEDC0
};
class CSystemManager {
public:
    unsigned char _pad00[0x60];
    CDisplayManager* PDisplayManager;                       // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();            // 0x009A4EC0
void __fastcall GFABoxToVBox(const C2DBoxF& abox, C2DBoxF& vbox);   // 0x009E2020

class CFontBank;
extern CFontBank* g_pSubtitleFont;                          // 0x013CA7F8
bool __fastcall FormatTextForVWindow(const C2DBoxF& box, const CFontBank& font, const CWideString& text,
                                     CWideString& outText, C2DBoxF& outBox);   // 0x009BC9D0
void __fastcall DisplayCenteredText(const CFontBank& font, const CWideString& text, const C2DBoxF& box);   // 0x009BCA20

namespace NGameText {
class CTextEntry {
public:
    unsigned char _pad00[0x1c];
    CCharString BankName;                                   // +0x1c
    unsigned char _pad20[0x24 - 0x20];
    CCharString SoundName;                                  // +0x24
    CWideString GetText() const;                            // 0x009C7A40
};
}
template <class T> class CIVCountedPointer {
public:
    T* Data;
    T* operator->() const { return Data; }
};

struct CSubTitle {                                          // 0x1c bytes
    CIVCountedPointer<const NGameText::CTextEntry> TextEntry;   // +0x00
    CWideString FormattedText;                              // +0x04
    CCharString Sound;                                      // +0x08
    long        FrameStart;                                 // +0x0c
    long        FrameEnd;                                   // +0x10
    float       StartTime;                                  // +0x14
    float       EndTime;                                    // +0x18
};

namespace std {
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> class list {
public:
    _List_node<T>* _M_node;
    unsigned int size() const
    {
        unsigned int n = 0;
        for (_List_node<T>* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    T& front() { return _M_node->_M_next->_M_data; }
    void pop_front();                                       // 0x009BE100
};
}

#define FABLE_RETAIL_CASOUNDBANK_SOUNDINDEX
#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
#define FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR
#include "engine/CASoundBank.h"
#include "engine/CSoundInit.h"
#include "engine/CASoundManager.h"
class CDataBank;

class CMovieSubtitles {
public:
    std::list<CSubTitle> SubTitles;                         // +0x00
    const CDataBank*     pTextBank;                         // +0x04
    long                 Fps;                               // +0x08
    CASoundManager*      PSoundManager;                     // +0x0c
    bool                 IsSubtitleTime;                    // +0x10
    CWideString          FormattedText;                     // +0x14
    C2DBoxF              VSubtitleWindow;                   // +0x18
    long                 FrameCounter;                      // +0x28
    void Play(float time, bool display);
};


// ---- end of row declarations ----
static char g_log[256]; static int g_bad = 0; static char g_cat[64];
CCharString::CCharString(const CCharString& other) { PStringData = other.PStringData; }
CCharString::~CCharString() { }
CWideString::~CWideString() { }
CCharString __fastcall operator+(const char* left, const CCharString& right)
{ strcpy(g_cat, left); strcat(g_cat, (const char*)right.PStringData); CCharString r(right); r.PStringData = g_cat; return r; }
static double g_dispRaw[1]; static double g_sysRaw[0x68 / 8];
CSystemManager* __fastcall GFGetSystemManager() { return (CSystemManager*)g_sysRaw; }
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions() { C2DExtentsI d; d.Width = 1000; d.Height = 800; return d; }
void __fastcall GFABoxToVBox(const C2DBoxF& abox, C2DBoxF& vbox) { vbox = abox; }
CFontBank* g_pSubtitleFont = (CFontBank*)g_dispRaw;
static unsigned short kText[] = { 'H', 'i', 0 };
CWideString NGameText::CTextEntry::GetText() const { CWideString w; w.PStringData = kText; return w; }
bool __fastcall FormatTextForVWindow(const C2DBoxF& box, const CFontBank&, const CWideString& text, CWideString& outText, C2DBoxF&)
{ outText.PStringData = text.PStringData; if (box.TLX != 100.0f || box.BRY != 770.0f) g_bad = 1; strcat(g_log, "format;"); return true; }
void __fastcall DisplayCenteredText(const CFontBank&, const CWideString& text, const C2DBoxF&) { if (text.PStringData != kText) g_bad = 1; strcat(g_log, "display;"); }
static double g_bankRaw[4];
CASoundBank* CASoundManager::GetPSoundBank(const CCharString& name) const { strcat(g_log, "bank["); strcat(g_log, (const char*)name.PStringData); strcat(g_log, "]"); return (CASoundBank*)g_bankRaw; }
long CASoundBank::GetSoundIndexFromSymbol(const CCharString& name) const
{ strcat(g_log, "index["); strcat(g_log, (const char*)name.PStringData); strcat(g_log, "]"); return strncmp((const char*)name.PStringData, "SND_", 4) == 0 ? 5 : 0; }
template <> void std::list<CSubTitle>::pop_front() { strcat(g_log, "pop;"); }
// sound manager vtable slot 9 (Get2DEmitter) and emitter slot 3 (PlaySample): thiscall-compatible __fastcall fixtures
static void* gEmitterVtbl[8]; static void* gEmitter[2] = { gEmitterVtbl, 0 }; static void* g_mgrVtbl[16];
extern "C" void* __fastcall NF420_Get2DEmitter(void*, int) { return gEmitter; }
extern "C" unsigned long __fastcall NF420_PlaySample(void*, int, void* bank, long sample, bool loop, const void*, bool last)
{ char b[48]; sprintf(b, "play[%d,%ld,%d,%d]", bank == (void*)g_bankRaw, sample, loop, last); strcat(g_log, b); return 1; }
extern "C" void __cdecl NF420_VirtualTrap() { NF420TrapHit("sound virtual"); }
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    for (int i = 0; i < 16; ++i) g_mgrVtbl[i] = (void*)NF420_VirtualTrap;
    for (int i = 0; i < 8; ++i) gEmitterVtbl[i] = (void*)NF420_VirtualTrap;
    g_mgrVtbl[9] = (void*)NF420_Get2DEmitter; gEmitterVtbl[3] = (void*)NF420_PlaySample;
    void* mgr[0x50 / 4] = { g_mgrVtbl };
    *(void**)((unsigned char*)g_sysRaw + 0x60) = g_dispRaw;
    static char bankName[] = "BANK_A", soundName[] = "VOICE_1";
    static unsigned char entry[0x28]; *(char**)(entry + 0x1c) = bankName; *(char**)(entry + 0x24) = soundName;
    static unsigned char node[0x8 + 0x1c]; static void* sentinel[2];
    sentinel[0] = node; sentinel[1] = node; *(void**)node = sentinel; *(void**)(node + 4) = sentinel;
    *(void**)(node + 8) = entry; *(float*)(node + 8 + 0x14) = 1.0f; *(float*)(node + 8 + 0x18) = 9.0f;
    static unsigned char subsRaw[0x30]; CMovieSubtitles* subs = (CMovieSubtitles*)subsRaw;
    *(void**)subsRaw = sentinel; subs->PSoundManager = (CASoundManager*)mgr; subs->IsSubtitleTime = false; subs->FrameCounter = 0;
    subs->Play(2.0f, true);
    bool ok = strcmp(g_log, "format;bank[BANK_A]index[VOICE_1]index[SND_VOICE_1]play[1,5,1,0]display;") == 0 && subs->IsSubtitleTime && subs->FrameCounter == 1;
    g_log[0] = 0; subs->Play(10.0f, false);
    ok = ok && strcmp(g_log, "pop;") == 0 && !subs->IsSubtitleTime && subs->FrameCounter == 2 && !g_bad;
    if (ok) printf("009bd910_TEST PASS\n");
    else printf("FAIL log=%s bad=%d\n", g_log, g_bad);
    return ok ? 0 : 1;
}
