// NTCScriptedControl::CActionTalkBase::OnCleanup() -- end a talk: make a listening target stop looking at the talker, restore the talker's conversation attitude (unless the conversation mode was added), let a running conversation mode (27) linger three seconds or remove it, show the subtitle background again and clear the subtitles when the profile has them switched off
// Leaf: ?OnCleanup@CActionTalkBase@NTCScriptedControl@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00714E00, 340 bytes (+ int3 padding), plain `ret`. Slot 5 of the CActionTalkBase
//   vtable 0x012659A8 (slot 3 is the landed OnAdd 0x00714D30); Ego_r.pdb ?OnCleanup@CActionTalkBase@NTCScriptedControl@@UAEXXZ.
// Callees: CScriptThing virtual slot 11 GetThing (+0x2C) on PTargetThing, CVectorMap::LowerBound @ 0x0040F020 (the
//   inlined GetInterface lookups: 0x43 CTCLook = bit 3 of the bitset dword +0x28, 0x0F CTCTalk = bit 15 of +0x20, 0x31
//   CTCCreatureModeManager = bit 17 of +0x24), CTCLook::StopLooking(long) @ 0x00751B80 (landed),
//   CTCCreatureModeManager::GetCurrentMode @ 0x0073A6E0 / RemoveMode @ 0x0073ABA0 and the mode's virtual GetType (slot 1),
//   __ftol2 @ 0x00BFEA70, CTBaseSingleton<CUserProfileManager>::Get @ 0x0040D2A0 (__fastcall),
//   CSubtitleRenderer::ClearText(bool) @ 0x00492BAB. Globals g_FramesPerSecond @ 0x01375550 and g_pPlayerGui @ 0x013B8790
//   (as the landed CActionTalkToThing::DisplayText 0x007132C0); 3.0f @ 0x0122DED4.
// Layout: retail CActionTalkBase = Ego_r.pdb less 4 after the Messages CArray (PreviousConversationAttitude +0x68,
//   AddedConversationMode +0x6C); CTCTalk's attitude +0x18 and the mode's linger counter +0x14 (descriptive); the user
//   profile manager's "profile loaded" flag +0x0C, its default subtitles flag +0x0D and the profile's +0xCD.
// Shape: the parent thing is read into a local before the second GetThing() compare (as the landed OnAdd); the mode
//   linger value is converted before GetCurrentMode() is fetched again; the subtitles setting is an inline accessor.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
struct CThing {
    void*         __vftable;
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];               // +0x20
    CInterfaceMap Components;                       // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CCPPointerInfo;
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;               // slot 11 (+0x2c)
    CScriptThing*   PImp_Data;                      // +0x04
    CCPPointerInfo* PImp_Info;                      // +0x08
};

class CTCLook {
public:
    void StopLooking(long timeToStop);              // 0x00751B80
};
enum EConversationAttitude { CONVERSATION_ATTITUDE_NONE = 0 };
class CTCTalk {
public:
    unsigned char         _pad00[0x18];
    EConversationAttitude ConversationAttitude;     // +0x18 (descriptive)
};
class CMode {
public:
    virtual void v00();
    virtual long GetType() const;                   // slot 1
    unsigned char _pad04[0x14 - 4];
    long          LingerFrames;                     // +0x14 (descriptive)
};
class CTCCreatureModeManager {
public:
    CMode* GetCurrentMode();                        // 0x0073A6E0
    void   RemoveMode(unsigned long mode);          // 0x0073ABA0
};

class CUserProfileManager {
public:
    unsigned char  _pad00[0x0c];
    bool           ProfileLoaded;                   // +0x0C
    bool           DefaultSubtitles;                // +0x0D
    unsigned char  _pad0e[0xcd - 0x0e];
    bool           ProfileSubtitles;                // +0xCD (the loaded profile's setting)
    bool AreSubtitlesOn() const { return ProfileLoaded ? ProfileSubtitles : DefaultSubtitles; }
    static CUserProfileManager& __fastcall Get();   // 0x0040D2A0 (CTBaseSingleton<CUserProfileManager>::Get)
};

struct CSubtitleRenderer {
    void ClearText(bool immediately);               // 0x00492BAB
};
struct CPlayerGui {
    unsigned char     _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;             // +0x18
    unsigned char     _pad19[0xff - 0x19];
    bool              ShowSubtitleBackground;       // +0xff
};
extern CPlayerGui* g_pPlayerGui;                    // 0x013B8790
extern long g_FramesPerSecond;                      // 0x01375550

struct CTCScriptedControl {
    void*   __vftable;
    CThing* ParentThing;                            // +0x04
};

namespace NTCScriptedControl {

enum { CREATURE_MODE_CONVERSATION = 27 };           // descriptive

class CActionBase {
public:
    virtual ~CActionBase();
    virtual void Update();                          // slot 1
    virtual void OnActivate();                      // slot 2
    virtual void OnAdd();                           // slot 3
    virtual void v4();
    virtual void OnCleanup();                       // slot 5
    CTCScriptedControl* PTCScript;                  // +0x04
    bool                Activated;                  // +0x08
    bool                Terminated;                 // +0x09
};

class CActionTalkBase : public CActionBase {
public:
    virtual void OnCleanup();
    CScriptThing          PTargetThing;             // +0x0C
    unsigned char         _pad18[0x68 - 0x18];      // UpdateCounter .. CameraToBeMovedDueToMovedSetupPositionsCount
    EConversationAttitude PreviousConversationAttitude;   // +0x68
    bool                  AddedConversationMode;    // +0x6C
};

void CActionTalkBase::OnCleanup()
{
    if (PTargetThing.GetThing()) {
        CThing* parent = PTCScript->ParentThing;
        if (PTargetThing.GetThing() != parent) {
            CTCLook* look;
            if (PTargetThing.GetThing()->GetInterface(&look, 0x43))
                look->StopLooking(2);
        }
    }
    if (!AddedConversationMode) {
        CTCTalk* talk;
        if (PTCScript->ParentThing->GetInterface(&talk, 0x0f))
            talk->ConversationAttitude = PreviousConversationAttitude;
    }
    CTCCreatureModeManager* modes;
    if (PTCScript->ParentThing->GetInterface(&modes, 0x31)) {
        if (modes->GetCurrentMode()->GetType() == CREATURE_MODE_CONVERSATION)
            modes->GetCurrentMode()->LingerFrames = (long)(g_FramesPerSecond * 3.0f);
        else
            modes->RemoveMode(CREATURE_MODE_CONVERSATION);
    }
    g_pPlayerGui->ShowSubtitleBackground = true;
    if (!CUserProfileManager::Get().AreSubtitlesOn())
        g_pPlayerGui->SubtitleRenderer.ClearText(false);
}
}