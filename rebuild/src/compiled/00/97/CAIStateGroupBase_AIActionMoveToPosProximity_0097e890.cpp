// CAIStateGroupBase::AIAction_MoveToPosProximity(EAIActionRepeat, C3DVector, float, bool&, EMovementType, bool, bool, bool, const CThing*, bool, float) -- steer the creature along its navigation route to within `proximity` of a position, one brain output per tick (move, look at the best face target or look around), until arrived (2), failed (3) or stopped (1)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097E890, 880 bytes (ret 34h). Twin of AIAction_MoveToPos 0x0097EC00.
// Callees: SetNavigationIgnoreThing 0x0097E270 / SetNavigationSpeedScale 0x0097E2D0 (state-group helpers on the creature's
//   navigation component; catalog "AssignCreatureToTask_Helper"/"SetTaskProperty_AtOffset" are propagated), IsStopping
//   0x008FD010, GetPBestTargetToFaceForCreature 0x0097E140 (__fastcall), CTCLook::TrackNearestInterestingThing 0x00753840,
//   CTCCreatureNavigation::GetNextNavPosOnRoute 0x006CBAC0 (CCreatureNavOutput by hidden pointer),
//   CThingCreatureBase::IsPerformingAnyAction 0x006622A0, the current action's virtual slot 49 (+0xC4),
//   CBrainOutput::CBrainOutput 0x00835620, CThingCreatureBase::GetPosition 0x006A4D00, CBrainOutput::SetAsLookAtThing
//   0x0093E760, CAIBrain::SetBrainOutput 0x0088B8F0, ~CBaseIntelligentPointer 0x00A01C10, operator delete 0x00BFE9BC,
//   the brain's virtual slot 2 (+8, yield). Epsilon 0.0001f @ 0x0129BA3C.
// Layout: state group per engine/CAIStateGroupBase.h (PBrain +4), brain creature +0x18; creature interface bitset +0x20
// (CTCLook 0x43, CTCCreatureNavigation 7) and interface map +0x44; current action +0x100; CBrainOutput per the landed
// 0x0097DF60 model (Types +0, LookFlags +4, NavigationInput +8, PAction +0x20, MoveToPos +0x28, LookAtThing +0x40).
// Shape: in this TU the interface map's lower_bound is inline (no 0x0040F020 call); the CTCLook GetTC runs the loop on its result iterator initialised from the member AFTER the length (the first pointer is read once into eax and copied to esi, `it = last` reuses esi), the type-7 lookup goes through the by-value LowerBound helper (no copy); the CBrainOutput constructor is out
// of line and its destructor inline; the arrival test is `status == 0 || |offset|^2 > eps^2`.
class CThing;
class C3DVector { public:
    float X, Y, Z;
    float GetLengthSquared() const { return X * X + (Y * Y + Z * Z); }
};
C3DVector operator+(const C3DVector& a, const C3DVector& b);
inline C3DVector operator+(const C3DVector& a, const C3DVector& b)
{
    C3DVector r;
    r.X = a.X + b.X;
    r.Y = a.Y + b.Y;
    r.Z = a.Z + b.Z;
    return r;
}
extern const float EPSILON;                     // 0x0129BA3C (0.0001f)

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
};
inline CInterfaceMap::SEntry* LowerBound(CInterfaceMap::SEntry* first, CInterfaceMap::SEntry* last, long key)
{
    long count = last - first;
    while (0 < count) {
        long count2 = count / 2;
        CInterfaceMap::SEntry* mid = first;
        mid += count2;
        if (mid->type < key) {
            first = ++mid;
            count -= count2 + 1;
        } else
            count = count2;
    }
    return first;
}

class CNavigationInput { public:
    C3DVector Destination;                      // +0x00
    float Proximity;                            // +0x0c
    long  MovementType;                         // +0x10
    float Speed;                                // +0x14
};
struct CCreatureNavOutput {
    long Status;                                // 0 arrived, 1 failed, 2 moving
    C3DVector Offset;
};
class CTCLook { public:
    bool TrackNearestInterestingThing(bool track);   // 0x00753840
};
class CTCCreatureNavigation { public:
    CCreatureNavOutput GetNextNavPosOnRoute(const CNavigationInput& input, bool b1, bool b2, bool b3, bool b4, bool b5);   // 0x006CBAC0
};
class CCreatureActionBase { public:
    virtual void v00();
    bool BlocksMovement();                      // virtual slot 49 (+0xc4), declared below
};
struct CCreatureActionVirtuals {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48();
    virtual bool BlocksMovement();              // slot 49 (+0xc4)
};

