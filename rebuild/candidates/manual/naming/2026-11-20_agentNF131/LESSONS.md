# NF131 lessons (one line each)
- Replacing a row-local CAIStateGroupBase model with the retail header (FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS) and re-basing the derived model
  on CAIStateGroup_WorkBase<CAIStateGroupBase> matched first time on all four IsInWorkHours binders: the shared headers carry no layout surprises.
- A class-template member self row can be the header's inline body plus `template R C<B>::f() const;` (pick-any COMDAT; check LINK_ONCE_DISCARD in objdump -h).
- A stand-in that "works" with the wrong arity (GetBlockData(id, size) + ctor(data)) is a stack-argument coincidence: the size pushed first is
  the real ctor's second argument; rewrite both together.
- A retail-size class can move the frame: the 0x18-byte stand-in stream gave `sub esp,18h` against retail's 1Ch; size the class as the PDB does.
- When the retail CAIStateGroupBase.h comes in, a row-local CCharString clashes with fable_string.h: drop it for FABLE_RETAIL_CCHARSTRING.
- Copy the define / include block of a landed sibling that already binds the PDB spellings (0x00915350 for 0x00914F00): one missing opt-in
  (CTHINGCREATUREBASE_ANIMATION) was the only fix needed.
- Before a respell that makes a callee const, check whether the call goes through other same-TU stand-in members: the const cascades into
  symbols of other addresses (the PeekThingSearchTools pointer rows).
