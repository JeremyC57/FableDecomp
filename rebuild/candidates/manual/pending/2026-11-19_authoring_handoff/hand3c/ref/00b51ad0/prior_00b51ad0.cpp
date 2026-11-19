// CEngineWeatherRenderer::BuildBoxPositionMatrix() -- snap the camera position to the weather box grid (floor each axis, then round to the nearest box multiple; Z only when not locked) and fill the 9 x 9 box-centre position table around it
// Grade: DIFFER(955v969) -- best b (snap part matches incl. inline GetBoxCoord) 965v969 dl=53: the 9x9 fill -- retail keeps the outer counter in [ebp-4] with a 9-trip down-counter and ebx/esi/edi live (pops after the loop), one imul operand order in the Z snap; the inner loop is j*BoxSizeY + CentreY fully unrolled (best try d; the bare-dword fistp itself is reproduced by the shared helper, see NOTES.md)
// Leaf: ?BuildBoxPositionMatrix@CEngineWeatherRenderer@@AAEXXZ
// Flags: none
// Retail-asm: 0x00B51AFC, 0x00B51B39, 0x00B51B9A, 0x00B51BD7, 0x00B51C46, 0x00B51C83 `fistp dword ptr [ebp-4]` -- each camera coordinate and
//   each box size floored by the engine's inline-assembly GFFloatToLongFloor (shared engine/GFFloatToLongFloor.h: 0.5f / 1.0f frame
//   constants, bare dword fistp, fild/fadd, integer compare, `inc` on memory). VC7.1 emits no bare dword fistp from C++ (NOTES.md
//   "C++ attempts before helper"). This row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B51AD0, 969 bytes, thiscall `ret`, ebp frame.
// Identity: Ego_r.pdb ?BuildBoxPositionMatrix@CEngineWeatherRenderer@@AAEXXZ (private; the reloc binding's QAE spelling is wrong,
//   NAME_FIXES.tsv); the snap is Ego_r's private ?GetBoxCoord@CEngineWeatherRenderer@@AAEXAAJJ@Z, inline here.
// Globals: g_pEngineCamera 0x01436EA0 (landed spelling; camera position +0x54).
// Layout: local model (engine/CEngineWeatherRenderer.h is opaque past +0xc): ZLocked +0x11b, box sizes +0x1d4, centre +0x1f0, the
//   9 x 9 box-centre table +0x1fc.
#include "engine/GFFloatToLongFloor.h"
class C3DVector { public: float X, Y, Z; };
class CEngineCamera { public: unsigned char _pad[0x54]; C3DVector Position; };
extern CEngineCamera* g_pEngineCamera;                         // 0x01436EA0
class CEngineWeatherRenderer {
public:
    unsigned char _pad0[0x11b];
    bool ZLocked;                                              // +0x11b
    unsigned char _pad11c[0xb8];
    float BoxSizeX, BoxSizeY, BoxSizeZ;                        // +0x1d4
    unsigned char _pad1e0[0x10];
    long CentreX, CentreY, CentreZ;                            // +0x1f0
    C3DVector Boxes[9][9];                                     // +0x1fc
private:
    void GetBoxCoord(long& coord, long size)
    {
        long remainder = coord % size;
        coord = coord / size * size;
        if (remainder >= size / 2)
            coord += size;
    }
    void BuildBoxPositionMatrix();
};
void CEngineWeatherRenderer::BuildBoxPositionMatrix()
{
    C3DVector& camera = g_pEngineCamera->Position;
    CentreX = GFFloatToLongFloor(camera.X);
    GetBoxCoord(CentreX, GFFloatToLongFloor(BoxSizeX));
    CentreY = GFFloatToLongFloor(camera.Y);
    GetBoxCoord(CentreY, GFFloatToLongFloor(BoxSizeY));
    if (!ZLocked) {
        CentreZ = GFFloatToLongFloor(camera.Z);
        GetBoxCoord(CentreZ, GFFloatToLongFloor(BoxSizeZ));
    }
    for (long i = 0; i < 9; ++i) {
        float x = (float)(i - 4);
        for (long j = -4; j <= 4; ++j) {
            Boxes[i][j + 4].X = x * BoxSizeX + CentreX;
            Boxes[i][j + 4].Y = j * BoxSizeY + CentreY;
            Boxes[i][j + 4].Z = (float)CentreZ;
        }
    }
}
