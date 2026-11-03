# chunk 721 (CEngineLightingManager + CNavigatorAStar)

Matched (payload.json, 6 rows): 00b44820 MATCH, 00b44a60 MATCH, 00b45fb0, 00b46280, 00b46660, 00a1e030 RELOCATION_MATCH.
The five lighting rows are grade `retail_asm` (sources in retail_asm/, each with a `// Retail-asm:` line citing the
retail block): 00b44820/00b44a60 are Lionhead's hand-SSE vertex loops; 00b45fb0/00b46280/00b46660 expand the inline-asm
floor helper GFFloatToLongFloor (bare `fistp dword`), and 00b46280 also the hand-SSE C3DBoundingBox::CalcShortestDistanceToPointSquared
kernel behind g_SSEAvailable (same kernel as landed 0x00BF7DF0).

Header overlays (hdr/, opt-in branches, default path unchanged per hdrcheck2.py):
- engine/C2DVector.h: FABLE_RETAIL_C2DVECTOR adds `#include <math.h>` and `float GetMagnitude() const { return sqrtf(X*X+Y*Y); }`
  (PDB ?GetMagnitude@C2DVector@@QBEMXZ). Needed by 00a1e030.
- engine/C3DBoundingBox.h: FABLE_RETAIL_C3DBOUNDINGBOX makes MinCoord/MaxCoord public, adds GetCenter() = (MaxCoord + MinCoord) * 0.5f
  and declares CalcShortestDistanceToPointSquared (defined __forceinline in the row because its body is the SSE asm). Needed by 00b46280.

Parked (differ/): 00a1a7e0 GetIntersectionPointsBetweenNodes DIFFER(862v862) after 13 probes -- one x87 scheduling swap;
00a1ad40 GetPosInDirection DIFFER(366v370) after 14 probes -- frame slots / shrink-wrapped ebx / cold fstp block / operand order.
Not attempted (out of time): 00b46c80 UpdateGlobalLightingStates, 00b47bc0 UpdateShadowedSpotLights, 00b484b0 FrameStart, 00a1ceb0 Initialise.

Levers that moved bytes:
- `bool inTheme = inChannel0 | (blend > 0 && mask & bit1);` -- an && that is an operand of `|` is materialised as int
  (`mov eax,1 / xor eax,eax`), the byte OR stays `or al,bl`. A named bool gives `mov al,1`. (Both GetLightsIn* rows.)
- C3DVector::GetSquaredMagnitude (X*X+Z*Z+Y*Y) reproduces retail's dz,dx,dy product order; explicit dx*dx+dy*dy+dz*dz does not.
- `link->Light->AlreadyFoundFlag = true` (through the link, before the re-link stores) places the flag store as retail.
- A local reference to the CArray lets VC keep _M_start across the loop; naming the member at each use re-reads it as retail.
- GetCenter as (MaxCoord + MinCoord) * 0.5f gives retail's Min.X+Max.X / Max.Y+Min.Y add order.
- Parallel q.py runs collide (OBJDUMP_ERR / identical outputs): grade serially.

Callee spellings used are the PDB ones; see NAME_FIXES.tsv for landed spellings that disagree (ClipLightCountAndBuildLightArray,
ResetAlreadyFoundFlags, CDynamicObjectHeap<12,16>::CreateMoreBlocks, the link heap global, SetGoal, GetNodeAt).
