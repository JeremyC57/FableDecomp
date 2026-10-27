# Hand chunk 501 (NScript::CQ_TraderEscortScript .. NScript::CV_RockTrollFirstEncounterScript): 40 of 40 byte-exact

15 MATCH, 25 RELOCATION_MATCH, 0 retail_asm, 0 DIFFER, 0 SKIP. Every row has a behaviour test that passes under g++ and
under the lane's VC7.1. The read-only vland run reports 40 wins. It has no RELOC_DUPLICATE and no RELOC_IDENTITY.
Its only warnings are SHARED_HEADER_AVAILABLE (see Header debt) and two RELOC_NAME warnings about old catalog labels
(see Label corrections).

Every row was a `_gapscan::sub_<addr>` placeholder; 0x00E63560 was `_unattributed::ClearFlags_00e63560`.

## What matched (40)
- Entity scripts (21 rows: Init, Main, OnPersist, OnPredicateFail). Each is a slot of an RTTI-less entity vtable that a
  landed Alloc stores. The slots are 0 dtor, 1 Main, 2 Init, 3 GetParentScript, 4 OnPersist, 5 OnPredicateFail,
  6 OnInterrupted. The agent499 models matched on the first grade: a polymorphic CScriptThing, a padded
  CGameScriptInterface, Game at +4, Me at +8, ParentClass at +0x14 and the parent's members at Ego_r - 0x14.
  - A by-value SetIsPushableByHero(Me) needs CScriptThing's user copy constructor. Retail stores both vtables.
  - In push_back, the copy is the implicit one and stores only 0x01238C8C.
  - 0x00EAE5A0: CScriptThing::operator== must be `PImp->IsEqualTo(&*other.PImp)`, through CCountedPointer's inline
    operator-> and operator*. Reading `.Data` directly swaps eax/edx and keeps &Me in ebp.
  - 0x00E38D80 OnPersist uses the two-argument Transfer(name, value). It passes CPersistContext::GetDefaultVal<T>():
    the CCharString instance is out of line at 0x0040E1E0 and the enum default is inline.
  - 0x00EA80F0 calls std::map<CCharString,bool>::operator[], declared only.
  - 0x00EAE5A0 and 0x00EAE820 use std::vector<CScriptThing> with erase and _M_insert_overflow. push_back is inline.
- Level scripts (14 rows: Init, OnPersist, WatchForTermination). Slot 3 or slot 4 of the level vtable; members sit at
  Ego_r - 0x14.
  - Pure flag resets matched on the first grade. Float zeros are integer stores. A bool[5] clear loop compiles to one
    dword store plus one byte store.
  - CTimer::operator=(long) is inline through g_pGameScriptInterface slot 89.
  - pGlobalScriptDef members sit at Ego_r offset minus 4 per Dinkumware vector before them: TollBanditFee +0x430,
    the BooksNumber* fields +0x4e0, HeroKillsSheriff* +0x708 and +0x71c.
  - WatchForTermination is the landed WoodsMelee shape.
- CConversation def (2 rows). Copy is `*this = *(const CConversation*)other`, which inlines the implicit operator=.
  Transfer is Transfer<long>("Lines") followed by four TransferVector<CCharString>. Both use the CFireHeartPatternDef
  model from agent499.
- Predicates (3 rows).
  - CIsVariableFalse::operator() is `!*Variable`.
  - CIsVariableFalse::Clone is `new CIsVariableFalse(*Variable)`. The bool& constructor reads Variable before the
    vtable store; the implicit-copy form stores the vtable first.
  - CPredicate_And<CIsThingAlive,CIsVariableFalse>::Clone is `new CPredicate_And(*this)`, an implicit copy that
    stores the vtables in order. It is emitted by explicitly instantiating Clone alone.

## Parked for Fable 5.1 (0)
None.

## Skipped
None. No row packs several functions.

## Names

### Proven
Entity rows: the vtable slot, the landed Alloc store site and the Ego_r.pdb method name. Level rows: the vtable slot
and the constructor store. Rows whose constructor is not landed add Ego_r.pdb layout uniqueness and the strings of the
level's Main:
- 0x00E57C00 BeggarAndChild: "LookoutPointBeggar" and "BeggarBully"
- 0x00E9C3B0 IntroductionToTrophies: "BriarRose" and "TrophiesIntroWatcher"
- 0x00EA8720 MurderTwist: the MurderTwistBrother* names
- 0x00E90780 GuildMaster: "GuildMasterGameFlow", plus the CCharString the landed constructor builds at +0x50

Other proven rows:
- 0x00E9FEF0: the spawned function of the landed KnotholeGladeGates Main, plus Ego_r.pdb.
- CConversation: vtable 0x012E5FF0 stored by the landed copy constructor, plus Ego_r.pdb Copy and Transfer.
- Predicates: vtables 0x012EA430 and 0x012EA43C stored by the landed CPredicate_And constructor 0x00E7E020, plus
  Ego_r.pdb.
- 0x00EA50E0: vtable 0x012EE034, whose slot 2 is the landed CLostTraderBrother::Init 0x00EA50A0. The layout
  (bool at +0x20) is the Brother's, not the Trader's.

