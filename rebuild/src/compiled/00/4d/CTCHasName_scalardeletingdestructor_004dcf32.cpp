#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004DCF4E now binds its PDB spelling ??1CTCHasName@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCHasName_Destruct@@YIXPAX@Z.
class CTCHasName { public: virtual ~CTCHasName(); };
void* __fastcall CTCHasName_ScalarDeletingDestructor(void* name, int, unsigned int flags)
{
    reinterpret_cast<CTCHasName*>(name)->CTCHasName::~CTCHasName();
    if (flags & 1)
        operator delete(name);
    return name;
}