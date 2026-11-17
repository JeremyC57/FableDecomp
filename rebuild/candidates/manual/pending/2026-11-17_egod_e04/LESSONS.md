- (e04, 0x00871FD0) A one-swap scheduling residue (a field load v the reload of a frame-parked result after a call) that five passes
  of row spellings did not move was the shared inline helper's text: parenthesising the helper's whole return expression
  (`return ((a*w+b)/(w+1.0f));`) closed it. Run fwsize, and for a DIFF helper try paren variants of its return as well as its size.
- (e04, 0x0057AEB4 / 0x005AD305) Def-handle register residues ("def kept in a callee-saved and released with dec [reg+4]" v frame
  copies / reloads / a three-way register cycle) in size-TU rows closed by porting onto the shared headers with the EXISTING visible
  GetDef / GetSubDef body opt-ins (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, _CDEFCLASSBASE_GETSUBDEF_BODY); start a row
  from a landed SAME-TU sibling's opt-in / include set (BuyItem 0x005AF64E for the trade TU). Caveat: GETDEF_GLOBAL_BODY graded at
  0x0042AF3C (GetDef<CThingBaseDef>) is DIFFER(101v98) -- the shared text is not retail's for that instance (visibility is the lever).
- (e04, 0x0057AEB4) Ego_d's `long item` loop index compared with vector::size() (signed/unsigned mix) is retail's `sar ; je` + `jb`;
  a `(long)size()` cast gives `test ecx,0FFFFFFF8h ; jle`.
- (e04, 0x00BA9830) A "callee-saved push not sunk past a join" residue parked as a lane gap was source shape: the frame getter is the
  FableWin bool GetTexture(const CTexture**, mip, flags) (existing GETTEXTURE_INLINE opt-in) with Ego_d's `texture` local, then
  `return frame_info->GetBlankTexture()`; matched at the first grade on the shared headers (the local-model version fails strict headers).
- (e04) VC10 Ego_d frame offsets: the first-declared local gets the highest bprel (closest to ebp); /GS moves char arrays up.
- (e04, 0x004B2890) A rename residue around a std::find over CCharString closed only when BOTH __find (FIND_NOINLINE_BODY) and CBasicString::Compare (COMPARE_BODY) were visible: the find is then known not to write memory, end() stays in a register (no named `last`, as Ego_d says) and the neighbouring argument registers follow. __find alone did nothing.
- (e04, 0x00954F40) A "frame slot order only" park on a local model closed on the shared headers in 4 grades: Ego_d's declaration order
  (`CThing* pMarkerThing;` first, `= 0` right before its use), the implicit char* -> CCharString conversion into a by-value filter
  (retail re-addresses the temporary: `lea edx,[esp+38h] ; push edx` for the copy ctor; an explicit CCharString(...) passes eax), and
  FABLE_RETAIL_C3DVECTOR_PACKED for a C3DVector member at an odd offset (+0x22). Check the draft's callee identities first: its
  "HasDefGlobalIndex" filter was really CThingFilter_HasScriptName (landed binding of the search instance).
- (e04, 0x00F3ABF0) Bisect a near-miss draft against an Ego_d-faithful port one region at a time: only one region (a strength-reduced
  loop) flipped the whole-function `this` register; the other Ego_d rewrites were neutral.
