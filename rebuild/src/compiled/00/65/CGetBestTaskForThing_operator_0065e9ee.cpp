// CGetBestTaskForThing::operator()(const CCountedPointer<T>& task) -- (T = CVillageTaskHorn) for_each functor: keep the task as the best one when it is valid and better than the current best for the thing
// Leaf: ??R?$CGetBestTaskForThing@VCVillageTaskHorn@@@@QAEXABV?$CCountedPointer@VCVillageTaskHorn@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065E9EE, 45 bytes (unaligned: a size-optimised TU), thiscall `ret 4`.
// Identity (proven): CGetBestTaskForThing<CVillageTaskHorn >::operator(); evidence: its only caller is the for_each 0x0065FB0E, reached only from 0x00660062, which is reached only from the CTCVillage member 0x006590D4 that passes this+0x50 -- the HornTasks list (Ego_r.pdb CTCVillage::HornTasks, list<CCountedPointer<CVillageTaskHorn> >; the six 4-byte retail task lists +0x4C..+0x60 are the PDB's TorchTasks, HornTasks, PatrolTasks, CropTasks, SecuritySweepTasks, CrateTasks in order), i.e. CTCVillage::GetBestHornTaskForThing; it calls IsBetterThan 0x008611F0, which is therefore CVillageTaskHorn::IsBetterThan.
// Label: the manifest's "operator()" is propagated (family H091: the six CTCVillage task-list functors, one per task class).
// Callees: CVillageTaskHorn::IsValid() const (virtual slot 3), CVillageTaskHorn::IsBetterThan(const CVillageTaskHorn*, const CThing&) const 0x008611F0.
// Layout: Ego_r.pdb CGetBestTaskForThing<CVillageTaskHorn > {CThing* Thing +0, CVillageTaskHorn* PBestTask +4}; CCountedPointer<T> {Data +0, Info +4}.
//   Local classes -- Review at landing.
#pragma optimize("s", on)
class CThing;
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
    T* operator->() const { return Data; }
};
class CVillageTaskHorn {
public:
    virtual void _v0();
    virtual void _v1();
    virtual void _v2();
    virtual bool IsValid() const;                            // slot 3
    bool IsBetterThan(const CVillageTaskHorn* other, const CThing& thing) const;   // 0x008611F0
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
template void CGetBestTaskForThing<CVillageTaskHorn >::operator()(const CCountedPointer<CVillageTaskHorn >& task);