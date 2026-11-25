#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x005B7049 now binds its PDB spelling ??1CTCInventoryClothing@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCInventoryClothing_Destruct@@YIXPAX@Z.
class CTCInventoryClothing { public: virtual ~CTCInventoryClothing(); };
void* __fastcall CTCInventoryClothing_ScalarDeletingDestructor(void* clothing, int, unsigned int flags)
{
    reinterpret_cast<CTCInventoryClothing*>(clothing)->CTCInventoryClothing::~CTCInventoryClothing();
    if (flags & 1)
        operator delete(clothing);
    return clothing;
}