// NTCScriptedControl::CActionOverheadTalkToThing::CActionOverheadTalkToThing(CThing*, const char*, long) -- overhead talk action: load the subtitled lines of a text group
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00717BC0, length 143 bytes (`ret 0Ch`).
// Callees: CActionTalkBase::CActionTalkBase(CThing*, bool, bool, bool) @ 0x00717950,
// GetMessagesFromTextName(CArray<CSubtitledMessage>*, const char*, const char*, ETextGroupSelectionMethod)
// @ 0x004CEBB0 (__fastcall: array in ecx, name in edx), CArray<CSubtitledMessage>::operator= @ 0x0071A160
// (manifest label "CCombatWheel::CopyAssign" is propagated), CSubtitledMessage::~CSubtitledMessage @ 0x004D1DB0
// (manifest label "~CPatchTesselationEdgeStrip" is propagated), free @ 0x00BFEA14 (MSVCR71 free thunk, not operator delete).
// Label: the manifest's "CActionTalkBase::CActionTalkBase" is wrong: the body calls the CActionTalkBase
// constructor (0x00717950) and stores vtable 0x012659FC = NTCScriptedControl::CActionOverheadTalkToThing.
//
// Shape: base constructor with three false flags (pushed from the ebx zero register), the vtable, ScrollText
// (+0x90) and FinishedScroll (+0x91) cleared (PDB names; the retail CArray is 12 bytes, so the PDB offsets
// sit 8 bytes high), then the body: a local CArray<CSubtitledMessage> (three zeroed words in the frame) is
// filled from the text group and assigned to Messages (+0x30); its inline destructor runs the 0x44-byte
// element destructors over [first, last) with `last` cached in ebp, then frees the re-read buffer.
// Leaf: ??0CActionOverheadTalkToThing
extern "C" void __cdecl free(void* block);   // MSVCR71 import thunk 0x00BFEA14

class CThing;

class CSubtitledMessage { public:                      // 0x44 bytes in retail
    unsigned char _body[0x44];
    ~CSubtitledMessage();                       // 0x004D1DB0
};

struct CSubtitledMessageArray {                 // CArray<CSubtitledMessage>: {first, last, end of storage}
    CSubtitledMessage* m_pFirst;
    CSubtitledMessage* m_pLast;
    CSubtitledMessage* m_pEnd;
    CSubtitledMessageArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    ~CSubtitledMessageArray()
    {
        CSubtitledMessage* last = m_pLast;
        for (CSubtitledMessage* p = m_pFirst; p != last; ++p)
            p->~CSubtitledMessage();
        if (m_pFirst != 0)
            free(m_pFirst);
    }
    CSubtitledMessageArray& operator=(const CSubtitledMessageArray& other);   // 0x0071A160
};

void __fastcall GetMessagesFromTextName(CSubtitledMessageArray* messages, const char* textName,
                                        const char* altTextName, long selectionMethod);   // 0x004CEBB0

namespace NTCScriptedControl {

struct CActionTalkBase {                        // retail 0x90 bytes
    virtual void v00();
    unsigned char _base[0x2c];                  // +0x04 CActionBase words, PTargetThing, UpdateCounter, times
    CSubtitledMessageArray Messages;            // +0x30
    unsigned char _talk[0x54];                  // +0x3c .. +0x8f
    CActionTalkBase(CThing* target, bool a, bool b, bool c);   // 0x00717950
};

struct CActionOverheadTalkToThing : CActionTalkBase {   // vtable 0x012659FC
    bool ScrollText;                            // +0x90
    bool FinishedScroll;                        // +0x91
    virtual void v00();
    CActionOverheadTalkToThing(CThing* target, const char* textName, long selectionMethod);
};

CActionOverheadTalkToThing::CActionOverheadTalkToThing(CThing* target, const char* textName, long selectionMethod)
    : CActionTalkBase(target, false, false, false)
    , ScrollText(false)
    , FinishedScroll(false)
{
    CSubtitledMessageArray messages;
    GetMessagesFromTextName(&messages, textName, 0, selectionMethod);
    Messages = messages;
}

}