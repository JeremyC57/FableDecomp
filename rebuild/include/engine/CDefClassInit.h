#ifndef FABLE_ENGINE_CDEFCLASSINIT_H
#define FABLE_ENGINE_CDEFCLASSINIT_H
// CDefClassInit (new header, row 0x004EE23F CTCBase::RegisterTCs): the record a definition class registers with
// CDefinitionManager::AddDefClass 0x009B0AC0 (?AddDefClass@CDefinitionManager@@QAEXABVCDefClassInit@@@Z, Ego_r.pdb + FableWin.pdb).
// Layout: Ego_r.pdb CDefClassInit (8 bytes): CCharString Name +0, AllocFunc +4 (the def class's static
// `CDefClassBase* __fastcall Alloc()`, e.g. ?Alloc@CVillageDef@@SIPAVCDefClassBase@@XZ).
// Constructor: FableWin.pdb ??0CDefClassInit@@QAE@ABVCCharString@@P6IPAVCDefClassBase@@XZ@Z, an inline body: retail
// RegisterTCs expands it at its first 8 uses (0x004EE321: Name copy-constructed, then `mov [ebp-698h],Alloc`) and calls the
// out-of-line copy 0x0042DAE0 (landed) at the other 103, once VC7.1's inline budget for that function is spent.
// Destructor: implicit (Ego_r.pdb ??1CDefClassInit@@QAE@XZ is the compiler-generated one: Name's destructor).
#include "fable_string.h"
class CDefClassBase;
class CDefClassInit {
public:
    typedef CDefClassBase* (__fastcall* TAllocFunc)();
    CDefClassInit(const CCharString& name, TAllocFunc allocFunc) : Name(name), AllocFunc(allocFunc) { }   // 0x0042DAE0 out of line
    CCharString Name;                                                  // +0x00
    TAllocFunc  AllocFunc;                                             // +0x04
};
#endif
