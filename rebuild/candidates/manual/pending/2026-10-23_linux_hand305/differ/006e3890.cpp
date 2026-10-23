// CTCTalk::PlaySpeech(unsigned long, const CCharString&, EDialogueLayer, bool, bool) -- look up the text entry (group or voiced), play its SND_ sample from the entry's speech bank and show the line as an emote
// Leaf: PlaySpeech
// Grade: DIFFER(531v529) -- VC threads the group path past the join `if (!entry)` test and moves the text-lookup else block to the end (retail keeps if/else layout with the join test); the emote temporaries (key 0x45, the text, the 6.0/0.5 limits, the length) take different dead argument slots
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E3890, 529 bytes, `ret 14h`; caller 0x006E3B10 (landed).
// Callees: CBankFile::GetEntryDataType 0x009CC210, NGameText::CDataBank::GetPGroupEntry 0x009C9070,
//   CDataBank::GetTextEntryFromGroup 0x009C9280, the speech-bank lookup 0x009917E0 on the global @ 0x013B8394
//   (catalog "CSymbolMap::FindFirstSymbolWithVal"; it takes the entry's SpeechBank string and its result is the
//   CASoundBank used next), operator+(const char*, const CCharString&) 0x0099F690, CASoundBank::GetSoundIndexFromSymbol
//   0x00A01920, CTCTalk::PlaySpeech(CASoundBank*, ...) 0x006E3580, the entry's text getter 0x009C7A40 (catalog
//   "CBankFile::GetBankHandle"; returns the CWideString), CWideString::GetLength 0x0099B220, GFLimitReturn 0x0057EF86,
//   CTCEmoteIcon::SetEmoteText 0x0071AC70, ~CWideString 0x0099B510, ~CCharString 0x0099EAE0, LowerBound 0x0040F020.
// Layout: Ego_r.pdb CTCTalk (LastTextIndex +0x2c), CTextEntry (SpeechBank +0x1c, Narrator +0x20, SoundName +0x24),
//   CTCEmoteIcon (Replaceable +0x25); the world's data bank at +0x14 of 0x013B86A0 as in the landed DoesSpeechExist.
struct CCharString {
    void* m_pData;
    ~CCharString();                                         // 0x0099EAE0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
struct CWideString {
    void* m_pData;
    ~CWideString();                                         // 0x0099B510
    long GetLength() const;                                 // 0x0099B220
};
const float& __fastcall GFLimitReturn(const float& value, const float& lo, const float& hi);   // 0x0057EF86

struct CGroupEntry;
struct CTextEntry {
    unsigned char _pad00[0x1c];
    CCharString SpeechBank;                                 // +0x1c
    CCharString Narrator;                                   // +0x20
    CCharString SoundName;                                  // +0x24
    CWideString GetText() const;                            // 0x009C7A40
};
struct CIVGroupEntryPointer {                               // CIVCountedPointer<const NGameText::CGroupEntry>
    struct CCounted {
        virtual void v00();
        virtual void Destroy();
        long RefCount;
        void DecRef() { --RefCount; }
        long GetRefCount() const { return RefCount; }
        void Release()
        {
            DecRef();
            if (GetRefCount() == 0)
                Destroy();
        }
    };
    CCounted* Data;
    __forceinline ~CIVGroupEntryPointer()
    {
        if (Data)
            Data->Release();
    }
};
struct CDataBank {
    unsigned long GetEntryDataType(unsigned long id);       // 0x009CC210
    CIVGroupEntryPointer GetPGroupEntry(unsigned long id, bool flag) const;   // 0x009C9070
    CTextEntry* GetTextEntryFromGroup(unsigned long id, const CCharString& group, bool* found, long* index, bool flag);   // 0x009C9280
};
struct CWorld {
    unsigned char _pad00[0x14];
    CDataBank* PDataBank;                                   // +0x14
};
extern CWorld* g_pWorld;                                    // 0x013B86A0
extern bool g_bForceSpeech;                                 // 0x013B86DA
extern bool g_bShowSpeechEmotes;                            // 0x013B86D9

struct CASoundBank {
    long GetSoundIndexFromSymbol(const CCharString& symbol);   // 0x00A01920
};
struct CSpeechBankList {
    CASoundBank* FindBank(const CCharString& name);         // 0x009917E0
};
extern CSpeechBankList* g_pSpeechBanks;                     // 0x013B8394

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T>
    __forceinline T* GetTC(long type, T*)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CTCEmoteIcon {
    unsigned char _pad00[0x25];
    bool Replaceable;                                       // +0x25
    void SetEmoteText(long priority, const CWideString& text, float duration, float fade);   // 0x0071AC70
};

typedef long EDialogueLayer;
typedef long ESpeechPlaybackResult;
struct CTCTalk {
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    unsigned char _pad08[0x2c - 0x08];
    long    LastTextIndex;                                  // +0x2c
    ESpeechPlaybackResult PlaySpeech(CASoundBank* bank, long index, bool flag, const CCharString& narrator,
                                     EDialogueLayer layer, bool b1, bool b2);   // 0x006E3580
    ESpeechPlaybackResult PlaySpeech(unsigned long id, const CCharString& voice, EDialogueLayer layer, bool b1, bool b2);
};

ESpeechPlaybackResult CTCTalk::PlaySpeech(unsigned long id, const CCharString& voice, EDialogueLayer layer, bool b1, bool b2)
{
    CDataBank* dataBank = g_pWorld->PDataBank;
    if (id == 0)
        return 1;
    CTextEntry* entry;
    if (dataBank->GetEntryDataType(id) == 0) {
        CIVGroupEntryPointer group = dataBank->GetPGroupEntry(id, b1 || g_bForceSpeech);
        entry = (CTextEntry*)group.Data;
        if (!entry) {
            if (b1 || g_bForceSpeech)
                return 1;
            return 2;
        }
    } else {
        bool found = false;
        entry = dataBank->GetTextEntryFromGroup(id, voice, &found, &LastTextIndex, b1 || g_bForceSpeech);
        if (!entry && found) {
            if (b1 || g_bForceSpeech)
                return 1;
            return 2;
        }
    }
    if (!entry)
        return 1;
    if (g_pSpeechBanks) {
        CASoundBank* soundBank = g_pSpeechBanks->FindBank(entry->SpeechBank);
        if (soundBank) {
            long index = soundBank->GetSoundIndexFromSymbol("SND_" + entry->SoundName);
            if (index > 0) {
                if (PlaySpeech(soundBank, index, false, entry->Narrator, layer, false, b2) != 1)
                    return 0;
            }
        }
    }
    if (g_bShowSpeechEmotes && ParentThing->HasTC(0x45)) {
        CTCEmoteIcon* emote = ParentThing->GetTC(0x45, (CTCEmoteIcon*)0);
        CWideString text = entry->GetText();
        float hi = 6.0f;
        float lo = 0.5f;
        float duration = text.GetLength() * 0.1f;
        emote->SetEmoteText(7, text, GFLimitReturn(duration, lo, hi), 0.3f);
        emote->Replaceable = false;
    }
    return 0;
}
