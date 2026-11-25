#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x0053B7F0 now binds its PDB spelling ??1CList@NUISystem@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CList_Destruct@@YIXPAX@Z.
namespace NUISystem { class CList { public: virtual ~CList(); }; }
void* __fastcall CList_ScalarDeletingDestructor(void* list, int, unsigned int flags)
{
    reinterpret_cast<NUISystem::CList*>(list)->NUISystem::CList::~CList();
    if (flags & 1)
        operator delete(list);
    return list;
}