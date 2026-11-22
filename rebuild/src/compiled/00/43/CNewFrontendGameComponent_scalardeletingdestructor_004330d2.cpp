#pragma optimize("s",on)
// vector deleting destructor whose deallocator is the engine free helper (0x00bfe9bc),
// cleaned with `pop ecx` (size peephole). __fastcall this=ecx, flags=stack (ret 4).
// Names (NF233, 2026-11-22): the destructor this calls is ??1CNewFrontendGameComponent@@UAE@XZ (0x0042F50B; Ego_r + FableWin + Ego_d,
//   public virtual), the spelling the self row 0x0042F50B emits; was the stand-in ?Dtor@CNewFrontendGameComponent@@QAEXXZ. The qualified
//   call is the direct (non-virtual) one retail makes. This row's own leaf is still the stand-in ScalarDeletingDestructor (retail:
//   ??_GCNewFrontendGameComponent@@UAEPAXI@Z, all three PDBs; the compiler emits that only with the class's vtable, i.e. in a TU that
//   defines its constructor or destructor, so it cannot be this row's leaf without a duplicate definition: NOTES).
class CNewFrontendGameComponent { public: virtual ~CNewFrontendGameComponent(); void* ScalarDeletingDestructor(unsigned flags); };
void operator delete(void*);
void* CNewFrontendGameComponent::ScalarDeletingDestructor(unsigned flags) {
    this->CNewFrontendGameComponent::~CNewFrontendGameComponent();
    if (flags & 1) operator delete(this);
    return this;
}
