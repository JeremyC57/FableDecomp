# Hand chunk 701 (CTCSummonableCreature .. NUISystem::CDynamicList): 10 of 10 byte-exact

All ten rows RELOCATION_MATCH under q.py and under the read-only vland check (WINS 10, behaviour PASS; jump tables: none).
Tests PASS under g++ (tall.sh) and VC7.1 (tall.py). Call targets checked with $S/w22/calls.py: every relocation of every leaf
goes to the retail target (imports: free 0x00BFEA14, operator delete 0x00BFE9BC). 1-3 probes per row; no flags, pragmas,
retail_asm or volatile. Seven of the ten catalog labels were wrong.

## What matched (10)
- CTCSummonableCreature::SetSummoner(CThingCreatureBase*) 0x0079BC80 -- `Summoner = summoner;` through CIntelligentPointer(T*)
  + operator= inline (chunk 700 setter shape), pack(1) on the pointer classes (Summoner at +0x11).
- CManagedTextureData::ReleaseTexture() 0x00A69A80 -- Allocated / Owner cleared, CCountedPointer::ReleaseData on VolumeTexture,
  boost::scoped_ptr<IDirect3DVolumeTexture9>::reset() (swap + bare delete), tail SetAsLRU.
- CBankFileAsync::Close() 0x009D58A0 -- `if (!ThreadedFile.IsNull() && !RetailMode) ThreadedFile->Close(); CBankFile::Close();`
- CArenaCreatureDef::~CArenaCreatureDef() 0x00F25E40 -- implicit def dtor: two out-of-line ~CCharString, the inline
  CDefClassBase / CDefPointeeBase chain (engine/CDefClassBase.h retail opt-in), jmp ~CResource.
- CCreatureAction_PerformExpression::~ 0x00718D50, CCreatureAction_PerformExpressionExtended::~ 0x0084ADC0,
  CCreatureAction_SendMessage::~ 0x009822E0 -- implicit action dtors: CIntelligentPointer member (out-of-line
  ~CBaseIntelligentPointer), engine/CDefPointer.h inline release, jmp ~CCreatureActionBase.
- CShaderDataBank::~CShaderDataBank() 0x00A620D0 -- user dtor `{ CloseShaders(); }`, implicit ~CShaderBankStateBlock (only the
  CBankStateBlock store), STLport vector free, jmp ~CBankFile.
- CHeightMap::~CHeightMap() 0x008F6170 -- user dtor `{ WorldSeed = 0; }`, STLport vector<CHeightMapCell> free, jmp
  ~CBaseClassNonCopyable.
- NUISystem::CDynamicList::~CDynamicList() 0x00541350 -- user empty dtor (three vtable stores), STLport vector<CComponent*> free,
  jmp ~CChangingStateComponent.

## Parked for Fable 5.1 (0)

## Skipped
None. Every extent checked against the padding: all ten end at their `jmp`/`ret` with int3 padding after; no extent fixes.

## Names and their evidence (all proven from two or more sources; none descriptive)
- 0x0079BC80 ?SetSummoner@CTCSummonableCreature@@QAEXPAVCThingCreatureBase@@@Z -- FableWin.pdb name; Ego_r Summoner
  (CIntelligentPointer<CThingCreatureBase>) at +0x11, the only Ego_r class with that pointer type there, built at retail +0x11 by
  the landed ctor 0x0079C240; temp vtable 0x0123E7D8 = .?AV?$CIntelligentPointer@VCThingCreatureBase@@@@ (COL type descriptor);
  CTCSummonSpell::CreateMinion 0x0079ACC0 inlines the identical body at 0x0079B095.
- 0x00A69A80 ?ReleaseTexture@CManagedTextureData@@QAEXXZ -- Ego_r + FableWin; Ego_r function order (CreateVolumeTexture,
  ReleaseTexture) mirrored at 0x00A699D0 / 0x00A69A80; caller CManagedTexture::Release 0x00A69AD0; members match Ego_r.
- 0x009D58A0 ?Close@CBankFileAsync@@UAEXXZ -- slot 5 of CBankFileAsync 0x0129B8D4 (also inherited by CWADBank / CStaticMapBankFile)
  where CBankFile's slot 5 is 0x009CE360; Ego_r spelling; ThreadedFile at the CBankFile boundary +0x110 (Ego_r first member).
- 0x00F25E40 ??1CArenaCreatureDef@@UAE@XZ -- callers ??_G 0x00F25E20 (slot 0 of vtable 0x012FB358, whose slot 20 returns
  sizeof 0x38) and the CArenaCreatureDef vector helpers; Ego_r layout (CreatureType +0x28, HUDType +0x30, 56 bytes).
- 0x00718D50 ??1CCreatureAction_PerformExpression@@UAE@XZ -- ??_G 0x0084B600 (slot 0 of .?AVCCreatureAction_PerformExpression@@
  0x012743DC) and the temporaries of ctor 0x008430B0; FableWin name; Ego_r layout.
- 0x0084ADC0 ??1CCreatureAction_PerformExpressionExtended@@UAE@XZ -- temporaries of ctor 0x00843310 (vtable 0x012744FC);
  ??_G 0x0084B620 inlines it; Ego_r + FableWin name.
