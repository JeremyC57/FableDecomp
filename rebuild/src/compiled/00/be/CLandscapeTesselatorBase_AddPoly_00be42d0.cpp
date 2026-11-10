// CLandscapeTesselatorBase::AddPoly(long a, long b, long c) -- append one triangle's three vertex indices (as 16-bit indices) to the bridge index buffer
// Leaf: ?AddPoly@CLandscapeTesselatorBase@@IAEXJJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE42D0, 163 bytes, thiscall `ret 0Ch`. Not called or referenced in retail (expanded at its
//   call sites; the out-of-line COMDAT kept, no /OPT:REF).
// Identity: Ego_r.pdb / FableWin.pdb ?AddPoly@CLandscapeTesselatorBase@@IAEXJJJ@Z (protected, three longs: `ret 0Ch`), declared in
//   the shared engine/CLandscapeTesselatorBase.h; the body is three inline STLport push_backs on BridgeIndexBuffer (+0x04,
//   CArray<unsigned short>: `mov word ptr [finish],dx ; add [finish],2`, else vector<unsigned short>::_M_insert_overflow
//   0x005135D0 with (pos, x, __true_type(), 1, true)). The TU is the landscape tesselator one (neighbours TesselatePatch
//   0x00BE3910, BuildBridgeVertexAndIndexBuffers 0x00BE3660, vector<CTVertexLandscapeBackground>::push_back 0x00BE4380).
// Shape: each long narrows to an unsigned short temporary homed in its own dead argument slot (`mov [esp+8],edx`), which
//   push_back takes by reference.
#include "engine/CLandscapeTesselatorBase.h"

void CLandscapeTesselatorBase::AddPoly(long a, long b, long c)
{
    BridgeIndexBuffer.push_back((unsigned short)a);
    BridgeIndexBuffer.push_back((unsigned short)b);
    BridgeIndexBuffer.push_back((unsigned short)c);
}