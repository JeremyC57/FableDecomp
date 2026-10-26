// CGetBestTaskForThing::operator()(const CCountedPointer<T>& task) -- (T = CVillageTaskSecuritySweep) for_each functor: keep the task as the best one when it is valid and better than the current best for the thing
// Leaf: ??R?$CGetBestTaskForThing@VCVillageTaskSecuritySweep@@@@QAEXABV?$CCountedPointer@VCVillageTaskSecuritySweep@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065EA48, 45 bytes (unaligned: a size-optimised TU), thiscall `ret 4`.
// Identity (proven): CGetBestTaskForThing<CVillageTaskSecuritySweep >::operator(); evidence: its only caller is the for_each 0x0065FB72, reached only from 0x006600BE, which is reached only from the CTCVillage member 0x006590F4 that passes this+0x5C -- the SecuritySweepTasks list (Ego_r.pdb CTCVillage::SecuritySweepTasks, list<CCountedPointer<CVillageTaskSecuritySweep> >; the six 4-byte retail task lists +0x4C..+0x60 are the PDB's TorchTasks, HornTasks, PatrolTasks, CropTasks, SecuritySweepTasks, CrateTasks in order), i.e. CTCVillage::GetBestSecuritySweepTaskForThing; it calls IsBetterThan 0x00860430, which is therefore CVillageTaskSecuritySweep::IsBetterThan.
// Label: the manifest's "operator()" is propagated (family H091: the six CTCVillage task-list functors, one per task class).
// Callees: CVillageTaskSecuritySweep::IsValid() const (virtual slot 3), CVillageTaskSecuritySweep::IsBetterThan(const CVillageTaskSecuritySweep*, const CThing&) const 0x00860430.
// Layout: Ego_r.pdb CGetBestTaskForThing<CVillageTaskSecuritySweep > {CThing* Thing +0, CVillageTaskSecuritySweep* PBestTask +4}; CCountedPointer<T> {Data +0, Info +4}.
//   Local classes -- Review at landing.
#pragma optimize("s", on)
class CThing;
struct CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
    T* operator->() const { return Data; }
};
class CVillageTaskSecuritySweep {
public:
    virtual void _v0();
    virtual void _v1();
    virtual void _v2();
    virtual bool IsValid() const;                            // slot 3
    bool IsBetterThan(const CVillageTaskSecuritySweep* other, const CThing& thing) const;   // 0x00860430
};
template <class T> class CGetBestTaskForThing {
public:
    CThing* Thing;                                           // +0x00
    T*      PBestTask;                                       // +0x04
    CGetBestTaskForThing(const CThing& thing);
    void operator()(const CCountedPointer<T>& task)
    {
        if (task->IsValid() && task->IsBetterThan(PBestTask, *Thing))
            PBestTask = task.Data;
    }
};
template void CGetBestTaskForThing<CVillageTaskSecuritySweep >::operator()(const CCountedPointer<CVillageTaskSecuritySweep >& task);