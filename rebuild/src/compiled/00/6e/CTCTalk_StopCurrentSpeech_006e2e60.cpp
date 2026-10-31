// CTCTalk::StopCurrentSpeech() -- when the owner has its speech emitter (interface 0x10), stop the playing sample through it, end the frame window now and reset the speech state
// Grade: RELOCATION_MATCH (109v109) -- retry wave 2 sec2b: the out-parameter `bool GetInterface(T**, long)` (one inline
//   test + lookup) replaces HasTC + GetTC. Was DIFFER(109v109) -- scheduling: retail `mov edx,[esi+28h] ; mov ecx,[eax+4] ; push edx`, ours `mov ecx,[esi+28h] ; push ecx ; mov ecx,[eax+4]` before StopSample
// Leaf: ?StopCurrentSpeech@CTCTalk@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E2E60, length 109 bytes (padded row 112).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetTC of interface 0x10 = bit 16 of CThing+0x20),
// CTCSoundPlayer::StopAttachedSound @ 0x0066ADD0 (manifest label "CGameScriptThing::SetToKillOnLevelUnload" is propagated),
// CMainGameComponent::GetCurrentServerFrame @ 0x00416225 on g_pMainGameComponent @ 0x013B86A0.
// Layout (Ego_r.pdb CTCTalk = retail, as the landed StopCurrentSpeech(CSpeechEmitter*) 0x006E28D0): SpeechSample +0x0C,
// SampleStartedFrame +0x10, SampleDurationFrames +0x14, MimeStartTime +0x1C, SampleLoadedTime +0x20, SampleID +0x28, LipSynching +0x35.
// Shape: this overload is the landed one-argument body inlined after the component lookup (`GetTC(k)` passed straight in, so the emitter
// goes `mov ecx,[eax+4]`); the zero stores share `xor eax,eax`, -1.0f is an immediate.
class CTCBase;
enum ETCInterfaceType { };
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap {                                  // retail STLport vector<pair<K, V> > + comparator (only the used members)
public:
    typedef std::pair<K, V> value_type;
    value_type* _M_start;                           // +0x00
    value_type* _M_finish;                          // +0x04
    value_type* LowerBound(const K& key);     // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*> CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template <class T> __forceinline bool GetInterface(T** out, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            CInterfaceMap::value_type* last = map->_M_finish;
            *out = (T*)(((it == last || interfaceType < it->first) ? last : it)->second);
            return true;
        }
        return false;
    }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->_M_finish || interfaceType < it->first)
            it = map->_M_finish;
        return it->second;
    }
};
class CTCSoundPlayer {                          // interface 0x10
public:
    void StopAttachedSound(unsigned long sound);   // 0x0066ADD0 Ego_r ?StopAttachedSound@CTCSoundPlayer@@QAEXK@Z (bound)
};
class CMainGameComponent {
public:
    long GetCurrentServerFrame() const;          // 0x00416225 `mov eax,[ecx+48h]` = Ego_r CurrentServerFrame +0x48 (bound ?GetCurrentServerFrame@CMainGameComponent@@QBEJXZ)
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

class CTCTalk {
public:
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
private:
    void StopCurrentSpeech(CTCSoundPlayer* emitter)   // out of line at 0x006E28D0 (Ego_r ?StopCurrentSpeech@CTCTalk@@AAEXPAVCTCSoundPlayer@@@Z)
    {
        emitter->StopAttachedSound(SampleID);
        SampleDurationFrames = g_pMainGameComponent->GetCurrentServerFrame() - SampleStartedFrame - 1;
        LipSynching = false;
        SpeechSample = 0;
        MimeStartTime = 0.0f;
        SampleLoadedTime = -1.0f;
        SampleID = 0;
    }
public:
    void StopCurrentSpeech();
};

void CTCTalk::StopCurrentSpeech()
{
    CTCSoundPlayer* emitter;
    if (ParentThing->GetInterface(&emitter, 0x10))
        StopCurrentSpeech(emitter);
}