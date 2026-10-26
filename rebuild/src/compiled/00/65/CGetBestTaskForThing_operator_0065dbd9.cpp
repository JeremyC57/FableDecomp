// CGetBestTaskForThing::operator()(const CCountedPointer<T>& task) -- (T = CVillageTaskPatrol) for_each functor: keep the task as the best one when it is valid and better than the current best for the thing
// Leaf: ??R?$CGetBestTaskForThing@VCVillageTaskPatrol@@@@QAEXABV?$CCountedPointer@VCVillageTaskPatrol@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065DBD9, 45 bytes (unaligned: a size-optimised TU), thiscall `ret 4`.
// Identity (proven): CGetBestTaskForThing<CVillageTaskPatrol >::operator(); evidence: its only caller is the for_each 0x0065E854, reached only from 0x0065F856, which is reached only from the CTCVillage member 0x00658E90 that passes this+0x54 -- the PatrolTasks list (Ego_r.pdb CTCVillage::PatrolTasks, list<CCountedPointer<CVillageTaskPatrol> >; the six 4-byte retail task lists +0x4C..+0x60 are the PDB's TorchTasks, HornTasks, PatrolTasks, CropTasks, SecuritySweepTasks, CrateTasks in order), i.e. CTCVillage::GetBestPatrolTaskForThing; it calls IsBetterThan 0x00861310, which is therefore CVillageTaskPatrol::IsBetterThan.
// Label: the manifest's "operator()" is propagated (family H091: the six CTCVillage task-list functors, one per task class).
// Callees: CVillageTaskPatrol::IsValid() const (virtual slot 3), CVillageTaskPatrol::IsBetterThan(const CVillageTaskPatrol*, const CThing&) const 0x00861310.
// Layout: Ego_r.pdb CGetBestTaskForThing<CVillageTaskPatrol > {CThing* Thing +0, CVillageTaskPatrol* PBestTask +4}; CCountedPointer<T> {Data +0, Info +4}.
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
class CVillageTaskPatrol {
public:
    virtual void _v0();
    virtual void _v1();
    virtual void _v2();
    virtual bool IsValid() const;                            // slot 3
    bool IsBetterThan(const CVillageTaskPatrol* other, const CThing& thing) const;   // 0x00861310
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
template void CGetBestTaskForThing<CVillageTaskPatrol >::operator()(const CCountedPointer<CVillageTaskPatrol >& task);