# Parked-row retry sweep, wave 2 section 4 (2026-10-30): 33 of 354 parked rows landed, 108 improved

Section 4 = retail 0x0088BF30-0x00988E80 (engine range). The first agent was killed by a container restart at 20:51 with its
files intact; a resumed agent finished the 59 rows with no STATUS line (51 worked: 5 matched, 12 improved, 34 held; 8 NOT
attempted: the parked skeletons 0x008BC1A0 / 0x008C6DB0 / 0x008D5AA0 / 0x008DCC80 / 0x008DE7A0 / 0x008DE080 / 0x008E8060 and
0x008DD0F0, 1083-1784 retail bytes each, still need a first full draft; section 4b). Landed 33, all PASS (31
RELOCATION_MATCH + 2 MATCH), all plain C++. New this run: 0x008DB030 (true extent 166), 0x0090F770 (inline `GetCreature()
const`), 0x008EEA80 (perpendicular on the `(point - Pos)` temporary), 0x00935AD0 (dead test via `CThing::IsDead() const`),
0x0092AFE0 (the global 0x013BB6B4 as a function-local static; the temporary's dtor resolves to `??1CCombatActionBase@@QAE@XZ`
0x0062E900). Improved sources and the header-moved sources are in `../../pending/2026-10-30_parked_retry2_sec4/{differ,hdr}`
(two differ copies are classified retail_asm; 0x00986210 and 0x00988E80 are retail_asm candidates).
Levers (matched / improved): `CThing::IsDead() const` (Ego_r `?IsDead@CThing@@QBE_NXZ`) in a nested-if IS_THING_ALIVE 1 / 2
(isdead.py applies it mechanically); function-local static instead of an extern global 1 / 0; inline const accessor 1 / 0;
perpendicular on a temporary 1 / 1; true-extent grading 1 / 0; each 0 / 1: FramesSince helper, `?true:false`, named packed-record
local, named argument-vector local, const-reference ctor parameter, flag fall-through, object-first via named local or
`operator->`, compare/read order, offset vector. Held 34 + 171 earlier with residues in STATUS.tsv: this/member colour swaps,
frame slots, zero preset kept in a memory slot, loop rotation/alignment, x87/CSE, scheduling, list sentinel register.
NAME_FIXES.tsv: 16 rows + 3 review rows (the 0x013BB6B4 static; the IS_THING_ALIVE<CIntelligentPointer<const CThing>> and copy
ctor duplicate bindings 0x006961F0/0x0069CE20 and 0x0091F990/0x004A9730; 0x0097C580 SpawnBabies vs UpdatePhase2).
Review notes: 0x00935AD0 carries a same-TU noinline stand-in `GetPBestLeafThing` (extra function in the object; only the leaf is
graded); 0x008F9320's LSocket family lacks the LHNetworkLib namespace and uses local CNetErrorString / CCharVector stand-ins;
0x0094D020's CBuildingPointer should be CIntelligentPointer<CThingBuilding>; some respelled sources keep the U struct/class
key; 0x008DB030 uses a local CModeStandBase although a header exists; header moves were not neutral on 0x0096DBC0 /
0x009392A0 / 0x00951D60 / 0x009735D0 / 0x008B0B60; 0x008A7A70 needs /Oa. SHARED_HEADER_AVAILABLE for CTCPhysicsBase,
CThingGameObject, CCreatureActionBase, CAIBrain, CAIStateGroupBase.
Coordinator: vland --land 33/33 (two RELOC_CONFLICT warnings on 0x00979F70, the duplicate-copy family), multi-check 0.
