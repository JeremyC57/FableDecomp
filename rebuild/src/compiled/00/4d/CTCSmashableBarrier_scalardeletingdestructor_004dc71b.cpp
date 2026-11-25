#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004DB004 now binds its PDB spelling ??1CTCSmashableBase@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCSmashableBarrier_Destruct@@YIXPAX@Z.
class CTCSmashableBase { public: virtual ~CTCSmashableBase(); };
void* __fastcall CTCSmashableBarrier_ScalarDeletingDestructor(void* barrier, int, unsigned int flags)
{
    reinterpret_cast<CTCSmashableBase*>(barrier)->CTCSmashableBase::~CTCSmashableBase();
    if (flags & 1)
        operator delete(barrier);
    return barrier;
}