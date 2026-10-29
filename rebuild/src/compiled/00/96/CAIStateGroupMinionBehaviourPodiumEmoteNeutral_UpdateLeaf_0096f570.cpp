// CAIStateGroup_MinionBehaviourPodiumEmoteNeutral::UpdateLeaf() -- once the current speech plus a random 2..5 s pause has elapsed, restamp the time, re-read the creature's speech duration, roll a new pause, and with a 10% chance play the ST_EMOTE animation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096F570, length 384 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::GetPWorld @ 0x008FCFC0, CWorld::GetWorldFrame @ 0x0049D870 (static), DivideByGameSpeed
//          @ 0x00415FD3 (__stdcall), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xf = CTCTalk, bit 15 of +0x20
//          as a masked dword: `test ch,ch ; jns`), CTCTalk::GetCurrentSpeechDuration @ 0x006E2840,
//          CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, CCharString(const char*, long) @ 0x0099EBF0
//          ("ST_EMOTE" @ 0x01294E2C) / ~CCharString @ 0x0099EAE0, CCreatureAction_PlayAnimation(creature, const CCharString&,
//          ...) @ 0x008424F0, AIAction_PerformActionAndWait @ 0x0097DB70, ~CCreatureActionBase @ 0x00693EF0.
// Globals: 4294967296.0f @ 0x0122DCB4 (unsigned fild fix-up), 0.03f @ 0x0126F1AC, 2.0f @ 0x0129707C, 0.01f @ 0x01230A5C,
//          0.1f @ 0x01297078.
// Shape: the *seed-form Random results convert as unsigned (fild + 2^32 fix-up through the dead key slot); the emote is
//          a block with a named CCharString and the action temporary in the PerformActionAndWait call.
// Layout: no engine header for this class; modelled locally on the PDB CAIStateGroupBase (PBrain +0x04) with the
//          podium emote state at +0x3c/+0x40/+0x44 (descriptive: speech duration, pause, time of the last emote check).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
class CTCTalk { public:                                // interface 0xf
    float GetCurrentSpeechDuration();           // 0x006E2840
};
class CWorld { public:
    static long __fastcall GetFrame();                // 0x0049D870
};
float __stdcall DivideByGameSpeed(float value); // 0x00415FD3
long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];                  // +0x04..+0xa7
    virtual ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];                  // +0xa8..+0xb3
    CCreatureAction_PlayAnimation(CThing* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

enum EAIActionReturn { };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThing* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    protected:
    unsigned long* GetPWorldSeed() const;             // 0x008FCFD0
    public:
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24A1U + 0x24DFU;
        *seed = GFROR13(*seed);
        return *seed % range;
    }
    protected: EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool stopWhenReplaced); public:   // 0x0097DB70
};

struct CAIStateGroup_MinionBehaviourPodiumEmoteNeutral : CAIStateGroupBase {
    unsigned char _pad14[0x3c - 0x14];          // +0x14 crowd-behaviour base state
    float SpeechDuration;                       // +0x3c (descriptive)
    float Pause;                                // +0x40 (descriptive)
    float LastEmoteTime;                        // +0x44 (descriptive)
    void UpdateLeaf();
};

void CAIStateGroup_MinionBehaviourPodiumEmoteNeutral::UpdateLeaf()
{
    float now = DivideByGameSpeed((float)GetPWorld()->GetFrame());
    if (now - LastEmoteTime > SpeechDuration + Pause) {
        CTCTalk* talk;
        GetCreature()->GetInterface(&talk, 0xf);
        SpeechDuration = talk->GetCurrentSpeechDuration();
        LastEmoteTime = now;
        Pause = (float)Random(100) * 0.03f + 2.0f;
        if ((float)Random(100) * 0.01f < 0.1f) {
            CCharString name("ST_EMOTE");
            AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), name, CSoundPair(),
                false, true, false, 0, 0), false);
        }
    }
}