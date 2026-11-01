// NProfileTimer::EndProfile(long) -- profiling end marker, compiled empty in the retail build
// Leaf: ?EndProfile@NProfileTimer@@YIXJ@Z
// Retail: Fable.exe 0x009D8250, 1 byte: `ret`.
// Identity: the catalog label `$E2` is wrong (a propagated label of a byte-identical one-byte body). 20 call sites
// (CWorld::ActivateWorld 0x0049F180, CMainGameComponent::PostInit 0x00416953, CDisplayEngine::SetAboutToRun 0x00434CD0
// tail-jumps with ecx = 1, ...) call this __fastcall stub with ecx = the stage id after the phase that the matching
// stub 0x009D8240 (landed NProfileTimer::StartProfile(const CCharString&, long), ecx = &label, edx = id) opened.
// Ego_r.pdb has exactly that pair in namespace NProfileTimer: ?StartProfile@NProfileTimer@@YIXABVCCharString@@J@Z and
// ?EndProfile@NProfileTimer@@YIXJ@Z (donor 0x00B2B540 / 0x00B2B460, adjacent, with the ProfilingInfo map); retail
// compiled both bodies out (profiling disabled), keeping the 16-byte-aligned `ret` stubs side by side.
// reloc_bindings carries the stand-in ?LoadingStageDone@@YIXJ@Z for this address (CWorld::ActivateWorld,
// CMainGameComponent::PostInit): NAME_FIXES.
namespace NProfileTimer {
    void __fastcall EndProfile(long id);
}

void __fastcall NProfileTimer::EndProfile(long id)
{
}