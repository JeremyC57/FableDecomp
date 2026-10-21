// CAIStateGroup_VillageTaskHorn::Update() -- accept the village horn task, walk to the horn and use it
// Leaf: Update
// Grade: DIFFER(416v431) -- same shape as 0x0093EA90 (VillageTaskTorch): argument-evaluation order for SetHornTaskAsAcceptedByThing, ebp vs edi for &PTask, mov/add vs lea for the horn IP, plus the action frame (0x110)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00940960; best of the probes graded under VC7.1 RTM (probe/00940960.cpp).
struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) { }
};
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
extern CCharString g_CCharString_013bac9c;      // "ACTION_POINT"
extern const float g_ProximityEpsilon;         // 0x0129BA3C (0.0001f)
struct CThingCreatureBase;
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* pos,
        CRightHandedSet* orientation, float scale);     // 0x004C08C0
};
struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06();
    virtual bool IsValidForThing(CThing* thing);    // slot 7 (+0x1c)
    unsigned char _pad04[0x64 - 0x04];
    const C3DVector& GetPosition() const;       // 0x004C73D0
    CTCGraphicAppearance* PGraphicAppearance;   // +0x64
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CVillageTaskHorn {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06();
    virtual bool IsValidForThing(CThing* thing);    // slot 7 (+0x1c)
    long _pad04;
    CBaseIntelligentPointer PHorn;             // +0x08
};
struct CIntelligentPointerTask : CBaseIntelligentPointer {         // CIntelligentPointer<CVillageTaskHorn>
    virtual void v00();
    CVillageTaskHorn* GetPItem() const;        // 0x00A01B50 (CBaseIntelligentPointer::GetPItem)
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<...> (vtable 0x0128B0A8)
    virtual void v00();
    __forceinline CIntelligentPointerConstThing(CVillageTaskHorn* item) : CBaseIntelligentPointer() { SetPItem((CThing*)item); }
};
struct CTCVillage {
    bool SetHornTaskAsAcceptedByThing(CVillageTaskHorn* task, CThing* thing);   // 0x006584F9
    void HornTaskCompleted(CIntelligentPointerConstThing task);       // 0x0065A00F
};
struct CTCVillageMember {
    CTCVillage* GetVillage();                   // 0x007EE540
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_UseObject : CCreatureActionBase {
    CBaseIntelligentPointer PObject;            // +0xa8
    unsigned char _use[0xb0 - 0xb0];
    CCreatureAction_UseObject(CThingCreatureBase* creature, CThing* object, bool a);   // 0x0069F230
};
struct CThingCreature : CThing { };
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreature* PCreature;                  // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    void Terminate();                           // 0x008FCF40
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movement, bool a, bool b,
        CThing* thing, bool c, float speed);    // 0x0097EC00
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float proximity, bool* reached, long movement,
        bool a, bool b, bool c, CThing* thing, bool d, float speed);   // 0x0097E890
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool b);   // 0x0097DB70
};
struct CAIStateGroup_VillageTaskHorn : CAIStateGroupBase {
    CTCVillageMember* PTCVillageMember;         // +0x14
    CIntelligentPointerTask PTask;              // +0x18
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)PBrain->PCreature; }
    bool IsTaskValid() { return PTask.GetPItem() != 0 && PTask.GetPItem()->IsValidForThing(PBrain->PCreature); }
    virtual void Update();
};

void CAIStateGroup_VillageTaskHorn::Update()
{
    if (PTask.GetPItem()) {
        CTCVillage* village = PTCVillageMember->GetVillage();
        if (village->SetHornTaskAsAcceptedByThing(PTask.GetPItem(), PBrain->PCreature)) {
            CThing* horn = PTask.GetPItem()->PHorn.GetPItem();
            horn->GetPosition();
            C3DVector pos;
            CRightHandedSet orientation;
            horn->PGraphicAppearance->GetDummyObject(g_CCharString_013bac9c, 0, 0, &pos, &orientation, 1.0f);
            long result;
            do {
                bool reached = false;
                result = AIAction_MoveToPosProximity(2, pos, g_ProximityEpsilon, &reached, 3, true, false, true, 0,
                    true, 0.0f);
                if (!IsTaskValid())
                    goto done;
            } while (result == 4);
            if (result != 2)
                goto done;
            for (int i = 0; i < 3; ++i) {
                result = AIAction_PerformActionAndWait(&CCreatureAction_UseObject(GetCreature(), horn, true), false);
                if (result != 2 || !IsTaskValid())
                    goto done;
            }
            village->HornTaskCompleted(CIntelligentPointerConstThing(PTask.GetPItem()));
        }
    }
done:
    Terminate();
}
