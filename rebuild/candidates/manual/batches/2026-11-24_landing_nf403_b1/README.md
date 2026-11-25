# Landing nf403 b1: 0x004DF4E7 is the copy constructor of CObstacleNavigatorExternalInterface

The self is respelt from the made-up `??0CObstacleNavigatorExternalInterface@@QAE@ABUCObstacleOwner@@@Z` to `??0CObstacleNavigatorExternalInterface@@QAE@ABV0@@Z`. Sources:
- FableWin.pdb tc_register.obj;
- its only caller, Clone 0x004DF4CA (`new X(*this)`), whose landed row already binds this spelling.

The table needed no change. The row's grade is RELOCATION_MATCH PASS, and its behaviour test was rewritten.

Model: the 0x004DF365 class.
- Members: Obstacle +4, a real STLport `vector<const CSteeringAgent*>` +8, sizeof 0x14, on the shared engine/CNavigatorExternalInterface.h base.
- Destructor form: 0x004DF507's.
- 0x004DF507's own model could not express the copy ctor: its stand-in `m_4` measured DIFFER.

New header opt-in, add-only and off by default: `FABLE_RETAIL_CNAVIGATOREXTERNALINTERFACE_INLINE_DTOR` (retail's base dtor is inline).
- With it, the object's two `??_G` copies equal retail 0x004DF507 (43 B) and 0x004DF532 (29 B).
- `??1CNavigatorExternalInterface` has a unique tier-2 home at off-catalog 0x006CB510, with no references.
- equiv_check over the 68 rows that reach the header shows 0 differ.

NF403 gates:
- vland HEAD/new: 4e7, Clone 4ca, 507 and 365 all grade the same.
- pk_all: no new NEQ, 0 reach flips.
- Census: no other definer.
- /FORCE link: duplicates 0 -> 0.

Coordinator checks:
- landed 1/1, 0 LITERAL_DIFF;
- self_symbols respelt, column 4 aligned;
- T3 nf400 0x004DF4E7 closed;
- baselines OK.

Follow-ups are in T3 nf403:
- converge 004df365 and 004df507 on this class text;
- port Clone;
- a row for 0x006CB510;
- the base `??_G` copies;
- the 0x004DF532 comment.
