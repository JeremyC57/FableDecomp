// CThingCreatureBase::AddToCreatureInteraction(const CCountedPointer<CCreatureInteraction>& interaction) -- take the interaction and register this creature with it
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00662550, 96 bytes (`ret 4`).
// Callees: the count block's destroy function (+4, object at +8 in ecx), operator delete @ 0x00BFE9BC,
//   CCreatureInteraction::AddCreature(CThingCreatureBase*) @ 0x008631D0 (manifest "CreatureInteraction_InsertAfter":
//   a list insert on the interaction's creature list at +0 that stores the pointer by value).
// Layout: PDB CCountedPointer<CCreatureInteraction> PCreatureInteraction is at +0x150 in Ego_r.pdb; retail keeps it at
//   +0x12c (object) / +0x130 (count block), 0x24 lower, so the class is modelled locally.
// Shape: the counted pointer's inline operator= (both source words read into locals first, then the self test on the block, release
//   re-reading the member block, object then block stores, count up), then the member call through the object.
void __cdecl operator delete(void* block);              // 0x00BFE9BC

#include "engine/CCountedPointer.h"

class CThingCreatureBase;
class CCreatureInteraction { public:
    void AddCreature(CThingCreatureBase* creature);     // 0x008631D0
};

class CThingCreatureBase { public:
    unsigned char _pad[0x12c];
    CCountedPointer<CCreatureInteraction> PCreatureInteraction;     // +0x12c
    void AddToCreatureInteraction(const CCountedPointer<CCreatureInteraction>& interaction);
};

void CThingCreatureBase::AddToCreatureInteraction(const CCountedPointer<CCreatureInteraction>& interaction)
{
    PCreatureInteraction = interaction;
    PCreatureInteraction->AddCreature(this);
}