00e04b50
00e10a90
00e25750
00e25d20
00e25d30
00e26bd0
00e38d80
00e3af30
00e3b230
00e478d0
00e49900
00e4a940
00e50890
00e54630
00e54d10
00e579e0
00e57c00
00e628b0
00e63560
00e76410
00e7ddd0
00e7dde0
00e7e060
00e8b880
00e90780
00e93d80
00e986d0
00e9c3b0
00e9fef0
00ea0030
00ea50e0
00ea80f0
00ea8720
00eae5a0
00eae820
00eaee10
00ebeec0
00ebfbb0
00ec1810
00ec3c50

### Probable
None.

### Descriptive
None. Three enum values have no name: ESheriffState 4, BriarRoseOpinion 0 and EscortTraderManagerPhase 0. The Ego_r
enum types are declared without enumerators.

## Label corrections
- All 40 rows: `_gapscan::sub_<addr>`, and `_unattributed::ClearFlags_00e63560`, become the names above.
- Landed 0x00E90750 `CVtable_012EC138::CVtable_012EC138` is NScript::CV_GuildMasterScript::CV_GuildMasterScript.
  Landed 0x00E92900 `Alloc_CVtable_012ec138` is CV_GuildMasterScript's Alloc.
- Landed 0x00F336F0 `CVtable_012e5ff0::CVtable_012e5ff0` is CConversation::CConversation(), the default constructor
  that stores vtable 0x012E5FF0.
- Manifest 0x00EA86F0 `CEngineSubPrimitiveBase::CEngineSubPrimitiveBase` is the NScript::CV_MurderTwistScript
  constructor. 0x00EAE890 `_global::Alloc` is its Alloc.
- Manifest 0x00E57BB0 and 0x00E9C360 `CV_ArcheryCompetitionScript::CV_ArcheryCompetitionScript` are the
  CV_BeggarAndChildScript and CV_IntroductionToTrophiesScript constructors. 0x00E5D0B0 and 0x00E9FB40 are their Allocs.
- Landed 0x0040E1E0, the free `GetDefaultVal<CCharString>`, is
  `??$GetDefaultVal@VCCharString@@@CPersistContext@@KI?AVCCharString@@XZ`, a protected static __fastcall member
  (Ego_r.pdb). Its siblings are GetDefaultVal<long> and GetDefaultVal<bool>.
- Catalog 0x00E257C0 `CPersistContext::Transfer<EOrchardFarmTraderEscortPhase>`, manifest `Transfer<signed_char>`, is
  Transfer<NScript::CQR_EscortTrader_ManagerScript::EscortTraderManagerPhase>(name, value, default). The enum name was
  invented.
- Manifest 0x00E38DD0 `Transfer<signed_char>` is Transfer<NScript::CV_AssassinAttacksScript::AA_LocationIdentifier>
  (name, value, default).
- 0x0049B8D0, labelled `CPersistContext::Transfer<CCharString>`, is TransferVector<CCharString>. Agent499 found the
  same; vland RELOC_NAME warns about it.
- 0x008AD5F0, labelled `std::vector_InsertRange`, is std::vector<CScriptThing>::_M_insert_overflow (protected).
- The 0x00CD3152 binding `?erase@CScriptThingVector@@...` uses an invented stand-in. The real symbol is
  std::vector<CScriptThing>::erase.
- 0x00EA4FF0 is CV_LostTraderScript::CLostTraderBrother's Alloc, and nothing labels it: the catalog row for the landed
  CLostTraderTrader::MakeRandomComment ends at 0x00EA4FF0, and own.py attributes the vtable store at 0x00EA5024 to
  MakeRandomComment only because it is the nearest earlier catalog entry.

## Header debt
- engine/CGameScriptInterface.h and engine/CScriptThing.h flatten the vptr into a data member. The virtual calls,
  by-value copies and inline copies need polymorphic local models. vland warns SHARED_HEADER_AVAILABLE: 17 rows for
  CGameScriptInterface and 3 for CScriptThing. These are the same models as agent499's.
- engine/CDefClassBase.h has no virtual interface, so CConversation Copy and Transfer use a local polymorphic model.
- The level and entity bases (CScriptBase at 0x40 in retail, CEntityScriptBase) are local models, as in agent499.

## Callee names
- Every callee uses an Ego_r.pdb or bound spelling: CCharString(const char*, long), ~CCharString,
  CEntityScriptBase::IsActiveThreadTerminating() const 0x00F35B30, CScriptBase::IsActiveThreadTerminating()
  0x00CB7940, CPersistContext::Transfer<T> and TransferVector<T>, CDefClassBase::operator=, CScriptThing(const
  CScriptThing&) 0x004ABE90, the operator new import 0x00BFEA1A, g_pGameScriptInterface 0x0143E8F8 and
  pGlobalScriptDef 0x0143E90C.
- STLport members use the real template spelling, with the allocator argument and namespace std:
  - vector<CCharString>::operator= 0x00432EE9
  - vector<CScriptThing>::erase 0x00CD3152
  - vector<CScriptThing>::_M_insert_overflow (IAE, `std::__false_type`) 0x008AD5F0
  - map<CCharString,bool>::operator[] 0x008ADF10
- Interface slot names come from ghidra_out/cgsi_slots_aligned.tsv.
