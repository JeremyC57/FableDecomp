# Hand chunk 336 (CThingSearchTools::GetAllLeafThings .. CTCEntityEvents::GetEvent): 37 of 40 byte-exact

All 37 grade RELOCATION_MATCH with no pragma or flag; tests pass under g++ and the lane's VC7.1 (tall.sh / tall.py).
The 28 thing-search rows come from one model (`gen.py` + `rows.py`), the 10 entity-event rows from another
(`gen2.py`); the tests are generated from the same template bodies (`gentest.py`, `gentest2.py`).

## What matched (37)

- **CThingSearchTools::GetAllThings / GetAllLeafThings (28 rows)**: walk the thing manager's all-things list
  (PeekAllThings 0x0049C770) or a per-type list (PeekTypeList 0x0049C780), keep each scan-visible thing
  (IsIncludedInThingScans inline: state byte +0x91) of the template's type (`(ThingTypeFlags & mask) == mask`)
  that passes the filter, and append it to the result container. Shape lessons:
  - the filter takes the thing by **reference** (`filter(*(TThing*)thing)`), which `std::const_mem_fun_ref_t`
    (0x00617B20) proves; CFilter_Bind2/3/5 hold their filters by reference, CFilter_Not holds one by reference;
    CFilter_Bind5's operator() needs `__forceinline` (VC7.1 otherwise calls it out of line, 0x006DFA50);
  - STLport `list<T*>` push_back = `insert(end(), x)` with `end()->_M_prev` read once into a local (else VC
    re-reads it after the first store);
  - STLport `vector<T*>` push_back with the overflow out of line: a same-TU `__declspec(noinline)` +
    `auto_inline(off)` stand-in (`#pragma auto_inline(off)` alone does not stop a TEMPLATE member from being
    inlined) lets VC7.1 home the value and the `__true_type` tag in the dead argument slot; for `T == CThing` the
    pushed value must be a by-value temporary (`ThingCast<T>(thing)`) or VC stores the address-taken local at
    the top of the loop;
  - `list<CIntelligentPointer<T> >` push_back is out of line (0x004BA4E0 / 0x006604EB); the pointer is a named
    temporary (base ctor, derived vtable store, SetPItem, push_back, base dtor), which gives the `and esp,-8` frame;
  - inline filters: CThingFilter_None, CThingFilter_HasBuildingAsHomeOrWork (work building +0x170 first, then home
    +0x168, both GetPItem 0x00A01B50), and a bool-selected component-bit filter (0x0091E660) whose
    `if (flag) { if (bits & 0x10) return true; } else if (bits & 8) return true; return false;` form gives retail's
    branch layout (a ternary of two `!= 0` tests produces `shr/and`).
- **CQuestCard::CQuestCard (0x004AEDF0, 0x004AEE30)**: the two constructors (def name, script name, and either the
  replayable flag with FeatIndex -1 or the feat index with Replayable false); default-constructed CCharStrings then
  operator= in the body, scalar stores last.
- **CTCEntityEvents::GetEvent (7 rows: 0x008D47A0, 0x008D4D90, 0x006C7F30, 0x00915760, 0x00922440, 0x00982610,
  0x0094E480)**: walk Events (+0x0c, STLport list of CCountedPointer<CEventBase>) newest first with a
  reverse_iterator, copy each counted pointer, return the first event whose GetType (vtable slot 1) is TEvent's
  and that passes the filter (`*out` stored before the copy is released). Shape: a user copy ctor + a releasing
  dtor on the counted pointer (DeleteFunc through the info block, operator delete 0x00BFE9BC), rend() re-read
  through `this` each iteration.

## Parked for Fable 5.1 (3)
- 0095d920 CTCEntityEvents::GetEvent<CEventFiredShot, CIsEventCreatedAfter> DIFFER(137v137): register swap only -- retail keeps the reverse iterator in edi and the event in ebx (ours ebx/edi) and pushes edi after `mov ebp,ecx` | tried: shared GetEvent template (matches 7 siblings), typed local inside the type test, function-scope typed local, direct-init copy + reinterpret_cast accessor | next: lane gap: register-allocation tie with a one-load filter (try a QFE-4035 build of the same source)
- 0096bc20 CTCEntityEvents::GetEvent<CEventHitBySpecialAbility, CIsEventCreatedAfter> DIFFER(137v137): same ebx/edi swap and prologue order as 0x0095D920 (same template, event type 0x1B) | tried: as 0x0095D920 | next: whatever fixes 0x0095D920 fixes this one
- 0094e530 CTCEntityEvents::GetEvent<CEventHitBy, CFilter_Bind2<CIsEventCreatedAfter, CIsHitEventHitByExplosion> > DIFFER(159v152): retail keeps `this` in ebp (no `push ecx` spill) and re-reads the filter from [esp+18h] for both members; ours keeps the filter in ebp and spills `this` | tried: reference Bind2 members, pointer Bind2 members | next: lane gap: register-allocation tie (the out-of-line second filter 0x00810F60 takes the event on the stack); try another Bind2 operator() spelling or the QFE toolset

