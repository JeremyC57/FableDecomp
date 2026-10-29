// GFXToY(float) -- rescale a virtual X extent into virtual Y units for the current render target: x * renderTargetWidth / renderTargetHeight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E23A0, length 70 bytes (+ int3 padding).
// PDB (FableWin): ?GFXToY@@YIMM@Z -- __fastcall with the float on the stack (`ret 4`).
// Callees: GFGetSystemManager (0x009A4EC0; +0x60 = display manager), twice, and
// CDisplayManager::GetRenderTargetDimensions (0x009BEDC0, C2DExtentsI {X, Y} by hidden pointer), twice.
// Shape: both display-manager pointers are fetched first (esi / edi) -- two named locals; the product
// `x * width` (a float) is parked in the dead argument slot across the second dimension call, then
// `fild height ; fdivr [x]`.
class C2DExtentsI
{ public:
    long X;
    long Y;
};

class CDisplayManager
{
public:
    const C2DExtentsI GetRenderTargetDimensions();     // 0x009BEDC0
};

class CSystemManager
{
public:
    unsigned char    _pad00[0x60];
    CDisplayManager* m_pDisplayManager;                // +0x60
};

CSystemManager* __fastcall GFGetSystemManager();       // 0x009A4EC0

float __fastcall GFXToY(float x)
{
    CDisplayManager* widthSource = GFGetSystemManager()->m_pDisplayManager;
    CDisplayManager* heightSource = GFGetSystemManager()->m_pDisplayManager;
    return x * widthSource->GetRenderTargetDimensions().X / heightSource->GetRenderTargetDimensions().Y;
}