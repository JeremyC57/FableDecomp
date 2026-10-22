// CAIStateGroupBase::AIAction_FindAGuardAndGiveMessage(CDefPointer<const CMessageEventDef>, C3DVector, long) -- find the nearest guard, flee to him (alerting) and hand him the message event; 3 when no guard or the hand-over failed, else the flee result / 2
// Grade: DIFFER(567v521) -- by-value CDefPointer param release keeps re-reading/materialising (retail `dec [ecx+4] ; jne`), guard=0 store and nearest-compare copy scheduled earlier, world-filter vtable/arg order
// Retail: Fable.exe (TLC, VC7.1) @ 0x009810B0, 528 bytes (ret 14h).
// Callees: CThingCreatureBase::GetPosition 0x006A4D00, CAIStateGroupBase::PeekThingSearchTools 0x008FD000,
//   CThingSearchTools::GetPBestLeafThing<filter, CThingCompare_Nearest, CThingObject> 0x009823D0 (catalog filter name
//   "CIsFreeHopscotchBoard" is propagated), CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem
//   0x00A01B50 / dtor 0x00A01C10 (CIntelligentPointer vtables 0x0123E7E0 and 0x0125AAEC), the state group's world
//   accessor 0x008FCFA0, CAIFilter_NoOtherYellForHelp (vtable 0x01299350), CCreatureAction_AlertToPlayer(CThingAICreature*)
//   0x00842270, AIAction_FleeToThingProximity 0x009804A0, ~CCreatureActionBase 0x00693EF0 (catalog "~CPhysicsMeshInfo"),
//   CCreatureAction_SendMessage 0x00982210, AIAction_PerformActionAndWait 0x0097DB70.
// Layout: state group per engine/CAIStateGroupBase.h (PBrain +4), modelled locally; brain creature +0x18. The two action
// temporaries share one frame block: AlertToPlayer adds nothing to the 0xA8-byte CCreatureActionBase, SendMessage adds
// the message def +0xA8, position +0xAC, receiver pointer +0xB8 and the parameter +0xC0 (destroyed inline).
// Shape: the nearest-guard search with a type filter and a nearest-to-creature compare built in the call; both action
// temporaries built inside the AIAction calls; the by-value message def is released at each exit.
struct CThing;
struct CWorld;
struct C3DVector {
    float X, Y, Z;
};

struct CDefCounted {
    virtual void v00();
    virtual void DeleteThis();                  // slot 1 (+4)
    long m_refCount;                            // +4
    void DecRef() { --m_refCount; }
    long GetRefCount() const { return m_refCount; }
    void Release() { DecRef(); if (GetRefCount() == 0) DeleteThis(); }
};
struct CMessageEventDef : CDefCounted { };
template <class T> struct CDefPointer {        // the by-value parameter: its null store dies before the returns
    T* m_pDef;
    ~CDefPointer() { T* def = m_pDef; if (def) { if (--def->m_refCount == 0) def->DeleteThis(); } }
};
template <class T> struct CDefPointerMember {  // a member destroyed inline: the def's own Release(), then null
    T* m_pDef;
    ~CDefPointerMember() { if (m_pDef) { m_pDef->Release(); m_pDef = 0; } }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7E0
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
struct CIntelligentPointerConstCreature : CBaseIntelligentPointer {   // CIntelligentPointer<const CThingCreatureBase>, vtable 0x0125AAEC
    virtual void v00();
    CIntelligentPointerConstCreature(const CBaseIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CThingCreatureBase {
    const C3DVector& GetPosition() const;       // 0x006A4D00
};
struct CThingObject;
struct CThingFilter_CreatureType {              // 4 bytes, no vtable
    long Type;
    CThingFilter_CreatureType(long type) : Type(type) { }
};
struct CThingCompare_Nearest {                  // 12 bytes: the reference position
    C3DVector Position;
    CThingCompare_Nearest(const C3DVector& position) : Position(position) { }
};
struct CThingSearchTools {
    bool GetPBestLeafThing(const CThingFilter_CreatureType& filter, const CThingCompare_Nearest& compare,
                           CThingObject** result);   // 0x009823D0
};

struct CAIFilterBase {
    virtual bool IsStillValid() const;
};
struct CAIFilter_NoOtherYellForHelp : CAIFilterBase {   // vtable 0x01299350
    CWorld* World;
    CAIFilter_NoOtherYellForHelp(CWorld* world) : World(world) { }
};

struct CCreatureActionBase {                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_AlertToPlayer : CCreatureActionBase {
    CCreatureAction_AlertToPlayer(CThingCreatureBase* creature);   // 0x00842270
};
struct CCreatureAction_SendMessage : CCreatureActionBase {
    CDefPointerMember<CMessageEventDef> PMessage;   // +0xa8
    C3DVector Position;                         // +0xac
    CIntelligentPointerCreature PReceiver;      // +0xb8
    long Parameter;                             // +0xc0
    CCreatureAction_SendMessage(CThingCreatureBase* creature, const CDefPointer<CMessageEventDef>& message,
                                CThing* receiver, const C3DVector& position, long parameter);   // 0x00982210
};

struct CAIBrain {
    unsigned char _pad00[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };

struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    CWorld* GetPWorld();                        // 0x008FCFA0
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    EAIActionReturn AIAction_PerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced);   // 0x0097DB70
    EAIActionReturn AIAction_FleeToThingProximity(CIntelligentPointerConstCreature thing, float proximity,
                                                  CCreatureActionBase* action, CAIFilterBase* filter);   // 0x009804A0
    EAIActionReturn AIAction_FindAGuardAndGiveMessage(CDefPointer<CMessageEventDef> message, C3DVector position, long parameter);
};

EAIActionReturn CAIStateGroupBase::AIAction_FindAGuardAndGiveMessage(CDefPointer<CMessageEventDef> message, C3DVector position,
                                                                    long parameter)
{
    CThingObject* guard = 0;
    if (!PeekThingSearchTools()->GetPBestLeafThing(CThingFilter_CreatureType(3), CThingCompare_Nearest(GetCreature()->GetPosition()),
                                                   &guard))
        return AI_ACTION_ABORTED;
    CIntelligentPointerCreature guardPointer((CThing*)guard);
    EAIActionReturn result = AIAction_FleeToThingProximity(guardPointer, 2.0f, &CCreatureAction_AlertToPlayer(GetCreature()),
                                                           &CAIFilter_NoOtherYellForHelp(GetPWorld()));
    if (result != AI_ACTION_DONE)
        return result;
    CThingCreatureBase* creature = GetCreature();
    bool failed = AIAction_PerformActionAndWait(&CCreatureAction_SendMessage(creature, message, guardPointer.GetPItem(), position,
                                                                             parameter), false) != AI_ACTION_DONE;
    if (failed)
        return AI_ACTION_ABORTED;
    return AI_ACTION_DONE;
}
