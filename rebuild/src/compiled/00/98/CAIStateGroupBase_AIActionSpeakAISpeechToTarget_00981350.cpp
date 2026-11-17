// CAIStateGroupBase::AIAction_SpeakAISpeechToTarget(mode, speaker, target, speechName, a5, a6, a7) -- resolve the AI speech name through the engine's speech map (CRC of the name) and forward to AIAction_SpeakToTarget
// Naming wave 18: the global stand-in g_pEngine (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00981350, length 181 bytes (`ret 24h`).
// Callees: Calc(ulong seed, const char* data, ulong length) @ 0x004014A0 (__fastcall CRC), the speech map's LookupValue
// @ 0x009CC410 (engine singleton 0x013B86A0 +0x14), CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10/
// 0x00A01B50/0x00A01B90/0x00A01C10, CAIStateGroupBase::AIAction_SpeakToTarget @ 0x00980BD0.
// Shape: the speech map is read into a local first (retail loads it into esi at entry); the speech id is a named local computed before the call (retail looks it up before any push); the two
// intelligent pointers are by-value parameters copied into the argument area through their inline copy ctors (vtables
// 0x01237F1C / 0x0123E7D8) and destroyed by this callee after the call; the CCharString accessors are inline null-safe
// reads of the string data {chars, length}; GetString is `s = ""; if (data) s = data->Chars;` (retail preloads the
// shared "" 0x0129AAF4 into edx -- the ?: forms branch both ways).
#include "fable_compiler.h"
class CThing;
class CThingCreatureBase;

class CBaseObject;
struct CBaseIntelligentPointer
{
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject*a0);                    // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer
{ public:
    virtual void v00();
    FABLE_FORCEINLINE CIntelligentPointer(const CIntelligentPointer& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); }
};

// NF47 (round 7): 0x009CC410 is Ego_r.pdb 0x00B1A5C0 / FableWin.pdb 0x02F978C0 ?FindIndexBySymbolCRC@CBankFile@@QBEKK@Z, called on the
//   game text bank (CMainGameComponent::GetTextBank(), PTextBank +0x14 / CNewFrontendGameComponent m_pTextBank +0x60;
//   NGameText::CDataBank : CBankFile, engine/NGameText_CDataBank.h); was the made-up CSpeechMap::LookupValue (in neither PDB).
//   The local CCharString / CCRC / CSpeechMap / CMainGameComponent models are replaced by fable_string.h (inline GetCRC),
//   engine/CMainGameComponent.h and engine/NGameText_CDataBank.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_SELECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC
#include "fable_string.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"

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
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    long speech = bank.FindIndexBySymbolCRC(speechName.GetCRC());
    return AIAction_SpeakToTarget(mode, speaker, target, speech, a5, a6, a7);
}