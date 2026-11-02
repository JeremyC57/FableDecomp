// CLandscapeBackgroundPatch::GetEdgeStrip(LANDSCAPE_EDGE_STRIP_SIDE side) -- the patch's tesselation edge strip for one of its four sides, 0 for any other side value
// Leaf: ?GetEdgeStrip@CLandscapeBackgroundPatch@@QAEPAVCPatchTesselationEdgeStrip@@W4LANDSCAPE_EDGE_STRIP_SIDE@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE5C60, 64 bytes = 48 bytes of code (`cmp eax,3 ; ja default ; jmp [eax*4+0BE5C90h]`, four
//   `lea eax,[ecx+N] ; ret 4` arms, `xor eax,eax ; ret 4`) + the 4-entry jump table 0x00BE5C90..0x00BE5C9F (cases 0..3 ->
//   0x00BE5C70 / 76 / 7C / 82 in order); the next function (InvalidateEdge 0x00BE5CA0) follows the table directly.
// Identity (proven, two sources): (1) Ego_r.pdb ?GetEdgeStrip@CLandscapeBackgroundPatch@@QAEPAVCPatchTesselationEdgeStrip@@W4LANDSCAPE_EDGE_STRIP_SIDE@@@Z
//   (one enum argument = `ret 4`), with the Ego_r donor order ClearTextures / GetEdgeStrip / InvalidateEdge(side) = retail
//   0x00BE5C30 / 0x00BE5C60 / 0x00BE5CA0 (both neighbours landed); (2) the only caller is CLandscapeBackgroundTreeNode::GetEdgeStrip
//   0x00BF6930 (Ego_r ?GetEdgeStrip@CLandscapeBackgroundTreeNode@@QAEPAVCPatchTesselationEdgeStrip@@JJW4LANDSCAPE_EDGE_STRIP_SIDE@@@Z,
//   the same return type), and the four arms return &EdgeStrips[0..3]: Ego_r EdgeStrips = CPatchTesselationEdgeStrip[4] at +0x08,
//   sizeof 0x2C (+0x08 / +0x34 / +0x60 / +0x8C). The forge label happens to be right.
// Shape: a dense switch over the four sides, each case returning its own strip, no-match returning 0 (VC7.1 builds the range
//   check + table; the cases are written in table order). The enumerator names are in neither name set: case values written as
//   numbers.
// Layout: engine/CLandscapeBackgroundPatch.h under FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH (chunk 706 retail opt-in, hdr/ overlay):
//   RTTI CLandscapeBackgroundPatch : CLandscapeBackgroundSubNodeBase : CMovableResource (vptr only, both new opt-ins), Parent +0x04,
//   EdgeStrips CPatchTesselationEdgeStrip[4] +0x08 (new engine/CPatchTesselationEdgeStrip.h : CBaseClassNonCopyable, 0x2C bytes),
//   then every Ego_r member to sizeof 0x11C.
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH
#include "engine/CLandscapeBackgroundPatch.h"

CPatchTesselationEdgeStrip* CLandscapeBackgroundPatch::GetEdgeStrip(LANDSCAPE_EDGE_STRIP_SIDE side)
{
    switch (side) {
    case 0: return &EdgeStrips[0];
    case 1: return &EdgeStrips[1];
    case 2: return &EdgeStrips[2];
    case 3: return &EdgeStrips[3];
    }
    return 0;
}