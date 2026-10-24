# Hand chunk 355 (_Find_if<C3DVector*,std .. _Insert<std::_List_const_iterator<std::_List_val<CClass*,std): 36 of 40 byte-exact

Grades: 9 MATCH + 27 RELOCATION_MATCH (gall.sh), 4 parked. All 36 tests pass under g++ (tall.sh) and VC7.1 (tall.py).
Every algorithm TU is /Gr, so each template carries an explicit `__fastcall`: a non-POD functor/iterator result goes
through the hidden pointer in ecx, a pointer `first` goes in edx, and class iterators and functors go on the stack.
Row extents: none of the 40 rows looked mis-cut. Each row's length is the padding-stripped code plus trailing
`int3`/`nop`, which the grader strips. The header byte counts in src/ are the stripped extents.

## What matched (36)
- **std::for_each over vector/array ranges (15).** The loop is pointer-first in edx, and the functor is returned through the
  hidden pointer.
  - 0043f310 / 0084ad60 `mem_fun_t<void,T>`: the bound member is pushed as an immediate, 0x43E420 (a vcall thunk) /
    0x7449E0. 0084ad60 is byte-identical to 0043f310.
  - 0043f360 `mem_fun_ref_t` over the 8-byte CEnginePrimitiveHandle (`Clear` 0x41BE70).
  - 00528720 `binder2nd<mem_fun1_t<void,CThing,bool> >` (SetInLimbo).
  - 004ad890 / 00726660 / 006fce00 / 0085b840: small functors with an out-of-line operator().
  - 008065c0: a 7-dword functor. Retail's copy-out is scheduled register moves, which needs a memberwise user copy ctor
    over the long members. An implicit copy gives `rep movsd`.
  - 006f3530: an 8-dword functor; its implicit copy is the retail `rep movsd`.
  - 00695b20 `CDispatchActionEventIfActivationFrame`: `if (m_frame > ev->m_activationFrame) { ev->Dispatch(c); ev->m_dispatched = true; }`.
  - 00738f30 `CGetMaxComboStage`: an out-of-line GetComboDef into a local; the stage compare is in source order.
  - 006fb640 `CGetEventsFilteredByTime`: CSmallVector `push_back = resize(m_size+1); back() = v;`. This matches only with a
    same-TU `__declspec(noinline)` resize that only reads its fill; with it, the never-written `T()` default temporary is
    homed over the live functor slot.
  - 008e2f10 `CGetBestIciclePointForTarget`: C3DVector needs a user copy ctor so the best point stays in memory and is
    not register-promoted. The functor copy stays implicit.
  - 0084f280: an empty POD functor returned in al. `bool operator()(CCountedPointer<CShape>)` is by value, with the
    inline counted-pointer copy and dtor.
- **std::for_each over Dinkumware lists (8).** The node is {next, prev, value@+8}; the iterator is a one-pointer class with a
  user copy ctor.
  - 007100a0 `CMultiplyAnimSetAlphaBy`: clamps to [0,1] with an inline `SetAlpha` that keeps the previous alpha.
  - 0071b590 / 0071b690: `CKillThing` / `CSetThingInLimbo`. A named `CThing* target = p.GetPItem(); target->F(arg)` puts the
    object load before the arg push.
  - 0071b700 `CSetThingAsDrawable`: a direct virtual slot-28 call.
  - 0085a960 / 0085a9c0 `CGetActiveStrikeEventThatIsFirstToFinish`: an inline `IsActiveOnFrame` member on the pointee
    gives retail's edx/esi assignment. 0085a9c0 is byte-identical to 0085a960.
  - 0091b890 `mem_fun_t` over `list<CTCPerceiveThing*>`.
  - 0065f5d2 `CNearestBuildingWithProperty`: a /Os TU with an out-of-line copy ctor and operator().
- **find_if / __find_if (4).**
  - 00811e20 `CIsEventOfTypeAfterFrame`: the cmp operand order follows the source, so `m_frame < ev->m_frame`.
  - 0082c720 `CIsMarkerBoundToThing`: uses engine/CMarkerInfo.h, with PThing read through a CBaseIntelligentPointer view.
  - 0065f96d `CIsHornTaskWithThisHorn` and 0065f9ab `HasGuardProperty`: STLport `__find_if(.., const input_iterator_tag&)`.
    The tag ref is the edx register argument. They are /Os TUs where VC picks the ebp frame itself, and the by-value
    predicate argument is built in its slot by the out-of-line copy ctor.
- **count_if (2).** 008e8a90 (plain) and 008e8b80 (`unary_negate`) over CIntelligentPointer<CThing>, with the out-of-line
  filter 0x8E8A10.
- **list::_Insert (2).** 0053caa0 / 005794c3 are a member template `for (; first != last; ++first) insert(where, *first);`
  with a by-value `input_iterator_tag` 4th parameter, in a /Os TU. The two are byte-identical except for the insert callee.
- **Functor operator()s on the CThing interface model (5).** 0090c060 CAddFreeBenchToList, 0092e540 CIsFreeBed,
  00946520 CIsFreeCookingPot, 00946c60 CIsCandleNeedingChanging and 0094b9b0 CIsBenchWithOnePerson. Three things made them match:
  - HasTC returns the masked word, not bool, which gives `test ch,20h` / `test dword [..],imm`.
  - Each body is a single nested if or `&&` chain with one trailing `return false`; early returns cause esi
    shrink-wrap and `neg/sbb/inc`.
  - Helpers are `__forceinline`.

