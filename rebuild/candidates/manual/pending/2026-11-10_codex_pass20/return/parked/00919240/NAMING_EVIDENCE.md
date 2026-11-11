# Exact identity and original owners

Private thiscall CThing* ?FindInterestingThing@CAIStateGroup_LookAtInterestingThings@@AAEPAVCThing@@XZ: Ego0093cbc0,FableWin02bb7e60. Full original TPI Ego0x5372b24/FW0x5f22b28,native24; actual genuine CAIStateGroupBase20. Complete original derived methods/FrameToLook14 restored in existing owner under row guard. Existing genuine PBrain pointer/GetCreature reference and original CAIBrain private Creature reference18 reused. Including state-group owner before CAIBrain is necessary for existing shared GetCreature inline definition; declaration-only body incorrectly added a native call in first rebase.

Original collection is vector<CThingGameObject*>,not vector<CThing*>. Original component exact CTCInterestingToVillagers TPI Ego0x2fb7b/FW0x32758,size16,baseCTCBase12,public CDefPointer<const CInterestingToVillagersDef>PDef+c. Full new genuine owner includes all original methods. Original def CInterestingToVillagersDef Ego0x2fb66/FW0x32743,size44,baseCSubDefClassBase40,Interest28; existing default preserved in row opt-in full original owner. Synthetic CTCInterest/CInterestDef/Def names removed. Real enumTCI_INTERESTING_TO_VILLAGERS179. Original full CThingFilter_HasTC derives unary_function<CThing,bool>,private ETCInterfaceType Type,not a long stand-in.

Full supplied ROW,best/sole prior,43line lessons,77line notes,41line STATUS and all six neighbours read before implementation. Prior hand258 is hash-identical to best. Old collector sink/flattened owner/helper models discarded. Full donor02bb7e60..02bb841a/native00919240..0091943b read;508byte extent exact. Raw module728 locals objects vector<GameObject*>,total_interest,chosen_interest,long n in each loop,and separate original CTCInterestingToVillagers* interesting locals retained. Loop GetTC output pointers are intentionally uninitialized when original assert-precondition fails; collector's HasTC filter guarantees component existence. No invented recovery behavior.

# Genuine collector and helper chain

Exact GetAllThingsInRoughArea<CThingGameObject,vector<CThingGameObject*>,CThingFilter_HasTC>(const C2DBoxI&,const Filter&,Container&)const is original Ego0093d190/FableWin02bb8770,called by donor. Real native00919780 identified by corresponding call/body; current emitted exact generic types agree. Full368line donor collector read: type mask,GetAllThingMapsInArea,search counters/ID,map clipping,each mapwho layer/cell/node,search stamping,checked type/filter,push_back,clear cached AreaMaps. Existing genuine shared ORIGINAL_ROUGHAREA body provides that real scan with original named static counters; no fabricated sink. Native collector itself calls filter operator006a80a0 while this compiler may expand the genuine body; selected leaf's call target/signature remains exact. Entire collector is not claimed byte-matched. Its semantics/storage are genuine; helper syntax/call boundary trials are preserved independently.

Original GetCreature returns CThingAICreature& through PBrain->GetCreature,not old raw CThing* stand-in. Original ToC2DCoordI and C2DBoxI(center,10) produce exact native X,Y truncations (two __ftol2 at00bfea70),real four corners. PeekThingSearchTools is protected const reference (native008fd000). Original HasTC/GetTC→rawGetTC→Components.Find→LowerBound chain used in both loops, mutable specialization0040f020; original actual CDefPointer operator-> preserves PDef/Interest reads and all source type identities. No GetTCIfPresent invented wrapper. Original GetPWorldSeed const008fcfd0,GFRandom/GFROR13 and protected static CMainGameComponent::ConstantFPS01375550/getter,CWorld::GetFrame0049d870 reused. Native empty schedule random120 thenfps snapshot thenframe,adds240 and multipliesfps. GFFloatRandom exact fastcall float(float,unsigned long*)native004978a0; all nonempty cumulative behavior preserved including chosen<=cumulative and zero/NaN boundaries. Vector real destructor frees via original _free00bfea14 at both exit paths; no scalar/array delete substitution.

# Honest remaining difference and operands

Selected s02 remains DIFFER508v508,162/164instructions equal. Entry schedule only: nativefilter179 store at0091924d before Creature load at255,ours Creature load24a then filter store25d. Same effective fields/values/frame and all subsequent instruction offsets. No invented compiler workaround retained. All15 external operands below independently decoded from native bytes; all17 internal branch targets exact.23 prefix byte differences explicitly retained by audit.py,not masked away or claimed matched. No literal or text operands.

Raw evidence sol_interesting_{Ego_r,FableWin}_types.json,sol_interesting_{Ego_r,FableWin}_components.json,sol_interesting_donor.txt,sol_interesting_collector.txt,sol_interesting_native.txt,sol_interesting_locals.txt,sol_interesting_operands.json.

|Offset|Native target|Original emitted symbol|
|---|---|---|
|38|004c73d0|`?GetPos@CThing@@QBEABVC3DVector@@XZ`|
|47|00bfea70|`__ftol2`|
|57|00bfea70|`__ftol2`|
|107|008fd000|`?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ`|
|114|00919780|`??$GetAllThingsInRoughArea@VCThingGameObject@@V?$vector@PAVCThingGameObject@@V?$allocator@PAVCThingGameObject@@@std@@@std@@VCThingFilter_HasTC@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_HasTC@@AAV?$vector@PAVCThingGameObject@@V?$allocator@PAVCThingGameObject@@@std@@@std@@@Z`|
|133|008fcfd0|`?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ`|
|156|00497890|`?GFROR13@@YIJK@Z`|
|173|01375550|`?ConstantFPS@CMainGameComponent@@1JA`|
|180|0049d870|`?GetFrame@CWorld@@SIJXZ`|
|204|00bfea14|`_free`|
|287|0040f020|`?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z`|
|337|008fcfd0|`?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ`|
|349|004978a0|`?GFFloatRandom@@YIMMPAK@Z`|
|415|0040f020|`?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z`|
|491|00bfea14|`_free`|
