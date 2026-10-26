// CAIStateGroup_SitForMeal::StandAroundAndEat() -- play ST_IDLE and wait; true when the action reports 2 (finished)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009454C0, length 110 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCreatureAction_PlayAnimation::
// CCreatureAction_PlayAnimation(CThingCreatureBase*, const CCharString&, const CSoundPair&, bool, bool, bool,
// long, long) @ 0x008424F0, CAIStateGroupBase::AIAction_PerformActionAndWait(CCreatureActionBase*, bool)
// @ 0x0097DB70, CCreatureActionBase::~CCreatureActionBase @ 0x00693EF0 (manifest "CPhysicsMeshInfo::
// ~CPhysicsMeshInfo" is propagated), CCharString::~CCharString @ 0x0099EAE0.
//
// Shape: the landed GoToPub::SitIdle (0x00947CA0) animation shape with the WAIT variant: `false` is pushed
// first (from the shared zero register ecx), the action temporary is built inside the call from the named
// animation string, a zeroed CSoundPair() temporary and (false, true, false, 0, 0); frame 0xc0 = string 4 +
// sound pair 8 + action 0xb4. The result is kept in esi and compared with 2 AFTER both destructors: the call
// sits in an inner block that owns the string, the compare is the return after it, written
// `result == 2 ? true : false` (a plain `result == 2` adds `xor eax,eax` before the `sete al`).
class CThingCreatureBase;

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];                      // +0x04..+0xa7
    virtual ~CCreatureActionBase();                         // 0x00693EF0
};

class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];                      // +0xa8..+0xb3
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                  // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad[0x0c];                       // +0x08..+0x13
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
};

struct CAIStateGroup_SitForMeal : CAIStateGroupBase {
    unsigned char _base2[0x20];                     // +0x14
    unsigned char PPot[8];                          // +0x34 CIntelligentPointer<CThing>
    long ActionPointIndex;                          // +0x3c
    bool HoldingBowl;                               // +0x40
    long DayLastAte;                                // +0x44
    bool StandAroundAndEat();
};

bool CAIStateGroup_SitForMeal::StandAroundAndEat()
{
    long result;
    {
        CCharString animation("ST_IDLE", -1);
        result = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), animation, CSoundPair(),
            false, true, false, 0, 0), false);
    }
    return result == 2 ? true : false;
}