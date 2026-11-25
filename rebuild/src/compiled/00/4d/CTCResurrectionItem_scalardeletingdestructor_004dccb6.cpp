#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004DCCD2 now binds its PDB spelling ??1CTCResurrectionItem@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCResurrectionItem_Destruct@@YIXPAX@Z.
class CTCResurrectionItem { public: virtual ~CTCResurrectionItem(); };
void* __fastcall CTCResurrectionItem_ScalarDeletingDestructor(void* item, int, unsigned int flags)
{
    reinterpret_cast<CTCResurrectionItem*>(item)->CTCResurrectionItem::~CTCResurrectionItem();
    if (flags & 1)
        operator delete(item);
    return item;
}