// CTCTalk::DoesSpeechExist(unsigned long entryID) -- asks the world's dialogue data bank whether the entry exists in this talker's voice group
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E2910, length 48 bytes (ret 4).
// Callee: CDataBank::GetTextEntryFromGroup(unsigned long, const CCharString&, bool* found, long* index, bool) @ 0x009C9280,
// called on the data bank held at +0x14 of the global world object @ 0x013B86A0.
// Ego_r.pdb CTCTalk: DialogueVoice (CCharString) +0x38. The returned entry pointer is dropped: only the `found`
// out-flag matters. It lives in the `push ecx` frame word (its high byte, `lea edx,[esp+0Bh]` after two pushes),
// is cleared right before the call and returned as `mov al,[esp+3]`. The bank is a named local taken first:
// retail loads it into ecx before any push (`mov eax,ecx` parks `this`); the chained form loads it last.
class CCharString { public: void* m_pData; };
struct CTextEntry;

struct CDataBank {
    CTextEntry* GetTextEntryFromGroup(unsigned long entryID, const CCharString& group, bool* found, long* index, bool flag);   // 0x009C9280
};
class CWorld { public:
    unsigned char _pad_0x00[0x14];
    CDataBank*    PDataBank;                 // +0x14
};
extern CWorld* g_pWorld;                     // 0x013B86A0

class CTCTalk { public:
    unsigned char _base[0x38];               // CTCBase + SpeechSample .. UseLipSynching
    CCharString   DialogueVoice;             // +0x38
    bool DoesSpeechExist(unsigned long entryID);
};

bool CTCTalk::DoesSpeechExist(unsigned long entryID)
{
    CDataBank* bank = g_pWorld->PDataBank;
    bool found = false;
    bank->GetTextEntryFromGroup(entryID, DialogueVoice, &found, 0, false);
    return found;
}