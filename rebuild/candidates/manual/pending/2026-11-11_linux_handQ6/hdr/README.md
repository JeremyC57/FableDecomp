# agentQ6 header overlay (export FABLE_HDR_OVERLAY=$A/hdr for every grade)

- engine/CV_HauntedHouseScript.h: new opt-in `FABLE_RETAIL_CM_UNDEADHERE_MAIN` (row 0x00E95770) adds the Ego_r.pdb
  CM_UndeadHere members (ParentClass +0x14, PMasterData +0x18, Counter +0x1c) and the private `virtual void Main()` override.
  Default path byte-identical (the block is only inside the #ifdef).
- stlport/fable_deque.h: new opt-in `FABLE_STLPORT_DEQUE_PUSH_BACK` (row 0x00912840) adds the STLport 4.6 members
  `_Deque_iterator_base::_M_decrement`, `_Deque_iterator::operator--`, `deque::back()`, `deque::push_back()`,
  `deque::_M_reserve_map_at_back()` (inline, library bodies) and the declarations of the out-of-line
  `deque::_M_push_back_aux_v` / `_M_reallocate_map`. Default path byte-identical.
- stlport/fable_deque.h: also `FABLE_STLPORT_DEQUE_PUSH_BACK_AUX_NOINLINE` (row 0x00912840): the STLport 4.6 _deque.c
  `_M_push_back_aux_v` body under auto_inline(off) (visible, kept out of line as retail 0x00913D60). Default path unchanged.
- stlport/fable_containers.h (full copy of the current repo header, additions only):
  - `FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT_NOINLINE` (row 0x00A66FD0): list `insert(iterator)` = `insert(pos, _Tp())` and
    `push_back()` with bodies, FABLE_STLP_NOINLINE (retail 0x009E4A20 / 0x009E4AA0). Independent of the declaration-only
    FABLE_RETAIL_STLP_LIST_INSERT_DEFAULT (do not define both).
  - `FABLE_RETAIL_STLP_LIST_ERASE_NOINLINE` (row 0x00A66FD0): FABLE_STLP_NOINLINE on list `erase(iterator)` (retail 0x009E47E0;
    the erase itself needs FABLE_RETAIL_STLP_LIST).
  - a separate `#if` block defines FABLE_STLP_NOINLINE / FABLE_STLP_FASTCALL for those two (the existing `#if` line is untouched).
  - `FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE` (differ row 0x00542780): `__declspec(noinline)` on the inline vector push_back
    (retail calls push_back<CEmbeddedIcon> 0x00543C90 out of line; its _M_insert_overflow body is already visible by default).
- engine/CInputEvent.h: `FABLE_RETAIL_CINPUTEVENT_MOUSE_SETTERS` (row 0x00A66FD0): declarations of GetMousePos 0x00A04380 and
  SetAs{Left,Right,Middle}MouseButtonHeld 0x00A03D10 / 0x00A03DC0 / 0x00A03E70, SetAsExtraMouseButtonHeld 0x00A03F20 (Ego_r.pdb
  spellings). No layout change.
- engine/CThingCreatureBase.h: `FABLE_RETAIL_CTHINGCREATUREBASE_HEIGHTZBELOW` (row 0x00912840): declaration of
  ?GetHeightZBelowAt@CThingCreatureBase@@QBEMABVC3DVector@@_N1@Z (0x00666D40). No layout change.
- engine/NUISystem_CIconText.h (differ row 0x00542780): `FABLE_RETAIL_CICONTEXT_EMBEDDEDICON_CTOR` (the PDB default ctor, inline:
  Offset 0,0 then Size 40,40) and `FABLE_RETAIL_CICONTEXT_EMBEDDEDICON_ARGS_CTOR` (an inline (graphic, charIndex, flags) ctor --
  no PDB symbol; the form that matches). Layout unchanged (the header already has retail's Icons +0x18c).
- NEW engine/CBezierCurve.h (row 0x00912840): Ego_r.pdb CBezierCurve, Point1..Point4, 0x30.
- NEW engine/CAIStateGroup_FlyingThingBase.h (row 0x00912840): hand-written retail class on CAIStateGroupBase's retail form, Ego_r
  members +0x14..+0x20 and the protected Ego_r method spellings (GetRandomFlightPos const with C3DVector& out, etc.).
