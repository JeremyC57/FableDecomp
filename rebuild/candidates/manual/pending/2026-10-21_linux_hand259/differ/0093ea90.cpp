// CAIStateGroup_VillageTaskTorch::Update() -- accept the village torch task, walk to the torch and use it
// Leaf: Update
// Grade: DIFFER(422v452) -- register/order: retail pushes the creature before calling PTask.GetPItem() for SetTaskAsAcceptedByThing, keeps &PTask in edi without ebp, and reaches the torch IP with lea ecx,[eax+8]; ours hoists the GetPItem call, uses ebp and mov/add; UseObject frame size unresolved
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093EA90; best of the probes graded under VC7.1 RTM (probe/0093ea90c.cpp).
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
extern CCharString g_CCharString_013baca0;      // "ACTION_POINT_1"
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
struct CVillageTaskTorch {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06();
    virtual bool IsValidForThing(CThing* thing);    // slot 7 (+0x1c)
    long _pad04;
    CBaseIntelligentPointer PTorch;             // +0x08
};
struct CIntelligentPointerTask : CBaseIntelligentPointer {         // CIntelligentPointer<CVillageTaskTorch>
    virtual void v00();
    CVillageTaskTorch* GetPItem() const;        // 0x00A01B50 (CBaseIntelligentPointer::GetPItem)
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>
    virtual void v00();
    CIntelligentPointerConstThing(const CIntelligentPointerTask& other);   // 0x0093E800
};
struct CTCVillage {
    bool SetTorchTaskAsAcceptedByThing(CVillageTaskTorch* task, CThing* thing);   // 0x006584C5
    void TorchTaskCompleted(CIntelligentPointerConstThing task);       // 0x00659FED
};
struct CTCVillageMember {
    CTCVillage* GetVillage();                   // 0x007EE540
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
};
struct CCreatureAction_UseObject : CCreatureActionBase {
    unsigned char _use[0xe0 - 0xa8];
    CCreatureAction_UseObject(CThingCreatureBase* creature, CThing* object, bool a);   // 0x0069F230
    ~CCreatureAction_UseObject();               // 0x006290E0
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
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool b);   // 0x0097DB70
};
struct CAIStateGroup_VillageTaskTorch : CAIStateGroupBase {
    CTCVillageMember* PTCVillageMember;         // +0x14
    CIntelligentPointerTask PTask;              // +0x18
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)PBrain->PCreature; }
    bool IsTaskValid() { return PTask.GetPItem() != 0 && PTask.GetPItem()->IsValidForThing(PBrain->PCreature); }
    virtual void Update();
};

void CAIStateGroup_VillageTaskTorch::Update()
{
    if (PTask.GetPItem()) {
        CTCVillage* village = PTCVillageMember->GetVillage();
        if (village->SetTorchTaskAsAcceptedByThing(PTask.GetPItem(), PBrain->PCreature)) {
            CThing* torch = PTask.GetPItem()->PTorch.GetPItem();
            if (torch) {
                C3DVector pos(0.0f, 0.0f, 0.0f);
                CRightHandedSet orientation;
                torch->PGraphicAppearance->GetDummyObject(g_CCharString_013baca0, 0, 0, &pos, &orientation, 1.0f);
                long result;
                do {
                    if (!IsTaskValid())
                        goto done;
                    bool reached = false;
                    result = AIAction_MoveToPos(2, pos, &reached, 3, true, true, 0, true, 0.0f);
                } while (result == 4);
                if (result == 2 && IsTaskValid() && IsTaskValid()
                    && AIAction_PerformActionAndWait(&CCreatureAction_UseObject(GetCreature(), torch, true), false) == 2) {
                    CTCVillage* owner = PTCVillageMember->GetVillage();
                    if (owner && PTask.GetPItem())
                        owner->TorchTaskCompleted(PTask);
                }
            }
        }
    }
done:
    Terminate();
}
