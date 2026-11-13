# 0x00651B04 NPlayerGui::CDrawAbilityButtons::Draw -- author bwf1 (session 1)
- grades ~40: best DIFFER(5120v5126) 115 hunks (normalised structural mismatches 42). Moved it: whole draft on shared headers;
  CEnginePrimitiveHandle::Clear / CIVCountedPointer Reset+Release forceinline opt-ins (retail expands Clear at 0x00651B64);
  vector clear() for both handle vectors; GFAnchorToBottomRight real body visible (auto_inline off) (85 -> 44 mismatches);
  `if (!(!IsAble ? ab==4 && HasTC : !PeekTC || IsAvail))` for the uncastable test; operand order GFAnchor()+Offset at special/switch/middle;
  tooltip `C2DVector anchored = GFAnchor(tip); GFACoordToVCoord(anchored, text)`.
- did NOT move: declaration order of normal/favourite/menu/melee (slots unchanged), goto layouts for the rings (worse),
  C2DVector named-result operator+ (copy pattern everywhere: worse), forceinline C2DVector ctor (no change).
- grades ~75: best DIFFER(5126v5126) 26 hunks (normalised mismatches 42), differ/00651b04.cpp. Moved it: `normal` written as
  `if (NTS || STN || !Show) normal = true; else { menu...; if (menu->IsActive()) normal = true; }` then `if (normal) rings else favourite`
  (the favourite flag removed): this alone fixed EVERY frame-slot residue (pos/normal and the [58]/[5C] swap; c2 packer trace with
  agent2003 pk_host.py + agent3016 sim2.py showed density ties); special-loop DrawButton position `C2DVector buttonPos = anchor + Offset;
  DrawButton(info, C2DVector(buttonPos), ...)` (size now equal).
- Left: x87 sums at uncastable / special / switch / middle-ring sites: retail computes X then stores, then Y (direct into the DrawButton
  argument temporary) where ours computes Y first (unnamed operator+) or X-first plus a register copy (named / C2DVector(x) copy);
  toy probes (work/toy/t*.cpp) show only `p = anchor + off;` (assignment) gives X-first direct, and it adds an integer copy.
  Middle ring: retail X-first + fld st(1) copy with the uv zero stores BEFORE the GFAnchor call (sum inside the ctor statement).
  Loop-1: `cmp [eax+ecx+3Ch]` scheduled before the copy stores in retail.
## Hand-back (session 1, ~124 grades): DIFFER(5126v5126) 26 hunks, source differ/00651b04.cpp (normalised structural mismatches 42)
- Headers: hdr/README.md (14 opt-ins). Flags: none, `#pragma optimize("s", on)` only. Made-up name: HUDRingAlpha (0x01375744 byte).
- Residue (all x87 / scheduling): (1) uncastable, special and switch DrawButton positions: retail computes X, stores it into the
  argument temporary, then Y; every C++ spelling tried gives Y-first direct (unnamed operator+) or X-first plus a register copy
  (named result, C2DVector(x) copy; current best uses the copy at the special site only). Toys in work/toy/ (tc.py compiles a toy
  with the lane flags): with a GFAnchorToBottomRight temporary as one operand VC7.1 always computes Y first; only `p = a + b;`
  (assignment) or `+=` in place give X first, and both add a copy. (2) middle ring sprite: retail zeroes the uv temporaries BEFORE the
  GFAnchorToBottomRight call and then copies an X-first sum with `fld st(1)` -- named middlePos + C2DVector(middlePos) gives the copy
  but after the call (35 hunks), the in-argument sum gives Y-first direct. (3) loop 1: `cmp [eax+ecx+3Ch]` scheduled before the drawPos
  copy stores. (4) string push 0x0125A6EC is a relocation (fine).
- Tried without effect: declaration order of normal/pos/menu/melee; goto layout for the rings; named-result / member-store / copy-then-+=
  operator+ texts (probe opt-ins, removed); C2DVector forceinline ctor; button references in the loops; IsNull()/Get() spellings;
  operand order flips one site at a time (ids move other sites); reusing `pos` for the DrawButton positions (worse).
- Best next idea: trace the x87 scheduling of one site with the c2 probe (tools/decomp_pipeline/linux_lane/lane_tools/c2probe, the
  agent2003 pk_host.py pattern worked here in ~1 min: work/pk/) to see which IL order retail's X-first needs; or find a landed row of
  the same TU with a `GFAnchorToBottomRight(..) + x` argument to a pushed call and copy its spelling.
- NAME_FIXES (from the dossier, checked against FableWin/Ego_r name lists): 0x004375E0 is ?GetAbilityButtonScale@CPlayerGui@@QBEMXZ
  (landed as CMap_DrawGetSeaLevel), 0x00625930 ?IsActive@CTCQuickAccessMenu@@QBE_NXZ, 0x00A03B30 ??1CInputEvent@@QAE@XZ,
  0x0072C430 ?IsAbilityAvailable@CTCSpecialAbilities@@QBE_NW4EHeroAbility@@@Z (manifest GetAbilityAvailability), 0x00446720
  ?GetActionName@CGamePlayerInterface@@SI_NABVCActionInputControl@@AAVCWideString@@_N@Z.
