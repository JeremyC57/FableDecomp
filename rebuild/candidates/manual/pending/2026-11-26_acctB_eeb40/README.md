# 0x008EEB40 Mass::ForceToVel

Landed 2026-11-26 on FullDecomp-B under VC7.1 RTM cl 13.10.3077. Grade MATCH, behaviour test `008eeb40_TEST PASS`. Retail extent 146 bytes.

PDB spelling, both tables: `?ForceToVel@Mass@@QAE?AVC2DVector@@V2@0@Z` (Ego_r 0x00914270, FableWin 0x02AE9B40). The manifest already uses Mass::ForceToVel, so this landing does not queue a NAME_FIXES row.

The body is `force * IMass + Right(pos - Pos) * Right(pos - Pos).Dot(force) * AIMass`, with `Dot` parenthesised (`FABLE_RETAIL_C2DVECTOR_DOT_GROUP`). The unmatched length estimate was 160. The 2026-10-30 draft stays in place.
