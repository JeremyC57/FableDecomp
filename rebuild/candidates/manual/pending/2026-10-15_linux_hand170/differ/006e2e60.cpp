// CTCTalk::StopCurrentSpeech() -- when the owner has its speech emitter (interface 0x10), stop the playing sample through it, end the frame window now and reset the speech state
// Grade: DIFFER(109v109) -- scheduling: retail `mov edx,[esi+28h] ; mov ecx,[eax+4] ; push edx`, ours `mov ecx,[esi+28h] ; push ecx ; mov ecx,[eax+4]` before StopSample
// Leaf: ?StopCurrentSpeech@CTCTalk@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E2E60, length 109 bytes (padded row 112).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetTC of interface 0x10 = bit 16 of CThing+0x20),
// the emitter's stop-sample member @ 0x0066ADD0 (manifest label "CGameScriptThing::SetToKillOnLevelUnload" is propagated), the world frame
// getter @ 0x00416225 on g_pWorld @ 0x013B86A0.
// Layout (Ego_r.pdb CTCTalk = retail, as the landed StopCurrentSpeech(CSpeechEmitter*) 0x006E28D0): SpeechSample +0x0C,
// SampleStartedFrame +0x10, SampleDurationFrames +0x14, MimeStartTime +0x1C, SampleLoadedTime +0x20, SampleID +0x28, LipSynching +0x35.
// Shape: this overload is the landed one-argument body inlined after the component lookup (`GetTC(k)` passed straight in, so the emitter
// goes `mov ecx,[eax+4]`); the zero stores share `xor eax,eax`, -1.0f is an immediate.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
struct CSpeechEmitter {
    void StopSample(unsigned long sampleID);    // 0x0066ADD0
};
struct CWorld {
    long GetFrame();                            // 0x00416225
};
extern CWorld* g_pWorld;                        // 0x013B86A0

struct CTCTalk {
    void*         __vfptr;                      // +0x00 (CTCBase)
    CThing*       ParentThing;                  // +0x04
    unsigned char _base08[4];
    long          SpeechSample;                 // +0x0c
    long          SampleStartedFrame;           // +0x10
    long          SampleDurationFrames;         // +0x14
    long          Attitude;                     // +0x18 EConversationAttitude
    float         MimeStartTime;                // +0x1c
    float         SampleLoadedTime;             // +0x20
    void*         PSpeechBank;                  // +0x24 CASoundBank*
    unsigned long SampleID;                     // +0x28
    long          LastTextIndex;                // +0x2c
    unsigned char LastNarrator[4];              // +0x30 CCharString
    bool          IsPureAISpeaker;              // +0x34
    bool          LipSynching;                  // +0x35
    bool          UseLipSynching;               // +0x36
    void StopCurrentSpeech(CSpeechEmitter* emitter)   // out of line at 0x006E28D0
    {
        emitter->StopSample(SampleID);
        SampleDurationFrames = g_pWorld->GetFrame() - SampleStartedFrame - 1;
        LipSynching = false;
        SpeechSample = 0;
        MimeStartTime = 0.0f;
        SampleLoadedTime = -1.0f;
        SampleID = 0;
    }
    void StopCurrentSpeech();
};

void CTCTalk::StopCurrentSpeech()
{
    if (ParentThing->HasTC(0x10)) {
        CSpeechEmitter* emitter = (CSpeechEmitter*)ParentThing->GetTC(0x10);
        StopCurrentSpeech(emitter);
    }
}
