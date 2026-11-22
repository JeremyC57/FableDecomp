// NUISystem::CManager::`scalar deleting destructor'(unsigned int flags) -- the out-of-line ~CManager, then operator delete on flag bit 0; returns this
// Leaf: ??_GCManager@NUISystem@@IAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042DB20, 30 bytes, thiscall `ret 4`: push esi ; mov esi,ecx ; call ~CManager 0x0041E641 ;
//   test [esp+8],1 ; je ; push esi ; call operator delete 0x00BFE9BC ; pop ecx ; mov eax,esi ; pop esi. No retail code references it:
//   the compiler emitted it beside Clear's `delete Instance` (landed 0x0042FB40, whose TU emits the same COMDAT).
// Names (NF224, 2026-11-22, q12560): Ego_r.pdb and FableWin.pdb ??_GCManager@NUISystem@@IAEPAXI@Z (protected, like the destructor) and
//   ??1CManager@NUISystem@@IAE@XZ for the callee 0x0041E641 (its self leaf; 0x0042FB40 binds it). Was a global stand-in
//   `struct CManager { void Dtor(); void* ScalarDeletingDestructor(unsigned); }` (self ?ScalarDeletingDestructor@CManager@@QAEPAXI@Z, callee
//   ?Dtor@CManager@@QAEXXZ); now on the shared engine/NUISystem_CManager.h.
// Fidelity: scaffolding, not retail: the body is compiler-generated; this row reaches it through a delete-expression in an
//   anonymous-namespace derived class (the destructor is protected: the nf86 / nf88 form of 0x009A0AE0 CMemoryAllocator) so VC7.1
//   emits the ??_G pick-any COMDAT. Internal to this TU; never called.
#include "engine/NUISystem_CManager.h"

namespace {
struct CManagerDeleteReference : NUISystem::CManager {
    static void Reference(NUISystem::CManager* manager);
};
void CManagerDeleteReference::Reference(NUISystem::CManager* manager)
{
    delete manager;
}
}
