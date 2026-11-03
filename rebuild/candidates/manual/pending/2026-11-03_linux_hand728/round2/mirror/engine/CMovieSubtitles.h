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
class CASoundManager;
class CSubTitle;
class CMovieSubtitles {
public:
    CMovieSubtitles();                                                   // 0x009BDBA0 ??0CMovieSubtitles@@QAE@XZ
    void ParseSubtitles(const CWideString&);                             // 0x009BD460 ?ParseSubtitles@CMovieSubtitles@@QAEXABVCWideString@@@Z
    void Play(float,bool);                                               // 0x009BD910 ?Play@CMovieSubtitles@@QAEXM_N@Z
    void SetTextBank(const NGameText::CDataBank* bank) { pTextBank = bank; }   // FableWin ?SetTextBank@CMovieSubtitles@@QAEXPBVCDataBank@NGameText@@@Z
    std::list<CSubTitle>        SubTitles;                               // +0x00 (STLport list: 4 bytes)
    const NGameText::CDataBank* pTextBank;                               // +0x04
    long                        Fps;                                     // +0x08
    CASoundManager*             PSoundManager;                           // +0x0C
    bool                        IsSubtitleTime;                          // +0x10
    CWideString                 FormattedText;                           // +0x14
    C2DBoxF                     VSubtitleWindow;                         // +0x18
    long                        FrameCounter;                            // +0x28
};
FABLE_STATIC_ASSERT(sizeof(CMovieSubtitles) == 0x2c);
#endif // FABLE_ENGINE_CMOVIESUBTITLES_H
