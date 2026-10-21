// CAIStateGroup_WifeGiveYouGift::Update() -- with a wife component and a live hero: resolve the gift speech id, then repeatedly walk to within 2 units of the hero; whenever the speak frame has passed, play the speech (waiting while it plays) and schedule the next one 10 s later; terminate when the move fails or the wait is interrupted
// Grade: DIFFER(ours v retail) -- 387v386: retail keeps the speech loop top-tested (one PlayAISpeech call at the aligned loop head, `je` back from the DoNothing test, and a private Terminate+epilogue on the DoNothing failure); VC7.1 in the lane inverts the loop (second PlayAISpeech call at the bottom, Terminate tail merged with the final one). Everything before the loop and after it matches.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00926060, length 386 bytes (plain `ret`, virtual).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: 215 = bit 23 of the bitset dword at +0x38, 15 = bit 15
//          of +0x20 = CTCTalk), CAIStateGroupBase::GetPPlayerManager @ 0x008FCF90, CPlayerManager::GetMainPlayer @ 0x00449970,
//          CPlayer::GetHeroThing @ 0x00487DC0, Calc (CRC) @ 0x004014A0, the speech map's LookupValue @ 0x009CC410 (engine
//          singleton 0x013B86A0 +0x14), CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 (vtable 0x01237F1C =
//          CIntelligentPointer<const CThing>), AIAction_MoveToThingProximity @ 0x0097E380, GetWorldFrame @ 0x0049D870,
//          CTCTalk::PlayAISpeech @ 0x006E3B60, AIAction_DoNothing @ 0x0097E200, Terminate @ 0x008FCF40; data: the gift speech
//          name CCharString @ 0x013BB690, "" @ 0x0129AAF4, the frame rate @ 0x01375550.
// Shape: DeliveredGiveSpeech is cleared first; both lookups are the spilled-bit template form (the wife lookup's out pointer
//          is never read, the talker is preloaded to 0 -- `xor ebx,ebx`); the hero is a named local (frame slot) reused by
//          the by-value pointer argument built in the call area each pass; the speech name is read null-case-first into
//          chars/length (retail preloads "" and 0); the move result is spilled and tested `== 4` at the loop bottom; the
//          speak block is `while (PlayAISpeech(..) == 2) if (DoNothing() != 4) { Terminate(); return; }` then the flag set
//          and the next frame (`lea` *5*2).
// Layout: Ego_r.pdb CAIStateGroup_WifeGiveYouGift: FrameToSpeakNext +0x14, GiftToGiveDef +0x18, DeliveredGiveSpeech +0x1c
//          over the landed CAIStateGroupBase model (no engine header).
struct CTCBase;
struct CThing;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    // Spilled-bit form (the presence test stored as a frame byte before the lookup).
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {     // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    __forceinline CIntelligentPointerConstThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

struct CCharStringData {
    const char* Chars;                          // +0x00
    long Length;                                // +0x04
};
struct CCharString {
    CCharStringData* PStringData;               // +0x00
};
extern CCharString g_WifeGiftSpeechName;        // 0x013BB690

unsigned long __fastcall Calc(unsigned long seed, const char* data, unsigned long length);   // 0x004014A0
struct CSpeechMap {
    unsigned long LookupValue(unsigned long crc);   // 0x009CC410
};
struct CEngine {
    unsigned char _pad00[0x14];
    CSpeechMap* SpeechMap;                      // +0x14
};
extern CEngine* g_pEngine;                      // 0x013B86A0
extern long g_FrameRate;                        // 0x01375550

struct CTCTalk {                                // interface 15
    long PlayAISpeech(unsigned long sampleID, bool force, long layer, bool b1, bool b2);   // 0x006E3B60
};

struct CPlayer {
    CThing* GetHeroThing();                     // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};
struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing target, float proximity,
        bool* pArrived, long movement, bool a, bool b, bool c, bool d, float e);    // 0x0097E380
    long AIAction_DoNothing();                  // 0x0097E200
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_WifeGiveYouGift : CAIStateGroupBase {
    long FrameToSpeakNext;                      // +0x14
    long GiftToGiveDef;                         // +0x18
    bool DeliveredGiveSpeech;                   // +0x1c
    virtual void Update();
};

void CAIStateGroup_WifeGiveYouGift::Update()
{
    DeliveredGiveSpeech = false;
    CThing* creature = PBrain->PCreature;
    bool hasWife = creature->TestTC(215);
    if (hasWife) {
        CThing* hero = GetPPlayerManager()->GetMainPlayer()->GetHeroThing();
        if (hero != 0 && !(hero->m_flags91 & 1)) {
            CTCTalk* talk = 0;
            PBrain->PCreature->GetInterfaceT(&talk, 15);
            const char* chars;
            unsigned long length;
            if (g_WifeGiftSpeechName.PStringData == 0) {
                length = 0;
                chars = "";
            } else {
                length = g_WifeGiftSpeechName.PStringData->Length;
                chars = g_WifeGiftSpeechName.PStringData->Chars;
            }
            CSpeechMap* map = g_pEngine->SpeechMap;
            unsigned long speech = map->LookupValue(Calc(0, chars, length));
            long result;
            do {
                bool arrived;
                result = AIAction_MoveToThingProximity(2, hero, 2.0f, &arrived, 3, true, false, true, true, 0.0f);
                if (!(FrameToSpeakNext > CWorld::GetWorldFrame())) {
                    if (talk != 0 && speech > 0) {
                        do {
                            if (talk->PlayAISpeech(speech, false, 1, false, true) != 2)
                                goto spoken;
                        } while (AIAction_DoNothing() == 4);
                        Terminate();
                        return;
                    spoken:;
                    }
                    DeliveredGiveSpeech = true;
                    long rate = g_FrameRate;
                    FrameToSpeakNext = CWorld::GetWorldFrame() + rate * 10;
                }
            } while (result == 4);
        }
    }
    Terminate();
}
