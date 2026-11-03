#ifndef FABLE_ENGINE_CVIDEOSYS_H
#define FABLE_ENGINE_CVIDEOSYS_H
#ifdef FABLE_RETAIL_CVIDEOSYS
// Retail opt-in (movie row 0x006286F0): the retail class with its member functions. Ego_r.pdb CVideoSys (class V, no
// vtable, 0x94 bytes with a Dinkumware list in Subtitles) at retail offsets: 0x8C (CMovieSubtitles 0x2C with the STLport
// list; the operator new in 0x006286F0 allocates 0x8C before ??0CVideoSys 0x00A3BC70). Member names, types and access
// are the PDB's (GBuilder .. pTextBank public; CriticalSection .. Width private). Offsets read or written by retail code:
// MEvent +0x10 / TexRenderer +0x20 / SubtitlesEnabled +0x5C / pTextBank +0x60 / CriticalSection +0x64 / RenderEvent +0x7C /
// PRenderTexture +0x80 / Height +0x84 / Width +0x88 (0x006286F0), Subtitles +0x30 (0x006286F0, Init 0x00A3B9D0), the
// interface pointers +0x00..+0x1C and State +0x28 (the landed CVideoSys rows). PTYPE and _FilterState (strmif.h) are
// declared without enumerators: the repo's PDB extracts hold no enumerator names.
// Member functions: Ego_r.pdb + FableWin.pdb spellings; the accessors FableWin.pdb alone lists (the debug build) are
// inline, as 0x006286F0 reads those members in place. GetCriticalSection is a const member returning a non-const
// reference (FableWin ?GetCriticalSection@CVideoSys@@QBEAAU_RTL_CRITICAL_SECTION@@XZ): CriticalSection is `mutable`.
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/Win32Types.h"
#include "engine/CMovieSubtitles.h"
struct IGraphBuilder; struct IMediaControl; struct IMediaPosition; struct IMediaSeeking;
struct IMediaEvent; struct IBasicAudio; struct IBaseFilter; struct IFileSourceFilter;
class CTextureRenderer; class CTexture;
namespace NGameText { class CDataBank; }
enum _FilterState;                                                       // MS forward enum (strmif.h FILTER_STATE)
class CVideoSys {
public:
    enum PTYPE { };
    IGraphBuilder*              GBuilder;                                // +0x00
    IMediaControl*              MControl;                                // +0x04
    IMediaPosition*             MPosition;                               // +0x08
    IMediaSeeking*              MSeeking;                                // +0x0C
    IMediaEvent*                MEvent;                                  // +0x10
    IBasicAudio*                BasicAudio;                              // +0x14
    IBaseFilter*                PAsfReader;                              // +0x18
    IFileSourceFilter*          PFileSource;                             // +0x1C
    CTextureRenderer*           TexRenderer;                             // +0x20
    PTYPE                       PlayMode;                                // +0x24
    int                         State;                                   // +0x28
    bool                        RenderVideo;                             // +0x2C
    CVideoSys();                                                         // 0x00A3BC70 ??0CVideoSys@@QAE@XZ
    ~CVideoSys();                                                        // 0x00A3BC20 ??1CVideoSys@@QAE@XZ
    long Init(const CWideString&,CTexture*,bool);                        // 0x00A3B9D0 ?Init@CVideoSys@@QAEJABVCWideString@@PAVCTexture@@_N@Z
    bool AttemptToPlay();                                                // 0x00A3B1A0
    bool Play();                                                         // 0x00A3B130 ?Play@CVideoSys@@QAE_NXZ
    bool Pause();                                                        // 0x00A3B1C0
    bool Stop();                                                         // 0x00A3B1F0 ?Stop@CVideoSys@@QAE_NXZ
    bool IsFinished();                                                   // 0x00A3B330
    float GetLength();                                                   // 0x00A3B220
    float GetCurTime();                                                  // 0x00A3B260
    float GetProgress();                                                 // 0x00A3B2A0
    CTexture* GetTexture();                                              // 0x00A3B320
    void WaitForState(_FilterState);                                     // 0x00A3B0F0 ?WaitForState@CVideoSys@@QAEXW4_FilterState@@@Z
    void SetVolume(float);                                               // 0x00A3B920 ?SetVolume@CVideoSys@@QAEXM@Z
    bool Process();                                                      // 0x00A3B430
    void TogglePause(bool);                                              // 0x00A3B4A0
    void Close();                                                        // 0x00A3B380 ?Close@CVideoSys@@QAEXXZ
    void SetTextBank(const NGameText::CDataBank* bank) { pTextBank = bank; }
    void SetSubtitlesEnabled(bool enabled) { SubtitlesEnabled = enabled; }
    CMovieSubtitles             Subtitles;                               // +0x30
    bool                        SubtitlesEnabled;                        // +0x5C
    const NGameText::CDataBank* pTextBank;                               // +0x60
    _RTL_CRITICAL_SECTION& GetCriticalSection() const { return CriticalSection; }
    void*& GetRenderEvent() { return RenderEvent; }
    bool GetSubtitlesEnabled() const { return SubtitlesEnabled; }
    CMovieSubtitles& GetMovieSubtitles() { return Subtitles; }
    CTexture* GetPRenderTexture() { return PRenderTexture; }
    void SetPRenderTexture(CTexture* texture) { PRenderTexture = texture; }
    void SetHeight(long height) { Height = height; }
    long GetHeight() { return Height; }
    void SetWidth(long width) { Width = width; }
    long GetWidth() { return Width; }
private:
    mutable _RTL_CRITICAL_SECTION CriticalSection;                       // +0x64
    void*                       RenderEvent;                             // +0x7C
    CTexture*                   PRenderTexture;                          // +0x80
    long                        Height;                                  // +0x84
    long                        Width;                                   // +0x88
public:
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CVideoSys,TexRenderer)==0x20);
        FABLE_STATIC_ASSERT(offsetof(CVideoSys,Subtitles)==0x30);
        FABLE_STATIC_ASSERT(offsetof(CVideoSys,SubtitlesEnabled)==0x5c);
        FABLE_STATIC_ASSERT(offsetof(CVideoSys,pTextBank)==0x60);
        FABLE_STATIC_ASSERT(offsetof(CVideoSys,CriticalSection)==0x64);
        FABLE_STATIC_ASSERT(offsetof(CVideoSys,Width)==0x88);
    }
};
FABLE_STATIC_ASSERT(sizeof(CVideoSys) == 0x8c);
#else // flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CVideoSys = 0x94.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CVideoSys = 0x8c.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

