# Identity and naming evidence

`?OnHit@CThingCreatureBase@@UAEXAAVCHitParameters@@@Z` is Ego_r00544c80 / FableWin01d8f090. Native CThingCreatureBase vtable0125ac0c slot65 binds00667d30. The1210-byte extent is one complete function ending at006681ea; no jump table or embedded data.

## Verified exact reconstruction

Selected grade18: exact1210/1210 bytes with relocated operands masked, zero normalized instruction differences. Grade17 first matched by preserving typed GetTC and spelling each full PlayCreatureOnHitEffect call inside its own if/else arm. The previous whole-call attempt used the enum lookup API; combining the genuine typed overload with the full branch-call source shape restores the native eax receiver copy. Grade18 repeats the final source against current shared headers. All44 relocation operands now align at the same object/retail offset.


## Header and inline evidence

Raw original TPI members/classes are preserved in creatures/types_core.json, types_onhit.json, types_hit_aux.json and types_definitionmanager.json. All new game classes use the original inheritance and complete native extents; unrelated members are explicitly opaque. Defaults of existing headers remain unchanged behind opt-in macros. CHitParameters retains its original unaligned vectors as12-byte opaque fields at2/e; no new packing directive.

Original FableWin typed GetTC/PeekTC instantiations prove the API. The owner exception in docs/EXTRA_RULES.md100–104 explicitly retains the stronger inline annotation for this family. Ordinary inline was graded in attempt10 and outlined every lookup; attempt11 restored the authorized annotation and fixed the former stack residues. F1 used the original full GetSubDef specialization, not a dummy class.

## Direct relocation evidence

44 relocated operands,28 unique targets. RELOCATION_REVIEW.json records every object offset, translated retail offset, emitted COFF symbol, PDB name-set matches and old bound spellings.

- `0x0040dce0`: FableWin018962e0 exact const-reference IS_THING_ALIVE<CThing*> template; native loads pointer then Dead91 bit0.
- `0x0040f020`: Raw CVectorMap TPI mutable LowerBound(const ETCInterfaceType&); native mutable component lookups; original STLport4.6 raw-pair-pointer iterator type replaces donor Dinkumware iterator wrapper.
- `0x004365b0`: Const LowerBound overload; native const component lookups and owner settled overload mapping; genuine STLport4.6 pointer iterator.
- `0x00437990`: Ego_r0044a640/FableWin01a80380 AddHitByMelee(CThing&,CThing&,const C3DVector&); native forwards PDrawCombatMultiplier150 to00643d8c, matching original nested GUI class.
- `0x004497c0`: Ego_r00445080/FableWin01a59400 IsThingMainPlayer const bool(const CThing&); native player-manager receiver and hitter pointer.
- `0x00485815`: Ego_r00491ca6 exact const GetSubDef<CCreatureDef>; native callee constructs CCreatureDef literal and uses def-manager index lookup.
- `0x004c70f0`: Raw CTCScriptedControl TPI0x21fc6 IsActive const bool, Ego_r004c8fe0/FableWin01cf8cc0; native reads Locked18 and calls007122c0.
- `0x004c7a00`: CThing raw TPI protected mutable GetPlayerManager reference; Ego_r004c95a0/FableWin01cf6240 and manager-return native access.
- `0x004c9d60`: Raw CThing TPI AddTC public, const CCharString&,bool,const CTCParamsBase*; both PDB entries and native three arguments.
- `0x006637d0`: Ego_r0053e740 anonymous CreateWeaponHitTargetEffect(const CThing*,const C3DVector&,const C3DVector&,bool,bool,bool); native fastcall plus16-byte stack arguments. Only TU-generated anonymous hash varies.
- `0x00667450`: Ego_r00544570/FableWin01d8fd50 protected ReactToGenericStrike(const CHitParameters&), sole native call from this row.
- `0x00669b50`: Real STLport4.6 _List_base::clear inherited by list<CCountedPointer<CCreatureActionBase> >; native node walk and list+108 receiver. No invented list::clear hiding declaration.
- `0x006e28a0`: Ego_r0070d6c0/FableWin023e7fe0 IsTalking const bool; CTCTalk caller and adjacent StopCurrentSpeech donor order.
- `0x006e2e60`: Ego_r0070de10/FableWin023e8230 StopCurrentSpeech void; native reached only when IsTalking and no active scripted control.
- `0x007331a0`: Ego_r0074b340/FableWin024fd110 IsAbleToBeKnockedDown const bool; native ParentCreature24 then combat-def9c flag.
- `0x0073a6e0`: Raw nested CModeBase and Ego_r007511e0/FableWin024c2fe0 const GetCurrentMode returning mutable reference; native PModes front load.
- `0x0073a8a0`: Ego_r007512c0/FableWin024c2340 IsModeActive(unsigned long) const; native sleep-bed enum13.
- `0x0079c750`: Ego_r007a2170/FableWin026a85b0 raw TPI original PlayCreatureOnHitEffect(CThingCreatureBase*), donor order between time-multiplier/update, native blur/intelligent-pointer creation.
- `0x007e33a0`: Ego_r007dec20/FableWin027969d0 SetLifeTime(long); native writes LifeTime14 and Timer18.
- `0x00839940`: Ego_r008147f0/FableWin027ec280 exact member, native base-class OnHit and CThingGameObject vtable.
- `0x00868860`: Ego_r0083e7d0/FableWin0280f900 IsMeleeStrike const bool; native CHitParameters receiver.
- `0x0099eae0`: Both PDB dtor names and CCharString lifetime boundaries in caller.
- `0x0099ebf0`: Both PDB ctor signatures and native two arguments (char pointer, length -1).
- `0x00a01b50`: Raw CBaseIntelligentPointer TPI, two PDB sets, native PData+4 dereference; owner settled spelling.
- `0x0123c2a0`: exact original script/class string CTCHaste. Native string read directly; no spelling substitution.
- `0x0123e020`: exact original script/class string CTCFlashOnHit. Native string read directly; no spelling substitution.
- `0x0129ba3c`: Owner ESTABLISHED_SPELLINGS raw Ego public+typed data record before GD3DFormats; exact float1e-4 and original assertion literal prove DELTA_NOTIONAL_ZERO.
- `0x013b8790`: Raw Ego_r public exact private static CCountedPointer<CPlayerGui>, singleton TPI0xaaf9; native GUI singleton read. See evidence/naming_support/PROOF.json.

