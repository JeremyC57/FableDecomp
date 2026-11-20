# NF127 header overlay (2026-11-20)

Every overlay header is a copy of the repo working-tree header (base copy in ../orig/, byte-equal to the repo at start) with
`#ifdef` opt-ins only. The default path (no opt-in macro defined) is token-identical to the repo header.

## engine/CAIStateGroup_PrepareMeal.h
- Opt-in `FABLE_RETAIL_CAISTATEGROUP_PREPAREMEAL_GETFREECOOKINGPOT` (inside the retail `FABLE_RETAIL_CAISTATEGROUP_PREPAREMEAL` class, private
  section): declares `CThing* GetFreeCookingPot() const;` (Ego_r.pdb 0x0096A3A0 + FableWin.pdb 0x02BB3570, retail 0x00946380). Declaration
  only; no layout change. Used by rows/00946380.cpp (q5032).
- pin_asserts.py: pinned 1 (the retail-branch `FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_PrepareMeal)==0x44)` became
  `FABLE_STATIC_ASSERT_AT(26, ...)`, its repo line); the flat-branch `_AT(44..51)` asserts unchanged.
- Landed includers: 0x008FDBA2, 0x00946190, 0x00946400, 0x0121F86F. None defines the new macro, so the opt-in changes none of them.

## engine/CThingSearchTools.h
- Nested opt-in `FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY_CASTARG` inside the existing `FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY` GetAllLeafThings
  body: the thing held as `CThing*` and cast to `T*` at the filter and as the push_back argument (homed just before the push_back, as
  retail 0x00910AE2). Without the macro the LEAFBODY body is token-identical to the repo. Used by rows/00910a90.cpp (q4992).
- pin_asserts.py: pinned 0.
- Landed includers: 335 rows include CThingSearchTools.h; 5 define LEAFBODY (0x006001E0, 0x00603000, 0x00600B60, 0x009104D0, 0x0050A650);
  none defines the new macro, so the opt-in changes none of them.

## engine/CAIStateGroupBase.h
- Nested opt-in `FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED` inside the existing `FABLE_RETAIL_CAISTATEGROUPBASE_PEEK` block: a `protected:`
  label before PeekWorld / PeekThingSearchTools and a `public:` label after them (Ego_r + FableWin spell both IBE; the block otherwise keeps
  the access of the label before it, public after FINDAGUARD_YELL). Access labels only; without the macro the block is token-identical.
  Used by rows/0097fea0.cpp (q5140).
- pin_asserts.py: pinned 0 (the header's asserts are already FABLE_STATIC_ASSERT_AT).
- Landed includers: 434 rows include CAIStateGroupBase.h; none defines the new macro, so the opt-in changes none of them.
- Sibling overlays: NF123 / NF126 carry no CAIStateGroupBase.h, CThingSearchTools.h or CAIStateGroup_PrepareMeal.h.
