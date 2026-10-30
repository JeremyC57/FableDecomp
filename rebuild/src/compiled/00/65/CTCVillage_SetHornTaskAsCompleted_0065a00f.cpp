// CTCVillage::SetHornTaskAsCompleted(CIntelligentPointer<CVillageTaskHorn> task) -- drop the completed horn-blowing task from the village's HornTasks list (remove_if with a CIsTask predicate on it)
// Leaf: ?SetHornTaskAsCompleted@CTCVillage@@QAEXV?$CIntelligentPointer@VCVillageTaskHorn@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065A00F, 34 bytes (size-optimised TU, no padding), thiscall, `ret 8` (the
//   8-byte CIntelligentPointer by value).
// Identity (proven): CTCVillage::SetHornTaskAsCompleted(CIntelligentPointer<CVillageTaskHorn>); evidence: Ego_r.pdb holds the four
//   CTCVillage::Set*TaskAsCompleted(CIntelligentPointer<...>) in the order Torch, Horn, SecuritySweep, Crate (0x0069DC8C ..
//   0x0069DCFB, this one 0x0069DCB1) as retail's 0x00659FED / 0x0065A00F / 0x0065A031 / 0x0065A053; it works on this+0x50
//   = engine/CTCVillage.h HornTasks (list<CCountedPointer<CVillageTaskHorn> >); caller: CAIStateGroup_VillageTaskHorn::Update 0x00940960.
// Label: the manifest's "GetItemDescription" is propagated (family J106: byte-identical by-value setters; retail has no identical-code
//   folding).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50, std::list<CCountedPointer<CVillageTaskHorn> >::remove_if<CIsTask>(CIsTask)
//   0x00660298 (FableWin.pdb ??$remove_if@VCIsTask@@@?$list@V?$CCountedPointer@VCVillageTaskHorn@@@@...@std@@QAEXVCIsTask@@@Z; it copies each
//   element to a CCountedPointer<CVillageTaskBase> for CIsTask::operator() and erases the matches), CBaseIntelligentPointer::
//   ~CBaseIntelligentPointer() 0x00A01C10 (the argument's destructor).
// Shape: `HornTasks.remove_if(CIsTask(*task));` -- CIntelligentPointer::operator* (FableWin.pdb ??D...QBEAAVCVillageTaskHorn@@XZ) is the
//   GetPItem call, CIsTask(const CVillageTaskBase&) (FableWin.pdb ??0CIsTask@@QAE@ABVCVillageTaskBase@@@Z; Ego_r.pdb CIsTask
//   {CVillageTaskBase* Task}) keeps its address and goes by value. Size-optimised TU: `#pragma optimize("s", on)` (J105).
// Layout: engine/CTCVillage.h under a layout name (HornTasks is an opaque 4-byte list there); the task chain is a local partial
//   view (RTTI CVillageTaskHorn <- CVillageTaskBase <- CBaseObject <- CBaseClass).
#pragma optimize("s", on)
#define CTCVillage CTCVillagePdb
#include "engine/CTCVillage.h"
#undef CTCVillage

namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
};
struct _List_node_base;
template <class _Tp, class _Alloc = allocator<_Tp> >
class list {                                                // retail STLport list (4 bytes)
public:
    _STLP_alloc_proxy<_List_node_base*, _List_node_base, _Alloc> _M_node;
    template <class _Predicate> void remove_if(_Predicate __pred);
};
}   // namespace std

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseObjectPointer;
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CVillageTaskBase : public CBaseObject { };            // partial view
class CVillageTaskHorn : public CVillageTaskBase { };   // partial view
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
};
#pragma pack(push, 1)
class CBaseIntelligentPointer {                             // 8 bytes, 1-aligned in retail
public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T& operator*() const { return *static_cast<T*>(GetPItem()); }
};
#pragma pack(pop)
class CIsTask {                                             // Ego_r.pdb 4 bytes
public:
    CIsTask(const CVillageTaskBase& task) : Task(const_cast<CVillageTaskBase*>(&task)) { }
    bool operator()(const CCountedPointer<CVillageTaskBase>& task) const;
    CVillageTaskBase* Task;                                 // +0x00
};

class CTCVillage : public CTCVillagePdb {
public:
    void SetHornTaskAsCompleted(CIntelligentPointer<CVillageTaskHorn> task);
};

void CTCVillage::SetHornTaskAsCompleted(CIntelligentPointer<CVillageTaskHorn> task)
{
    reinterpret_cast<std::list<CCountedPointer<CVillageTaskHorn> >&>(HornTasks).remove_if(CIsTask(*task));
}