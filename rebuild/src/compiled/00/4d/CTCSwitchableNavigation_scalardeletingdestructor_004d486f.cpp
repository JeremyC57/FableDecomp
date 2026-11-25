#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x007876D0 now binds its PDB spelling ??1CTCSwitchableNavigation@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCSwitchableNavigation_Destruct@@YIXPAX@Z.
class CTCSwitchableNavigation { public: virtual ~CTCSwitchableNavigation(); };
void* __fastcall CTCSwitchableNavigation_ScalarDeletingDestructor(void* navigation, int, unsigned int flags)
{
    reinterpret_cast<CTCSwitchableNavigation*>(navigation)->CTCSwitchableNavigation::~CTCSwitchableNavigation();
    if (flags & 1)
        operator delete(navigation);
    return navigation;
}