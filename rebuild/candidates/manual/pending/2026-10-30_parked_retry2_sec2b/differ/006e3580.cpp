// CTCTalk::PlaySpeech(CASoundBank* bank, long sample, bool loop, const CCharString& name, EDialogueLayer layer, bool attached, bool lipSync) -- play a speech sample through the owner's sound player (1 = no player): stop a speech still running, drop the bank check once the name is missing from the sound manager's speech list, play the sample, start the dialogue on the world, apply the owner def's speech volume, set up lip-sync mesh symbols and restart the speech timer
// Grade: DIFFER(495v502) (9 difflines, was 490v502/15) -- the iterator tag passed as an unnamed temporary (`__find(first, last, name, random_access_iterator_tag())`): name now loads into ebp before the bank check with the list end in edi, as retail; residue: retail keeps the dead `player = 0` entry store (`mov [esp+4],0`) that ours drops, and retail's tag temporary sits in the lipSync argument slot (`lea ecx,[esp+58h]`, no store) where ours zeroes a byte in the name slot (`mov byte [esp+54h],0`); the CDefPointer null is an immediate in retail and ebp in ours (retry wave 2 sec2b)
// Was: DIFFER(490v502) -- near miss; gaps: retail keeps the dead `player = 0` store into the key/player shared frame word, puts the __find iterator-tag temporary in the lipSync argument slot (ours the dead name slot), and loads `name` into ebp before the bank check with the list end in edi (ours swaps them)
// Retail: Fable.exe @ 0x006E3580, 503 bytes (`ret 0x1c`, no EH frame).
// Callees: LowerBound 0x0040F020, CWorld frame 0x00416225, StopCurrentSpeech 0x006E28D0, definition manager 0x0044C6B0, __find 0x004B8FF0 (__fastcall),
//   CCharString::operator= 0x0099EFB0, CTCSoundPlayer::PlayAttachedSample 0x0066B300, CTCBase::GetWorld 0x00686A70, OnDialogueStart 0x006E31E0 (__fastcall),
//   CDefClassBase::GetSubDef<T> 0x0065F604, CTCSoundPlayer::SetSampleVolume 0x0066AFA0, SetMeshBankAndSymbol 0x006E29A0, FindAndAddMeshSymbol 0x006E2940
class CTCBase;
struct CCharString {
    void* m_data;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type);               // 0x0040F020
};
struct CRefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
struct CTalkDef : CRefBase {
    unsigned char _pad08[0x4c - 0x08];
    float SpeechVolume;                                 // +0x4c
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
};
struct CDefClassBase : CRefBase {
    bool GetSubDef(CDefPointer<CTalkDef>* def);         // 0x0065F604
};
CTalkDef* FindTalkDef(CDefClassBase* defClass);
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<CTalkDef>* out)
{
    CTalkDef* def = FindTalkDef(this);
    out->Object = def;
    return def != 0;
}
#pragma auto_inline(on)
struct CDefClassPointer {
    CDefClassBase* Object;
    CDefClassPointer(CDefClassBase* p) : Object(p) { if (Object) ++Object->RefCount; }
    ~CDefClassPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* DefClass;                            // +0x70
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CASoundBank;
struct CSoundPair {
    long Sample;
    long Lipsync;
    CSoundPair(long sample, long lipsync) : Sample(sample), Lipsync(lipsync) { }
};
struct CSoundInit {
    float Volume;
    float Pitch;
    long Delay;
    long Priority;
    long Count;
    long Flags;
    long Group;
    long Channel;
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Delay(0), Priority(1), Count(1), Flags(0), Group(0), Channel(-1) { }
};
struct CTCSoundPlayer {
    unsigned long PlayAttachedSample(CASoundBank* bank, CSoundPair* pair, CSoundInit* init, bool attached, bool loop, bool positional);   // 0x0066B300
    void SetSampleVolume(unsigned long handle, float volume);   // 0x0066AFA0
};
struct random_access_iterator_tag { };
struct CSpeechEntry { CCharString Name; long _pad[3]; };   // 16-byte entry
CSpeechEntry* __fastcall __find(CSpeechEntry* first, CSpeechEntry* last, const CCharString& name, const random_access_iterator_tag& tag);   // 0x004B8FF0
struct CSoundManager { unsigned char _pad[0x100]; CSpeechEntry* SpeechFirst; CSpeechEntry* SpeechLast; };
struct CDefinitionManager { unsigned char _pad[0xd0]; CSoundManager* Sounds; };
CDefinitionManager* __fastcall GetDefinitionManager();  // 0x0044C6B0
struct CWorld { long GetFrame(); };                     // 0x00416225
extern CWorld* g_pWorld;                                // 0x013B86A0
extern unsigned long g_InvalidSoundHandle;              // 0x0129D25C
bool __fastcall OnDialogueStart(CWorld* world, CThing* thing, unsigned long handle, long layer);   // 0x006E31E0
enum ESpeechPlaybackResult { SPEECH_PLAYING = 0, SPEECH_NO_PLAYER = 1 };
struct CTCTalk {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x10 - 0x08];
    long SpeechStartFrame;                              // +0x10
    long SpeechLengthFrames;                            // +0x14
    unsigned char _pad18[0x20 - 0x18];
    float SpeechTimer;                                  // +0x20
    unsigned char _pad24[0x28 - 0x24];
    unsigned long SpeechHandle;                         // +0x28
    unsigned char _pad2c[0x30 - 0x2c];
    CCharString SpeechName;                             // +0x30
    bool CheckSpeechBank;                               // +0x34
    unsigned char _pad35;
    bool LipSync;                                       // +0x36
    CWorld* GetWorld();                                 // 0x00686A70
    void StopCurrentSpeech(CTCSoundPlayer* player);     // 0x006E28D0
    void SetMeshBankAndSymbol(CASoundBank* bank, long sample);   // 0x006E29A0
    long FindAndAddMeshSymbol(long sample);             // 0x006E2940
    ESpeechPlaybackResult PlaySpeech(CASoundBank* bank, long sample, bool loop, const CCharString& name, long layer, bool attached, bool lipSync);
};