- 0x009822E0 ??1CCreatureAction_SendMessage@@UAE@XZ -- follows ctor 0x00982210 (vtable 0x0129947C); ??_G 0x00982550 inlines the
  same body; FableWin name; Ego_r layout (PDef +0xB4, PTarget +0xC4 -> retail +0xA8 / +0xB8, unique in Ego_r).
- 0x00A620D0 ??1CShaderDataBank@@UAE@XZ -- stores .?AVCShaderDataBank@@ 0x0129D644; callers walk stride-0x128 CShaderDataBank
  arrays (CEngineResourceManager dtor); FableWin name.
- 0x008F6170 ??1CHeightMap@@UAE@XZ -- stores .?AVCHeightMap@@; ??_G 0x008F6DE0 inlines it; Ego_r order AlterHeightBy / ??1 /
  ctor mirrored at 0x008F60C0 / here / 0x008F63C0.
- 0x00541350 ??1CDynamicList@NUISystem@@UAE@XZ -- stores the three CDynamicList vtables; ??_G 0x00541870 inlines it; Ego_r name
  in the CDynamicList block retail keeps here.

## Callee names (all match the bound spelling or the PDB; none invented)
??1CCharString@@QAE@XZ 0x0099EAE0, ??1CResource@@UAE@XZ 0x009FC550, ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10 and its
ctor / GetPItem / SetPItem (0x00A01B10 / B50 / B90, chunk 700 spellings), ??1CCreatureActionBase@@UAE@XZ 0x00693EF0,
?CloseShaders@CShaderDataBank@@AAEXXZ 0x00A62050 (Ego_r, private), ??1CBankFile@@UAE@XZ 0x009CE180, ??1CBaseClassNonCopyable@@MAE@XZ
0x0099A300, ??1CChangingStateComponent@NUISystem@@UAE@XZ 0x0052CCF0, ?SetAsLRU@CResource@@QAEXXZ 0x009FC450 (Ego_r; order
RemoveFromResourceList / SetAsLRU / SetAsMRU mirrored at 0x009FC450 / 0x009FC4A0), ?Close@CThreadedFile@@QAEXXZ 0x0098EC40,
?Close@CBankFile@@UAEXXZ 0x009CE360. No new symbol collides with a landed row's spelling: the four new ??1 names that already
appear in reloc_bindings.tsv (??1CArenaCreatureDef, ??1CShaderDataBank, ??1CCreatureActionBase, ??1CChangingStateComponent)
bind exactly these addresses.

## Label corrections
Seven catalog labels were wrong (NAME_FIXES.tsv): 0x0079BC80 (CVillageTaskCarryCrate::AssignCreatureToTask -> SetSummoner),
0x00F25E40 (~CAIStateGroup_HoverHornetQueen -> ~CArenaCreatureDef), 0x00718D50 / 0x009822E0 (~PerformExpressionExtended ->
~PerformExpression / ~SendMessage), 0x00A620D0 / 0x008F6170 / 0x00541350 (~CParticleEffect -> ~CShaderDataBank / ~CHeightMap /
NUISystem::~CDynamicList). Neighbouring rows found mislabelled on the way: 0x00F25E10 (GetSizeofClass), 0x00F25E20 (??_G),
0x00F25F00 (Copy), 0x00F263D0 (ctor file name) of CArenaCreatureDef; the manifest ??_G labels of 0x0084B600 / 0x0084B620 /
0x00982550; landed bindings of 0x00A69A80 (CFrameData::Close), 0x009D58A0 (QAE, h2), 0x00A62050 (DeleteShaderEntries),
0x009CE360 (three invented spellings); coverage.tsv SetAsMRU on 0x009FC450.

## Header debt
- engine/CBankFile.h (hold): flattened (vptr data member), so 0x009D58A0 / 0x00A620D0 declare a local polymorphic CBankFile
  (0x009D58A0 copies the header's retail member list; 0x00A620D0 an opaque body). Needs the polymorphic retail class with
  ~CBankFile 0x009CE180 and the five Ego_r virtuals (OpenReadOnly, OpenReadWrite, OpenFileReadOnly, OpenFileReadWrite, Close).
- engine/CCreatureActionBase.h: retail size 0xA8 but vptr flattened; the three action dtors use the landed 0x007ED750 local base.
- engine/CResource.h: no SetAsLRU / SetAsMRU declarations; 0x00A69A80 models CResource over engine/CIVCountedPointeeBase.h.
- engine/CManagedTextureData.h, engine/CShaderDataBank.h (Ego_r-only parser members; retail 0x128 bytes),
  engine/CCreatureAction_PerformExpression.h, engine/CCreatureAction_PerformExpressionExtended.h: flattened generated layouts.
- engine/CObserver.h: global namespace and flattened; retail is NUISystem::CObserver (0x00541350 uses the landed 0x0052CCF0 model).
- engine/CTCBase.h: flattened vptr, `struct` key (as chunk 700).
- No engine headers for CArenaCreatureDef, CTCSummonableCreature, CHeightMap, CCreatureAction_SendMessage, C3DVector,
  NUISystem::CDynamicList / CComponent / CChangingStateComponent, CBankStateBlock / CShaderBankStateBlock, CThreadedFile.
