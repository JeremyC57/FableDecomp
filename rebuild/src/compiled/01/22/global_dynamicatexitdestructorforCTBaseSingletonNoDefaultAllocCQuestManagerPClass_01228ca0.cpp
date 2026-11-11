// `dynamic atexit destructor for 'CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass'' -- releases the quest manager singleton's counted pointer at exit
// Leaf: _$E2
// (VC7.1 names the atexit destructor of the static member it defines `_$E2`; the dynamic initializer is `_$E1`. Its PDB name is the line-1 name.)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01228CA0, 57 bytes.
// Identity (agentNF7 2026-11-10): Ego_r.pdb name set "`dynamic atexit destructor for 'CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass''"
//   (donor 0x0101B7C0, between the CGameJoystickManager and CTBaseSingleton<CEngineManager> PClass atexit destructors, as retail's
//   0x0122xxxx atexit block); the body is ~CCountedPointer<CQuestManager> on the PClass slot 0x013B89FC (Data) / 0x013B8A00 (Info):
//   the release, then Data = 0 and Info = 0 in that order. Was "Release_013b89fc" over a private CCountedPointerStatic stand-in.
// Source: the static member's definition (the dynamic initializer is the CCountedPointer default ctor; VC7.1 emits the
//   ??__E / ??__F pair beside it), through the shared engine/CTBaseSingletonNoDefaultAlloc.h and engine/CCountedPointer.h.
#include "fable_compiler.h"
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
class CQuestManager;
template<class T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
template class CTBaseSingletonNoDefaultAlloc<CQuestManager>;
