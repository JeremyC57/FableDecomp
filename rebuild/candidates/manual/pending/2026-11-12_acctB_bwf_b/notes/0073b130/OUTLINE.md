# OUTLINE 0x0073B130 CTCCreatureModeManager::RegisterModes (static void __fastcall (), 6673 bytes)

## Frame
- `push ebp ; mov ebp,esp ; and esp,-8 ; sub esp,0Ch ; push ebx` ... `pop ebx ; mov esp,ebp ; pop ebp ; ret` (0x0073CB3C). No EH, no /GS.
- Locals (offsets relative to esp after `push ebx`; base B = esp+4):
  - [esp+7]  (B+3) 1 byte: the `__false_type` tag temporary passed by address to _M_insert_overflow (`lea ecx,[esp+0Fh]` after 2 pushes).
  - [esp+8]  (B+4) 8 bytes: THE pair temporary `pair<unsigned long, CRegisteredModeInfo>` {first=key, second.AllocFunc=Alloc} -- one
    slot reused by all 85 Inserts (the CRegisteredModeInfo temporary is folded away).
  - ebx = 1 throughout (Dirty=true, fill_len=1, atend=true).
- No constructors / destructors touch the frame (all trivially destructible).

## Body (retail order)
```
static void __fastcall CTCCreatureModeManager::RegisterModes()
{
    RegisteredModes.Insert(NCreatureMode::MODE_NULL, CRegisteredModeInfo(NCreatureMode::CModeNull::Alloc));   // 0x0073B139
    ... 84 more, exact order in the table below ...
    RegisteredModes.Update();                                                                                  // 0x0073CB16
}
```
Each Insert = `push_back(value_type(key, value)); Dirty = true;` (FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE + _INSERT_VALUE_TYPE).

### Expanded-block template (entries 1..70), e.g. entry 2 at 0x0073B190..0x0073B1E7
```
  eax = finish(0x13BAD48)                  ; carried from the previous block (`mov eax,[13BAD48h]` at its join)
  cmp eax, eos(0x13BAD4C)
  mov ecx, offset CModeX::Alloc            ; value
  mov Dirty(0x13BAD51), bl                 ; <- the PREVIOUS Insert's Dirty=true (absent in block 1)
  mov [esp+8], key ; mov [esp+0Ch], ecx    ; pair temp
  je OVERFLOW
  test eax,eax ; je SKIP                   ; placement-new null test (_Construct)
  mov [eax], key ; mov [eax+4], ecx ; mov eax,[finish]
SKIP: add eax,8 ; mov [finish],eax ; jmp NEXT
OVERFLOW: push ebx ; push ebx ; lea ecx,[esp+0Fh] ; push ecx ; lea edx,[esp+14h] ; push edx ; push eax
  mov ecx, offset RegisteredModes ; call 0x00743270 (_M_insert_overflow(finish, pair, __false_type(), 1, true))
  mov eax,[finish]
NEXT:
```
Block stride 0x57 bytes (0x0073B190, 0x0073B1E7, ...); block 1 (0x0073B139..0x0073B190) has no Dirty store and the first cmp
reads both globals; entry 70's join (0x0073C8EB) falls into the call-form blocks.

### Called-block template (entries 71..85), stride 0x25, e.g. entry 71 at 0x0073C8EB
```
  lea eax,[esp+8] ; push eax ; mov ecx, offset RegisteredModes
  mov Dirty, bl                                       ; previous Insert's Dirty=true
  mov [esp+0Ch], key ; mov [esp+10h], offset CModeX::Alloc
  call 0x00743B30 (vector::push_back(const value_type&))
```
(entry 82 at 0x0073CA82: Dirty store first, pair stored at [esp+8]/[esp+0Ch], then lea/push/mov ecx/call -- scheduling only)

