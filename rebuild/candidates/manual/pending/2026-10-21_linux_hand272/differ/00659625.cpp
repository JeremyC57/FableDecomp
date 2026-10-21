// CTCVillage::UpdateTorchTasks() -- every five seconds: note day/night changes (restarting the one-minute grace period), then for each live building light (TC 0x27): when it is in the wrong state and has no torch task, toggle it directly if the village has no villagers or queue a new torch task for it; when it is right and still has a task, drop the task; when it is wrong, has an idle task and the grace period has passed, toggle it and drop the task
// Grade: DIFFER(466v475) -- first pass, control flow in place (three-way task decision reproduces retail's branch web); gaps: retail keeps `night` in bl and the building in edi with a [ebp-4] spill (ours spills night to [ebp-1] and keeps the building in ebx), loads g_FramesPerSecond before the second GetWorldFrame call, and re-uses edi for the light
// Retail: Fable.exe @ 0x00659625, 475 bytes (frame-pointer, `leave ; ret`; unaligned entry = /Os TU).
// Callees: CWorld::GetWorldFrame 0x0049D870 (__fastcall), CTCBase::GetWorld 0x00686A80, CGameTimeManager::IsDay 0x006BB2C0, GetPItem 0x00A01B50,
//   std::bitset::test 0x00410DE0, LowerBound 0x0040F020, IsThingInTaskList<CVillageTaskTorch> 0x0065FF5E, IsThingsTaskActive<CVillageTaskTorch> 0x0065FF8E,
//   CTCLight::SetActive 0x006412B0, operator new 0x00BFEA1A, CVillageTaskTorch ctor 0x0065ED08, CCountedPointer ctor 0x0065E25A / dtor 0x0065D574,
//   torch task list push_back 0x0066051F, RemoveThingFromTaskList<CVillageTaskTorch> 0x00660787
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type);               // 0x0040F020
};
struct CTCBitset {
    unsigned long Words[9];
    bool test(unsigned long bit) const;                 // 0x00410DE0
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x1c];
    CTCBitset TCBits;                                   // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                             // +0x91 bit0
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CThingNode {
    CThingNode* Next;
    CThingNode* Prev;
    CBaseIntelligentPointer Thing;                      // +0x08
};
struct CTCLight {
    unsigned char _pad00[0x1c];
    bool Active;                                        // +0x1c
    void SetActive(bool active);                        // 0x006412B0
};
struct CVillageTaskTorch {
    unsigned char _pad[0x18];
    CVillageTaskTorch(CThing* light);                   // 0x0065ED08
};
template <class T> struct CCountedPointer {
    T* Object;
    long _pad;
    CCountedPointer(T* object);                         // 0x0065E25A
    ~CCountedPointer();                                 // 0x0065D574
};
struct CTorchTaskList {
    void* Head;
    void push_back(const CCountedPointer<CVillageTaskTorch>& task);   // 0x0066051F
};
struct CGameTimeManager { bool IsDay(); };              // 0x006BB2C0
struct CWorld { unsigned char _pad[0x1c]; CGameTimeManager* TimeManager; };
long __fastcall GetWorldFrame();                        // 0x0049D870
extern long g_FramesPerSecond;                          // 0x01375550
struct CTCVillage {
    virtual void v00();
    unsigned char _pad04[0x0c - 0x04];
    long NextTorchUpdateFrame;                          // +0x0c
    unsigned char _pad10[0x2c - 0x10];
    CThingNode* Villagers;                              // +0x2c (STLport list sentinel)
    CThingNode* Buildings;                              // +0x30 (STLport list sentinel)
    unsigned char _pad34[0x4c - 0x34];
    CTorchTaskList TorchTasks;                          // +0x4c
    unsigned char _pad50[0xcc - 0x50];
    bool IsNight;                                       // +0xcc
    unsigned char _padcd[0xd0 - 0xcd];
    long TorchGraceEndFrame;                            // +0xd0
    CWorld* GetWorld();                                 // 0x00686A80
    bool IsThingInTaskList(CThing* thing, CTorchTaskList* list);      // 0x0065FF5E
    bool IsThingsTaskActive(CThing* thing, CTorchTaskList* list);     // 0x0065FF8E
    void RemoveThingFromTaskList(CThing* thing, CTorchTaskList* list);   // 0x00660787
    unsigned long GetVillagerCount() const
    {
        unsigned long n = 0;
        for (CThingNode* node = Villagers->Next; node != Villagers; node = node->Next)
            ++n;
        return n;
    }
    void UpdateTorchTasks();
};

void CTCVillage::UpdateTorchTasks()
{
    static long s_interval = g_FramesPerSecond * 5;
    if (NextTorchUpdateFrame >= GetWorldFrame())
        return;
    NextTorchUpdateFrame = GetWorldFrame() + s_interval;
    bool night = !GetWorld()->TimeManager->IsDay();
    if (night != IsNight) {
        IsNight = night;
        TorchGraceEndFrame = GetWorldFrame() + g_FramesPerSecond * 60;
    }
    for (CThingNode* node = Buildings->Next; node != Buildings; node = node->Next) {
        CThing* building = node->Thing.GetPItem();
        if (building->Dead || !building->TCBits.test(0x27))
            continue;
        CTCLight* light = (CTCLight*)building->GetTC(0x27);
        if (light->Active != night && !IsThingInTaskList(building, &TorchTasks)) {
            if (GetVillagerCount() == 0)
                light->SetActive(!light->Active);
            else
                TorchTasks.push_back(CCountedPointer<CVillageTaskTorch>(new CVillageTaskTorch(building)));
        } else if (light->Active == night && IsThingInTaskList(building, &TorchTasks)) {
            RemoveThingFromTaskList(building, &TorchTasks);
        } else if (light->Active != night && IsThingInTaskList(building, &TorchTasks) && !IsThingsTaskActive(building, &TorchTasks)
                   && GetWorldFrame() > TorchGraceEndFrame) {
            light->SetActive(!light->Active);
            RemoveThingFromTaskList(building, &TorchTasks);
        }
    }
}
