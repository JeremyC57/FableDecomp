// CAIStateGroup_BalverineWander::Update() -- wander near the start position (straight back to it once 10 or more away), then play an animation (HOWL with its sound 20% of the time, else ST_IDLE), repeating while idling keeps running; terminate when a move fails
// Retail: Fable.exe (TLC, VC7.1) @ 0x0091BD10, 638 bytes (aligned frame). ?Update@CAIStateGroup_BalverineWander@@UAEXXZ (Ego_r.pdb).
// Callees: CThing::GetPosition 0x004C73D0, CTCCreatureNavigation::GetNearbyWanderPosition 0x006CB6E0 (interface 7, inline
//   lower_bound), AIAction_MoveToPosProximity 0x0097E890, GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, the creature's GetSoundPair (slot 89, +0x164),
//   CCreatureAction_PlayAnimation ctor 0x008424F0, AIAction_PerformAction 0x0097D8B0, ~CCreatureActionBase 0x00693EF0,
//   AIAction_DoNothing 0x0097E200, Terminate 0x008FCF40. Literals "HOWL" @ 0x0128FF58, "ST_IDLE" @ 0x0127138C; 100.0f @ 0x01230A08.
// Layout: state group PBrain +4, brain creature +0x18; retail CThing interface bitset +0x20, map +0x44.
// Shape: the navigation lookup is the GetInterfaceR form with the lower_bound loop inline (length first); the offset is a
//   memberwise operator- (X, Y, Z order) into a named vector and a named squared distance; the far test is written
//   `!(d < 100)` first so both `dest = startPos` copies survive; GetCreature() is written twice in the HOWL action so the
//   implicit CCharString temporaries are built before the creature load.
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const
    {
        C3DVector r;
        r.X = X - o.X;
        r.Y = Y - o.Y;
        r.Z = Z - o.Z;
        return r;
    }
    float GetLengthSquared() const { return X * X + (Y * Y + Z * Z); }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
};
unsigned long __fastcall GFROR13(unsigned long value);    // 0x00497890
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
class CTCCreatureNavigation { public:                  // interface type 7
    bool GetNearbyWanderPosition(const C3DVector& from, float radius, C3DVector* out, bool flag);   // 0x006CB6E0
};
class CThing { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b();
    virtual void v0c(); virtual void v0d(); virtual void v0e(); virtual void v0f(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v1a(); virtual void v1b(); virtual void v1c(); virtual void v1d();
    virtual void v1e(); virtual void v1f(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v2a(); virtual void v2b(); virtual void v2c(); virtual void v2d(); virtual void v2e(); virtual void v2f();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v3a(); virtual void v3b();
    virtual void v3c(); virtual void v3d(); virtual void v3e(); virtual void v3f(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v4a(); virtual void v4b(); virtual void v4c(); virtual void v4d();
    virtual void v4e(); virtual void v4f(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual CSoundPair GetSoundPair(const CCharString& name);   // slot 89 (+0x164)
    const C3DVector& GetPosition() const;       // 0x004C73D0
    unsigned char _pad04[0x20 - 4];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline void GetInterfaceR(T** ppOut, long interfaceType, bool& has)
    {
        has = TestTC(interfaceType);
        if (has) {
            long count = m_interfaces.last - m_interfaces.first;
            CInterfaceMap::SEntry* it = m_interfaces.first;
            while (0 < count) {
                long count2 = count / 2;
                CInterfaceMap::SEntry* mid = it + count2;
                if (mid->type < interfaceType) {
                    it = mid + 1;
                    count = count - count2 - 1;
                } else
                    count = count2;
            }
            if (it == m_interfaces.last || interfaceType < it->type)
                it = m_interfaces.last;
            *ppOut = (T*)it->component;
        }
    }
};
class CThingCreatureBase : public CThing { public:
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _members[0xb4 - 0xa8];
    CCreatureAction_PlayAnimation(CThingCreatureBase* owner, const CCharString& animationName, const CSoundPair& sound,
                                  bool stayOnLastFrame, bool b, bool c, long d, long e);   // 0x008424F0
};
class CAIBrain { public:
    unsigned char _pad00[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 0, AI_ACTION_REPEAT_UNTIL_DONE = 1, AI_ACTION_REPEAT_POLL = 2 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
enum EMovementType { MOVEMENT_DEFAULT, MOVEMENT_WALK = 3, MOVEMENT_RUN = 5 };
class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0
    void Terminate();                           // 0x008FCF40
    unsigned long Random(unsigned long range) const
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24a1 + 0x24df;
        *seed = GFROR13(*seed);
        return *seed % range;
    }
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat repeat, C3DVector pos, float proximity, bool& moving,
                                                EMovementType anim, bool b1, bool b2, bool b3, const CThing* target,
                                                bool b4, float f);     // 0x0097E890
    EAIActionReturn AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
    EAIActionReturn AIAction_DoNothing();       // 0x0097E200
};
struct CAIStateGroup_BalverineWander : CAIStateGroupBase {
    void Update();
};

void CAIStateGroup_BalverineWander::Update()
{
    C3DVector startPos = GetCreature()->GetPosition();
    CTCCreatureNavigation* nav;
    do {
        C3DVector dest;
        C3DVector offset = GetCreature()->GetPosition() - startPos;
        float distanceSquared = offset.GetLengthSquared();
        if (!(distanceSquared < 100.0f))
            dest = startPos;
        else {
            CThingCreatureBase* creature = GetCreature();
            bool found;
            creature->GetInterfaceR(&nav, 7, found);
            if (!nav->GetNearbyWanderPosition(creature->GetPosition(), 4.0f, &dest, true))
                dest = startPos;
        }
        bool moving;
        if (AIAction_MoveToPosProximity(AI_ACTION_REPEAT_UNTIL_DONE, dest, 1.5f, moving, MOVEMENT_WALK, true, false, true, 0,
                                        true, 0.0f) != AI_ACTION_DONE) {
            Terminate();
            return;
        }
        if (Random(100) < 20) {
            AIAction_PerformAction(&CCreatureAction_PlayAnimation(GetCreature(), "HOWL",
                                   GetCreature()->GetSoundPair("HOWL"), false, true, false, 0, 0));
        } else
            AIAction_PerformAction(&CCreatureAction_PlayAnimation(GetCreature(), "ST_IDLE", CSoundPair(), false,
                                   true, false, 0, 0));
    } while (AIAction_DoNothing() == AI_ACTION_RUNNING);
}