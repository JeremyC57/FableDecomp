#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004DB004 now binds its PDB spelling ??1CTCSmashableBase@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCSmashableWindow_Destruct@@YIXPAX@Z.
class CTCSmashableBase { public: virtual ~CTCSmashableBase(); };
void* __fastcall CTCSmashableWindow_ScalarDeletingDestructor(void* window, int, unsigned int flags)
{
    reinterpret_cast<CTCSmashableBase*>(window)->CTCSmashableBase::~CTCSmashableBase();
    if (flags & 1)
        operator delete(window);
    return window;
}