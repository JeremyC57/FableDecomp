// CCountedPointer::CCountedPointer(const CCountedPointer<U>&) -- converting copy: take Data and Info, then add a reference (T = CVillageTaskBase, U = CVillageTaskPickCrops)
// Leaf: ??$?0VCVillageTaskPickCrops@@@?$CCountedPointer@VCVillageTaskBase@@@@QAE@ABV?$CCountedPointer@VCVillageTaskPickCrops@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065DB5C, 25 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_CopyCtor_0065db5c (stand-in name).
// T = CVillageTaskBase, proven by: CTCVillage::RemoveThingFromTaskList<CVillageTaskPickCrops> and the std::__find_if / list::remove_if instances over list<CCountedPointer<CVillageTaskPickCrops>> (CTCVillage member of that type, Ego_r) up-cast each element through it to the CIsTask / CIsThingInvolvedWithTask predicate taking const CCountedPointer<CVillageTaskBase>&; the landed caller binds this member template; FableWin.pdb has this exact symbol.
// Member: the in-class copy constructor of engine/CCountedPointer.h (member template), emitted out of line as its own COMDAT.
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CVILLAGETASKPICKCROPS
#include "engine/CVillageTaskPickCrops.h"
template CCountedPointer<CVillageTaskBase>::CCountedPointer(const CCountedPointer<CVillageTaskPickCrops>&);
