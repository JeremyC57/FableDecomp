- start: siblings 00651b04 006096e0 0061e6d0 00601310 all RELOCATION_MATCH on hdr/ (C39 start)
- C39 g1-7: (b) frame +8 = pos: retail has temp C2DVector(0,0) for SetGuildSealLevelUpPos, 'C2DVector pos = GFAnchorToBottomLeft(ringsPos)' decl-init (RVO slot), loop-local 'C2DVector pos = ...' in both loops; implicit CCharString conversion for GetDefGlobalIndexFromName (lea, arg-first); 'alpha = (long)(GetGuildSealAlpha()*alpha)' before the top sprite -> this/def registers flipped to retail (esi/edi). 4698v4653.
- C39 g8-14: loop1 'button.Offset' used twice (no offset local) -> button computed before the graphic call (0x00653587); twin tip/anchored locals in the seal tooltip; CDefPointer null stores / dec [eax+4] form / slot sharing = GOTCHAS Q19: same-TU bodies FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY + FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY (+ new nested opt-in CABILITYDEF_STATGETTYPENAME). 4654v4653. (DIAG x1/x2: bare block / decl-first also drop the null store -- not needed.)
- C39 g15-30 (v18 = differ/00652fa4.cpp, DIFFER 4663v4653, nd 40 hunks): same-TU bodies GFAnchorToBottomLeft 0x00643CDA, DrawIcon 0x00645292,
  DrawEffectAtPos 0x0064546F (ported onto the header model, auto_inline off; twin c2a did the same with DrawButton) -> `pos +=` and loop-1 sums fold X-first.
  Twin idiom `C2DVector(C2DVector(def.DPadMiddleOffset + GFAnchorToBottomLeft(ringsPos)))` at the flash / d-pad sprites gives retail's two `fst` (0x006540C3, 0x00654169):
  FITTED (same review item as twin c2a README). Tried and rejected: NAMED_RESULT_ADD operator+ (fst at 9 sites, 4705), named `middle` local (GFAnchor call moves
  before the uv zero stores), C2DVector::ToC2DCoordI (new opt-in FABLE_RETAIL_C2DVECTOR_TOC2DCOORDI kept in hdr, NOT used: plain inline is called out of line
  under optimize("s") -- inline trace sz 37 sav 8, largest expanded c=23; forced inline gives Y-first), ringsPos as pointer / declared earlier / as right operand (no change).
  OPEN: (A) retail stores ringsPos (&def.DPadRingsPos) in [ebp+14h] at 0x00653108 and reloads it at 6 sites; ours rematerialises `lea [edi+174h]` (twin has the stored form
  with the same text!). (C) 0x00653F29: retail reads the GFAnchorToBottomLeft result of the main-seal effect through the returned pointer kept in [ebp+30h], X first, direct
  stores into the C2DCoordI temp; `const C2DVector& anchor = ...` reads the frame temp. DIAG x4 (static inline helper taking const C2DVector&, `c.X += (long)v.X`) has retail's
  shape except add-v-mov => an inlined helper with a reference parameter and X-then-Y member stores. (D) CRGBColour byte stores scheduled after the pushes at 0x006534BC.
