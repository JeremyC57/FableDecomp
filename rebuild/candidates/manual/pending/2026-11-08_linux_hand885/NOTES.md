# Hand chunk 876 (_global .. _global): 0 of 1 byte-exact

## What matched (0)
- None. The only row, 0x00CBFB7D RunCutsceneMacro (74,778 bytes), is written in full (every verb of the four if/else
  chains, retail order) and parked at DIFFER(74965v74778).

## Parked for Fable 5.1 (1)
- 0x00CBFB7D _global::RunCutsceneMacro DIFFER(74965v74778): first diff ins 1 (`mov eax,18E4h` v `18DCh` frame); strict 96.4%,
  register/frame-masked 98.7%, call sequence 99.2%. Every inline decision now agrees with retail (session 4: `// Flags: /d2inlS8`
  plus per-site devices -- `#pragma inline_depth(0)` regions at the 11 sites retail calls, `__forceinline` twins
  GetLengthExpanded / StartsWithExpanded / IsEnd at the 4 sites retail expands); what is left is register allocation (the split
  constant pieces 0 / -1 / 1 / "HERO" and the prologue roles: retail name->ebx, -1->esi, def->ebx), the frame packing and the
  cross-jump tails that follow from it | tried: 41 graded versions (ATTEMPTS.log), session 4: per-site inline devices,
  header-weight GetLength / IsEmpty spellings, same-TU helper bodies (IsTrue / IsFalse / IsNull / IsCutsceneSkipped / CSFindThing /
  ReadLightScene / CSExtractParameterList kept; PlayCutsceneSound, optimize("y", off) on them: no effect), the local fixes listed
  in WORKLOG.md | next: register roles region by region with c2prio.py --trace (prologue first), then the StartsWith("$") bool
  0x00CC0418, SlideTeleport x87 0x00CC5947 / 0x00CC5C02, CrowdKill loop 0x00CCF7BE.

## Skipped
- None.

## Behaviour fix found in session 4
- `.Sheathe`: retail's last arm sheathes the weapons unless the mode is "none" (`setne` at 0x00CC3A34, the body on the not-equal
  path); the parked v34 sheathed only when the mode WAS "none". v41 has `else if (!mode.EqualsCaseInsensitive("none"))`.

## Label corrections
- No landed spelling proven wrong (NAME_FIXES.tsv is empty).
- The probe annotations (probe/ann.txt, manifest labels) name several callees by an ICF-folded sibling: 0x0041C820
  "CPixelFormat::IsEquivalent" is the folded body of `std::_Rb_tree_base_iterator::operator==` here (reloc symbol
  ??8_Rb_tree_base_iterator@std@@QBE_NABU01@@Z), 0x00CD3187 / 0x00515700 "CRegion::SetMiniMapRegionExitTextOffsetX" are the
  map<CCharString, CScriptGameResourceObjectScriptedThingBase> / map<CCharString, long> find bodies, 0x00414DE0 / 0x00CD2D28 /
  0x008AC5A0 "CIndexBuffer::CIndexBuffer" are vector ctors at these call sites, 0x0040F110
  "CSmallVector<CANavQuadTreeNode_const*,8>::operator[]" is a pointer-vector operator[] body (4-byte stride: the vector of
  CCharString). One address serves several template instantiations after /OPT:ICF, so these are not relabels; they only
  matter when reading the listing.
