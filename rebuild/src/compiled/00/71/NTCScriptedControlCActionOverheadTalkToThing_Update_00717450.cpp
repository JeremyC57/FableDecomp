// NTCScriptedControl::CActionOverheadTalkToThing::Update() -- base talk update; then, while scrolling an unfinished line through the creature's emote icon: show the next 24 characters of the current line (or the whole line once the end is near), flag the scroll finished past the end, and push the text to the icon for the remaining turn time
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00717450, length 380 bytes (plain `ret`).
// Callees: CActionTalkBase::Update @ 0x00716DE0, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 69 = bit 5 of
//          the bitset dword at +0x28 = CTCEmoteIcon), CWideString() @ 0x0099AED0 / ~CWideString @ 0x0099B510, the current-line
//          text getter @ 0x00713290 (CWideString by value; manifest "CCategory::GetHighlightedItemCountInCategory" is
//          propagated), CWideString::GetLength @ 0x0099B220 (manifest "CCharString::GetLength", byte twin), GetAsCharArray()
//          @ 0x0099AF10, CWideString::SetAsSubString @ 0x0099B770, CWideString::operator= @ 0x0099B7D0,
//          CTCEmoteIcon::SetEmoteText @ 0x0071AC70; data: the frame rate @ 0x01375550, 0.3f @ 0x3E99999A (immediate).
// Shape: three byte tests then the out-parameter lookup; the shown-character count is a signed `/ 10` (`imul 66666667h`)
//          and an inline `max(const long&, const long&)` returning a reference (the `lea` select) in their own block; the text
//          local and the rest sit in a second block so they reuse those two frame words; each line read is a
//          by-value temporary whose destructor the compiler merges after the if/else; the two bools ride in bl; the
//          remaining time is `(float)(frames) / g_FrameRate` (`fild ; fidiv`).
// Layout: Ego_r.pdb NTCScriptedControl::CActionTalkBase 4 bytes high after Messages (CArray 16 vs retail 12): retail
//          Terminated +9, UpdateCounter +0x18, GameTurnDuration +0x48, ScrollText +0x90, FinishedScroll +0x91 (as the landed
//          ctor @ 0x00717BC0); CActionBase PTCScript +4 (CTCBase: ParentThing +4).
class CTCBase;
class CThing;

class CWideString { public:
    void* PStringData;
    CWideString();                                       // 0x0099AED0
    ~CWideString();                                      // 0x0099B510
    long GetLength() const;                              // 0x0099B220
    const unsigned short* GetAsCharArray() const;              // 0x0099AF10
    void SetAsSubString(const unsigned short* text, long length);   // 0x0099B770
    const CWideString& operator=(const CWideString& other);    // 0x0099B7D0
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CTCEmoteIcon { public:                           // interface 69
    void SetEmoteText(long style, const CWideString& text, float seconds, float fade);   // 0x0071AC70
};
class CTCScriptedControl { public:                     // a CTCBase
    virtual void v00();
    CThing* ParentThing;                        // +0x04
};
extern long g_FrameRate;                        // 0x01375550

inline const long& Max(const long& a, const long& b) { return a < b ? b : a; }

namespace NTCScriptedControl {

struct CActionTalkBase {
    virtual ~CActionTalkBase();
    CTCScriptedControl* PTCScript;              // +0x04
    bool Activated;                             // +0x08
    bool Terminated;                            // +0x09
    unsigned char PTargetThing[12];             // +0x0c CScriptThing
    long UpdateCounter;                         // +0x18
    unsigned char _pad1c[0x48 - 0x1c];          // +0x1c times, Messages, indices, conversation
    long GameTurnDuration;                      // +0x48
    unsigned char _pad4c[0x90 - 0x4c];          // +0x4c .. +0x8f
    void Update();                              // 0x00716DE0
    CWideString GetCurrentMessageText();        // 0x00713290
};

struct CActionOverheadTalkToThing : CActionTalkBase {
    bool ScrollText;                            // +0x90
    bool FinishedScroll;                        // +0x91
    void Update();
};

void CActionOverheadTalkToThing::Update()
{
    CActionTalkBase::Update();
    if (Terminated)
        return;
    if (!ScrollText)
        return;
    if (FinishedScroll)
        return;
    CTCEmoteIcon* emote;
    if (!PTCScript->ParentThing->GetInterface(&emote, 69))
        return;
    long shown;
    {
        long chars = UpdateCounter - GameTurnDuration / 10;
        shown = Max(0L, chars);
    }
    {
        CWideString text;
        bool partial = shown < GetCurrentMessageText().GetLength();
        if (partial)
            text.SetAsSubString(GetCurrentMessageText().GetAsCharArray() + shown, 24);
        else
            text = GetCurrentMessageText();
        bool finished = shown > GetCurrentMessageText().GetLength() - 25;
        if (finished)
            FinishedScroll = true;
        emote->SetEmoteText(7, text, (float)(GameTurnDuration - UpdateCounter) / g_FrameRate, 0.3f);
    }
}

}