// CAIStateGroup_TavernServer::PickUpJug() -- collect the jug from the bar: free the bar point and fail when the jug
// is dead; otherwise run a CCreatureAction_PickUpJug ("COLLECT_JUG_FROM_BAR", mode 0x29) to completion with the
// creature unpushable, then free the bar point, restore pushability and report whether the action succeeded.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00967930, length 232 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PJug +0x14, three times), FreeBarPoint @ 0x00967110
// (manifest files it under CAIStateGroup_TavernKeeper; it sits among the server bodies), CAIStateGroupBase::
// SetPushableByHero(bool) @ 0x0097E330, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
// CCreatureAction_PickUpThingAtTable ctor @ 0x008440C0 (owner, thing, const CCharString&, mode), the derived vtable
// 0x012963A4 (CCreatureAction_PickUpJug, rtti-vtables.tsv), CAIStateGroupBase::AIAction_PerformActionAndWait
// @ 0x0097DB70 (action, false; 2 = done), the action's pointer member dtor @ 0x00A01C10 (+0xa8) and the action base
// dtor @ 0x00693EF0 (manifest "CPhysicsMeshInfo::~CPhysicsMeshInfo" is propagated).
// Shape: the creature a named local read before the jug fetch; the action is a block-scoped local (0xb4 bytes) built by an inline derived ctor whose animation name comes
// from an inline by-value helper (string built in the one-word slot before the pushes, destroyed before the vtable
// store); the `!= 2` bool (`setne bl`) is declared outside the block so it survives the action's destructors; the
// two tails each repeat FreeBarPoint + SetPushableByHero(true); IS_THING_ALIVE tests the dead bit through an inline
// CThing::IsDead(), which gives retail's colouring (&PJug/jug in edi, creature in ebx). Engine header is padding only,
// so a local model.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CThing { public:
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
};
class CThingCreatureBase;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CCharString { public:                            // 4 bytes: the shared string data pointer
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CCreatureActionBase { public:                    // the action root (vptr + body up to the pointer member)
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    virtual ~CCreatureActionBase();                     // 0x00693EF0
};

class CCreatureAction_PickUpThingAtTable : public CCreatureActionBase { public:   // retail sizeof 0xb4
    CIntelligentPointer<CThing> PThing;             // +0xa8
    unsigned char _pad_0xb0[4];
    CCreatureAction_PickUpThingAtTable(CThingCreatureBase* owner, CThing* thing, const CCharString& animation, long mode);   // 0x008440C0
};

static inline CCharString PickUpJugAnimationName() { return CCharString("COLLECT_JUG_FROM_BAR", -1); }   // 0x012964BC

class CCreatureAction_PickUpJug : public CCreatureAction_PickUpThingAtTable { public:   // vtable 0x012963A4
    virtual void v00();
    CCreatureAction_PickUpJug(CThingCreatureBase* owner, CThing* jug)
        : CCreatureAction_PickUpThingAtTable(owner, jug, PickUpJugAnimationName(), 0x29) { }
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
    void SetPushableByHero(bool pushable);                              protected: // 0x0097E330
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool interruptible); public:   // 0x0097DB70
};

struct CAIStateGroup_TavernServer : CAIStateGroupBase {
    CIntelligentPointer<CThing> PJug;               // +0x14 CIntelligentPointer<CThingObject>
    void FreeBarPoint();                        // 0x00967110
    bool PickUpJug();
};

bool CAIStateGroup_TavernServer::PickUpJug()
{
    if (!IS_THING_ALIVE(PJug)) {
        FreeBarPoint();
        return false;
    }
    SetPushableByHero(false);
    bool failed;
    {
        CThingCreatureBase* creature = (CThingCreatureBase*)PBrain->PCreature;
        CCreatureAction_PickUpJug action(creature, PJug.GetPItem());
        failed = AIAction_PerformActionAndWait(action, false) != 2;
    }
    if (failed) {
        FreeBarPoint();
        SetPushableByHero(true);
        return false;
    }
    FreeBarPoint();
    SetPushableByHero(true);
    return true;
}