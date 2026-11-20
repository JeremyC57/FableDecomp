// C2DBoxF::ToC2DBoxI() const -- the four float edges truncated to longs into a C2DBoxI returned through the hidden pointer
// Leaf: ?ToC2DBoxI@C2DBoxF@@QBE?AVC2DBoxI@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A55A30, length 65 bytes.
// Name (NF197, 2026-11-20): Ego_r.pdb 0x00B9D930 + FableWin.pdb 0x03217AA0 ?ToC2DBoxI@C2DBoxF@@QBE?AVC2DBoxI@@XZ (was the made-up
//   ToIntBox); engine/C2DBoxF.h (FABLE_RETAIL_C2DBOXF_CENTRE) declares it out of line at this address, and the landed callers
//   (0x00969E70 ...) already bind this spelling. Ported from local C2DBoxF / C2DBoxI models onto the shared headers.
// Callees: __ftol2 @ 0x00BFEA70 (four times). Shape: the constructor's four arguments are evaluated right to left (BRY first, kept in edi, then BRX in ebx,
// TLY in ebp, TLX last in eax) and stored into the hidden result after the last conversion; `ret 4` pops the hidden pointer. The chunk label
// "CEditWorldMap::GetPenArea" is propagated (`this` is a box of four floats, not the editor map); the box types follow the PDB (C2DBoxF / C2DBoxI: TLX, TLY, BRX, BRY).
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CENTRE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_ORIGINAL_FOUR_COORD_CTOR
#include "engine/C2DBoxI.h"
#include "engine/C2DBoxF.h"

C2DBoxI C2DBoxF::ToC2DBoxI() const
{
    return C2DBoxI((long)TLX, (long)TLY, (long)BRX, (long)BRY);
}