struct CDataBank;
class CTexture;
struct CTextureRenderer;
struct IBaseFilter;
struct IBasicAudio;
struct IFileSourceFilter;
struct IGraphBuilder;
struct IMediaControl;
struct IMediaEvent;
struct IMediaPosition;
struct IMediaSeeking;
struct _RTL_CRITICAL_SECTION_DEBUG;

#pragma pack(push, 1)
struct CVideoSys {
    IGraphBuilder*               GBuilder;                   // +0x00
    IMediaControl*               MControl;                   // +0x04
    IMediaPosition*              MPosition;                  // +0x08
    IMediaSeeking*               MSeeking;                   // +0x0c
    IMediaEvent*                 MEvent;                     // +0x10
    IBasicAudio*                 BasicAudio;                 // +0x14
    IBaseFilter*                 PAsfReader;                 // +0x18
    IFileSourceFilter*           PFileSource;                // +0x1c
    CTextureRenderer*            TexRenderer;                // +0x20
    unsigned char                PlayMode[0x4];              // +0x24 PTYPE
    int                          State;                      // +0x28
    bool                         RenderVideo;                // +0x2c
    unsigned char                _pad_0x2d[0x3];             // +0x2d
    unsigned char                Subtitles[0x2c];            // +0x30 CMovieSubtitles
    bool                         SubtitlesEnabled;           // +0x5c
    unsigned char                _pad_0x5d[0x3];             // +0x5d
    CDataBank*                   pTextBank;                  // +0x60
    _RTL_CRITICAL_SECTION_DEBUG* CriticalSection_DebugInfo;  // +0x64 _RTL_CRITICAL_SECTION::DebugInfo
    long                         CriticalSection_LockCount;  // +0x68 _RTL_CRITICAL_SECTION::LockCount
    long                         CriticalSection_RecursionCount; // +0x6c _RTL_CRITICAL_SECTION::RecursionCount
    void*                        CriticalSection_OwningThread; // +0x70 _RTL_CRITICAL_SECTION::OwningThread
    void*                        CriticalSection_LockSemaphore; // +0x74 _RTL_CRITICAL_SECTION::LockSemaphore
    unsigned long                CriticalSection_SpinCount;  // +0x78 _RTL_CRITICAL_SECTION::SpinCount
    void*                        RenderEvent;                // +0x7c
    CTexture*                    PRenderTexture;             // +0x80
    long                         Height;                     // +0x84
    long                         Width;                      // +0x88
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CVideoSys) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, GBuilder) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, MControl) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, MPosition) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, MSeeking) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, MEvent) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, BasicAudio) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, PAsfReader) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, PFileSource) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, TexRenderer) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, PlayMode) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, State) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, RenderVideo) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, Subtitles) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, SubtitlesEnabled) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, pTextBank) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, CriticalSection_DebugInfo) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, CriticalSection_LockCount) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, CriticalSection_RecursionCount) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, CriticalSection_OwningThread) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, CriticalSection_LockSemaphore) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, CriticalSection_SpinCount) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, RenderEvent) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, PRenderTexture) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, Height) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CVideoSys, Width) == 0x88);

#endif // FABLE_RETAIL_CVIDEOSYS

#endif // FABLE_ENGINE_CVIDEOSYS_H
