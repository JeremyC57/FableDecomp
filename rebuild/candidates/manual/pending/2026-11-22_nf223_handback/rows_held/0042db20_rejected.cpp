// NUISystem::CManager::`scalar deleting destructor'(unsigned int flags) -- the out-of-line ~CManager, then operator delete on flag bit 0; returns this
// Leaf: ??_GCManager@NUISystem@@IAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042DB20, 30 bytes, thiscall `ret 4`. No retail code references it: the compiler emitted it beside
//   Clear's `delete Instance` (landed 0x0042FB40), which expands it.
// Names (NF223, 2026-11-22, q12556): Ego_r.pdb and FableWin.pdb ??_GCManager@NUISystem@@IAEPAXI@Z (protected, like the destructor) and
//   ??1CManager@NUISystem@@IAE@XZ for the callee 0x0041E641 (its self leaf; 0x0042FB40 binds it). Was a global stand-in
//   `struct CManager { void Dtor(); void* ScalarDeletingDestructor(unsigned); }` (self ?ScalarDeletingDestructor@CManager@@QAEPAXI@Z, callee
//   ?Dtor@CManager@@QAEXXZ). The HEAD-local model moves into namespace NUISystem in place (same class token), with the destructor
//   protected as engine/NUISystem_CManager.h declares it.
// Shape: VC7.1 emits a class's ??_G when the TU calls its destructor explicitly and unqualified (`p->~CManager()`); a qualified call does
//   not. The protected destructor is reached from a TU-internal friend.
// Fidelity: scaffolding, not retail: EmitScalarDeletingDestructor (anonymous namespace, internal) only makes this TU emit the ??_G COMDAT.
namespace NUISystem { class CManager; }
namespace { void EmitScalarDeletingDestructor(NUISystem::CManager* manager); }
namespace NUISystem {
class CManager {
protected:
    ~CManager();                                         // 0x0041E641
    friend void ::EmitScalarDeletingDestructor(CManager* manager);
};
}
void __cdecl operator delete(void* p);                   // 0x00BFE9BC
namespace {
void EmitScalarDeletingDestructor(NUISystem::CManager* manager) { manager->~CManager(); }
}
