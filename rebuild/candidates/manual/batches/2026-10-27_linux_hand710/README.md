# Chunk 710 (NA053) -- 10 of 10 byte-exact (2 MATCH + 8 RELOCATION_MATCH), two opt-in retail headers (2026-11-02)

Rows (`verify.log`: WINS 10, all PASS; 18 probes; no asm, volatile, flags or pragmas -- an inherited `pack(1)` was stripped because the bytes
hold without it): the libpk pair CVtable_012b25a8::Unlock 0x00C0E5C0 / Lock 0x00C0E5E0 (MATCH; descriptive, no PDB; bracket the bank load in
libpk CSystem::LoadBank 0x00C08D40), CTCCreatureModeManager::ResetModes 0x0073B0D0, and seven destructors named from the ??_G in slot 0 of their
own vtables (FableWin ??1 names; Ego_r inlines implicit dtors): ~CCombatSequenceGeneric 0x006F1770, ~CTCHeroSpecialMovement 0x007E0B20,
NCreatureMode ~CModeJackOfBladesFloat 0x00740420 / ~CModeScreamerAggressive 0x00741390 / ~CModeScreamerSuperAttack 0x008AC4D0, ~CTCNoise 0x00749A70
and NCreatureMode::~CModeDragonHover 0x008E31A0. All ten catalog labels were wrong. Each dtor source defines retail's own PDB ctor so VC7.1
emits the implicit dtor out of line, as retail did.

Headers (hdr/README.md): opt-in FABLE_RETAIL_CMODESTANDBASE (engine/CModeStandBase.h: derives from CTCCreatureModeManager::CModeBase, virtual
dtor, Ego_r members, sizeof 0x68) and FABLE_RETAIL_CCOMBATSEQUENCEGENERIC (engine/CCombatSequenceGeneric.h: : CCombatSequenceBase, Def at
+0x28). Default text unchanged under `cpp`; the 24 landed includers and the two respelled rows rechecked 26/26 RELOCATION_MATCH + PASS.

Second spellings at landing: the landed rows 0x0073AF60 SetupDefaultMode and 0x0073AFC0 SetupInitialMode declared the functions public; both
PDBs say private (`AAE`), as chunk 710 binds them; respelled in this commit (access only), recheck RELOCATION_MATCH + PASS. Queued PRIORITY
(NAME_FIXES_QUEUE.tsv, c710): 0x008B0A60 (landed own row declares ~CCombatSequenceBase non-virtual), 0x0073E6A0 (STLport list clear vs
`CRetailList_CountedPointer`), 0x008E31A0 (the landed FireBeam ??_G's fastcall stand-in), 0x0073AE70 RemoveAllModes (landed as ClearModes;
caller 0x00664370 binds a CTCPrimitiveOwner stand-in). Decision on the libpk class at vtable 0x012B25A8: `CVtable_012b25a8` (the catalog's
vtable-address convention, as the ??_7 symbols are spelled); the landed CCombatAbilityStrafeDef_00c0ece0 / _00c0edf0 and other stand-ins are
in the agent's NAME_FIXES and get respelled to it in the naming pass. 0x0073AEA0 (~CTCCreatureModeManager, 93 bytes) is uncatalogued.
