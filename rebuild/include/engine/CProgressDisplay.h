#ifndef FABLE_ENGINE_CPROGRESSDISPLAY_H
#define FABLE_ENGINE_CPROGRESSDISPLAY_H
// New header (chunk 890, row 0x00498490 CProgressDisplay::DrawRetailDisplay). Ego_r.pdb CProgressDisplay (struct_layouts_egor.tsv,
// 0x98 with 16-byte Dinkumware vectors) at the retail offsets: STLport vector 0xC, the doubles 8-aligned, sizeof 0x88 (the landed
// constructor 0x00499CE0 / destructor 0x00499FB0 / StopProgress 0x00499D90 / RenderProgress 0x00499880 read every member at these
// offsets; fable_boot.h keeps the older descriptive stand-in). Base CAProgressDisplay (vptr +0), vtable 0x012388B4: slot 0 the
// deleting dtor 0x0049B7D0, 1 StartProgress 0x00499AA0, 2 StopProgress 0x00499D90, 3 DisplayProgress 0x00499C50,
// 4 InitialiseTextures 0x0049A0B0, 5 SetToDisplayText 0x00499A70, 6 IsActive 0x0049B460 (rtti-vtables.tsv).
// Member functions: FableWin.pdb spellings (0x018AD6E0 .. 0x018B5330, the CProgressDisplay TU), retail addresses in TU order
// (WordWrapText 0x00497A60, GetPTextBank 0x00497B30, CalculateNextTextTag 0x00497B60, FormatTextForVWindow 0x00497F80,
// DisplayCenteredText 0x00497FD0, GetCharWidthW 0x00498410, DrawRetailDisplay 0x00498490, RenderProgress 0x00499880).
// Static PFont: ?PFont@CProgressDisplay@@... 0x013B8998 (FableWin / Ego_r "dynamic initializer for 'CProgressDisplay::PFont'").
// Needs FABLE_RETAIL_CAPROGRESSDISPLAY (the polymorphic base) and FABLE_RETAIL_CTEXTURE (the 8-byte CTexture with its inline dtor).
#ifndef FABLE_RETAIL_CAPROGRESSDISPLAY
#define FABLE_RETAIL_CAPROGRESSDISPLAY
#endif
#include "engine/CAProgressDisplay.h"
#include "engine/CTexture.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
class CFontBank; class C2DBoxF;
namespace NGameText { class CDataBank; }
class CProgressDisplay : public CAProgressDisplay {
public:
    CProgressDisplay();                                                  // 0x00499CE0 ??0CProgressDisplay@@QAE@XZ
    virtual ~CProgressDisplay();                                         // 0x00499FB0
    virtual void StartProgress(bool, bool, bool, bool, bool, bool);      // 0x00499AA0
    virtual void StopProgress(bool, bool);                               // 0x00499D90
    virtual void DisplayProgress(const CCharString& text_tag, float progress, bool, bool);   // 0x00499C50
    virtual void InitialiseTextures(bool, bool);                         // 0x0049A0B0
    virtual void SetToDisplayText(bool);                                 // 0x00499A70
    virtual bool IsActive() const;                                       // 0x0049B460
private:
    void DrawDebugDisplay(const CCharString& text_tag, float progress);
    void DrawRetailDisplay(const CCharString& text_tag, float progress, bool use_progress);   // 0x00498490
    void RenderProgress(const CCharString& text_tag, float progress, bool, bool);            // 0x00499880
    void CalculateNextTextTag();                                         // 0x00497B60
    const NGameText::CDataBank* GetPTextBank() const;                    // 0x00497B30
    CWideString WordWrapText(const C2DBoxF& window, const CFontBank& font, const CWideString& text) const;   // 0x00497A60
    bool FormatTextForVWindow(const C2DBoxF& window, const CFontBank& font, const CWideString& text,
                              CWideString& formatted_text, C2DBoxF& formatted_window) const;   // 0x00497F80
    void DisplayCenteredText(const CFontBank& font, const CWideString& text, const C2DBoxF& window) const;   // 0x00497FD0
    static unsigned int __fastcall GetCharWidthW(wchar_t c);             // 0x00498410
    static CCountedPointer<CFontBank> PFont;                             // 0x013B8998
    std::vector<CTexture> ProgressTextures;                              // +0x04
    std::vector<CTexture> GuildSealTextures;                             // +0x10
    std::vector<CTexture> LoadingScreenTextures;                         // +0x1C
    long LoadingScreenIndex;                                             // +0x28
    std::vector<CCharString> TextTags;                                   // +0x2C
    std::vector<CCharString> TextTagsPC;                                 // +0x38
    long TextTagIndex;                                                   // +0x44
    CWideString Text;                                                    // +0x48
    CCharString TextTag;                                                 // +0x4C
    CCharString LastTextDisplayed;                                       // +0x50
    long GuildSealFrameIndex;                                            // +0x54
    long StageCount;                                                     // +0x58
    double LastUpdateTime;                                               // +0x60
    double LastScreenUpdate;                                             // +0x68
    double LastProgressUpdate;                                           // +0x70
    bool DebugDisplay;                                                   // +0x78
    bool Initialised;                                                    // +0x79
    bool FullScreen;                                                     // +0x7A
    bool BlackScreen;                                                    // +0x7B
    bool DisplayText;                                                    // +0x7C
    bool DisplayResetProgressTipText;                                    // +0x7D
    bool ForRelaunch;                                                    // +0x7E
    bool ForFrontEnd;                                                    // +0x7F
    bool ForFileInstaller;                                               // +0x80
    bool FileInstallerFinished;                                          // +0x81
    bool IgnoreNextBeginRenderCall;                                      // +0x82
    bool IgnoreNextEndRenderCall;                                        // +0x83
};
FABLE_STATIC_ASSERT(sizeof(CProgressDisplay) == 0x88);
#endif // FABLE_ENGINE_CPROGRESSDISPLAY_H
