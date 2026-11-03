// CEngine::SetWindow(const C2DBoxF* window) -- hand the game window box to the engine camera (a CIEngine interface forwarder)
// Leaf: ?SetWindow@CEngine@@UAEXPBVC2DBoxF@@@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23B80, length 11 bytes: `mov ecx,[0x01436EA0] ; jmp 0x00B2FC00`.
// Identity: slot 5 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?SetWindow@CEngine@@UAEXPBVC2DBoxF@@@Z (the virtual CIEngine method, `this` never read).
// Callee: CEngineCamera::SetGameWindow @ 0x00B2FC00, Ego_r + FableWin ?SetGameWindow@CEngineCamera@@QAEXPBVC2DBoxF@@@Z;
//   its `ret 4h` pops the same 1 stack argument, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436EA0 (CEngineCamera*) under its landed spelling g_pEngineCamera; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).
class C2DBoxF;

class CEngineCamera { public:
    void SetGameWindow(const C2DBoxF* window);   // 0x00B2FC00
};
extern CEngineCamera* g_pEngineCamera;   // 0x01436EA0

class CEngine { public:                      // vtable 0x012A0F3C, slot 5
    virtual void SetWindow(const C2DBoxF* window);
};

void CEngine::SetWindow(const C2DBoxF* window)
{
    g_pEngineCamera->SetGameWindow(window);
}
