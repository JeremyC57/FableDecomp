// CTCAssassinRush::SetFinalAction(CCountedPointer<CCreatureActionBase> pool) -- adopt the counted pointer into FinalAction (the parameter copy is released on return)
// Leaf: ?SetFinalAction@CTCAssassinRush@@QAEXV?$CCountedPointer@VCCreatureActionBase@@@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008ceae0, 106 bytes, thiscall, `ret 8` (the 8-byte counted pointer by value).
// Identity (proven): Ego_r.pdb ?SetFinalAction@CTCAssassinRush@@QAEXV?$CCountedPointer@VCCreatureActionBase@@@@@Z (bound in reloc_bindings with the stand-in parameter UCCountedAction); caller CCreatureAction_CastAssassinRush::Initialise 0x008CD470 (landed).
// Callees: operator delete 0x00BFE9BC (each released count block); CCountedPointer's operator= and destructor inline.
// Layout: Ego_r.pdb FinalAction (PDB +148) at +0x90.
// Header debt: engine/CTCAssassinRush.h is a flat layout without methods; modelled locally.
#include "engine/CCountedPointer.h"
class CCreatureActionBase;
class CTCAssassinRush {
public:
    unsigned char _pad00[0x90];
    CCountedPointer<CCreatureActionBase> FinalAction;                          // +0x90
    void SetFinalAction(CCountedPointer<CCreatureActionBase> pool);
};
void CTCAssassinRush::SetFinalAction(CCountedPointer<CCreatureActionBase> pool)
{
    FinalAction = pool;
}