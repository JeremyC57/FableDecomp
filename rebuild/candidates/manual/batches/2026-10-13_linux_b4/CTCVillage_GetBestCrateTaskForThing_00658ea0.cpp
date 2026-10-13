// CTCVillage::GetBestCrateTaskForThing(CThing*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00658EA0, oracle length 35 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push esi ; mov esi,ecx ; call CTCVillage::IsThereACratePutDownTask ; test al,al ; jne G ; xor eax,eax ; jmp R ; G: lea eax,[esi+60h] ; push eax ; push [esp+0Ch] ; mov ecx,esi ; call <best task in list> ; R: pop esi ; ret 4
// With no put-down task available there is nothing to hand out; otherwise the best crate task for
// the villager is chosen from the CrateTasks list (retail +0x60) by the selector at 0x0065F884
// (__thiscall: thing, list).
// Layout note: in the shipped binary a std::vector / CArray / map / set / CVectorMap is 4 bytes
// smaller than in Ego_r.pdb (the editor build carries an extra word per container) and a
// std::list is 8 bytes smaller (retail keeps only the head pointer; the PDB list is 0xC bytes).
// A retail std::vector is {first, last, end} with `first` at +0. Members after such containers
// therefore sit earlier than the generated engine header says: 4 bytes per vector/map/set/CArray
// and 8 bytes per list that precede them.

// The crate-task list holds CCountedPointer<CVillageTaskCarryCrate> {Data, Info}: node+0x08 is the task.
struct CVillageTaskCarryCrateCalls {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual bool IsPutDownTask();          // slot 4 (+0x10): a put-crate-down (as opposed to pick-up) task
    unsigned char _pad_0x04[0x14];
    bool          Taken;                   // +0x18 (name inferred: already accepted by a villager)
};

struct CVillageTaskListNode {
    CVillageTaskListNode*         Next;         // +0x00
    CVillageTaskListNode*         Prev;         // +0x04
    CVillageTaskCarryCrateCalls*  Task_Data;    // +0x08
    void*                         Task_Info;    // +0x0C
};

// Retail CTCVillage: the 15 task/thing lists from +0x28 are 4 bytes each (PDB 0xC), the PlayersCrimes
// set is 0xC (PDB 0x10) and the two lists after it are 4 bytes each, so everything from
// Initialised (PDB +0x104) on is 0x8C bytes earlier than engine/CTCVillage.h says.
struct CVillageTaskListNode;
struct CTCVillage_Retail {
    unsigned char          _pad_0x00[0x60];
    CVillageTaskListNode*  CrateTasks_Head;              // +0x60 list<CCountedPointer<CVillageTaskCarryCrate>> (PDB +0xD0)
    unsigned char          _pad_0x64[0x3C];
    long                   AmountOfFineAlreadyPaid;      // +0xA0 (PDB +0x12C)
    unsigned char          _pad_0xA4[0x30];
    long                   HeroEnemyOfGuardsUntil;       // +0xD4 (PDB +0x160)
    unsigned char          _pad_0xD8[0x0C];
    bool                   CurrentIsHeroCriminal;        // +0xE4 (PDB +0x170)
    bool IsThereACratePutDownTask();                                                          // 0x00658E09
    CVillageTaskCarryCrateCalls* FindBestCrateTaskForThing(struct CThing* thing, CVillageTaskListNode** taskList);   // 0x0065F884
    CVillageTaskCarryCrateCalls* GetBestCrateTaskForThing(struct CThing* thing);
};

CVillageTaskCarryCrateCalls* CTCVillage_Retail::GetBestCrateTaskForThing(CThing* thing)
{
    if (!IsThereACratePutDownTask())
        return 0;
    return FindBestCrateTaskForThing(thing, &this->CrateTasks_Head);
}
