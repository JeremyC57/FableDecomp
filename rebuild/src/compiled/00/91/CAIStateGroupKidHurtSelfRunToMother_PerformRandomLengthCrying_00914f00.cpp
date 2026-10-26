// CAIStateGroup_KidHurtSelfRunToMother::PerformRandomLengthCrying() -- play the crying-on-the-spot into animation, the loop animation 0..2 extra times (world-seed random), then the out-of animation; returns the first non-success action result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00914F00, length 384 bytes (+ int3 padding).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("ST_CRYING_ON_THE_SPOT_INTO"
//          @ 0x0128FA60, "..._LOOP" @ 0x0128FA44, "..._OUTOF" @ 0x0128FA28), CThingCreatureBase::GetAnimFromName
//          @ 0x00662A00, CCreatureAction_PlayAnimation ctors @ 0x00842570 (8 args) / 0x008425E0 (11 args, looping),
//          CAIStateGroupBase::AIAction_PerformActionAndWait @ 0x0097DB70, ~CCreatureActionBase @ 0x00693EF0,
//          CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890.
// Shape: each animation is a block with a named CCharString; GetCreature() is written twice (ctor argument and `this` of
//          GetAnimFromName) and VC7.1 loads it once into edi at the second use; the action temporary is built in the PerformActionAndWait call; the loop count is the
//          *seed-form random (`% 3`), kept in ebp, pushed mid-function.
// Layout: engine/CAIStateGroup_KidHurtSelfRunToMother.h (PBrain is the first dword of the base at +0x04).
#include "engine/CAIStateGroup_KidHurtSelfRunToMother.h"

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
class CAnimationEntry;
class CThing { public:
    CAnimationEntry* GetAnimFromName(const CCharString& name, long index);   // 0x00662A00
};
class CCreatureActionBase { public:
    unsigned char _body[0xa4];                  // +0x04..+0xa7
    virtual ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];                  // +0xa8..+0xb3
    CCreatureAction_PlayAnimation(CThing* creature, CAnimationEntry* animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x00842570
    CCreatureAction_PlayAnimation(CThing* creature, CAnimationEntry* animation, bool loop, bool f, long loops,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008425E0
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CAIStateGroupKidHurtSelfRunToMotherMethods : CAIStateGroup_KidHurtSelfRunToMother {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThing* GetCreature() const { return Brain()->PCreature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24A1U + 0x24DFU;
        *seed = GFROR13(*seed);
        return *seed % range;
    }
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced);   // 0x0097DB70
    long PerformRandomLengthCrying();
};

long CAIStateGroupKidHurtSelfRunToMotherMethods::PerformRandomLengthCrying()
{
    long result;
    {
        CCharString name("ST_CRYING_ON_THE_SPOT_INTO");
        result = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(),
            GetCreature()->GetAnimFromName(name, 0), CSoundPair(), false, true, false, 0, 0), false);
    }
    if (result != 2)
        return result;
    long loops = Random(3);
    {
        CCharString name("ST_CRYING_ON_THE_SPOT_LOOP");
        result = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(),
            GetCreature()->GetAnimFromName(name, 0), false, true, loops, CSoundPair(), false, true, false, 0, 0), false);
    }
    if (result != 2)
        return result;
    {
        CCharString name("ST_CRYING_ON_THE_SPOT_OUTOF");
        result = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(),
            GetCreature()->GetAnimFromName(name, 0), CSoundPair(), false, true, false, 0, 0), false);
    }
    return result;
}