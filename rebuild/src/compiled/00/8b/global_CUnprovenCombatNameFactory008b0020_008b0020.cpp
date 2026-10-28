// _global::CUnprovenCombatNameFactory_008b0020() -- construct the named combat-sequence string.
// Leaf: ?CUnprovenCombatNameFactory_008b0020@@YI?AVCCharString@@XZ
// Identity: descriptive; own literalCOMBAT_SEQUENCE_FODDER_MAIN_ATTACK and constructor0099EBF0; unreferenced callable body, no RTTI method association; caller role and original name unproven. By-value fastcall string factory is a descriptive ABI contract.
// Signature audit: original CCharString(const char*,long) uses PDB ??0CCharString@@QAE@PBDJ@Z; old int/H alias removed, actual test constructor corrected too.
// Layout: no shared CCharString header; original four-byte PStringData wrapper and constructor signature reused.
#ifndef _MSC_VER
#define __fastcall
#endif
class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString(const char*,long=-1);};
CCharString __fastcall CUnprovenCombatNameFactory_008b0020(){return CCharString("COMBAT_SEQUENCE_FODDER_MAIN_ATTACK");}