// CActionTalkToThing::CActionTalkToThing(CThing&, const char*, const char*, const CCharStringValue&, const CCharStringValue&, const CCharStringValue&, bool, bool) -- initialize a question-talk action, retain its three button strings, and load its subtitle messages
// Retail: Fable.exe 0x00717d40, length 267; `ret 20h`.
// Callees: CActionTalkBase::CActionTalkBase @ 0x00717950; CCharString copy construction @ 0x0099ec30; GetMessagesFromTextName @ 0x004cebb0; CArray<CSubtitledMessage>::operator= @ 0x0071a160; CSubtitledMessage copy construction @ 0x004cdb70; CSubtitledMessage destruction @ 0x004d1db0; global CPlayerGui @ 0x013b8790.
// Native vtable slots are base [dtor, Update, OnActivate, OnAdd, pure GetActionName() const, OnCleanup] and derived [dtor, Update, OnActivate, OnAdd, GetActionName() const, OnCleanup, DisplayText]. Retail CSubtitledMessage is 0x44 bytes: the three donor 0x14-byte vector maps are 0x10-byte native maps. The base message array is at +0x30; button strings are at +0x94/+0x98/+0x9c.
#include "fable_string.h"
#define CPlayerGui CPlayerGui_DonorPDB
#include "engine/CPlayerGui.h"
#undef CPlayerGui
#include "engine/CSubtitleRenderer.h"

namespace NTCScriptedControl {
struct CThing;
void __cdecl operator_delete(void* p); // 0x00bfea14
struct CSubtitledMessage { unsigned char nativeFields[0x44]; CSubtitledMessage(); CSubtitledMessage(const CSubtitledMessage&); ~CSubtitledMessage(); };
struct CSubtitledMessageArray {
    CSubtitledMessage* first; CSubtitledMessage* last; CSubtitledMessage* end;
    CSubtitledMessageArray() : first(0), last(0), end(0) {}
    ~CSubtitledMessageArray() { CSubtitledMessage* e=last; CSubtitledMessage* p=first; for (; p!=e; ++p) p->~CSubtitledMessage(); if (first) operator_delete(first); }
    CSubtitledMessageArray& operator=(const CSubtitledMessageArray& other);
    void resize(unsigned long, const CSubtitledMessage&); // 0x0071a5c0
};

// One-dword retail ABI view over the real fable_string.h CCharString. The derived copy
// constructor is out-of-line in retail and preserves the native counted-string lifetime.
struct CCharStringValue : ::CCharString {
    CCharStringValue() : ::CCharString() {}
    CCharStringValue(const CCharStringValue& other); // retail copy ctor @ 0x0099ec30
};

struct CActionTalkBase {
    virtual ~CActionTalkBase(); // scalar deleting dtor slot 0, thunk 0x0071a530
    virtual void Update();      // slot 1, 0x00716de0
    virtual void OnActivate(); // slot 2, 0x00717710
    virtual void OnAdd();      // slot 3, inherited by derived
    virtual ::CCharString GetActionName() const = 0; // slot 4, pure in base
    virtual void OnCleanup();  // slot 5, inherited by derived
    unsigned char pad[0x2c];
    CSubtitledMessageArray Messages;
    unsigned char tail[0x54];
    CActionTalkBase(CThing*, bool, bool, bool);
};
struct CActionTalkToThing : CActionTalkBase {
    bool AddedDuringCutscene;
    bool AskingHeroQuestion;
    bool DisplayOverScreenFade;
    CCharStringValue QuestionButton1;
    CCharStringValue QuestionButton2;
    CCharStringValue QuestionButton3;
    virtual void Update();              // override slot 1
    virtual void OnActivate();          // override slot 2
    virtual ::CCharString GetActionName() const; // override slot 4
    virtual void DisplayText();         // new slot 6, 0x007132c0
    CActionTalkToThing(CThing&, const char*, const char*, const CCharStringValue&, const CCharStringValue&, const CCharStringValue&, bool, bool);
};
void __fastcall GetMessagesFromTextName(CSubtitledMessageArray*, const char*, const char*, long); // 0x004cebb0
// The PDB headers place CPlayerGui::SubtitleRenderer at +0x18 and
// CSubtitleRenderer::InCutscene at +0xe6, or composite donor offset +0xfe.
// Native storage is eight bytes smaller before that flag, so this field-free
// byte view reads retail +0xf6 without inventing a direct PDB member.
enum {
    NATIVE_PLAYERGUI_INCUTSCENE_OFFSET =
        offsetof(CPlayerGui_DonorPDB, SubtitleRenderer) + offsetof(CSubtitleRenderer, InCutscene) - 8
};
#if defined(_MSC_VER)
typedef char check_playergui_subtitle_renderer_offset[(offsetof(CPlayerGui_DonorPDB, SubtitleRenderer) == 0x18) ? 1 : -1];
typedef char check_subtitle_renderer_cutscene_offset[(offsetof(CSubtitleRenderer, InCutscene) == 0xe6) ? 1 : -1];
typedef char check_native_playergui_cutscene_offset[(NATIVE_PLAYERGUI_INCUTSCENE_OFFSET == 0xf6) ? 1 : -1];
#endif
extern CPlayerGui_DonorPDB* g_pPlayerGui; // native global type CPlayerGui @ 0x013b8790

CActionTalkToThing::CActionTalkToThing(CThing& target, const char* textName, const char* groupName,
                                       const CCharStringValue& button1, const CCharStringValue& button2,
                                       const CCharStringValue& button3, bool makeTargetListen, bool soundIn2D)
    : CActionTalkBase(&target, makeTargetListen, true, soundIn2D),
      AskingHeroQuestion(true), DisplayOverScreenFade(false),
      QuestionButton1(button1), QuestionButton2(button2), QuestionButton3(button3)
{
    CSubtitledMessageArray messages;
    GetMessagesFromTextName(&messages, textName, groupName, 0);
    if (messages.first != messages.last) {
        messages.resize(1, CSubtitledMessage());
    }
    Messages = messages;
    CPlayerGui_DonorPDB* playerGui = g_pPlayerGui;
    AddedDuringCutscene = *(const bool*)((const unsigned char*)playerGui + NATIVE_PLAYERGUI_INCUTSCENE_OFFSET);
}

}