## Skipped
None. All 40 row bounds are right (the catalog lengths include trailing int3 padding; bodies end at the `ret`).

## Label corrections
- 0x004AEDF0 / 0x004AEE30 "GetDefaultVal<std::list<COpinionTransientOffset> >" are the CQuestCard constructors
  (Ego_r.pdb CQuestCard {DefName, ScriptName, Replayable, FeatIndex}).
- The GetAllThings / GetAllLeafThings / GetEvent labels are propagated across byte-similar bodies; the instance
  arguments were read from each body (type mask, type-list index, filter calls, push path, event type):
  - containers: 0x00617860 / 0x00617970 / 0x004ACD70 / 0x004BAA50 / 0x00617CA0 / 0x00618E60 / 0x004ACF90 /
    0x008AC5D0 / 0x00617B20 / 0x00617BB0 push into an STLport **list** (malloc'd 12-byte node), not a vector;
    0x007EA040 / 0x007EA0E0 push into a list of CIntelligentPointer<CThing>;
  - filters: 0x005198B0 is HasTCOfLeafType<CTCActionUseScriptedHook> (0x00518DC0), not Bind2<InMap,
    unary_negate<InArea> >; 0x005ED860 / 0x0062E840 call CThingFilter_HasScriptName 0x006A7E30 (manifest
    "HasScriptData"; landed siblings say HasScriptName); 0x006C1DD0 is Bind2<HasTC, InMap>; 0x00518850 is
    Bind3<IsInQuestOrNullQuest 0x006A7C60, HasTC, InArea 0x006A7990>; 0x008266E0 is Bind2<BoundingBoxOverlapsMap,
    HasNavigationInformation 0x0050EEF0 (manifest "CScriptThing::IsAlive")>; 0x0052B270 / 0x0066E5A0 / 0x006D4600
    use CThingFilter_None (no call); 0x0052B300 is Bind2<None, HasTC>; 0x008E3BB0 is CThingFilter_IsNot;
    0x0091E660 is an inline component-bit filter (descriptive name CThingFilter_HasEitherTC);
  - thing types: 0x008AC5D0 mask 0x87 (CThingDeadCreature), 0x0066E5A0 mask 0x47, 0x0062E840 / 0x00799FB0 mask
    0x8007 (CThingMarker), 0x008266E0 mask 3 (CThingPhysical); 0x00617860 / 0x00617970 read type list 11 (the
    marker list of the landed GetPFirstLeafThing siblings), 0x006D4600 type list 4 (kept the label's
    CThingBuilding; the landed 0x005C0D9D puts CThingBuilding at list 3, so one of the two names is wrong);
  - GetEvent<CEventChildHurtGoToMother, CIsEventOfType> is the label of four different events: 0x006C7F30 type
    0x15 CEventHeroHasBeenSpottedStealing, 0x00915760 0x18 CEventChildHurtGoToMother, 0x00922440 3
    CEventExpressionPerformedTo, 0x00982610 0x19 CEventVillagerInteraction; 0x0096BC20 (labelled CEventFiredShot)
    is event 0x1B CEventHitBySpecialAbility (event ids from each RTTI vtable's slot-1 GetType).
- Callee labels: 0x0049C770 "CVertexBufferWin32::DoSizeof" is CThingSearchTools::PeekAllThings (landed siblings
  already say so); 0x006A80A0 "Bitset_Test" is CThingFilter_HasTC::operator(); 0x00810FC0 / 0x00810F00 / 0x00810F60
  ("_Cons_val<...>", "CTCShotDefinitionBase::CanBeHitByMeleeStrike") are CEventHitBase attacker / knocked-down
  accessors and CIsHitEventHitByExplosion::operator().
