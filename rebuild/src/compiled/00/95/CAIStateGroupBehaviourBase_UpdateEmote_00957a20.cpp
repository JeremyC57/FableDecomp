// CAIStateGroupBehaviourBase::UpdateEmote() -- after the current speech plus a random wait, re-arm the emote timer and sometimes play ST_EMOTE
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00957A20, length 339 bytes.
// Callees: GetPWorld @ 0x008FCFC0 and the static frame counter @ 0x0049D870, DivideByGameSpeed(float) @ 0x00415FD3 (__stdcall),
//          CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xF = CTCTalk, bit 15 of +0x20: `test ch,ch ; jns`),
//          CTCTalk::GetCurrentSpeechDuration @ 0x006E2840, GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, GFFloatRandom(float,
//          unsigned long*) @ 0x004978A0 (__fastcall), CCharString(const char*, long) @ 0x0099EBF0 ("ST_EMOTE" @ 0x01294E2C),
//          CCreatureAction_PlayAnimation(creature, anim, sound, ...) @ 0x008424F0, AIAction_PerformActionAndWait @ 0x0097DB70,
//          ~CCreatureActionBase @ 0x00693EF0, ~CCharString @ 0x0099EAE0.
// Constants: 0.03f @ 0x0126F1AC and 2.0f @ 0x01294E28 (random wait = rnd(100) * 0.03 + 2), 0.25f @ 0x01294E20, the unsigned
//          conversion bias @ 0x0122DCB4.
// Shape: `now - TriggerTime > RandomWaitTime + ThisSpeechDuration` (`test ah,5 ; jp`); the talk component through the
//          out-parameter GetInterface with the one-ternary pick (the component lands in ecx; a miss reads the never-written
//          slot); the integer random is unsigned; a 25% float
//          roll plays the emote with a named CCharString and the action temporary in the call (the creature through an
//          inline GetCreature() accessor: its brain load schedules before the zero pushes). Always returns true.
// Layout: engine/CAIStateGroupBehaviourBase.h (TriggerTime +0x28, RandomWaitTime +0x2C, ThisSpeechDuration +0x30) behind
//          engine/CAIStateGroupBase.h.
#include "engine/CAIStateGroupBehaviourBase.h"

class CTCBase;
class CThing;
class CThingCreatureBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CTCTalk { public:                                // interface 0xf
    float GetCurrentSpeechDuration();           // 0x006E2840
};

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
    virtual ~CCreatureActionBase();                         // 0x00693EF0
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CThingWorld {
    static long GetCurrentFrame();              // 0x0049D870
};
class CDisplayEngine { public:
    static float __stdcall DivideByGameSpeed(float value);   // 0x00415FD3
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

struct CAIStateGroupBehaviourBaseMethods : CAIStateGroupBehaviourBase {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)Brain()->PCreature; }
    CThingWorld* GetPWorld();                   // 0x008FCFC0
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced);   // 0x0097DB70
    bool UpdateEmote();
};

bool CAIStateGroupBehaviourBaseMethods::UpdateEmote()
{
    float now = CDisplayEngine::DivideByGameSpeed((float)GetPWorld()->GetCurrentFrame());
    if (now - TriggerTime > RandomWaitTime + ThisSpeechDuration) {
        CTCTalk* talk;
        Brain()->PCreature->GetInterface(&talk, 0xf);
        ThisSpeechDuration = talk->GetCurrentSpeechDuration();
        TriggerTime = now;
        RandomWaitTime = (float)Random(100) * 0.03f + 2.0f;
        if (GFFloatRandom(1.0f, GetPWorldSeed()) < 0.25f) {
            CCharString animation("ST_EMOTE");
            AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), animation,
                CSoundPair(), false, true, false, 0, 0), false);
        }
    }
    return true;
}