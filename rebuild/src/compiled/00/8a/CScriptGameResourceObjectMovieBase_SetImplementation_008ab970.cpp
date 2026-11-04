// CScriptGameResourceObjectMovieBase::SetImplementation(CScriptGameResourceObjectMovieBase* imp) -- make the movie handle own a new implementation (reset its counted pointer to imp)
// Leaf: ?SetImplementation@CScriptGameResourceObjectMovieBase@@QAEXPAV1@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AB970, 8 bytes: `add ecx,8 ; jmp 0x008AB2B0` (PImp.Reset(imp), tail call).
// Identity (two sources): (1) FableWin.pdb ?SetImplementation@CScriptGameResourceObjectMovieBase@@QAEXPAV1@@Z, declared in
//   engine/CScriptGameResourceObjectMovieBase.h with this address as its candidate; the twin of the ScriptedThingBase SetImplementation
//   0x008AB960 right before it; (2) its only caller, CGameScriptInterface::StartMovieSequence 0x0089B1EA, `new`s a 0x18-byte movie
//   implementation and passes it here on the CScriptGameResourceObjectMovieBase& argument; the target is the landed-bound
//   ?Reset@?$CCountedPointer@VCScriptGameResourceObjectMovieBase@@@@QAEXPAVCScriptGameResourceObjectMovieBase@@@Z 0x008AB2B0 on PImp (+8).
//   The landed StartMovieSequence binds this address as the stand-in ?Reset@MovieResourceHandle@@QAEXPAUMovieResource@@@Z (NAME_FIXES.tsv).
// Callees: ?Reset@?$CCountedPointer@VCScriptGameResourceObjectMovieBase@@@@QAEXPAVCScriptGameResourceObjectMovieBase@@@Z 0x008AB2B0.
// Header: engine/CScriptGameResourceObjectMovieBase.h (FABLE_RETAIL_BASE_OBJECT), engine/CCountedPointer.h.
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
void CScriptGameResourceObjectMovieBase::SetImplementation(CScriptGameResourceObjectMovieBase* imp)
{
    PImp.Reset(imp);
}