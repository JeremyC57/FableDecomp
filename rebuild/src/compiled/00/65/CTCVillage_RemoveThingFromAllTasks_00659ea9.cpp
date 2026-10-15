#pragma optimize("s", on)
// CTCVillage::RemoveThingFromAllTasks(const CThing&) -- removes the thing from each of the six village task lists (torch, horn, patrol, pick-crops, security sweep, carry crate)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00659EA9, length 83 bytes (PDB `?RemoveThingFromAllTasks@CTCVillage@@AAEXABVCThing@@@Z`).
// Callees: CTCVillage::RemoveThingFromTaskList<T>(const CThing&, list<CCountedPointer<T> >&) for T = CVillageTaskTorch
// @ 0x00660787, CVillageTaskHorn @ 0x00660894, CVillageTaskPatrol @ 0x0066090A, CVillageTaskPickCrops @ 0x00660980,
// CVillageTaskSecuritySweep @ 0x006609F6, CVillageTaskCarryCrate @ 0x00660A6C (the manifest names every one
// "<CVillageTaskTorch>", a propagated label).
// Size TU (every list address formed in eax; the speed form rotates eax/ecx/edx): `optimize("s",on)`.
// Shape: six calls in source order; `this` is still in ecx for the first (`mov ecx,esi` only from the second on).
// Layout: CTCVillage.h is the PDB layout (task lists +0x94..+0xD0, 12 bytes each); the retail one-pointer lists put
// them at +0x4C..+0x60, so a local model.
struct CThing;
struct CVillageTaskTorch;
struct CVillageTaskHorn;
struct CVillageTaskPatrol;
struct CVillageTaskPickCrops;
struct CVillageTaskSecuritySweep;
struct CVillageTaskCarryCrate;

template<class T> struct CTaskList {        // retail std::list<CCountedPointer<T> >, sentinel pointer only
    void* _M_node;
};

struct CTCVillage {
    void*                                 __vftable;          // +0x00
    unsigned char                         _pad_0x04[0x48];
    CTaskList<CVillageTaskTorch>          TorchTasks;         // +0x4C
    CTaskList<CVillageTaskHorn>           HornTasks;          // +0x50
    CTaskList<CVillageTaskPatrol>         PatrolTasks;        // +0x54
    CTaskList<CVillageTaskPickCrops>      CropTasks;          // +0x58
    CTaskList<CVillageTaskSecuritySweep>  SecuritySweepTasks; // +0x5C
    CTaskList<CVillageTaskCarryCrate>     CrateTasks;         // +0x60
    template<class T> void RemoveThingFromTaskList(const CThing& thing, CTaskList<T>& tasks);   // 0x00660787 ..
    void RemoveThingFromAllTasks(const CThing& thing);
};

void CTCVillage::RemoveThingFromAllTasks(const CThing& thing)
{
    RemoveThingFromTaskList(thing, TorchTasks);
    RemoveThingFromTaskList(thing, HornTasks);
    RemoveThingFromTaskList(thing, PatrolTasks);
    RemoveThingFromTaskList(thing, CropTasks);
    RemoveThingFromTaskList(thing, SecuritySweepTasks);
    RemoveThingFromTaskList(thing, CrateTasks);
}