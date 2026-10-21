// CNewFrontendGameComponent::CompileDefs() -- compiles the front-end definitions (when enabled) and then the main game definitions
// Leaf: CompileDefs@CNewFrontendGameComponent
// Retail: Fable.exe @ 0x0042F70B, 23 bytes; Ego_r.pdb ?CompileDefs@CNewFrontendGameComponent@@SIXXZ (static __fastcall).
// Calls the static __fastcall helper 0x0042F5A9 (?CompileDefsPlatformSpecific@CNewFrontendGameComponent@@SIX_N@Z; its bool
// in cl picks one of two def-file names) once with true when g_bCompileFrontendDefinitions (0x013B8648, as named by landed code) is set, then with false as a
// tail jump. Callers: CGame::Play 0x00412FA6, CNewFrontendGameComponent::InitialiseDefs 0x0042F729.
// Shape: size TU (`cmp byte ptr [mem],0` instead of loading al).
#pragma optimize("s", on)
extern bool g_bCompileFrontendDefinitions;              // 0x013B8648
class CNewFrontendGameComponent {
public:
    static void __fastcall CompileDefs();
    static void __fastcall CompileDefsPlatformSpecific(bool frontend);   // 0x0042F5A9
};
void __fastcall CNewFrontendGameComponent::CompileDefs()
{
    if (g_bCompileFrontendDefinitions)
        CompileDefsPlatformSpecific(true);
    CompileDefsPlatformSpecific(false);
}