### Tail (0x0073CB16): Update() with Dirty known true
```
  mov edx,[finish] ; mov ecx,[start] ; xor eax,eax ; mov al,[Compare 0x13BAD50]
  mov Dirty, bl                                       ; last Insert's Dirty=true
  push eax ; call 0x007441D0 (std::sort<pair*,CKeyPairCompareLess> __fastcall(begin,end,Compare))
  mov byte ptr [Dirty],0
  pop ebx ; mov esp,ebp ; pop ebp ; ret
```
No cold blocks, no jump tables.

## Registration table (n, block key-store address, key, EMode, Alloc address, class, push_back form)
```
1	0073b14a	0	MODE_NULL	0x00742430	NCreatureMode::CModeNull	inline
2	0073b1a6	4	MODE_CARRYING_WEAPON_STAND	0x0073A0A0	NCreatureMode::CModeCarryingWeaponStand	inline
3	0073b1fd	5	MODE_CARRYING_PLATE_STAND	0x007426F0	NCreatureMode::CModeCarryingPlateStand	inline
4	0073b254	6	MODE_CARRYING_GOODS_STAND	0x00742710	NCreatureMode::CModeCarryingGoodsStand	inline
5	0073b2ab	1	MODE_STAND	0x0073EBE0	NCreatureMode::CModeStandFront	inline
6	0073b2fa	1	MODE_STAND	0x0073EC10	NCreatureMode::CModeStandLeft	inline
7	0073b349	1	MODE_STAND	0x0073EC40	NCreatureMode::CModeStandRight	inline
8	0073b398	7	MODE_SNEAK	0x00742730	NCreatureMode::CModeCrouch	inline
9	0073b3ef	8	MODE_SIT_CHAIR	0x00742750	NCreatureMode::CModeSitChair	inline
10	0073b446	9	MODE_SIT_BENCH	0x00742790	NCreatureMode::CModeSitBench	inline
11	0073b49d	10	MODE_SIT_FLOOR	0x00742770	NCreatureMode::CModeSitFloor	inline
12	0073b4f4	13	MODE_SLEEP_BED	0x0073A130	NCreatureMode::CModeSleepBed	inline
13	0073b54b	12	MODE_BLOCK	0x007427B0	NCreatureMode::CModeBlock	inline
14	0073b5a2	14	MODE_COOK	0x00742630	NCreatureMode::CModeCook	inline
15	0073b5f9	46	MODE_HANDS_BOUND	0x00742650	NCreatureMode::CModeHandsBound	inline
16	0073b650	15	MODE_SCARED	0x007424F0	NCreatureMode::CModeScared	inline
17	0073b6a7	17	MODE_CREEPING	0x00742570	NCreatureMode::CModeCreeping	inline
18	0073b6fe	18	MODE_CHEERING	0x00742590	NCreatureMode::CModeCheering	inline
19	0073b755	19	MODE_HECKLING	0x007425B0	NCreatureMode::CModeHeckling	inline
20	0073b7ac	16	MODE_OUTRAGED	0x00742510	NCreatureMode::CModeOutraged	inline
21	0073b803	20	MODE_SAD	0x00742530	NCreatureMode::CModeSad	inline
22	0073b85a	21	MODE_ATTRACTED	0x00742550	NCreatureMode::CModeAttracted	inline
23	0073b8b1	22	MODE_HERO_IMITATION	0x007425D0	NCreatureMode::CModeHeroImitation	inline
24	0073b908	23	MODE_CURIOUS	0x007425F0	NCreatureMode::CModeCurious	inline
25	0073b95f	24	MODE_BULLY	0x00742610	NCreatureMode::CModeBully	inline
26	0073b9b6	25	MODE_STRAFE	0x007431C0	NCreatureMode::CModeStrafe	inline
27	0073ba0d	26	MODE_HOVER	0x00744250	NCreatureMode::CModeHoverNormal	inline
28	0073ba64	27	MODE_CONVERSATION	0x0073A350	NCreatureMode::CModeConversation	inline
29	0073babb	28	MODE_STAND_IN_ALL_DIRECTIONS	0x00744160	NCreatureMode::CModeWalkInAllDirections	inline
30	0073bb12	29	MODE_STUNNED	0x007427F0	NCreatureMode::CModeStunned	inline
31	0073bb69	30	MODE_UNDERGROUND	0x00742A20	NCreatureMode::CModeUnderground	inline
32	0073bbc0	3	MODE_STAND_BORED	0x007424B0	NCreatureMode::CModeStandBored	inline
33	0073bc17	2	MODE_STAND_HAPPY	0x00742490	NCreatureMode::CModeStandHappy	inline
34	0073bc6e	31	MODE_KNOCKED_DOWN	0x00742A40	NCreatureMode::CModeKnockedDown	inline
35	0073bcc5	32	MODE_FORCE_PUSHED	0x00742A60	NCreatureMode::CModeForcePushed	inline
36	0073bd1c	34	MODE_CARRYING_GENERIC_BOX	0x007426D0	NCreatureMode::CModeCarryingGenericBox	inline
37	0073bd73	35	MODE_BATTLE_CHARGE	0x00742AC0	NCreatureMode::CModeBattleCharge	inline
38	0073bdca	36	MODE_BERSERK	0x00742A80	NCreatureMode::CModeBerserk	inline
39	0073be21	37	MODE_ASSASSIN_RUSH	0x00742AE0	NCreatureMode::CModeAssassinRush	inline
40	0073be78	38	MODE_WADING	0x00742AA0	NCreatureMode::CModeWading	inline
41	0073becf	39	MODE_DRINKING_FROM_TANKARD	0x00742B00	NCreatureMode::CModeDrinkingFromTankard	inline
42	0073bf26	40	MODE_EATING_AT_TABLE	0x00742B20	NCreatureMode::CModeEatingAtTable	inline
43	0073bf7d	41	MODE_HOLD_JUG	0x00742B40	NCreatureMode::CModeHoldJug	inline
44	0073bfd4	42	MODE_SERVE_CUSTOMER	0x00742B60	NCreatureMode::CModeServeCustomer	inline
45	0073c02b	43	MODE_FILL_JUG	0x00742B80	NCreatureMode::CModeFillJug	inline
46	0073c082	44	MODE_DRUNK	0x00742BA0	NCreatureMode::CModeDrunk	inline
47	0073c0d9	45	MODE_ARMS_OFF_TABLE	0x00742BC0	NCreatureMode::CModeArmsOffTable	inline
48	0073c130	58	MODE_ROCK_TROLL_VULNERABLE	0x0073A1D0	NCreatureMode::CModeRockTrollVulnerable	inline
49	0073c187	49	MODE_JACK_OF_BLADES_FLOAT	0x00742850	NCreatureMode::CModeJackOfBladesFloat	inline
50	0073c1de	51	MODE_JACK_OF_BLADES_SCREAM	0x00742880	NCreatureMode::CModeJackOfBladesScream	inline
51	0073c235	50	MODE_JACK_OF_BLADES_FLASH	0x0073A2E0	NCreatureMode::CModeJackOfBladesFlash	inline
52	0073c28c	52	MODE_JACK_OF_BLADES_VULNERABLE	0x00742670	NCreatureMode::CModeJackOfBladesVulnerable	inline
53	0073c2e3	53	MODE_JACK_OF_BLADES_DEATH	0x007428B0	NCreatureMode::CModeJackOfBladesDeath	inline
54	0073c33a	54	MODE_THUNDER_LIGHTNING_STORM	0x007428E0	NCreatureMode::CModeThunderLightningStorm	inline
55	0073c391	55	MODE_SCORPION_KING_VULNERABLE	0x0073A210	NCreatureMode::CModeScorpionKingVulnerable	inline
56	0073c3e8	47	MODE_SCORPION_KING_TAIL_PLUNGE	0x0073A250	NCreatureMode::CModeScorpionKingTailPlunge	inline
57	0073c43f	57	MODE_CREATURE_IS_PROJECTILE	0x0073A190	NCreatureMode::CModeCreatureIsProjectile	inline
58	0073c496	56	MODE_SCORPION_KING_EXHAUSTED	0x0073A290	NCreatureMode::CModeScorpionKingExhausted	inline
59	0073c4ed	59	MODE_GUILDMASTER_CONCENTRATE	0x00742CC0	NCreatureMode::CModeGuildmasterConcentrate	inline
60	0073c544	60	MODE_GUILDMASTER_POWERUP_HEAL	0x007434E0	NCreatureMode::CModeGuildmasterPowerUpHeal	inline
61	0073c59b	61	MODE_GUILDMASTER_POWERUP_SHIELD	0x007435B0	NCreatureMode::CModeGuildmasterPowerUpShield	inline
62	0073c5f2	62	MODE_GUILDMASTER_POWERUP_SPEED	0x00743680	NCreatureMode::CModeGuildmasterPowerUpSpeed	inline
63	0073c649	63	MODE_BRIAR_ROSE_CONCENTRATE	0x007430D0	NCreatureMode::CModeBriarRoseConcentrate	inline
64	0073c6a0	67	MODE_DAZED	0x00742820	NCreatureMode::CModeDazed	inline
65	0073c6f7	83	MODE_VULNERABLE	0x00742690	NCreatureMode::CModeVulnerable	inline
66	0073c74e	84	MODE_UNDERWATER	0x007426B0	NCreatureMode::CModeUnderwater	inline
67	0073c7a5	69	MODE_DRAINED	0x00742C10	NCreatureMode::CModeDrained	inline
68	0073c7fc	68	MODE_SCREAMER_AGGRESSIVE	0x00742BE0	NCreatureMode::CModeScreamerAggressive	inline
69	0073c853	74	MODE_DRAGON_REAR_UP	0x00742F80	NCreatureMode::CModeDragonRearUp	inline
70	0073c8aa	75	MODE_DRAGON_FIRE_BEAM	0x0073A540	NCreatureMode::CModeDragonFireBeam	inline
71	0073c8fb	76	MODE_DRAGON_STRAFE	0x00743CF0	NCreatureMode::CModeDragonStrafe	call
72	0073c920	77	MODE_DRAGON_STRAFE_BURNINATE	0x00743D10	NCreatureMode::CModeDragonStrafeBurninate	call
73	0073c945	78	MODE_DRAGON_LANDED	0x00742F20	NCreatureMode::CModeDragonLanded	call
74	0073c96a	79	MODE_DRAGON_GET_HIT	0x00742F50	NCreatureMode::CModeDragonGetHit	call
75	0073c98f	80	MODE_DRAGON_FLYING	0x00743CC0	NCreatureMode::CModeDragonFlying	call
76	0073c9b4	71	MODE_DRAGON_HOVER	0x00742FA0	NCreatureMode::CModeDragonHover	call
77	0073c9d9	81	MODE_INTANGIBLE	0x007430A0	NCreatureMode::CModeIntangible	call
78	0073c9fe	86	MODE_CAGED	0x00742C60	NCreatureMode::CModeCaged	call
79	0073ca23	87	MODE_SCRIPTED	0x007424D0	NCreatureMode::CModeScripted	call
80	0073ca48	88	MODE_BASIC	0x00742460	NCreatureMode::CModeBasic	call
81	0073ca6d	91	MODE_CHARGE_FORCE_PUSH	0x00742CA0	NCreatureMode::CModeChargeForcePush	call
82	0073ca88	92	MODE_SUMMONER_DIE	0x00742C80	NCreatureMode::CModeSummonerDie	call
83	0073cab7	93	MODE_ICE_TROLL_ICICLE_ASSAULT	0x00743F90	NCreatureMode::CModeIceTrollGroundSmashIcicleAssault	call
84	0073cadc	70	MODE_ROLL	0x00742C40	NCreatureMode::CModeRollAround	call
85	0073cb01	95	MODE_HIT_BY_EPIC_SPELL	0x00743080	NCreatureMode::CModeHitByEpicSpell	call
```
