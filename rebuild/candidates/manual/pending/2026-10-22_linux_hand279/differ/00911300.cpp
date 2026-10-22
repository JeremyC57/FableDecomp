// Grade: DIFFER(666v666) -- one instruction: retail's zero-mask IsThingOfType stores the creature's ThingTypeFlags (+0x6c) to a frame word before `xor al,al ; jne`; this source (the landed `const CThing* const&` form) stores the thing pointer instead, and the by-value flags getter folds the store away (DIFFER 654v666); next: a reference-to-flags parameter bound to a copied dword; 4 grades spent
// CAIStateGroup_MinionGuardReactToBodyFoundMessage::Update() -- take the body-found message: when its body is alive, consume the message, alert and walk up to the body unless already within 2.5, look at it, walk to the reported spot and play the inspect action; terminate in every case
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00911300, length 672 bytes (+ int3 padding), plain `ret`, `and esp,-8` frame 0xc8.
// Callees: FindBodyFoundEvent @ 0x009112A0, CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 /
// dtor @ 0x00A01C10, CIntelligentPointer<const CThing> copy ctor @ 0x00654D58 (vtable 0x01237F1C), CAIStateGroupBase::GetPWorld
// @ 0x008FCFC0 / Terminate @ 0x008FCF40, CMessageEventManager::RemoveMessage @ 0x00493B5B, CThing::GetPosition @ 0x004C73D0,
// GFWorld3DDistanceSmallerThan @ 0x006614F0 (__fastcall), the alert action ctor @ 0x00911180 and the inspect action ctor @ 0x009110F0
// (both CCreatureActionBase-sized, ~CCreatureActionBase @ 0x00693EF0), AIAction_PerformActionAndWait @ 0x0097DB70,
// AIAction_MoveToThingProximity @ 0x0097E380, AIAction_LookAtThing @ 0x0097D6F0, AIAction_MoveToPosProximity @ 0x0097E890.
// Layout: local model (PBrain +4; message Position +0x0c, POriginator +0x48, thing type flags +0x6c; creature virtual slot 92 (+0x170)
// the look speed in degrees; world message manager +0x60).
// Shape: the action temporaries' results are compared before their destructor (setne); failures `Terminate(); return;` inside the
// pointer's scope, success falls out of the scope to the final Terminate.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); 
    virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b(); virtual void v0c(); virtual void v0d(); virtual void v0e(); virtual void v0f(); 
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); 
    virtual void v18(); virtual void v19(); virtual void v1a(); virtual void v1b(); virtual void v1c(); virtual void v1d(); virtual void v1e(); virtual void v1f(); 
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); 
    virtual void v28(); virtual void v29(); virtual void v2a(); virtual void v2b(); virtual void v2c(); virtual void v2d(); virtual void v2e(); virtual void v2f(); 
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); 
    virtual void v38(); virtual void v39(); virtual void v3a(); virtual void v3b(); virtual void v3c(); virtual void v3d(); virtual void v3e(); virtual void v3f(); 
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); 
    virtual void v48(); virtual void v49(); virtual void v4a(); virtual void v4b(); virtual void v4c(); virtual void v4d(); virtual void v4e(); virtual void v4f(); 
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); 
    virtual void v58(); virtual void v59(); virtual void v5a(); virtual void v5b(); 
    virtual float GetLookSpeed() const;         // slot 92 (+0x170)
    unsigned char _pad04[0x68];
    unsigned long ThingTypeFlags;               // +0x6c
    unsigned char _pad70[0x21];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    enum { ThingTypeMask = 0 };
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CThingAICreature : CThing { };
template<class T> inline bool IsThingOfType(const CThing* const& thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
bool __fastcall GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float distance);   // 0x006614F0
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    __forceinline CIntelligentPointer(T* item) { SetPItem(item); }
    template<class U> __forceinline CIntelligentPointer(const CIntelligentPointer<U>& other) { SetPItem(other.GetPItem()); }
    CIntelligentPointer(const CIntelligentPointer& other);   // 0x00654D58
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    template<class U> U* GetTypedPItem() const
    {
        U* result = 0;
        if (GetPItem() && IsThingOfType<U>((U*)GetPItem()))
            result = (U*)GetPItem();
        return result;
    }
};
inline bool IsThingAlive(const CBaseIntelligentPointer& thing) { return thing.GetPItem() != 0 && !(thing.GetPItem()->m_flags91 & 1); }
struct CMessageEvent {
    unsigned char _pad00[0x0c];
    C3DVector Position;                         // +0x0c
    unsigned char _pad18[0x30];
    CIntelligentPointer<CThing> POriginator;    // +0x48
};
struct CMessageEventManager {
    void RemoveMessage(const CMessageEvent* message);   // 0x00493B5B
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageEventManager; // +0x60
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_AlertToPlayer : CCreatureActionBase {
    CCreatureAction_AlertToPlayer(CThingAICreature* creature);   // 0x00911180
};
struct CCreatureAction_InspectBody : CCreatureActionBase {
    CCreatureAction_InspectBody(CThingAICreature* creature);     // 0x009110F0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingAICreature* GetCreature() const { return PBrain->Creature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    void Terminate();                           // 0x008FCF40
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointer<CThing> thing, float proximity, bool* arrived,
                                       long movement, bool a, bool b, bool c, bool d, float e);   // 0x0097E380
    long AIAction_LookAtThing(long repeat, CIntelligentPointer<const CThing> thing, long frames, float speed);   // 0x0097D6F0
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float proximity, bool* arrived, long movement,
                                     bool a, bool b, bool c, const CThing* ignore, bool d, float e);   // 0x0097E890
};
struct CAIStateGroup_MinionGuardReactToBodyFoundMessage : CAIStateGroupBase {
    const CMessageEvent* FindBodyFoundEvent();  // 0x009112A0
    void Update();
};

void CAIStateGroup_MinionGuardReactToBodyFoundMessage::Update()
{
    const CMessageEvent* message = FindBodyFoundEvent();
    if (message) {
        CIntelligentPointer<const CThing> body(message->POriginator.GetTypedPItem<CThing>());
        if (!IsThingAlive(body)) {
            Terminate();
            return;
        }
        C3DVector bodyPos = message->Position;
        CMessageEventManager* manager = GetPWorld()->PMessageEventManager;
        manager->RemoveMessage(message);
        if (!GFWorld3DDistanceSmallerThan(GetCreature()->GetPosition(), body.GetPItem()->GetPosition(), 2.5f)) {
            if (AIAction_PerformActionAndWait(&CCreatureAction_AlertToPlayer(GetCreature()), false) != 2) {
                Terminate();
                return;
            }
            bool arrived = false;
            long result;
            do {
                if (!IsThingAlive(body)) {
                    Terminate();
                    return;
                }
                result = AIAction_MoveToThingProximity(2, body, 2.5f, &arrived, 5, true, false, true, true, 0.0f);
            } while (result == 4);
            if (result != 2) {
                Terminate();
                return;
            }
        }
        if (IsThingAlive(body)) {
            if (AIAction_LookAtThing(1, body, 0xf, GetCreature()->GetLookSpeed() * 0.0027777778f) == 2) {
                bool arrived = false;
                if (AIAction_MoveToPosProximity(1, bodyPos, 1.5f, &arrived, 5, true, false, true, 0, true, 0.0f) != 2) {
                    Terminate();
                    return;
                }
                if (AIAction_PerformActionAndWait(&CCreatureAction_InspectBody(GetCreature()), false) != 2) {
                    Terminate();
                    return;
                }
            }
        }
    }
    Terminate();
}
