// GFXToY(float) -- convert an X-axis length into the matching Y-axis length: x * render-target height / render-target width
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E23F0, length 64 bytes. PDB: ?GFXToY@@YIMM@Z (float __fastcall GFXToY(float)).
// Callees: GFGetSystemManager @ 0x009A4EC0 (twice), CDisplayManager::GetRenderTargetDimensions @ 0x009BEDC0 (twice,
//          returns a C2DExtentsI {X = width, Y = height} through the hidden pointer).
// Shape: both GFGetSystemManager() calls run before either dimensions call, the first result kept in esi for the
//   height and the second used straight away for the width (`fild [height] ; fmul x ; fidiv [width]`): two named
//   display-manager locals, fetched height-source first. The single-expression form interleaves the calls instead.
//   The system manager keeps its display manager at +0x60 (same view as the landed GFAXToVX 0x009E1DA0); no engine
//   header exists for CSystemManager / CDisplayManager, so both are local views.
// Leaf: ?GFXToY@@YIMM@Z
class C2DExtentsI                                    // PDB: 8 bytes {X, Y}
{
public:
    long X;                                           // width
    long Y;                                           // height
};

class CDisplayManager
{ public:
    C2DExtentsI GetRenderTargetDimensions() const;   // 0x009BEDC0
};

class CSystemManager
{ public:
    unsigned char _pad000[0x60];
    CDisplayManager* DisplayManager;                  // +0x60
};

CSystemManager* __fastcall GFGetSystemManager();     // 0x009A4EC0

float __fastcall GFXToY(float x)
{
    CDisplayManager* heightSource = GFGetSystemManager()->DisplayManager;
    CDisplayManager* widthSource = GFGetSystemManager()->DisplayManager;
    return x * heightSource->GetRenderTargetDimensions().Y
             / widthSource->GetRenderTargetDimensions().X;
}