class CThingCreatureBase { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0x100 - 0x92];
    CCreatureActionVirtuals* CurrentAction;     // +0x100
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type)
    {
        long count = m_interfaces.last - m_interfaces.first;
        CInterfaceMap::SEntry* it = m_interfaces.first;
        while (0 < count) {
            long count2 = count / 2;
            CInterfaceMap::SEntry* mid = it + count2;
            if (mid->type < type) {
                it = mid + 1;
                count = count - count2 - 1;
            } else
                count = count2;
        }
        if (it == m_interfaces.last || type < it->type)
            it = m_interfaces.last;
        return it->component;
    }
    template<class T> bool GetInterface(T** ppOut, long type)
    {
        if (HasTC(type)) {
            CInterfaceMap::SEntry* it = LowerBound(m_interfaces.first, m_interfaces.last, type);
            *ppOut = (T*)(it == m_interfaces.last || type < it->type ? m_interfaces.last : it)->component;
            return true;
        }
        return false;
    }
    bool IsPerformingAnyAction();               // 0x006622A0
    const C3DVector& GetPosition() const;       // 0x006A4D00
};
CThing* __fastcall GetPBestTargetToFaceForCreature(CThingCreatureBase* creature);   // 0x0097E140

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CCountBlock {
    long Count;
    void (__fastcall* Destroy)(void* object);
    void* Object;
};
struct CCountedPointer {
    void* Object;
    CCountBlock* Block;
    ~CCountedPointer()
    {
        if (Block) {
            --Block->Count;
            if (Block->Count == 0) {
                Block->Destroy(Block->Object);
                delete Block;
            }
        }
        Object = 0;
        Block = 0;
    }
};
class CBrainOutput { public:
    unsigned long           Types;              // +0x00
    long                    LookFlags;          // +0x04
    CNavigationInput        NavigationInput;    // +0x08
    CCountedPointer         PAction;            // +0x20
    C3DVector               MoveToPos;          // +0x28
    C3DVector               LookAtPos;          // +0x34
    CBaseIntelligentPointer LookAtThing;        // +0x40
    CBrainOutput();                             // 0x00835620
    void SetAsLookAtThing(const CThing* thing, long lookFlags);   // 0x0093E760
    void SetAsMoveToPos(const C3DVector& pos, const CNavigationInput& input)
    {
        Types |= 1;
        MoveToPos = pos;
        NavigationInput = input;
    }
};
class CAIBrain { public:
    virtual void v00(); virtual void v01();
    virtual void Yield();                       // slot 2 (+0x08)
    unsigned char _pad04[0x14];
    CThingCreatureBase* Creature;               // +0x18
    void SetBrainOutput(const CBrainOutput& output);   // 0x0088B8F0
};

enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 0, AI_ACTION_REPEAT_UNTIL_DONE = 1, AI_ACTION_REPEAT_POLL = 2 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
enum EMovementType { MOVEMENT_DEFAULT };
inline bool IsDead(const CThing* thing) { return (((const CThingCreatureBase*)thing)->m_flags91 & 1) != 0; }

class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    bool IsStopping();                          // 0x008FD010
    void SetNavigationIgnoreThing(const CThing* thing);   // 0x0097E270
    void SetNavigationSpeedScale(float scale);            // 0x0097E2D0
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat repeat, C3DVector pos, float proximity, bool& moving,
                                                EMovementType anim, bool lookAround, bool b7, bool b8, const CThing* ignoreThing,
                                                bool b10, float speedScale);
};

EAIActionReturn CAIStateGroupBase::AIAction_MoveToPosProximity(EAIActionRepeat repeat, C3DVector pos, float proximity, bool& moving,
                                                               EMovementType anim, bool lookAround, bool b7, bool b8,
                                                               const CThing* ignoreThing, bool b10, float speedScale)
{
    SetNavigationIgnoreThing(ignoreThing);
    SetNavigationSpeedScale(speedScale);
    if (IsStopping())
        return AI_ACTION_STOPPING;
    EAIActionReturn result = AI_ACTION_RUNNING;
    CNavigationInput input;
    input.Destination = pos;
    input.Proximity = proximity;
    input.MovementType = anim;
    input.Speed = 0.0f;
    do {
        CThing* faceTarget = GetPBestTargetToFaceForCreature(GetCreature());
        if (GetCreature()->HasTC(0x43))
            ((CTCLook*)GetCreature()->GetTC(0x43))->TrackNearestInterestingThing(true);
        CTCCreatureNavigation* navigation;
        GetCreature()->GetInterface(&navigation, 7);
        CCreatureNavOutput output = navigation->GetNextNavPosOnRoute(input, b7, b8, b10, true, false);
        moving = output.Status == 0 || output.Offset.GetLengthSquared() > EPSILON * EPSILON;
        if (!GetCreature()->IsPerformingAnyAction() || !GetCreature()->CurrentAction->BlocksMovement()) {
            switch (output.Status) {
            case 0:
                result = AI_ACTION_DONE;
                break;
            case 1:
                result = AI_ACTION_ABORTED;
                break;
            case 2: {
                CBrainOutput brainOutput;
                brainOutput.SetAsMoveToPos(output.Offset + GetCreature()->GetPosition(), input);
                if (faceTarget && !IsDead(faceTarget))
                    brainOutput.SetAsLookAtThing(faceTarget, 0x97);
                else if (lookAround) {
                    brainOutput.Types |= 0x10;
                    brainOutput.LookFlags = 0x51;
                }
                PBrain->SetBrainOutput(brainOutput);
            }
                result = AI_ACTION_RUNNING;
                break;
            }
        }
        PBrain->Yield();
        if (IsStopping())
            return AI_ACTION_STOPPING;
    } while (repeat == AI_ACTION_REPEAT_UNTIL_DONE && result == AI_ACTION_RUNNING);
    return result;
}