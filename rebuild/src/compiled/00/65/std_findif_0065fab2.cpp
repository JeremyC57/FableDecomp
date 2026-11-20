// std::find_if<list<CCountedPointer<CVillageTaskCarryCrate> >::iterator, CIsTask>(first, last, pred) -- STLport 4.6 _algo.h: forwards to the
//   input-iterator __find_if 0x0065EC98 with an iterator-category tag; returns the first task the predicate accepts (or last)
// Leaf: ??$find_if@U?$_List_iterator@V?$CCountedPointer@VCVillageTaskCarryCrate@@@@U?$_Nonconst_traits@V?$CCountedPointer@VCVillageTaskCarryCrate@@@@@std@@@std@@VCIsTask@@@std@@YI?AU?$_List_iterator@V?$CCountedPointer@VCVillageTaskCarryCrate@@@@U?$_Nonconst_traits@V?$CCountedPointer@VCVillageTaskCarryCrate@@@@@std@@@0@U10@0VCIsTask@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065FAB2, 42 bytes, /Gr-style `__fastcall` (hidden result pointer in ecx, both iterators and the
//   4-byte CIsTask by value on the stack, `ret 0Ch`); the tag reference goes to __find_if in edx (`lea edx,[ebp+13h]`, a dead frame byte).
// Identity: only caller CTCVillage::RemoveThingFromTask<CVillageTaskCarryCrate> 0x00660D49 (std::find_if(task_list.begin(), task_list.end(),
//   CIsTask(*task)) right before list<CCountedPointer<CVillageTaskCarryCrate> >::erase 0x006601E9), whose landed row binds this address under
//   this spelling; callee __find_if<..., CIsTask> 0x0065EC98 (landed, same element type). FableWin.pdb has the std::find_if<..., CIsTask>
//   instantiation per task type as __fastcall (YI); retail is STLport 4.6, so the iterator is _List_iterator<T, _Nonconst_traits<T> >
//   (descriptive-by-library). Was the stand-in `CRangeString_0065fab2::Construct_CRangeString_0065fab2` (local SIterator model).
// Shape: explicit instantiation of stlport/fable_algorithm.h find_if (FABLE_RETAIL_STLP_FIND_IF_INPUT) in the binder's size TU
//   (frame pointer kept, optimize("y", off)).
// Flags: none
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_REMOVE_TASK
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_LIST
#include "engine/CTCVillage.h"
#include "engine/CIsTask.h"
#define FABLE_RETAIL_CVILLAGETASKCARRYCRATE
#include "engine/CVillageTaskCarryCrate.h"
#include "stlport/fable_algorithm.h"

template std::list<CCountedPointer<CVillageTaskCarryCrate> >::iterator FABLE_FASTCALL std::find_if(
    std::list<CCountedPointer<CVillageTaskCarryCrate> >::iterator, std::list<CCountedPointer<CVillageTaskCarryCrate> >::iterator, CIsTask);
