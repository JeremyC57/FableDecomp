# Hand chunk 876 (_global .. _global): 0 of 1 byte-exact

## What matched (0)
- None. The only row, 0x00CBFB7D RunCutsceneMacro (74,778 bytes), is written in full (every verb of the four if/else
  chains, retail order) and parked at DIFFER(74879v74778).

## Parked for Fable 5.1 (1)
- 0x00CBFB7D _global::RunCutsceneMacro DIFFER(74879v74778): first diff ins 1 (`mov eax,18E4h` v `18DCh` frame); strict
  95.3%, register/frame-masked 97.8%, call sequence 99.0% (4026 of 4068 targets). Inline decisions: retail expands
  GetLength at 0x00CC0040 / 0x00CC03EA and iterator `==` at 0x00CC61B8 / 0x00CC6DA9 (ours calls), and calls `==` 0x00CC8407,
  vector<CCharString>::operator[] 0x0040F110 x7 (SetLightScene), vector ctors 0x00414DE0 / 0x00CD2D28 / 0x008AC5A0 (ours
  expands). Then register roles (chains 1-3: retail -1 in edi / "HERO" in esi, ours swapped; chain 4: retail edi=0 and
  `push -1` immediates, ours edi=-1) and frame-slot order (apparently use-weight sorted round the biased ebp) | tried: 34 graded
  versions (ATTEMPTS.log): hoisted call arguments, if/else-call spellings of bool/float ternaries, block-local iterators
  assigned from find, `if (GetDef) {...}` body, `/d2inlS5/S7/S8/S9`, `/d2inlT5/T40`, header opt-ins listed in
  hdr/README.md; loop spellings for the top-tested CrowdKill wait loop (mini t24/t26/t27) | next: lane gap: inline budget
  (retail's IL weight lies between /d2inlS8 and the default S9 on our headers; needs header IL parity with the original
  STLport 4.6 / CCharString inlines), then chain-4 constant register, CrowdKill wait loop 0x00CCF7BE, SlideTeleport
  `fld st(0)` 0x00CC5947 / 0x00CC5C02, cross-jump survivor 0x00CC8231.

## Skipped
- None.

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
