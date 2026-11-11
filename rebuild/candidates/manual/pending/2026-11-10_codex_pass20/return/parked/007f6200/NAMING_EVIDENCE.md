# Identity and extent

CTCTargetingAI::UpdatePBestEnemyCreatureForAttack(bool), exact public thiscall leaf ?UpdatePBestEnemyCreatureForAttack@CTCTargetingAI@@QAEX_N@Z. Both original PDB names: Ego007f15c0/FableWin02535510; full original donor, module260 local/type records, all supplied neighbours, and native control flow independently read. Complete original CTCTargetingAI76-byte shared owner retains inherited targeting state, script/best intelligent pointers and exclusion list.

Assigned1472 includes1458 bytes of one function followed by14 alignment bytes. Native ret4 at007f67af ends007f67b2; next start007f67c0. Input ROWS.tsv unchanged. Selected1458/1458 is NOT a byte match: frame0x80 agrees but parent/owner spills14/18 swap, comparison construction stores cross a call differently, filter copy/call-result lifetime differs, and later parent caching differs. Best strict alignment86.42%,445 emitted vs446 native instructions. This percentage is diagnostic, not a matching-function count.

# Original types and helper graph

Both PDB type records (Ego_types/FableWin_types and filter_types) prove complete CTCTurncoat76, CTargetingDef144, comparator24 and anonymous valid-attack predicate32. ETargetingType enum values come from both original enum records. CTargetingDef inherits CSubDefBase and contains the genuine map plus weights128/132/136/140. Six predicate members, awareness/ghost flags and exact const signatures retained. Anonymous namespace suffix is compiler-generated per TU; only that hash differs across build paths.

The comparator accepts const CThingCreatureBase&,const CThing*,four floats, with a CThingCreatureBase operator specialization at007f6f20. Original valid-target operator at007f6df0 accepts const CThingCreatureBase&, not the old guessed struct CThing&. GetPBestLeafThing007f7040 is the Bind2<In2DRange,anonymous Valid>/BestCreatureToAttack/CThingAICreature instance. Its predicates refer to their genuine classes, never stand-ins. Original const interface-map LowerBound is004365b0. Original STLport4.6 reserve and protected _M_insert_overflow retain vector<const CThing*>, allocator and __true_type identities. Free is the exact CRT free thunk00bfea14.

Original helper donor evidence proves CIntelligentPointer Get/arrow -> private GetPointer -> GetPItem and assignment -> private SetPointer(T*) -> SetPItem. Existing GetPLastAttackedByCreature accesses the original intelligent pointer. Peek/Get pointer constness follows both PDBs. No added packing. Scoped conversion inline_depth only reproduces the missing native00668b60 call at007f6686; plain source and per-row budgets tried first, whose unrelated call changes are logged. It is not used for allocation or scheduling.

# Semantics and locals

Original locals recovered: pos,parent_creature,overridden_by_script,pold_best_enemy,ptc_turncoat,tc_perception,radius,exclusion_list,it,pthing,update_frequency_attenuator,dis_to_player,pbest_for_attack,pthing_ai_creature,prefer_non_combatant_weight,pplayer_creature,prefer_player_weight,pref_cur_target,pref_last_attacker,comparison_filter,pperceived_thing,pCreature,pCreatureConst. Added named predicate/attacker temporaries are source-shape proposals, not claimed recovered names.

Script override requires alive/not incapacitated; turncoat requires enemy. Search refresh is bounded by map and forced/unsigned64 frame+UID modulo attenuated frequency. Both donor/native zero-extend signed frame/frequency through unsigned long before64-bit arithmetic. Search and player filtering use awareness=true; fallbacks usefalse. Perceived fallback type flag is8(Creature), not32(AICreature). Changed old/new player targets refresh minimap. All70 direct calls retain original order and matching identity; no byte-verification claim is made for unmatched nonrelocated instructions.

# Data and names

PMainGameComponent013b86a0, ConstantFPS01375550, CPlayerGui singleton013b8790 are original established symbols also proved in both original PDB globals. Native zero literal0122dedc contains00000000; intelligent-pointer vtable is the native CreatureBase instantiation. Every callee is listed below with independently decoded native call site and PDB-set occurrence. STLport differences from VS2010 donor template spelling are explained by original STLport4.6 headers. NAME_FIXES reports wrong prior binding spellings only.

