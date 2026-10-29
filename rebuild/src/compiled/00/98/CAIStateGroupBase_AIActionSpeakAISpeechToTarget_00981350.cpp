// CAIStateGroupBase::AIAction_SpeakAISpeechToTarget(mode, speaker, target, speechName, a5, a6, a7) -- resolve the AI speech name through the engine's speech map (CRC of the name) and forward to AIAction_SpeakToTarget
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00981350, length 181 bytes (`ret 24h`).
// Callees: Calc(ulong seed, const char* data, ulong length) @ 0x004014A0 (__fastcall CRC), the speech map's LookupValue
// @ 0x009CC410 (engine singleton 0x013B86A0 +0x14), CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10/
// 0x00A01B50/0x00A01B90/0x00A01C10, CAIStateGroupBase::AIAction_SpeakToTarget @ 0x00980BD0.
// Shape: the speech map is read into a local first (retail loads it into esi at entry); the speech id is a named local computed before the call (retail looks it up before any push); the two
// intelligent pointers are by-value parameters copied into the argument area through their inline copy ctors (vtables
// 0x01237F1C / 0x0123E7D8) and destroyed by this callee after the call; the CCharString accessors are inline null-safe
// reads of the string data {chars, length}; GetString is `s = ""; if (data) s = data->Chars;` (retail preloads the
// shared "" 0x0129AAF4 into edx -- the ?: forms branch both ways).
class CThing;
class CThingCreatureBase;

struct CBaseIntelligentPointer
{
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer
{ public:
    virtual void v00();
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};

class CCharStringData
{ public:
    const char* Chars;                              // +0x00
    long Length;                                    // +0x04
};

class CCharString
{ public:
    CCharStringData* PStringData;                   // +0x00
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
    const char* GetString() const { const char* s = ""; if (PStringData) s = PStringData->Chars; return s; }
};

class CCRC { public: static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0

struct CSpeechMap
{
    long LookupValue(unsigned long crc);            // 0x009CC410
};

class CEngine
{ public:
    unsigned char _pad00[0x14];
    CSpeechMap* SpeechMap;                          // +0x14
};
extern CEngine* g_pEngine;                          // 0x013B86A0

class CAIStateGroupBase
{ public:
    virtual void v00();
    unsigned char _pad04[0x10];
    long AIAction_SpeakToTarget(long mode, CIntelligentPointer<CThingCreatureBase> speaker,
                                CIntelligentPointer<const CThing> target, long speech,
                                long a5, long a6, long a7);                 // 0x00980BD0
    long AIAction_SpeakAISpeechToTarget(long mode, CIntelligentPointer<CThingCreatureBase> speaker,
                                        CIntelligentPointer<const CThing> target,
                                        const CCharString& speechName, long a5, long a6, long a7);
};

long CAIStateGroupBase::AIAction_SpeakAISpeechToTarget(long mode, CIntelligentPointer<CThingCreatureBase> speaker,
                                                       CIntelligentPointer<const CThing> target,
                                                       const CCharString& speechName, long a5, long a6, long a7)
{
    CSpeechMap* map = g_pEngine->SpeechMap;
    long speech = map->LookupValue(CCRC::Calc(0, speechName.GetString(), speechName.GetLength()));
    return AIAction_SpeakToTarget(mode, speaker, target, speech, a5, a6, a7);
}