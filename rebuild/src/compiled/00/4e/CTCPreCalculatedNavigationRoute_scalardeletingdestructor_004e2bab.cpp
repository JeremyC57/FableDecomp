#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004E2BC7 now binds its PDB spelling ??1CTCPreCalculatedNavigationRoute@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCPreCalculatedNavigationRoute_Destruct@@YIXPAX@Z.
class CTCPreCalculatedNavigationRoute { public: virtual ~CTCPreCalculatedNavigationRoute(); };
void* __fastcall CTCPreCalculatedNavigationRoute_ScalarDeletingDestructor(void* route, int, unsigned int flags)
{
    reinterpret_cast<CTCPreCalculatedNavigationRoute*>(route)->CTCPreCalculatedNavigationRoute::~CTCPreCalculatedNavigationRoute();
    if (flags & 1)
        operator delete(route);
    return route;
}