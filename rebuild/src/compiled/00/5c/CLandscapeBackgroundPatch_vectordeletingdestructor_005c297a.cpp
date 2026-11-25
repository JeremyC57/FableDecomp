#pragma optimize("s", on)
// Leaf: ?CLandscapeBackgroundPatch_ScalarDeletingDestructor@@YIPAXPAXHI@Z
// Naming (NF423 gen, 2026-11-25): the callee 0x005C2739 now binds its PDB spelling ??1CCategory@NInventory@@QAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?AutoTinyScalarDestructor_005c2739@@YIXPAX@Z.
namespace NInventory { class CCategory { public: ~CCategory(); }; }
void* __fastcall CLandscapeBackgroundPatch_ScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    reinterpret_cast<NInventory::CCategory*>(self)->NInventory::CCategory::~CCategory();
    if (flags & 1)
        operator delete(self);
    return self;
}