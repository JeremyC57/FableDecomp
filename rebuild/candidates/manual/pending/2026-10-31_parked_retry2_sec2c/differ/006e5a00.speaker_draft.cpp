// CScriptConversation::FrameUpdate() -- drop an invalid conversation; while the conversation is paused (and the world says to stop) silence the current speaker and drop it; otherwise wait for the current line's speech to finish and count down its display time, then advance to the next line: play its speech (or show its subtitle), aim the speaker and listener at each other, and finish when the lines run out
// Grade: DIFFER(120v1772) -- skeleton draft, parked on sight (lane gap): the ebp frame and the fld/fadd 0.5/fistp/
//   fild/fsub 1.0 round-down sequence (twice) come from an inline-asm rounding helper, which pure C++ cannot emit.
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E5A00, 1772 bytes, ebp frame (the speech-duration rounding is an
//   inline-asm fistp helper), plain `ret`.
// Skeleton draft: only the invalid / stop early-outs are modelled.
class CTCBase;
struct CVectorMapTC { struct value_type { long first; CTCBase* second; }; value_type* _M_start; value_type* _M_finish;
    value_type* LowerBound(const long& key); };                        // 0x0040F020
struct CThing { unsigned char _pad00[0x20]; unsigned long UsingComponents[9]; CVectorMapTC Components;
    unsigned char _pad4c[0x91 - 0x4c]; unsigned char Dead : 1; };
struct CTCTalk { void StopCurrentSpeech(); };      // 0x006E2E60
struct CWorldFlags { char Pad[0x54]; bool StopConversations; };
struct CWorldVT { virtual void v0(); virtual void v1(); virtual void v2(); virtual CWorldFlags* GetFlags(); };
struct CMainGameComponent { char Pad[0x24]; CWorldVT* World; };
extern CMainGameComponent* g_pMainGameComponent;  // 0x013B86A0
struct CScriptConversationManager;
void RemoveConversation(CScriptConversationManager* manager, long id, bool now);   // 0x006E5990
struct CScriptConversation {
    CScriptConversationManager* Manager;          // +0x00
    long                        ID;               // +0x04
    char                        Pad0[0x20 - 8];
    long                        CurrentLine;      // +0x20
    long                        LineTimer;        // +0x24
    bool                        Unused28;
    bool                        Speaking;         // +0x29
    bool                        Paused;           // +0x2a
    bool IsValid();                               // 0x006E5600
    CThing* GetCurrentSpeaker();                  // 0x006E55B0
    void FrameUpdate();
};
void CScriptConversation::FrameUpdate()
{
    if (!IsValid()) {
        RemoveConversation(Manager, ID, false);
        return;
    }
    if (!Paused && g_pMainGameComponent->World->GetFlags()->StopConversations) {
        if (CurrentLine != -1) {
            CThing* speaker = GetCurrentSpeaker();
            if (speaker && !speaker->Dead && (speaker->UsingComponents[0] & 0x8000)) {
                long key = 0xf;
                CVectorMapTC::value_type* it = speaker->Components.LowerBound(key);
                if (it == speaker->Components._M_finish || key < it->first)
                    it = speaker->Components._M_finish;
                ((CTCTalk*)it->second)->StopCurrentSpeech();
            }
        }
        RemoveConversation(Manager, ID, false);
        return;
    }
    if (CurrentLine != -1 && !Speaking)
        --LineTimer;
}