| Native call | Target | Emitted name | Original PDB sets |
|---|---|---|---|
|007f620f|004c73d0|`?GetPos@CThing@@QBEABVC3DVector@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6224|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6232|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6241|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6255|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f625c|0085f0c0|`?IsIncapacitated@NCreatureMode@@YI_NABVCThing@@@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6281|004365b0|`?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z`||
|007f629a|007c7890|`?IsTurncoated@CTCTurncoat@@QBE_NXZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f62a5|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f62b0|0076d2f0|`?AreThingsEnemies@CTCEnemy@@SI_NABVCThing@@0@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f62be|00668b80|`??4?$CIntelligentPointer@VCThingCreatureBase@@@@QAEAAV0@ABV0@@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f62ca|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f62d4|00a01b90|`?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f630c|007463e0|`?reserve@?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@QAEXI@Z`|fablewin_pdb_names.tsv|
|007f6323|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f635c|0049c370|`?_M_insert_overflow@?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@IAEXPAPBVCThing@@ABQBV3@ABU__true_type@2@I_N@Z`||
|007f6383|004365b0|`?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z`||
|007f63a1|00744950|`?GetExtendedSightRadius@CTCPerceiveThing@@QBEMXZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f63ad|00686d20|`?PeekWorldMap@CTCBase@@IBEABVCWorldMap@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f63b4|004fc600|`?IsPosInMap@CWorldMap@@QBE_NABVC3DVector@@@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f63c9|00449970|`?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f63d0|00487dc0|`?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f63d7|004c73d0|`?GetPos@CThing@@QBEABVC3DVector@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f63f3|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6409|00686a70|`?GetWorld@CTCBase@@IBEAAVCWorld@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6417|0049d870|`?GetFrame@CWorld@@SIJXZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6442|00bfea70|`__ftol2`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6462|00bfebb0|`__aullrem`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f64a8|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f64d3|00686d40|`?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f64da|00449970|`?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f64e2|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6522|007f6d70|`??0CThingFilter_IsValidAttackTargetForCreature@?A0x8afeb9f0@@QAE@ABVCThingAICreature@@ABV?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@_N@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f653c|00686d90|`?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6543|007f7040|`??$GetPBestLeafThing@V?$CFilter_Bind2@VCThingFilter_In2DRange@@VCThingFilter_IsValidAttackTargetForCreature@?A0x8afeb9f0@@@@VCThingCompare_BestCreatureToAttack@@VCThingAICreature@@@CThingSearchTools@@QBE_NABV?$CFilter_Bind2@VCThingFilter_In2DRange@@VCThingFilter_IsValidAttackTargetForCreature@?A0x8afeb9f0@@@@ABVCThingCompare_BestCreatureToAttack@@PAPAVCThingAICreature@@@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f658e|007f6d70|`??0CThingFilter_IsValidAttackTargetForCreature@?A0x8afeb9f0@@QAE@ABVCThingAICreature@@ABV?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@_N@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f659a|006a76b0|`??RCThingFilter_In2DRange@@QBE_NABVCThing@@@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f65a6|007f6df0|`??RCThingFilter_IsValidAttackTargetForCreature@?A0x8afeb9f0@@QBE_NABVCThingCreatureBase@@@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f65b9|007f6f20|`??$?RVCThingCreatureBase@@@CThingCompare_BestCreatureToAttack@@QBEPAVCThingCreatureBase@@PAV1@0@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f65d8|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f65e5|00a01b10|`??0CBaseIntelligentPointer@@QAE@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f65f7|00a01b90|`?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6600|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6608|00a01b90|`?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6611|00a01c10|`??1CBaseIntelligentPointer@@UAE@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6618|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6623|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f663d|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6651|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f666a|007f6d70|`??0CThingFilter_IsValidAttackTargetForCreature@?A0x8afeb9f0@@QAE@ABVCThingAICreature@@ABV?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@_N@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6671|007f6df0|`??RCThingFilter_IsValidAttackTargetForCreature@?A0x8afeb9f0@@QBE_NABVCThingCreatureBase@@@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f667c|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6686|00668b60|`??0?$CIntelligentPointer@VCThingCreatureBase@@@@QAE@PAVCThingCreatureBase@@@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f668f|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6697|00a01b90|`?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f66a0|00a01c10|`??1CBaseIntelligentPointer@@UAE@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f66a7|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f66b2|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f66d6|007446a0|`?GetPerceivedThing@CTCPerceiveThing@@QBEPBVCThing@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6704|007f6d70|`??0CThingFilter_IsValidAttackTargetForCreature@?A0x8afeb9f0@@QAE@ABVCThingAICreature@@ABV?$vector@PBVCThing@@V?$allocator@PBVCThing@@@std@@@std@@_N@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f670b|007f6df0|`??RCThingFilter_IsValidAttackTargetForCreature@?A0x8afeb9f0@@QBE_NABVCThingCreatureBase@@@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6719|00668b60|`??0?$CIntelligentPointer@VCThingCreatureBase@@@@QAE@PAVCThingCreatureBase@@@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6727|00668b80|`??4?$CIntelligentPointer@VCThingCreatureBase@@@@QAEAAV0@ABV0@@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6730|00a01c10|`??1CBaseIntelligentPointer@@UAE@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f673e|00bfea14|`_free`||
|007f674a|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f6773|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f677e|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f678e|00a01b50|`?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
|007f67a0|004378c0|`?UpdateMiniMapMarkers@CPlayerGui@@QAEX_N@Z`|egor_pdb_names.tsv,fablewin_pdb_names.tsv|
