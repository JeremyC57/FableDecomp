# Chunk Q5 notes (deep pass on Q4's four near misses)

Grade every source with `FABLE_HDR_OVERLAY=$A/hdr` (gg.sh does it). Matched: 0. src/ and test/ are empty, so finish.py writes an empty payload.

## Results (residue on line 2 of each differ/ file)
- 0x00497FD0 CProgressDisplay::DisplayCenteredText: DIFFER(1076v1076), ratio 0.928 -> 0.967. The noLines spill is fixed (if + do-while with a
  `lineNo` copy after the guard). Only the 4th draw's `shadow = position` copy schedule is left (3 lines).
- 0x007CCF50 CTCMultiStrike::ApplyEffectOnWeapon: DIFFER(1088v1088), unchanged (reload order edi/esi at 0x007CD314). c2prio shows the
  split pieces never move under code-neutral spellings.
- 0x008F61A0 CHeightMap::Resize: DIFFER(518v530), unchanged. The sizeX register piece and the sizeX+1 home ([esp+1Ch]) are still open.
- 0x0078C900 CTCBerserk::InitAbility: DIFFER(1132v1152), unchanged. The flag word wins ebx over the && bools (11 v 10/9/9).

## Identities and names
No new row identities. Names found in the PDBs (inline, no retail address, so not in LABELS.tsv): ?GetExtraCells@CHeightMap@@CIJXZ.
The CHeightMap.h GetIndexIntoCells default body (`sizeX * y + x`) is probably not retail's: the retail index is top down (see LESSONS).
The fix is behind the FABLE_RETAIL_CHEIGHTMAP_INDEXFLIP opt-in only. NAME_FIXES.tsv: none.

## Header overlay
hdr/engine/CHeightMap.h: two new draft-only opt-ins, FABLE_RETAIL_CHEIGHTMAP_GETEXTRACELLS and FABLE_RETAIL_CHEIGHTMAP_INDEXFLIP
(hdr/README.md). The default path is unchanged.

## Process note
My first gg.sh call used a relative path, and the old script logged one ERROR line to /home/user/FableDecomp/ATTEMPTS.log. The coordinator
fixed the script, and that file is gone. All later grades logged to $A/ATTEMPTS.log. Probe sources are in $A/v/ (c_* = 007CCF50,
h_* = 008F61A0, d_* = 00497FD0, b_* = 0078C900, plus *.prio c2prio traces).