## Indirect calls

Physics GetRHSet() const is native slot72/+0x120; original overloaded declarations are ordered so VC7.1 emits the correct slot. Forward is CRightHandedSet+12. CModeBase::GetType() const is slot1/+4. Def deletion is CIVCountedPointeeBase::Delete() const at slot1/+4; original reference count at+4.

## Behaviour tests and limits

The self-contained test expands76 complete original header dependencies and passes GNU64 C++03/-fno-rtti and VC7.1 RTM /GR-. Receipt: behavior_tests/00667d30/fd5cbe13e617/RESULT.json. Fourteen cases retain the original eight null/dead/blocked/negated/health/physics-vector cases and add six exact BulletTime branch cases: attacker precedence when both components exist, victim fallback when the attacker lacks one, dead-attacker fallback, neither component, null attacker, and attacker-only component. The new cases use real CTCBulletTime objects, the actual component vector and bitset, real typed GetTC/LowerBound code, and assert the exact effect receiver/victim and call count.

The BulletTime constructor declaration is added only in the flattened test, with its original public CThing& signature proven by types_onhit.json TPI0x2f3ea. A test-derived class supplies the two required pure virtuals. Original container fields are exposed only through the fixture friend for initialization; actual objects own and destroy their heap-backed pair arrays. The engine effect implementation itself is a controlled external boundary. Hero GUI/hit effects, FlashOnHit, active Talk/ScriptedControl, Haste, and mode/combat component branches still abort if reached; their relocation/branch evidence is static, not a claim of engine behavior coverage.


Fresh raw procedure records are saved in evidence/naming_support/onhit_original_locals.json. The unoptimized FableWin function retains original names hit_params, pweapon, ptc_phys, pthing_hitting, point_of_hit, hit_direction, pdef, ptc_bullet_time, ptc_my_combat, ptc_cmm, ptc_talk, ptc_flash_on_hit, ptc_scripted_control, phero_combat_def, ptc_hero_exp, ptc_haste, and decapitate. Ego retains hit_params, pweapon and pdef. These records support the exact component and definition types; the selected code still uses descriptive reconstructed local spellings and pointer aliases for the original position/direction references. No claim is made that those local spellings reproduce the source text. Game class/member/function/enum names above come from original PDB evidence.

## Native-constant fixture review

Independent main receipt21 confirms the unchanged1210-byte production function and all44 relocated operands. The prior fixture used a controlled epsilon; the current fixture loads the actual retail global bits38d1b717 by C++03 memcpy. Both compilers pass16 cases. Fourteen original hit cases now include actual native threshold equality, plus half/twice-epsilon cases. The strict less-than behavior is checked. Hero GUI/hit effects, FlashOnHit, active Talk/ScriptedControl, Haste, and mode/combat component paths remain statically reviewed but unexecuted; their test boundaries abort. Prior fixtures, metadata and successful receipts are preserved under evidence/00667d30/main_native_epsilon/prior. This is a verification build, not a new source-shape strategy or an additional matched function.
