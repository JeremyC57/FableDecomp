// CEngine::Set2DPrimitiveLightingParameters(const C3DVector& lightDirection, const CRGBFloatColour& diffuse, const CRGBFloatColour& ambient, const CRGBFloatColour& backlight) -- set the 2D primitive renderer's light direction and colours (a CIEngine interface forwarder)
// Leaf: ?Set2DPrimitiveLightingParameters@CEngine@@UAEXABVC3DVector@@ABVCRGBFloatColour@@11@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23EB0, length 11 bytes: `mov ecx,[0x01436E7C] ; jmp 0x00B4A5F0`.
// Identity: slot 67 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?Set2DPrimitiveLightingParameters@CEngine@@UAEXABVC3DVector@@ABVCRGBFloatColour@@11@Z (the virtual CIEngine method, `this` never read).
// Callee: CEnginePrimitiveRenderer2D::SetLightingParameters @ 0x00B4A5F0, Ego_r + FableWin ?SetLightingParameters@CEnginePrimitiveRenderer2D@@QAEXABVC3DVector@@ABVCRGBFloatColour@@11@Z;
//   its `ret 10h` pops the same 4 stack arguments, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E7C (CEnginePrimitiveRenderer2D*) under its landed spelling g_pEnginePrimitiveRenderer2D; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).
class C3DVector;
class CRGBFloatColour;

class CEnginePrimitiveRenderer2D { public:
    void SetLightingParameters(const C3DVector& lightDirection, const CRGBFloatColour& diffuse, const CRGBFloatColour& ambient, const CRGBFloatColour& backlight);   // 0x00B4A5F0
};
extern CEnginePrimitiveRenderer2D* g_pEnginePrimitiveRenderer2D;   // 0x01436E7C

class CEngine { public:                      // vtable 0x012A0F3C, slot 67
    virtual void Set2DPrimitiveLightingParameters(const C3DVector& lightDirection, const CRGBFloatColour& diffuse, const CRGBFloatColour& ambient, const CRGBFloatColour& backlight);
};

void CEngine::Set2DPrimitiveLightingParameters(const C3DVector& lightDirection, const CRGBFloatColour& diffuse, const CRGBFloatColour& ambient, const CRGBFloatColour& backlight)
{
    g_pEnginePrimitiveRenderer2D->SetLightingParameters(lightDirection, diffuse, ambient, backlight);
}