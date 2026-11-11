// CCountedPointer::CCountedPointer(const CCountedPointer<U>&) -- converting copy: take Data and Info, then add a reference (T = CVillageTaskBase, U = CVillageTaskCarryCrate)
// Leaf: ??$?0VCVillageTaskCarryCrate@@@?$CCountedPointer@VCVillageTaskBase@@@@QAE@ABV?$CCountedPointer@VCVillageTaskCarryCrate@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065DB8E, 25 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_CopyCtor_0065db8e (stand-in name).
// T = CVillageTaskBase, proven by: CTCVillage::RemoveThingFromTaskList<CVillageTaskCarryCrate> and the std::__find_if / list::remove_if instances over list<CCountedPointer<CVillageTaskCarryCrate>> (CTCVillage member of that type, Ego_r) up-cast each element through it to the CIsTask / CIsThingInvolvedWithTask predicate taking const CCountedPointer<CVillageTaskBase>&; the landed caller binds this member template; FableWin.pdb has this exact symbol.
// Member: the in-class copy constructor of engine/CCountedPointer.h (member template), emitted out of line as its own COMDAT.
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CVILLAGETASKCARRYCRATE
#include "engine/CVillageTaskCarryCrate.h"
template CCountedPointer<CVillageTaskBase>::CCountedPointer(const CCountedPointer<CVillageTaskCarryCrate>&);
