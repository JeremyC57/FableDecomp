// NProfileTimer::StartProfile(const CCharString&, long) -- profiling start marker, compiled empty in the retail build
// Retail: Fable.exe 0x009D8240, 1 byte: `ret`.
// Identity: the catalog label `$E2` is wrong (47 call sites). Every caller builds a temporary CCharString from a phase
// label ("Setup Basic install files", "Activate Initial Quests", "Init Characters", "Init Atmos", ...) and calls this
// __fastcall stub with ecx = &string, edx = 0, then destroys the string; the matching stub 0x009D8250 (`ret`, its own
// catalog row) is called after the phase with ecx = 0. Ego_r.pdb has exactly that pair in namespace NProfileTimer:
// ?StartProfile@NProfileTimer@@YIXABVCCharString@@J@Z and ?EndProfile@NProfileTimer@@YIXJ@Z (the only PDB free
// function taking (const CCharString&, long)). reloc_bindings carries the stand-in ?LoadingStage@@YIXABVCCharString@@J@Z.
// Leaf: ?StartProfile@NProfileTimer@@YIXABVCCharString@@J@Z
class CCharString {
public:
    void* PStringData;
};

namespace NProfileTimer {
    void __fastcall StartProfile(const CCharString& name, long id);
}

void __fastcall NProfileTimer::StartProfile(const CCharString& name, long id)
{
}