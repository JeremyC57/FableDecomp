#ifndef FABLE_ENGINE_CMOVIESUBTITLES_H
#define FABLE_ENGINE_CMOVIESUBTITLES_H
// Retail CMovieSubtitles (CVideoSys::Subtitles +0x30; movie row 0x006286F0). Not polymorphic. Members: Ego_r.pdb
// (0x34 bytes with a Dinkumware list) at retail offsets with the STLport list (4 bytes), 0x2C in all: the CVideoSys
// layout puts SubtitlesEnabled right after it at +0x5C (read by 0x006286F0 and written by the CVideoSys constructor
// 0x00A3BC70), and CVideoSys::Init 0x00A3B9D0 stores pTextBank at Subtitles+0x04 (inline SetTextBank).
// Member functions: ??0 0x009BDBA0 (called by ??0CVideoSys 0x00A3BC70; bound ??0CMovieSubtitles@@QAE@XZ),
// ParseSubtitles 0x009BD460, Play 0x009BD910 (Ego_r + FableWin); SetTextBank / GetFrameNumber are FableWin-only (inline).
// The destructor is the implicit one (no Ego_r ??1; ~CVideoSys 0x00A3BC20 destroys the string and the list in place).
#include "rebuild_abi.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DBoxF.h"
namespace NGameText { class CDataBank; }
#ifdef FABLE_RETAIL_CMOVIESUBTITLES_GETCHARWIDTHW
#include "engine/CCountedPointer.h"
class CFontBank;
#endif
class CASoundManager;
#ifdef FABLE_RETAIL_CSUBTITLE_STRUCT
// Retail opt-in (NF132, q4100): Ego_r.pdb and FableWin.pdb key CSubTitle as a struct (`UCSubTitle@@`: ??0CSubTitle@@QAE@XZ,
// ?clear@?$list@UCSubTitle@@...); the list's out-of-line clear 0x009BE060 is ?clear@?$_List_base@UCSubTitle@@V?$allocator@UCSubTitle@@@std@@@std@@QAEXXZ.
#ifdef FABLE_RETAIL_MOVIE_PARSE_PASS29
#include "engine/NGameText_CDataBank.h"
struct CSubTitle {
    CIVCountedPointer<const NGameText::CTextEntry> TextEntry;
    CWideString FormattedText;
    CCharString Sound;
    long FrameStart;
    long FrameEnd;
    float StartTime;
    float EndTime;
    CSubTitle() {}
    CSubTitle(const CSubTitle& other);                  // 0x009BDE00
};

#else
struct CSubTitle;
#endif
#else
class CSubTitle;
#endif
class CMovieSubtitles {
public:
    CMovieSubtitles();                                                   // 0x009BDBA0 ??0CMovieSubtitles@@QAE@XZ
    void ParseSubtitles(const CWideString&);                             // 0x009BD460 ?ParseSubtitles@CMovieSubtitles@@QAEXABVCWideString@@@Z
#ifdef FABLE_RETAIL_MOVIE_PARSE_PASS29
    long GetFrameNumber(long mins,float secs) { float time=mins*60+secs; return (long)(Fps*time); }
#endif
    void Play(float,bool);                                               // 0x009BD910 ?Play@CMovieSubtitles@@QAEXM_N@Z
    void SetTextBank(const NGameText::CDataBank* bank) { pTextBank = bank; }   // FableWin ?SetTextBank@CMovieSubtitles@@QAEXPBVCDataBank@NGameText@@@Z
#ifdef FABLE_RETAIL_MOVIE_PARSE_PASS29
protected:
#endif
    std::list<CSubTitle>        SubTitles;                               // +0x00 (STLport list: 4 bytes)
    const NGameText::CDataBank* pTextBank;                               // +0x04
    long                        Fps;                                     // +0x08
    CASoundManager*             PSoundManager;                           // +0x0C
    bool                        IsSubtitleTime;                          // +0x10
    CWideString                 FormattedText;                           // +0x14
    C2DBoxF                     VSubtitleWindow;                         // +0x18
    long                        FrameCounter;                            // +0x28
#ifdef FABLE_RETAIL_CMOVIESUBTITLES_GETCHARWIDTHW
    // Opt-in (w23 e15, row 0x009BCEF0): Ego_r.pdb ?GetCharWidthW@CMovieSubtitles@@SII_W@Z (public static __fastcall; retail VC7.1
    // wide char = unsigned short -> SIIG) and the protected static CCountedPointer<CFontBank> PFont (Ego_d.pdb LF_STMEMBER; Ego_r.pdb
    // `dynamic initializer for 'CMovieSubtitles::PFont''), 0x013CA7F8 (also read by Play 0x009BD910, landed under the descriptive
    // extern g_pSubtitleFont). Statics only: no layout change.
#ifdef FABLE_RETAIL_MOVIE_PARSE_PASS29
public:
#endif
    static unsigned int __fastcall GetCharWidthW(unsigned short c);             // 0x009BCEF0
protected:
    static CCountedPointer<CFontBank> PFont;                             // 0x013CA7F8
public:
#endif
};
FABLE_STATIC_ASSERT_AT(32, sizeof(CMovieSubtitles) == 0x2c);
#endif // FABLE_ENGINE_CMOVIESUBTITLES_H
