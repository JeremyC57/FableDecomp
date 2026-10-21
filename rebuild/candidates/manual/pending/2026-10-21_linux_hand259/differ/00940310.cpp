// CAIStateGroup_VillageTaskPickCrops::Update() -- accept the crop task, walk to the crop while the task is valid in work hours, then pick it
// Leaf: Update
// Grade: DIFFER(493v494) -- frame-slot packing only after 4 grades: retail keeps the crop pointer at frame+0xc apart from the reset/key slot (+0x4) and the reached flag below the presence byte; ours shares the crop with the key slot; plus `mov eax,[eax+4] ; mov ecx,eax` for the villager handed to GFGetPVillageInterface (ours loads ecx directly)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00940310; best of the probes graded under VC7.1 RTM (probe/00940310c.cpp).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        bool has = (unsigned char)(m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1;
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {     // vtable 0x0125A858
    virtual void v00();
    CIntelligentPointerConstThing(CThing* item);    // 0x0065E133 (out of line)
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {          // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerThing(const CBaseIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CIntelligentPointerTask : CBaseIntelligentPointer {           // CIntelligentPointer<CVillageTaskPickCrops>, vtable 0x0128B0A0
    virtual void v00();
    CIntelligentPointerTask(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerTask& operator=(const CIntelligentPointerTask& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CVillageTaskPickCrops {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06();
    virtual bool IsStillValidFor(CThing* creature);  // slot 7 (+0x1c)
    long _pad04;
    CBaseIntelligentPointer PCrop;              // +0x08
    CThing* GetCrop() const { return PCrop.GetPItem(); }
};
struct CTCVillager;
struct CTCVillage {
    bool SetCropTaskAsAcceptedByThing(CVillageTaskPickCrops* task, CThing* creature);   // 0x00658561
};
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540
struct CThingCreatureBase;
struct CThingObject;
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
};
struct CCreatureAction_PickCrop : CCreatureActionBase {
    unsigned char _pick[0x10];
    CCreatureAction_PickCrop(CThingCreatureBase* creature, CThingObject* crop);   // 0x009405F0
    ~CCreatureAction_PickCrop();                // 0x009400F0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
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
    CThingCreatureBase* GetCreatureBase() const { return (CThingCreatureBase*)PBrain->PCreature; }
    CThing* GetCreature() const { return PBrain->PCreature; }
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerThing target, float proximity, bool* reached,
        long movement, bool a, bool b, bool c, bool d, bool e);    // 0x0097E380
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool b);   // 0x0097DB70
};
struct CAIStateGroup_VillageTaskPickCrops : CAIStateGroupBase {
    CIntelligentPointerTask PTask;              // +0x14
    bool IsInWorkHours();                       // 0x00940120
    CVillageTaskPickCrops* GetTask() const { return (CVillageTaskPickCrops*)PTask.GetPItem(); }
    virtual void Update();
};

void CAIStateGroup_VillageTaskPickCrops::Update()
{
    CTCVillager* villager;
    if (PTask.GetPItem() != 0 && PBrain->PCreature->GetInterface(&villager, 0x23)) {
        CTCVillage* village = GFGetPVillageInterface(villager);
        if (village != 0 && village->SetCropTaskAsAcceptedByThing(GetTask(), GetCreature())) {
            CIntelligentPointerConstThing crop(GetTask()->GetCrop());
            bool reached = false;
            long result;
            do {
                if (PTask.GetPItem() == 0 || !GetTask()->IsStillValidFor(PBrain->PCreature)
                    || !IsInWorkHours())
                    goto done;
                result = AIAction_MoveToThingProximity(2, crop, 0.2f, &reached, 3, true, false, true, true, false);
            } while (result == 4);
            if (result == 2 && PTask.GetPItem() != 0
                && GetTask()->IsStillValidFor(PBrain->PCreature)) {
                if (AIAction_PerformActionAndWait(&CCreatureAction_PickCrop(GetCreatureBase(), (CThingObject*)crop.GetPItem()), false) != 2)
                    Terminate();
                else
                    Terminate();
                return;
            }
        done:
            Terminate();
            return;
        } else {
            PTask = CIntelligentPointerTask(0);
        }
    }
    Terminate();
}