ESpeechPlaybackResult CTCTalk::PlaySpeech(CASoundBank* bank, long sample, bool loop, const CCharString& name, long layer, bool attached, bool lipSync)
{
    CTCSoundPlayer* player = 0;
    if (!ParentThing->GetInterface(&player, 0x10))
        return SPEECH_NO_PLAYER;
    if (g_pWorld->GetFrame() - SpeechStartFrame < SpeechLengthFrames)
        StopCurrentSpeech(player);
    if (CheckSpeechBank) {
        CSoundManager* sounds = GetDefinitionManager()->Sounds;
        CSpeechEntry* last = sounds->SpeechLast;
        if (__find(sounds->SpeechFirst, last, name, random_access_iterator_tag()) == last)
            CheckSpeechBank = false;
    }
    SpeechName = name;
    CSoundInit init;
    init.Flags = 0x8040;
    SpeechHandle = player->PlayAttachedSample(bank, &CSoundPair(sample, sample), &init, true, loop, attached);
    if (SpeechHandle != g_InvalidSoundHandle) {
        CThing* thing = ParentThing;
        OnDialogueStart(GetWorld(), thing, SpeechHandle, layer);
    }
    CDefPointer<CTalkDef> def;
    bool hasVolume;
    {
        CDefClassPointer defClass(ParentThing->DefClass);
        if (defClass.Object->GetSubDef(&def) && def.Object->SpeechVolume != 1.0f)
            hasVolume = true;
        else
            hasVolume = false;
    }
    if (hasVolume)
        player->SetSampleVolume(SpeechHandle, def.Object->SpeechVolume);
    LipSync = lipSync;
    SetMeshBankAndSymbol(bank, sample);
    SpeechTimer = -1.0f;
    SpeechStartFrame = g_pWorld->GetFrame();
    SpeechLengthFrames = FindAndAddMeshSymbol(sample);
    return SPEECH_PLAYING;
}
