# NF69 header overlay (`hdr/engine/`)

Each file is a copy of the current repo header (`rebuild/include/engine/<name>.h`, re-diffed at hand-back: every repo line is
still present, `diff | grep '^<'` is empty) with `#ifdef` / `#ifndef` opt-ins added and nothing else changed. Without the opt-in
macros each overlay preprocesses to the same tokens as the repo header (checked with `cpp -P` on a one-line TU for the default and
the retail branches; the only difference is the `__LINE__`-numbered FABLE_STATIC_ASSERT array names, which move with the added lines).

| Header | Opt-in macro | What it adds | Rows |
|---|---|---|---|
| NHitLocations_CHitLocation.h | `FABLE_RETAIL_CHITLOCATION_TREGISTEREDHITLOCATIONCODELETMAP` | inside the FABLE_RETAIL_HITLOCATIONS_BOUND branch, after the private static `RegisteredHitLocationCodelets`: Ego_d's nested typedef `TRegisteredHitLocationCodeletMap` (EGOD_FINDINGS section 1) | 0x008162F0, 0x00815E80 |
| CCombatManager.h | `FABLE_RETAIL_CCOMBATMANAGER_REGISTERCOMBATSEQUENCES` | declarations of the private `RegisterCombatSequences()` (Ego_r + FableWin `AAEXXZ`, 0x008B0D10) and the landed binding `RegisterCombatSequence(creator, CCharString)` 0x006EAB60 | 0x008B0D10 |
| CCombatSequenceGeneric.h | `FABLE_RETAIL_CCOMBATSEQUENCEGENERIC_MODULES` | in the FABLE_RETAIL_CCOMBATSEQUENCEGENERIC branch: `virtual bool IsValid() const` (public) and protected `virtual void OnStart()` / `OnStop()` (Ego_r + FableWin spellings) | 0x008B07E0, 0x008B0830, 0x008B0870 |
| CTCTargetingBase.h | `FABLE_RETAIL_CTCTARGETINGBASE_IMPLICIT_DTOR` (`#ifndef`) | in the typed FABLE_RETAIL_UPDATE_NEAREST_INTERESTING branch: leaves out the PDB-generated `virtual ~CTCTargetingBase();` so the destructor is VC7.1's implicit one (retail 0x004DA8B7 stores no vtable; same symbol `??1CTCTargetingBase@@UAE@XZ`) | 0x004DA8B7, 0x004E37BF, 0x004E0080, 0x004E1023, 0x004E37A3, 0x004E0064 (0x007F5D00 does not define it) |
| CTCTargetingAI.h | `FABLE_RETAIL_CTCTARGETINGAI_IMPLICIT_DTOR` (`#ifndef`) | the same for `virtual ~CTCTargetingAI();` in the FABLE_RETAIL_CTCTARGETINGAI_ATTACK branch (retail 0x004E0080 stores no vtable) | 0x004E0080, 0x004E0064 |
| NGameText_CDataBank.h | `FABLE_RETAIL_NGAMETEXT_GETPGROUPENTRY` (nested in FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT; CGroupEntry part also needs FABLE_RETAIL_NGAMETEXT_GROUPENTRY) | (1) CDataBankGroupEntry with Ego_r's `CIVCountedPointer<CGroupEntry> PGroup` (+0) and the private const BeginLoading 0x009C80F0 / BuildGroup 0x009C8700 (friend CDataBank); (2) `CGroupEntry : public CResource` (EGOD_FINDINGS section 5) with UnloadResource, Entries +0x1C, PFileData +0x28, Owner +0x30, in place of the CIVCountedPointeeBase-based default; (3) `CDataBank : public CBankFileAsync, public CResourceBank` (RTTI, CResourceBank at +0x164) in place of the 0x164..0x1E0 padding, plus `BeginLoadingGroup(unsigned long) const` 0x009C8220 | 0x009C8890 |

The `#ifndef` form of the two IMPLICIT_DTOR opt-ins removes a declaration only when the macro is defined; the default text is untouched.
The NGameText_CDataBank.h opt-in replaces the CGroupEntry / CDataBank class heads under `#ifdef ... #else <original lines> #endif`.
None of these is a default-path change; the coordinator can merge them as opt-ins. NF67 / NF68 may add opt-ins to other copies of
the same headers: merge by macro.
