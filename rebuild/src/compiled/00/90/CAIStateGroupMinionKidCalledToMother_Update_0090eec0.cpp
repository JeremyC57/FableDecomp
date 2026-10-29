// CAIStateGroup_MinionKidCalledToMother::Update() -- while the mother's "call home" message is still held and the hero lives: when the kid is within 1.2 units (2D) of its living mother it looks at the hero, otherwise it walks to the mother (retrying while the move is in progress); a vanished message or mother terminates the group
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090EEC0, length 515 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: FindCallHomeEvent @ 0x0090EE20, CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 /
// SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10 (the held message: CIntelligentPointer<CMessageEvent> vtable 0x0128B0C0;
// the by-value targets: CIntelligentPointer<const CThing> vtable 0x01237F1C), the player-manager accessor @ 0x008FCF80,
// GetPMainPlayer @ 0x00449960, CPlayer::PeekPControlledCreature @ 0x00487DD0, CThing::GetPos @ 0x004C73D0,
// the creature's virtual slot 92 (+0x170, an angle in degrees), AIAction_LookAtThing @ 0x0097D6F0,
// AIAction_MoveToThingProximity @ 0x0097E380, AIAction_DoNothing @ 0x0097E200, Terminate @ 0x008FCF40.
// Data: 1.44f (1.2 squared) @ 0x0128F570, 1/360 @ 0x01238E00 (`/ 360.0f` becomes a reciprocal multiply).
// Layout: the engine header CAIStateGroup_MinionKidCalledToMother.h flattens the base into a pad, so the model is local
// (PBrain +4, brain PCreature +0x18, creature PMother +0x180, dead bit +0x91 bit 0), as in the landed 0x0090EE20.
// Shape: the move result is a function-scope long initialised to 3 before the loop (retail keeps it in ebp and tests it
// after the inner Terminate); the 8-byte held pointer forces the `and esp,-8` frame.
class CTCBase;
class CBaseObject { public:
    virtual ~CBaseObject();
};
class C3DVector { public: float X, Y, Z; };
class CThing : public CBaseObject { public:
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84();
    virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    virtual void v90(); virtual void v91();
    virtual float GetTurnAngleDegrees();        // slot 92 (+0x170)
    unsigned char _pad4[0x91 - 4];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class CThingPlayerCreature : public CThing { };
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                       // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
class CMessageEvent : public CBaseObject { public: };
struct CIntelligentPointerMessage : CBaseIntelligentPointer {    // CIntelligentPointer<CMessageEvent>, vtable 0x0128B0C0
    virtual void v00();
    __forceinline CIntelligentPointerMessage(CMessageEvent* item) { SetPItem(item); }
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer { // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    __forceinline CIntelligentPointerConstThing(const CThing* item) { SetPItem(const_cast<CThing*>(item)); }
};
class CThingAICreature : public CThing { public:
    unsigned char _pad94[0x180 - 0x94];
    CBaseIntelligentPointer PMother;            // +0x180
};
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;            // 0x00487DD0
};
class CPlayerManager { public:
    CPlayer* GetPMainPlayer();                  // 0x00449960
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    CPlayerManager* GetPPlayerManager_008fcf80();        // 0x008FCF80
    void Terminate();                           // 0x008FCF40
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long frames, float speed);   // 0x0097D6F0
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing target, float distance, bool* reached,
        long movement, bool a, bool b, bool c, bool d, float speed);         // 0x0097E380
    long AIAction_DoNothing();                  // 0x0097E200
};
struct CAIStateGroup_MinionKidCalledToMother : CAIStateGroupBase {
    CMessageEvent* FindCallHomeEvent();         // 0x0090EE20
    virtual void Update();
};

void CAIStateGroup_MinionKidCalledToMother::Update()
{
    CIntelligentPointerMessage callHome(FindCallHomeEvent());
    if (((CThing*)callHome.GetPItem()) == 0)
        return;
    long result = 3;
    do {
        if (((CThing*)callHome.GetPItem()) == 0) {
            Terminate();
            return;
        }
        const CThing* hero = GetPPlayerManager_008fcf80()->GetPMainPlayer()->PeekPControlledCreature();
        if (hero != 0 && !(hero->m_flags91 & 1)) {
            CThing* mother = ((CThing*)GetCreature()->PMother.GetPItem());
            if (mother != 0 && !(mother->m_flags91 & 1)) {
                CThingAICreature* creature = GetCreature();
                float distanceSquared;
                {
                    const C3DVector& motherPos = ((CThing*)creature->PMother.GetPItem())->GetPos();
                    const C3DVector& pos = creature->GetPos();
                    float dx = pos.X - motherPos.X;
                    float dy = pos.Y - motherPos.Y;
                    distanceSquared = dy * dy + dx * dx;
                }
                if (distanceSquared < 1.44f) {
                    if (AIAction_LookAtThing(1, CIntelligentPointerConstThing(hero), 9,
                            GetCreature()->GetTurnAngleDegrees() / 360.0f) != 2)
                        return;
                } else {
                    bool reached = false;
                    do {
                        if (((CThing*)callHome.GetPItem()) == 0 || (mother = ((CThing*)GetCreature()->PMother.GetPItem())) == 0
                                || (mother->m_flags91 & 1)) {
                            Terminate();
                            break;
                        }
                        CThing* target = ((CThing*)GetCreature()->PMother.GetPItem());
                        result = AIAction_MoveToThingProximity(2, CIntelligentPointerConstThing(target), 1.2f, &reached, 5,
                            true, false, true, true, 0.0f);
                    } while (result == 4);
                    if (result != 2)
                        return;
                }
            }
        }
    } while (AIAction_DoNothing() == 4);
}