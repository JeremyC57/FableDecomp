// CAIStateGroup_ButterflyWander::Update() -- fly to a random point near the start position, then idle there one to six times (ST_IDLE), then finish the update
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093D030, 241 bytes (`ret`).
// Callees: CAIStateGroup_FlyingThingBase::GetRandomFlightPos(const C3DVector*, float, C3DVector*, bool) 0x00912700,
// ExecuteFlyingPath(const C3DVector*) 0x009130D0, CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890
// (`__fastcall`), CCharString(const char*, long) 0x0099EBF0 ("ST_IDLE" 0x0127138C) / ~CCharString 0x0099EAE0,
// CCreatureAction_PlayAnimation(creature, anim, sound, bool, bool, bool, long, long) 0x008424F0 / ~CCreatureActionBase
// 0x00693EF0, AIAction_PerformActionAndWait 0x0097DB70, CAIStateGroupBase::Terminate 0x008FCF40 (manifest "CXMVPlaybackInfo").
// Shape: the inlined world LCG (`seed = seed * 0x24A1 + 0x24DF`, then GFROR13) `% 6` picks the idle count; the loop
// `for (long i = 0; i <= count; ++i)` keeps i in the frame; each pass builds a named CCharString in its own block
// and the action as an address-of-temporary (the landed 0x00942920 PlayAnimation model), the `!= 2` result in a bool
// declared outside the block (tested after both dtors). The flight-position failure is its own `Terminate(); return;`
// (VC cross-jumps it into the common Terminate call with `mov ecx,esi` hoisted above the `je`).
// Layout: Ego_r.pdb CAIStateGroup_ButterflyWander StartPos +0x24.
// Leaf: Update@CAIStateGroup_ButterflyWander
class CThingCreatureBase;

class C3DVector { public:
    float X, Y, Z;
};

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];                     // +0x04..+0xa7
    virtual ~CCreatureActionBase();                        // 0x00693EF0
};

class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];                     // +0xa8..+0xb3
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                 // +0x18
};

long __fastcall GFROR13(unsigned long);   // 0x00497890

enum EAIActionReturn { };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                              // +0x04
    unsigned char _pad[0x0c];                      // +0x08..+0x13
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    protected: EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait); public:   // 0x0097DB70
    protected:
    unsigned long* GetPWorldSeed() const;                // 0x008FCFD0
    public:
    void Terminate();                              // 0x008FCF40
};

struct CAIStateGroup_FlyingThingBase : CAIStateGroupBase {
    unsigned char _pad14[0x10];
    C3DVector StartPos;                            // +0x24
    bool GetRandomFlightPos(const C3DVector* centre, float radius, C3DVector* out, bool flag);   // 0x00912700
    long ExecuteFlyingPath(const C3DVector* target);   // 0x009130D0
};

struct CAIStateGroup_ButterflyWander : CAIStateGroup_FlyingThingBase {
    void Update();
};

void CAIStateGroup_ButterflyWander::Update()
{
    C3DVector target;
    if (!GetRandomFlightPos(&StartPos, 15.0f, &target, true)) {
        Terminate();
        return;
    }
    if (ExecuteFlyingPath(&target) == 2) {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24a1 + 0x24df;
        *seed = GFROR13(*seed);
        long count = *seed % 6;
        for (long i = 0; i <= count; ++i) {
            bool failed;
            {
                CCharString animation("ST_IDLE");
                failed = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), animation, CSoundPair(),
                    false, true, false, 0, 0), false) != 2;
            }
            if (failed)
                break;
        }
    }
    Terminate();
}