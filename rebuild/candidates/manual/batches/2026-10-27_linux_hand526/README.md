# Chunk 526 (agent526, 2026-10-28): 12 of 20 re-queued single functions landed (176-373 bytes)

Re-queue chunk of 20 rows of 176-373 bytes (0x008C6CE0..0x009A01C0): creature actions (troll, hobbe, nymph, fire missile
weapon, get in bed), AI state groups (handyman, seats, shops), CDataInputStream::Read3DAngles and _HyperlinkProc. One
Opus 5.5 agent at high effort.

- **Landed:** 12 rows with `vland.py --land`, all RELOCATION_MATCH, every test PASS under g++ and VC7.1.
- **Held (1, pending/2026-10-27_linux_hand526/held/):** 0x00946F90 WanderAroundShops::FindASimplyDelightfulShop matches,
  but binds 0x00436AC0 as `vector<const CThingBuilding*>::_M_insert_overflow` (through its callee 0x00947780, proven by
  Ego_r donor order as the CThingBuilding GetAllLeafThings). Landed rows bind 0x00436AC0 as the exact STLport
  `vector<CThing*>::_M_insert_overflow` (0x0048D400, 0x006A22D0) and the CThingBuilding overflow to 0x004AC610 (landed
  0x004AD780, identity "probable"). Which instantiation owns 0x00436AC0 vs 0x004AC610 is a naming-pass decision.
- **Parked (6, differ/):** frame-slot / register residues (0x008C9A40, 0x00924520, 0x00947110, 0x009A01C0) and the
  GetObjectWithScriptName twins 0x00929FC0 / 0x00947EB0 (VC7.1 inlines `~list` on the found path; retail calls
  `_List_base::~_List_base` out of line).
- **Skipped (1):** 0x0099C7E0 STLport `basic_string<wchar_t>::append<const wchar_t*>`: the catalog row (334 bytes) runs
  past the 306-byte function and its int3 padding into 0x0099C920 (not a catalog start). differ/0099c7e0.cpp matches the
  true 306-byte extent; it lands once the boundary is fixed and a test is written.

Name fixes (NAME_FIXES.tsv, 29 rows, deferred): 0x007FEF40 CTCUserActionPointBase::SetActionPointAsFree (not
FreeActionPoint), 0x008FCFD0 CAIStateGroupBase::GetPWorldSeed (IBE; 16 bound spellings), 0x009950F0 Read3DVector on
CDataInputStream, FireMissileWeapon ctor and GetSubDef<CTrollBattleDef> spellings. 14 catalog labels corrected (NOTES.md).
