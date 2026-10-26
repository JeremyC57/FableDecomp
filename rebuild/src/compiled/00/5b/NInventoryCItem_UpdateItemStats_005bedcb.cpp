// NInventory::CItem::UpdateItemStats(CThing* thing) -- refresh the item's per-item data from the thing it was made from: a
// weapon (interface 0x48) copies its kill count and wield time and its augmentations (interface 0xbf, else cleared), a
// trophy (0x39) its witnesses-ahead count when positive, a fish (0xdc) takes the last-caught fish weight when heavier (then
// zeroes it); while the quest-persistent ref count is unset (-1, or no data yet), an analogue-loading weapon (0x1a) marks the
// item quest-persistent unless the quest manager is between start and finish screens, and resets the ref count to 0
// Leaf: UpdateItemStats
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BEDCB, 574 bytes; size-optimised frame-pointer TU (`push 48h ; pop esi`, `leave`).
// Callees: std::bitset::test 0x00410DE0, CVectorMap::LowerBound 0x004365B0 (const overload; the second copy of 0x0040F020),
//   operator new 0x00BFEA1A, CPerItemData ctor 0x005C14A6, CCountedPointer<CPerItemData>::Reset 0x005C1946 (the landed
//   CItem::AddItem 0x005BEC92 names), the augmentation component's vector fill 0x00766920 (labelled
//   CParticleMorphs_InitFromDef; clears then refills the out vector), vector<long>::erase 0x00440590 (the inline clear()),
//   CTCWeapon::IsAnalogueLoading 0x005D9270, CQuestManager::IsBetweenQuestStartAndFinishScreens 0x004AF290 on the global
//   quest manager 0x013B89FC; global last fish weight float 0x013BAE80.
// Layout: retail CPerItemData is 0x24 bytes (Ego_r.pdb 0x28 with a 16-byte vector; retail STLport vector is 12), so every
//   member after Augmentations is -4 from the PDB; CItem as engine/CItem.h (the counted pointer at +0x08 spelled as a
//   struct so Reset can be called); the thing's presence bitset +0x20 and interface map +0x44 as the landed
//   unattributed_GetInterface0x53 0x005BB999.
// Shape: every interface fetch goes through the inline out-parameter GetInterface (the key words then share one frame slot,
//   the last one the dead argument slot); writing the lookups out longhand gives a third frame word.
#pragma optimize("y", off)
#pragma optimize("s", on)
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A
class CTCBase;
struct CPresenceBits {
    bool test(long bit);                   // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
struct vector_long {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* erase(long* first, long* last);  // 0x00440590
    void clear() { erase(_M_start, _M_finish); }
};
struct CPerItemData {                      // 0x24 bytes
    vector_long Augmentations;             // +0x00
    long  NumberOfKills;                   // +0x0c
    long  TimeWielded;                     // +0x10
    long  BestTrophyWitnessesAheadToDate;  // +0x14
    bool  QuestPersistent;                 // +0x18
    long  QuestPersistentRefCount;         // +0x1c
    float BestFishWeight;                  // +0x20
    CPerItemData();                        // 0x005C14A6
};
struct CCountedPointer_PerItemData {
    CPerItemData* Data;
    void*         Info;
    void Reset(CPerItemData* object);      // 0x005C1946
};
class CTCWeapon { public:
    unsigned char _pad[0x1c];
    long NumberOfKills;                    // +0x1c
    long TimeWielded;                      // +0x20
    bool IsAnalogueLoading_005d9270();              // 0x005D9270
};
struct CTCAugmentations {
    void GetAugmentations(vector_long* out);   // 0x00766920
};
class CTCTrophy { public:
    unsigned char _pad[0x18];
    long WitnessesAhead;                   // +0x18
};
class CThing { public:
    unsigned char _pad0[0x20];
    CPresenceBits m_present;               // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;            // +0x44
    template <class T> __forceinline bool GetInterface(T** out, long interfaceType)
    {
        if (m_present.test(interfaceType)) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CQuestManager { public:
    bool IsBetweenQuestStartAndFinishScreens();  // 0x004AF290
};
extern CQuestManager* g_pQuestManager;     // 0x013B89FC
extern float g_lastFishWeight;             // 0x013BAE80

struct CItem {
    long ItemType;                         // +0x00
    long NumberOfItems;                    // +0x04
    CCountedPointer_PerItemData PPerItemData;   // +0x08
    long AverageBoughtPrice;               // +0x10
    void UpdateItemStats(CThing* thing);
    __forceinline void EnsurePerItemData() { if (!PPerItemData.Data) PPerItemData.Reset(new CPerItemData()); }
};

void CItem::UpdateItemStats(CThing* thing)
{
    CTCWeapon* weapon;
    CTCTrophy* trophy;
    if (thing->GetInterface(&weapon, 0x48)) {
        EnsurePerItemData();
        PPerItemData.Data->NumberOfKills = weapon->NumberOfKills;
        PPerItemData.Data->TimeWielded = weapon->TimeWielded;
        CTCAugmentations* aug;
        if (thing->GetInterface(&aug, 0xbf))
            aug->GetAugmentations(&PPerItemData.Data->Augmentations);
        else
            PPerItemData.Data->Augmentations.clear();
    } else if (thing->GetInterface(&trophy, 0x39)) {
        if (trophy->WitnessesAhead > 0) {
            EnsurePerItemData();
            PPerItemData.Data->BestTrophyWitnessesAheadToDate = trophy->WitnessesAhead;
        }
    } else if (thing->m_present.test(0xdc)) {
        EnsurePerItemData();
        CPerItemData* data = PPerItemData.Data;
        if (g_lastFishWeight > data->BestFishWeight) {
            data->BestFishWeight = g_lastFishWeight;
            g_lastFishWeight = 0.0f;
        }
    }
    if (!PPerItemData.Data || PPerItemData.Data->QuestPersistentRefCount == -1) {
        CTCWeapon* loader;
        if (thing->GetInterface(&loader, 0x1a) && loader && loader->IsAnalogueLoading_005d9270()) {
            EnsurePerItemData();
            if (g_pQuestManager->IsBetweenQuestStartAndFinishScreens())
                PPerItemData.Data->QuestPersistent = false;
            else
                PPerItemData.Data->QuestPersistent = true;
            PPerItemData.Data->QuestPersistentRefCount = 0;
        }
    }
}