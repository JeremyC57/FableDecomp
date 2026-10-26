// CEnginePrimitiveRenderer::ResetWorldTransform() -- hands an identity matrix to the shader render manager's and the render core's world transform
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B32100, length 133 bytes.
// Signature from Ego_r.pdb: ?ResetWorldTransform@CEnginePrimitiveRenderer@@AAEXXZ (private, `this` unused, plain `ret`).
// Callees: 0x009881F0 CShaderRenderManager::SetWorldTransform(const CMatrix3x4&) on the global manager @ 0x01436E14,
// 0x00A0B140 CRenderManagerCore::SetWorldTransform(const CMatrix3x4&) on the global render core @ 0x01436E18
// (catalogued as SetViewTransform; the PDB has both setters with this signature on CRenderManagerCore, and the
// method reset here is the world transform).
// Shape: one 0x30-byte identity matrix local built by the inline default constructor -- the landed SetIdentity
// chained assignment (0x00B09F90): nine zero stores in descending address order, then the 1.0f diagonal from
// ZAxis.Z down to XAxis.X. The first manager pointer is loaded before the argument is formed (the global is read
// into ecx first, then `lea eax ; push eax`), the second after (`lea ecx ; push ecx ; mov ecx,[global]`):
// VC schedules the first global load early because the matrix stores are independent of it.
#include "engine/CEnginePrimitiveRenderer.h"

class C3DVector  { public: float X, Y, Z; };
class CMatrix3x4 { public:
    C3DVector XAxis, YAxis, ZAxis, Position;
    CMatrix3x4() { SetIdentity(); }
    void SetIdentity()                                   // chained assignments: the stores run right to left
    {
        XAxis.Y = XAxis.Z = YAxis.X = YAxis.Z = ZAxis.X = ZAxis.Y = Position.X = Position.Y = Position.Z = 0.0f;
        XAxis.X = YAxis.Y = ZAxis.Z = 1.0f;
    }
};

class CShaderRenderManager { public:
    void SetWorldTransform(const CMatrix3x4& transform);     // 0x009881F0
};
struct CRenderManagerCore {
    void SetWorldTransform(const CMatrix3x4& transform);     // 0x00A0B140
};
extern CShaderRenderManager* g_pShaderRenderManager;         // 0x01436E14
extern CRenderManagerCore*   g_pRenderManagerCore;           // 0x01436E18

struct CEnginePrimitiveRenderer_Methods : CEnginePrimitiveRenderer {
    void ResetWorldTransform();
};

void CEnginePrimitiveRenderer_Methods::ResetWorldTransform()
{
    CMatrix3x4 identity;
    g_pShaderRenderManager->SetWorldTransform(identity);
    g_pRenderManagerCore->SetWorldTransform(identity);
}