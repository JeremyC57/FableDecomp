// CTCVillage::RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing) -- dispatches on the task's type to the typed RemoveThingFromTask<T> over the matching task list (torch, patrol, crops, security sweep, horn, crate)
// Leaf: ?RemoveThingFromTask@CTCVillage@@QAEXV
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00659EFC, 241 bytes (`ret 0Ch`, the task pointer by value and destroyed
// here; Ego_r.pdb `?RemoveThingFromTask@CTCVillage@@QAEXV?$CIntelligentPointer@VCVillageTaskBase@@@@ABVCThing@@@Z`).
// Callees: CBaseIntelligentPointer::GetPItem() 0x00A01B50; the task's GetTaskType (virtual slot 2, +0x08);
//          CIntelligentPointer<CVillageTaskBase>(const CIntelligentPointer&) 0x0065E37E (each by-value argument built in its
//          stack slot); CTCVillage::RemoveThingFromTask<T>(CIntelligentPointer<CVillageTaskBase>, const CThing&,
//          list<CCountedPointer<T> >&) const for T = CVillageTaskTorch 0x00660AE2, CVillageTaskPatrol 0x00660BD8,
//          CVillageTaskPickCrops 0x00660C53, CVillageTaskSecuritySweep 0x00660CCE, CVillageTaskHorn 0x00660B5D,
//          CVillageTaskCarryCrate 0x00660D49 (the manifest names every one "<CVillageTaskPickCrops>", propagated);
//          ~CBaseIntelligentPointer 0x00A01C10.
// Layout: the retail one-pointer task lists, as the landed RemoveThingFromAllTasks 0x00659EA9: TorchTasks +0x4c,
//          HornTasks +0x50, PatrolTasks +0x54, CropTasks +0x58, SecuritySweepTasks +0x5c, CrateTasks +0x60
//          (engine/CTCVillage.h carries the PDB offsets, so a local model).
// Shape: frame-pointer size TU; a six-case switch compiled as a `sub eax,0 / dec eax` chain with the case bodies laid out
//        in source order (crate, horn, sweep, crops, patrol, torch), every arm breaking to the parameter's destructor.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThing;
class CVillageTaskTorch;
class CVillageTaskHorn;
class CVillageTaskPatrol;
class CVillageTaskPickCrops;
class CVillageTaskSecuritySweep;
class CVillageTaskCarryCrate;

class CVillageTaskBase { public:
    virtual void v00();
    virtual void v01();
    virtual long GetTaskType();                 // slot 2 (+0x08)
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(const CIntelligentPointer& other);   // 0x0065E37E (T = CVillageTaskBase)

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
template<class T> class CCountedPointer { public: T* Data; void* Info; };
template<class T> struct CTaskList {            // retail std::list<CCountedPointer<T> >, sentinel pointer only
    void* _M_node;
};

class CTCVillage { public:
    void*                                 __vftable;          // +0x00
    unsigned char                         _pad_0x04[0x48];
    CTaskList<CVillageTaskTorch>          TorchTasks;         // +0x4c
    CTaskList<CVillageTaskHorn>           HornTasks;          // +0x50
    CTaskList<CVillageTaskPatrol>         PatrolTasks;        // +0x54
    CTaskList<CVillageTaskPickCrops>      CropTasks;          // +0x58
    CTaskList<CVillageTaskSecuritySweep>  SecuritySweepTasks; // +0x5c
    CTaskList<CVillageTaskCarryCrate>     CrateTasks;         // +0x60
    template<class T> void RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing,
                                               CTaskList<T>& tasks) const;   // 0x00660AE2 ..
    void RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing);
};

void CTCVillage::RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing)
{
    switch (task.GetPItem()->GetTaskType()) {
    case 5:
        RemoveThingFromTask(task, thing, CrateTasks);
        break;
    case 4:
        RemoveThingFromTask(task, thing, HornTasks);
        break;
    case 3:
        RemoveThingFromTask(task, thing, SecuritySweepTasks);
        break;
    case 2:
        RemoveThingFromTask(task, thing, CropTasks);
        break;
    case 1:
        RemoveThingFromTask(task, thing, PatrolTasks);
        break;
    case 0:
        RemoveThingFromTask(task, thing, TorchTasks);
        break;
    }
}