## Parked for Fable 5.1 (4)
- 008ab710 std::remove_copy_if DIFFER(102v102): x87 operand order only, retail `fld st(1) ; fmul st(2)` twice (dx squared first) vs ours `fld st(0) ; fmul st(1) ; fld st(2) ; fmul st(3)` (dy first) | tried: dx*dx+dy*dy, dy*dy+dx*dx, a C3DVector operator- temporary, `r*r < lenSq` | next: compute the dy difference before dx, or a C2DVector difference with a LengthSquared member; else lane gap: x87 stack scheduling
- 00754d80 std::for_each DIFFER(88v92): retail keeps CGetBestTurningAnim::m_bestAngle on the x87 stack across the loop (`fld [f+4]` before, `fstp st(0) ; fld [edx]` on a hit, `fstp [f+4]` after, dead `fxch st(1)` pair); ours stores it with integer moves on each hit | tried: plain member stores, inline by-value SetBest(entry, angle), memberwise user copy ctor, FP-typed `(float)fabs(..)` store | next: operator() taking the pair by value; else lane gap: x87 register promotion of a functor member
- 00938db0 CAddFreeSeatToList::operator() DIFFER(163v159): retail keeps `seat` in esi with its home in the `push ecx` slot [esp+8] (one store at the GetPItem) and the LowerBound key in the dead `thing` slot; ours homes `seat` in the dead `thing` slot, reloads it per use, and puts the key in the push-ecx slot | tried: inner-scope seat, function-scope seat after early returns, inline then __forceinline Consider(CThing* seat) | next: a by-value copy local handed to push_back, or a same-TU noinline push_back stand-in that only reads its const ref
- 009438f0 CAddOccupiedSeatToList::operator() DIFFER(163v159): same slot swap as 00938db0 (seat in the dead `thing` slot instead of the push-ecx slot, key slot swapped, seat reloaded per use) | tried: inner-scope seat, function-scope seat after early returns, inline then __forceinline Consider(CThing* seat) | next: as 00938db0 (by-value local for push_back, or a same-TU read-only push_back stand-in)

## Skipped
None.

## Label corrections
Most of the catalog `_For_each`/`_Find_if` labels in this chunk are propagated across byte-identical or look-alike bodies.
- 0043f310 / 0084ad60 / 0043f360 / 004ad890 / 00726660 are not `_For_each<pair<long,CCharString>, CAddDefNamesToComboBox>`.
  - 0043f310 / 0084ad60 are mem_fun_t for_each instantiations.
  - 0043f360 is a mem_fun_ref_t for_each over CEnginePrimitiveHandle.
  - 004ad890 / 00726660 are 4-byte functor for_each instantiations.
- 00528720 is `for_each<CThing**, binder2nd<mem_fun1_t<void,CThing,bool> > >`, not CGetBestTargetInDirection. The same
  catalog name also sits on 006fce00 / 008065c0 / 0085b840, which are three different functors.
- 008ab710 is STLport `remove_copy_if<C3DVector*, unary_negate<CVectorFilter_FurtherFromPointThan> >` (its caller
  0x8AC060 is remove_if), not `_Find_if`.
- 008e8a90 / 008e8b80 are `count_if` (plain and not1), not `_Find_if`.
- 0084f280 is a `for_each` returning an empty functor in al, not `_Find_if<.., CIsShapeTrack>`.
- 0091b890 is `for_each` with mem_fun_t over `list<CTCPerceiveThing*>`, not `_Find_if<.., CIsIntelligentPointerToThing>`.
- 0065f96d is `__find_if` with CIsHornTaskWithThisHorn (op 0x65F116) over `list<CCountedPointer<CVillageTaskHorn> >`, not
  HasGuardProperty. Only 0065f9ab is HasGuardProperty.
  - Their find_if front ends 0x65FFCA / 0x66000A carry the wrong manifest labels
    `CGuiControlTree::SelectNextEntryByName` / `CPersistTraits<C2DVector>::TransferIn`.
- Five rows catalogued as `_Find_if`/`_For_each` instantiations are functor operator()s: 0092e540 CIsFreeBed,
  00946c60 CIsCandleNeedingChanging, 0094b9b0 CIsBenchWithOnePerson, 0090c060 CAddFreeBenchToList and
  00946520 CIsFreeCookingPot.
  - 00946520 is a cooking-pot predicate: its caller is CAIStateGroup_SitForMeal::GetPCookingPot via find_if 0x9465E0.
    It is not CAddFreeBenchToList (that name belongs to 0090c060).
  - The parked 00938db0 / 009438f0 are also operator()s, of CAddFreeSeatToList / CAddOccupiedSeatToList.
- Manifest callee labels:
  - 0x43E420 `CThingFilter_IsValid::operator()` is a slot-0 vcall thunk (`mov eax,[ecx] ; jmp [eax]`).
  - 0x8E8A10 `CAIStateGroup_VersusScorpionKing::FindChargeEvent` is a thing filter operator().
  - 0x6FCC80 `ParticleMorphs_BuildEntry` is the 006fce00 functor's operator().
  - 0x4AD3D0 `Physics_CheckNavigability` is the 004ad890 functor's operator()(CThing*).
  - 0x738980 `GetTCInterface_LowerBound` is CAnimationEntry::GetComboDef(const CComboDef**).
  - 0x6A8A30 `CThingCreatureBase::IsBlocking` is a filter operator()(CThing*) on interface 0xAC.
  - 0x85B720 `CDefendingCombatantInfo_SetupHelperRings` and 0x6F2C00 `CAttackerPositionEvaluator::EvaluateAttacker` are
    functor operator()s called by for_each.
- Names that are descriptive, not recovered: 006f3530 / 008065c0 element and functor names, 00946520 CIsFreeCookingPot,
  and 0092e540's embedded CThingFilter_BedOccupant.
