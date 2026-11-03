// CEngine::OverrideSnow(float amount) -- override the weather renderer's snow amount (a CIEngine interface forwarder)
// Leaf: ?OverrideSnow@CEngine@@UAEXM@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B24020, length 11 bytes: `mov ecx,[0x01436E44] ; jmp 0x00B50F40`.
// Identity: slot 81 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?OverrideSnow@CEngine@@UAEXM@Z (the virtual CIEngine method, `this` never read).
// Callee: CEngineWeatherRenderer::OverrideSnow @ 0x00B50F40, Ego_r + FableWin ?OverrideSnow@CEngineWeatherRenderer@@QAEXM@Z;
//   its `ret 4h` pops the same 1 stack argument, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E44 (CEngineWeatherRenderer*) under its landed spelling g_pEngineWeatherRenderer; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).

class CEngineWeatherRenderer { public:
    void OverrideSnow(float amount);   // 0x00B50F40
};
extern CEngineWeatherRenderer* g_pEngineWeatherRenderer;   // 0x01436E44

class CEngine { public:                      // vtable 0x012A0F3C, slot 81
    virtual void OverrideSnow(float amount);
};

void CEngine::OverrideSnow(float amount)
{
    g_pEngineWeatherRenderer->OverrideSnow(amount);
}