// NTCScriptedControl::CActionOverheadTalkToThing::DisplayText() -- decide whether the current line is long enough (25+ characters) to scroll; a short line goes straight to the creature's emote icon for the line's duration
// Leaf: ?DisplayText@CActionOverheadTalkToThing@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00714F60, 195 bytes (plain `ret`, +13 int3 padding); vtable 0x012659FC slot 6.
// FableWin.pdb: ?DisplayText@CActionOverheadTalkToThing@NTCScriptedControl@@MAEXXZ (slot 6 = DisplayText, as the landed
//          CActionTalkToThing::DisplayText @ 0x007132C0).
// Callees: CWideString(const CWideStringText&) @ 0x0099B720, CWideString::GetLength @ 0x0099B220, ~CWideString @ 0x0099B510,
//          CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x45 = CTCEmoteIcon: bit 5 of the bitset
//          byte +0x28), CTCEmoteIcon::SetEmoteText @ 0x0071AC70 (style 7, fade 0.3f); data: the frame rate @ 0x01375550.
// Layout: retail CActionTalkBase Messages (CArray data) +0x30, CurrentMessageIndex +0x3C, GameTurnDuration +0x48 (as the
//          landed 0x007132C0 / 0x00717450); retail CSubtitledMessage 0x44 bytes, Text +0x0C; CActionOverheadTalkToThing
//          ScrollText +0x90, FinishedScroll +0x91 (as the landed Update @ 0x00717450 and ctor @ 0x00717BC0).
// Shape: the line is a by-value temporary from the landed family's __forceinline GetCurrentText(); the scroll test is
//          `GetLength() >= 25` stored straight into ScrollText (`setge`) before the temporary dies, then re-read; the
//          duration is `(float)GameTurnDuration / g_FrameRate` (`fild ; fidiv`).
struct CTCBase;
class CWideStringText {                            // the message text as stored (converted by 0x0099B720)
public:
    void* PStringData;
};
class CWideString {
public:
    void* PStringData;
    CWideString(const CWideStringText& other);     // 0x0099B720
    ~CWideString();                                // 0x0099B510
    long GetLength() const;                        // 0x0099B220
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x0040F020
};
struct CThing {
    void*         __vftable;
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];              // +0x20
    CInterfaceMap Components;                      // +0x44
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
struct CTCEmoteIcon {                              // interface 0x45
    void SetEmoteText(long style, const CWideString& text, float seconds, float fade);   // 0x0071AC70
};
struct CTCScriptedControl {
    void*   __vftable;
    CThing* ParentThing;                           // +0x04 (CTCBase)
};
extern long g_FrameRate;                           // 0x01375550
struct CSubtitledMessage {                         // retail 0x44
    unsigned char   _pad00[0x0c];
    CWideStringText Text;                          // +0x0C
    unsigned char   _pad10[0x44 - 0x10];
};

namespace NTCScriptedControl {
class CActionBase {
public:
    virtual ~CActionBase();
    CTCScriptedControl* PTCScript;                 // +0x04
    bool                Activated;                 // +0x08
    bool                Terminated;                // +0x09
};

class CActionTalkBase : public CActionBase {
public:
    unsigned char      _pad0c[0x30 - 0x0c];        // PTargetThing, UpdateCounter, line times
    CSubtitledMessage* Messages;                   // +0x30 (CArray data)
    unsigned char      _pad34[0x3c - 0x34];
    long               CurrentMessageIndex;        // +0x3C
    unsigned char      _pad40[0x48 - 0x40];        // Conversation
    long               GameTurnDuration;           // +0x48
    unsigned char      _pad4c[0x90 - 0x4c];        // tag indices .. SkippedCurrentLine
    __forceinline CWideString GetCurrentText() const { return CWideString(Messages[CurrentMessageIndex].Text); }
};

class CActionOverheadTalkToThing : public CActionTalkBase {
public:
    virtual void DisplayText();
    bool ScrollText;                               // +0x90
    bool FinishedScroll;                           // +0x91
};

void CActionOverheadTalkToThing::DisplayText()
{
    ScrollText = GetCurrentText().GetLength() >= 25;
    FinishedScroll = false;
    if (ScrollText)
        return;
    CTCEmoteIcon* emote;
    if (!PTCScript->ParentThing->GetInterface(&emote, 0x45))
        return;
    emote->SetEmoteText(7, GetCurrentText(), (float)GameTurnDuration / g_FrameRate, 0.3f);
}
}