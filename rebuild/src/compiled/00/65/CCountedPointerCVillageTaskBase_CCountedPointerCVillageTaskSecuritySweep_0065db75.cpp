// CCountedPointer::CCountedPointer(const CCountedPointer<U>&) -- converting copy: take Data and Info, then add a reference (T = CVillageTaskBase, U = CVillageTaskSecuritySweep)
// Leaf: ??$?0VCVillageTaskSecuritySweep@@@?$CCountedPointer@VCVillageTaskBase@@@@QAE@ABV?$CCountedPointer@VCVillageTaskSecuritySweep@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065DB75, 25 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_CopyCtor_0065db75 (stand-in name).
// T = CVillageTaskBase, proven by: CTCVillage::RemoveThingFromTaskList<CVillageTaskSecuritySweep> and the std::__find_if / list::remove_if instances over list<CCountedPointer<CVillageTaskSecuritySweep>> (CTCVillage member of that type, Ego_r) up-cast each element through it to the CIsTask / CIsThingInvolvedWithTask predicate taking const CCountedPointer<CVillageTaskBase>&; the landed caller binds this member template; FableWin.pdb has this exact symbol.
// Member: the in-class copy constructor of engine/CCountedPointer.h (member template), emitted out of line as its own COMDAT.
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CVILLAGETASKSECURITYSWEEP
#include "engine/CVillageTaskSecuritySweep.h"
template CCountedPointer<CVillageTaskBase>::CCountedPointer(const CCountedPointer<CVillageTaskSecuritySweep>&);