- C39 g31-36 (v21): (C) solved: pos.ToC2DCoordI() at the five DrawEffectAtPos sites with FABLE_FORCEINLINE in the new opt-in (EVIDENCED: 0x00653F29 shape appears only with the expansion AND the same-TU DrawEffectAtPos body; plain inline is called). CRGBColour ctor through Initialise (FABLE_RETAIL_CRGBCOLOUR + _CTOR4_INITIALISE; FableWin /Od size 18i/44b) 40->37 hunks. Now 1524 v 1524 instructions, 4668v4653: the 15 bytes = five ringsPos rematerialisations (lea 6 v mov 3). Only (A) + x87 operand orders + CRGB store scheduling left.
- C39 g37 (v22, 4654v4653, 25 hunks): (A) solved. ringsPos is stored in [ebp+14h] because retail CANNOT rematerialise def+174h at its last use: at 0x00654133 def's register is advanced in place (add edi,1C8h = &def.DPadMiddleOffset, the operator+ ARGUMENT evaluated before the GFAnchor call) so def is dead at the ringsPos use -> root needs a memory home -> every use loads it. Spelling = the twin's operand order GFAnchorToBottomLeft(ringsPos) + def.DPadMiddleOffset (C38's draft had it reversed).
- C39 g38-52 (v25 = differ/00652fa4.cpp, 4654v4653, 1524 v 1524 instructions, 23 nd hunks). Residue is ONLY x87 commutative operand order + one store scheduling:
  measured facts: (1) the X component order of each sum is c2 record-id keyed: a no-byte ruler at the function top flips sites (units k=1: loop-1 X + d-pad X ok;
  k=2: tip X + loop-1 X + d-pad X ok; k=3: main-seal X + flash X + d-pad X ok; k=4 == k=0; half units h=1/3/5 similar) -- no single k fixes all; local rulers before the
  seal / loop-2 statements only moved d-pad X. (2) the Y component order is NOT ruler sensitive: ours is always `fld vector.Y ; fadd this.Y` when one operand is the
  GFAnchor call result (role swap fixes it: loop 1 = `const C2DVector& offset = button.Offset; C2DVector pos = offset + GFAnchorToBottomLeft(ringsPos);` with
  DrawChargeGraphic(info, button.Offset, i) keeping `button` two-use -> 0x006535C6 fixed, EVIDENCED by bytes) but role-independent (def member first) when one operand
  is ringsPos (tip ok, main seal 0x00653F18 not: retail has ringsPos first for X and Y there). (3) flash / d-pad: retail Y = fld anchor.Y ; fadd def.Y (0x006540A8,
  0x00654151) wants `def.DPadMiddleOffset + GFAnchor(ringsPos)` roles, but that order (also with a named `const C2DVector& middleOffset`) loses the def-dead-before-call
  fact and flips the this/def registers back (v27, 4669) -- unresolved conflict; retail forms &def.DPadMiddleOffset BEFORE the GFAnchor call (0x00654133) yet orders Y as if it were `this`.
  (4) loop 2 X (0x00653961): retail loads ringsPos.X before the strength-reduced button address (fld [ecx] ; add eax,[i*50h] ; add eax,48h ; fadd [eax]); ours forms the
  address with lea first -- the 1 byte. (5) 0x006534AE: retail stores the three 0xFF colour bytes after the pushes of the Draw2DText arguments (ours before); CRGBColour ctor
  through Initialise (FableWin size) did not move it. NAMED_RESULT_ADD retested with all bodies visible: still 4702 (fst at many sites) -> unnamed operator+ text stays (twin too).
  Next ideas (not tried): named references at the remaining sites in the twin's style (C2 notes item 4: `const C2DVector& offset` / buttonDef / effectPos), the loop-2
  `offset` reference dropped or `def.QuickAccessItemButtons[i].Offset` nested, `tip` computed through a named `const C2DVector& tooltipOffset`, a `font` reference before
  `tip` (twin order) for the 0x006534AE scheduling, then a ruler scan (tools/rul.py, period 4 units) for the X orders once every Y order is right.
- C39 g53-62: v29 (twin `font` reference before `tip`) loads the font early and keeps it (4660) -> font stays nested in the Draw2DText call. v30 (named levelUpOffset + ringsPos
  at the main seal) byte-identical to v25. v27 / v32 / v33 (named `const C2DVector& middleOffset = def.DPadMiddleOffset` + `middleOffset + GFAnchorToBottomLeft(ringsPos)`
  at the d-pad / flash sprites) give retail's Y order at the d-pad sprite (anchor first) BUT raise def's priority (680 v this 651; v25 has this 652 v def 647) -> this/def
  swap registers and ringsPos is rematerialised again (4669/4670). So the last structural step is: named middle-offset reference(s) + something that keeps prio(this) > prio(def)
  (+30 for this or -33 for def: c2prio via tools/tr1.sh <src> <tag> this, 30 s per trace). Not found in this session.
- HUDRingAlpha 0x01375744 (descriptive name): ONE TYPE = `long`. Retail reads it as a dword at 0x006532B7 (row 0x00652FA4) and as a byte for ScaleAlpha. The twin
  0x00651B04 with `extern long HUDRingAlpha;` and `ScaleAlpha((unsigned char)HUDRingAlpha)` at its four sites still grades RELOCATION_MATCH
  (probe/00651b04_long_HUDRingAlpha.cpp) -> respell the twin at landing.
- Row identity (two sources): vtable ??_7CDrawQuickAccessIcons@NPlayerGui@@6B@ 0x0125A704 slot 2 = 0x00652FA4 (xref .rdata 0x0125A70C; the PC subclass's Draw 0x00657580 calls it),
  Ego_r.pdb + FableWin.pdb ?Draw@CDrawQuickAccessIcons@NPlayerGui@@UAEXABVCEnginePrimitivePrepareInfo@@@Z. Length 4,653 confirmed (push ebp .. ret 4 at 0x006541CE).
- Siblings regraded on the final hdr/ (after the two header additions): 0x00651B04, 0x006096E0, 0x0061E6D0, 0x00601310 all RELOCATION_MATCH.
- Row 2 0x0058807D CTCInventory::OpenPCInventoryMenu: NOT OPENED (row 1 did not match; one row at a time).
