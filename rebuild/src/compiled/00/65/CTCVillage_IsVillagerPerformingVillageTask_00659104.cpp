#pragma optimize("s", on)
// CTCVillage::IsVillagerPerformingVillageTask(const CThingAICreature*) -- true when the villager carries the task-performing type bit (0xB4) or any carry-crate task reports it as its performer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00659104, length 71 bytes (PDB `?IsVillagerPerformingVillageTask@CTCVillage@@QAE_NPBVCThingAICreature@@@Z`).
// Size TU (shared epilogue for both results, no loop-head alignment): `optimize("s",on)`.
// Callees: std::bitset<>::test(size_t) const @ 0x00410DE0 (the villager's type bitset at +0x20, bit 0xB4), the
// carry-crate task's virtual slot 7 (+0x1C) taking the villager (descriptive name).
// Shape: the early `return true` is `mov al,1 ; jmp` to the shared epilogue; the loop over the retail one-pointer
// std::list is entry-tested with the head in eax and re-reads the head at the bottom (`cmp esi,[edi+60h]`): the
// virtual call may change the list; the found arm's `mov al,1` sits after the loop.
// Layout: CTCVillage.h is the PDB layout (CrateTasks +0xD0); with 4-byte retail lists it sits at +0x60, so a local model.
class CThingAICreature;

struct CBitset {
    bool test(unsigned long position) const;   // 0x00410DE0
};

class CThingAICreature { public:
    unsigned char _pad[0x20];
    CBitset       TypeBits;                 // +0x20
};

class CVillageTaskCarryCrate { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06();
    virtual bool IsThingPerformingTask(const CThingAICreature* villager);   // slot 7 (+0x1C)
};

struct SCrateTaskNode {                     // retail std::list node of CCountedPointer<CVillageTaskCarryCrate>
    SCrateTaskNode*         next;           // +0x00
    SCrateTaskNode*         prev;           // +0x04
    CVillageTaskCarryCrate* Object;         // +0x08
};

class CTCVillage { public:
    void*           __vftable;              // +0x00
    unsigned char   _pad_0x04[0x5C];
    SCrateTaskNode* CrateTasks;             // +0x60 retail std::list (sentinel pointer only)
    bool IsVillagerPerformingVillageTask(const CThingAICreature* villager);
};

bool CTCVillage::IsVillagerPerformingVillageTask(const CThingAICreature* villager)
{
    if (villager->TypeBits.test(0xB4))
        return true;
    for (SCrateTaskNode* n = CrateTasks->next; n != CrateTasks; n = n->next) {
        if (n->Object->IsThingPerformingTask(villager))
            return true;
    }
    return false;
}