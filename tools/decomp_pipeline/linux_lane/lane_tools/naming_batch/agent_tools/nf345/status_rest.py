#!/usr/bin/env python3
"""NF345: append STATUS lines for the entries not yet in STATUS.tsv (idempotent: skips rows already present)."""
import os
F = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CW = 'CopyBackBufferToTexture stand-in cluster: R is a junk CDisplayManager self over an STLport map-ctor body; the table line binds the PAV spelling (PDB is QAEAAVCTexture) through witness 0x00B88ED0; A 0x009BF5E0 and the witness are T3_PORT_QUEUE (nf311 0x009BF5E0 port, nf303 census). R\'s real name needs a self port; no change.'
ID = 'two landed selves define the same symbol (self_symbols: R and A both grade {s}); the table line binds it to A (witness {w}, retail rel32 to A). Landing either alone leaves a second strong definition: hold R and A as one identity unit (brief line 241). No change.'
S = {
 '0040b050': ('table-fixed', 'U2 (batches/b1): L = SetCameraYZInverted is R\'s real name (0x0040B050 stores +0x10/+0xD0; engine/CUserProfileManager.h comments SetCameraYZInverted at 0x0040B050; 0x0040D2F0 stores the Bow fields = PDB SetCameraBowYZInverted). Binder 0059697a respelled to SetCameraBowYZInverted; TABLE_CHANGES delete (YZ, 0040d2f0) + add (YZ, 0040b050, witness 0041e6d3, its only retail caller per xscan). pk_all of 0040cca0 under the applied table: its YZ pick-any copy NEQ@0040d2f0 -> EQ@0040b050 (evidence/pk_table_effect.txt).'),
 '0042b587': ('respelled', 'b3 (batches/b3): self respell J -> K (map<CCharString, unsigned long>): only retail caller 0x0042BCB5 is a proven unsigned-long map user binding K at 0042b587; the J leaf belongs to 0046ece7. objsec ALL_EQ, vland HEAD/new RELOCATION_MATCH, pk_all unchanged apart from J->K; TABLE_CHANGES replace the pair copy-ctor J -> K at 004297db (only other retail caller unlanded 0x0042A911). Catalog label still says long: NAME_FIXES.'),
 '0042d9e0': ('skipped-port', 'ego_r CInventoryDef ctor calls CParentDefClassBase at A 0x00430370; the witness 004550ac binds the wrong base (CParentDefClassBase spelling at A is right, R\'s self is the other ctor): fixing the witness changes its base-class model (port). A 0x00430370 is T3 (nf303 identity research).'),
 '00433cec': ('research', ID.format(s='??$__copy@PAVCWideString@@PAV1@H@std@@...', w='0059ae20')),
 '004345a0': ('research', ID.format(s='??4CConfigOptionsDefaultsDef@@QAEAAV0@ABV0@@Z', w='006314c0')),
 '00442760': ('research', 'R is a 1-byte `ret` stub claiming NPlayerGui::CDrawBase::ClearPrimitivesIfNecessary; A 0x00644627 is the real owner (CDrawQuestInfo::ClearPrimitivesIfNecessary 0x00644D0E calls it as its base, retail rel32). R needs a different (tiny-stub identity) name; A is T3 (nf303 census port). No change.'),
 '00453554': ('skipped-port', CW + ' R itself is also T3 (nf303 census).'),
 '0045c5f6': ('skipped-port', 'table line EndAttackBeam -> 0x007FC800 is right (ego_r KrakenLowAttackOutOf::Initialise calls EndAttackBeam twice; retail calls 0x007FC800 twice). R\'s self is wrong and R is T3 (nf334 port); A\'s self is the free stand-in CTCKraken_EndAttackBeam (A in nf302 T3 reason). NAME_FIXES for 007fc800.'),
 '0047dd9d': ('table-fixed', 'b2 (batches/b2, DECISION D1): R = PCSetToLoad is correct (~493 bytes; ego_r PCSetToLoad ~489); A 0x0047D008 is the 10-byte LoadGameIndex(+0x64) setter = ego_r CTCInGameMenu::LoadGame (~10 bytes). Binder 0041e6d3 respelled to LoadGame (PDB), TABLE_CHANGES replace PCSetToLoad -> LoadGame at 0047d008. Lands only on a yes to D1 (owner 0047d008 still defines a stand-in: reference unresolved until its port).'),
 '004c2dd0': ('research', 'witness 0x00C72600 carries two lines at 0x00C59BA0 (C3DAnimationBlendState and CCodaVoice spellings); the C3DAnimationBlendState line is likely stale, but the witness is T3 (nf285 standin_alias port). Not compiled.'),
 '004c7aa0': ('skipped-port', 'ego_r DrawIsDrawable calls CThing::PeekComponent at A 0x004C7A70 (table right); the witness 0x0083B3B0 respell changes the return type (port); R is T3 (nf314 census port).'),
 '004df8cb': ('skipped-port', CW),
 '004df956': ('skipped-port', CW),
 '004e76df': ('skipped-port', CW),
 '004e78d4': ('research', 'stand-ins on both sides: R defines CNavigationLayer ctor over a body the table at R names CSkeletalMorphs; A 0x00C20390 is a libpk SEventRecord / COcclusionBase stand-in bound through witness 0x00C10A30 (CPKVoiceParams). No PDB-proved name for either; no change.'),
 '004e793c': ('research', 'as 004e78d4 (table at R: CParticleMorphs ctor); stand-ins on both sides, A 0x00C20390 libpk. No change.'),
 '004f9cfc': ('skipped-port', 'R is T3 (flatten2 shared-name cluster CTCInventoryItem_OnDie); A 0x00C10940 is T3 (nf294 entry 40: free fastcall dtor for the virtual ~CCardPositionsDef). No change.'),
 '0050fa40': ('skipped-port', 'table line _Vector_base<void*> -> 0x00450E3A by witness 0x00477428; the witness is T3 (nf289 entry 15: stand-in class CRetailLayerVector_00477428). No change.'),
 '00531ec0': ('skipped-held', 'NF281 D4: 0x0052C7E0 held until 0x005367F3 is fixed (RELOC_CONFLICT); not compiled.'),
 '0057986e': ('skipped-port', CW),
 '005d85de': ('research', ID.format(s='??0?$vector@JV?$allocator@J@std@@@std@@QAE@ABV01@@Z', w='00f38770') + ' A 0x00514160 is T3 (nf303 census).'),
 '00643d8c': ('research', 'unit proposal: R is NPlayerGui::CDrawCombatMultiplier::AddHitByMelee (ego_r forwarder jmps to it, body = ego_r\'s, NPlayerGui region); A 0x00437990 is CPlayerGui::AddHitByMelee (mov ecx,[ecx+150h]; jmp 00643d8c; ego_r CTCHero::OnHit calls CPlayerGui::AddHitByMelee). A needs a port onto engine/CPlayerGui.h; R\'s self respell and A\'s port land together or OnHit\'s resolved reference breaks. NAME_FIXES for both.'),
 '00656ba2': ('skipped-port', 'R and A 0x00656BD8 are T3 (nf301 / nf309 ports: CQuestInfoElement nesting); witness 0x0064FC23 is T3 (nf297).'),
 '0066a690': ('skipped-port', 'witness 0x00909FD0 binds the wrong ctor: donor gives the real callee CCreatureAction_StrafeBackward ctor at A; fixing it is a base / class model change (port); A 0x008BFD60 is T3 (nf303 census).'),
 '00699970': ('research', ID.format(s='?PushLookAround@CGameCameraManager@@QAEXXZ', w='007d34a0') + ' R is named in an nf313 T3 reason (0x0084E420 line).'),
 '006a3c70': ('skipped-port', ID.format(s='vector<CScreenFilterSThingByPass>::push_back', w='00cbfb7d') + ' R is T3 (nf303 census) and the witness is T3 (nf311).'),
 '006aeb10': ('research', 'R\'s // Leaf: is ??8CDefString@@QBE_NABVCCharString@@@Z, the same symbol A 0x004B7980 defines (self_symbols RELOCATION_MATCH); table line witness 0x00891DB0 (retail rel32 to A). Identity: hold R and A as one unit; A is named in an nf295 T3 reason.'),
 '006c4560': ('research', ID.format(s='??RCThingFilter_AICreatureHasProperty@@QBE_NABVCThingAICreature@@@Z', w='009823d0')),
 '0076d850': ('research', ID.format(s='?FindThingInList@@YI?AUCThingListIterator@@U1@0ABQAVCThing@@@Z', w='0076cc00') + ' Witness named in an nf311 T3 reason.'),
 '0078915c': ('skipped-port', 'element-type port (CThingGameObject* v CThing* vector push_back); R is T3 (nf311 port).'),
 '007f9f00': ('skipped-port', 'A 0x008098B0 and witness 0x0080A140 are T3 (nf310 port: same-TU noinline comparators). No change.'),
 '0081a590': ('research', ID.format(s='uninitialized_fill_n<pair<unsigned long,CSoundPair>*>', w='00470778') + ' The table at R binds the CSkeletalMorphs::CEntry instance there (0081ce50): R is likely that instance (self respell candidate, element-type change = port).'),
 '0081a9e0': ('research', ID.format(s='__unguarded_insertion_sort_aux<pair<unsigned long,CSoundPair>*>', w='00470d36') + ' As 0081a590: R is likely the CSkeletalMorphs::CEntry instance.'),
 '0085e200': ('research', ID.format(s='vector<CTextureManagerPoolInfo>::_M_insert_overflow', w='00b3cb30') + ' The table at R binds the N0085e570::Element instance (0085e570): R is a stand-in for another element type (port).'),
 '00863260': ('research', 'R\'s leaf EnableNavigator@CTCPhysicsNavigator (MATCH) v A 0x007BED30 (byte +0x66 setter, no self in self_symbols) bound by witness 0x008C89D0 (nf308 T3 reason). Which of the two setters is EnableNavigator is unproven; no change.'),
 '008cbea0': ('skipped-port', 'witness 0x0096FCD0 binds the wrong ctor: donor gives the anonymous-namespace CCreatureAction_SummonerLightningIdle ctor at A 0x00970240; fixing it is a class model change (port).'),
 '00901658': ('skipped-port', 'witness 0x00902873 binds the wrong ctor: donor gives CAIStateGroup_MinionBehaviourCrowdBase ctor at A 0x009009EA; base/class model change (port).'),
 '0090310e': ('skipped-port', 'table line CVertexShaderInput ctor -> 0x00AB3B10 (witness 0x00A986B0) is right per donor; R\'s self is the wrong class; fixing it is a class model change (port).'),
 '009c3510': ('research', 'R self ?OnReadFinished@CActiveFile@@ (MATCH) and A 0x004DF9CC self ?OnReadFinished@@YIXPAUAutoTinyActiveFile@@@Z: propagated OnReadFinished labels on byte-identical bodies; the table line (witness 0x004E108C) names R\'s spelling at A. Needs label_trust work; no change.'),
 '00b3e390': ('research', 'R\'s // Leaf: is the CTextureManagerPoolInfo __uninitialized_fill_n that A 0x0085D7A0 also carries in the table (witness 0x0085E200); A\'s table also binds the N0085e570::Element instance; the table at R binds the same symbol at 0x00B3E920. Identity / instance question tied to 0085e200: hold as one unit.'),
 '00be7d70': ('skipped-port', 'R = CLandscapeBackgroundPatch::Load (ego_r ~2656 bytes), A 0x00BE8920 = LoadCompressed (~352); witness 0x00BF8030 must respell to LoadCompressed(CMemoryDataInputStream&), a parameter-type port; witness named in an nf314 T3 reason.'),
 '00c21ba0': ('skipped-port', 'R\'s Leaf ??0CSoundEvent@ matches nothing (fallback); witness 0x005ED190 binds a global-scope CScriptModuleBase that should be NCameraModes::; A 0x005ED070 and the witness are T3 (nf301 port).'),
 '00c22040': ('research', 'libpk propagated AddLearnedExpression self on R; table line (witness 0x006AC430) is right: ego_r LearnExpression calls CTCInventoryAbilities::AddLearnedExpression at A 0x005EEDCF, whose self is the stand-in FableAddLearned_005eedcf (NAME_FIXES). R needs its libpk name (light naming); witness named in an nf309 T3 reason.'),
 '00c222b0': ('research', 'as 00c22040 (second propagated AddLearnedExpression self in libpk).'),
 '00c36170': ('research', 'libpk witness 0x00C1DC40 binds ?Release@CSemaphore@@ at A 0x00C2A150 whose self is PKCSemaphore::Release: possible light-naming binder respell (CSemaphore -> PKCSemaphore), not done (R\'s own name CSemaphore::Release unproven v CSemaphoreHandle).'),
 '00c3b650': ('research', 'as 009c3510 (propagated OnReadFinished labels).'),
 '00c53400': ('research', 'R = GameCODA CHLRvbParams ctor (3 bytes) v A 0x00C6ADB0 SetDefaults (173 bytes) bound by witness 0x00C6B050 (CFloatReverbEngine::Create); a 3-byte body cannot be what Create calls for defaults, so the table line probably names the wrong callee; needs the GameCODA SDK. No change.'),
 '00cbd310': ('skipped-port', 'R is T3 (nf328 debt); A 0x008ADF90 T3 (nf311 port); witness 0x008A80F0 T3 (nf289 entry 49).'),
 '00cd31d2': ('skipped-port', 'R defines the STLport ~_Rb_tree<CCharString,...>; A 0x004AC8F0 is the stand-in CRetailTree_erase004aba70 dtor; witness 0x00CBFB7D is T3 (nf311 port).'),
 '00e1a870': ('table-fixed', 'b2 (batches/b2, DECISION D1): R = CQ_WizardBattleScript::Init is correct (FableWin object order Init < Main < InitialiseVariables = retail 00e1a870 < 00e1a990 < 00e1ad30); A 0x00E1AD30 = InitialiseVariables (FableWin Main calls it first; retail Main calls 0x00E1AD30 first). Binder 00e1a990 respelled, TABLE_CHANGES replace Init -> InitialiseVariables at 00e1ad30. Owner 00e1ad30 still defines a stand-in (port onto engine/CQ_WizardBattleScript.h).'),
 '01211e57': ('research', 'EH funclet of 0x00C174A0 LoadBankFile: the parent TU binds LoadLookupTableBank at 0x00C16F00 while 0x00C16F00\'s own row is named ReadDeferredBank (libpk naming split). Needs the libpk name of 0x00C16F00; no change.'),
 '01211e62': ('research', 'as 01211e57.'),
 '01211e6d': ('research', 'as 01211e57.'),
 '01212a58': ('skipped-port', 'EH funclet of 0x00C21BA0 (parent\'s graded symbol): as 00c21ba0, A 0x005ED070 and witness T3 (nf301).'),
 '0063b890': ('respelled', 'U1 (batches/b1): BR -> TR. ego_r FindGround/FindTheme call order TR,BL,TL,BR = retail 63b890,63b920,63b9b0,63ba30; Ego_r sizes 0x90/0x90/0x80/0x80 v retail 129/129/123/123; fadd only in TR and BL in both. objsec ALL_EQ, vland RELOCATION_MATCH, link 0 dups; binders 0063d0a0/0063d4a0 already bind these names here.'),
 '0063b920': ('respelled', 'U1 (batches/b1): TL -> BL (row gains its // Leaf: line). Evidence as 0063b890.'),
 '0063b9b0': ('respelled', 'U1 (batches/b1): BL -> TL. Evidence as 0063b890.'),
 '0063ba30': ('respelled', 'U1 (batches/b1): TR -> BR. Evidence as 0063b890.'),
 '006403d0': ('table-fixed', 'U3 (batches/b1): R = RemoveMap is correct (retail callers 004ff8b0 / 005022f0 / 00506940 all call 0x006403D0 and bind RemoveMap there, xr); A 0x00640430 = SetMapAsUnloaded (self MATCH; ego_r UnloadLevel/UnloadRegion call it). Binders 004fede0 / 004feec0 respelled; TABLE_CHANGES replace RemoveMap -> SetMapAsUnloaded at 00640430 + add RemoveMap 006403d0 (004ff8b0). ref_scan: no other RemoveMap referencer binds 00640430.'),
}
done = {l.split('\t')[0] for l in open(f'{F}/STATUS.tsv').read().splitlines()[1:] if l}
ent = {l.split('\t')[0]: l.split('\t') for l in open(f'{F}/ENTRIES.tsv').read().splitlines()[1:] if l}
miss = [a for a in ent if a not in done and a not in S]
with open(f'{F}/STATUS.tsv', 'a') as o:
    for a in ent:
        if a in done or a not in S: continue
        st, ev = S[a]; o.write(f'{a}\t{st}\t{ent[a][2]}\t{ev}\n')
print('missing', miss)
