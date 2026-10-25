// CAIBrain::DestroyStateGroupInfo() -- the dynamic atexit destructor for the static `CAIBrain::StateGroupInfo[203]` table: destroy the 203 (state-group name, allocator function) entries through the vector destructor iterator
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0122A20A, 23 bytes (+15 int3), in `.text$yd`; registered by the table's
//   dynamic initializer (the 203 `CAIStateGroupInfo(CCharString("SG_..."), Alloc)` constructions ending at 0x01221830
//   `push 0x0122A20A ; call atexit`). VC7.1 names the pair `$E1` / `$E2` in the TU.
// Identity: the 8-byte entries at 0x013BB008 (0x013BB008 + 203*8 = 0x013BB660) are read by CAIBrain::InitStateGroups
//   0x0088BF30 (the first entry is "SG_MINION_WANDER", the last the empty name with a null allocator). FableWin.pdb: "`dynamic atexit destructor for
//   'CAIBrain::StateGroupInfo''"; Ego_r.pdb: CAIStateGroupInfo { CCharString StateGroupName +0;
//   CAIStateGroupBase* (__fastcall *AllocFunc)() +4 } with `??0CAIStateGroupInfo@@QAE@VCCharString@@P6IPAVCAIStateGroupBase@@XZ@Z`.
//   The catalog name "Init_ParticleMorphsVector" is invented (the TU's neighbours are CParticleMorphs code).
// Callees: `vector destructor iterator' @ 0x00401400 (`__stdcall` (array, 8, 203, dtor); the manifest calls it the
//   constructor iterator), ~CAIStateGroupInfo @ 0x008FE93B (compiler-generated: `jmp ~CCharString`).
// Shape: the compiler's array teardown with no /EH: four pushes (dtor address, count 0xCB, size 8, array) and the call.
//   Two things are needed: the destructor is USER-DECLARED and out of line (PDB `??1CAIStateGroupInfo@@QAE@XZ`; with the
//   implicit one VC7.1 inlines ~CCharString into a counted loop), and the TU is size-optimised (`#pragma optimize("s",
//   on)`; under /O2 VC7.1 expands the iterator into an inline backwards loop calling the destructor). The unaligned
//   address (0x0122A20A, packed after the previous thunk) is the same /Os signature.
#pragma optimize("s", on)
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
};

class CAIStateGroupBase;

class CAIStateGroupInfo {                                // 8 bytes (Ego_r.pdb)
public:
    CCharString StateGroupName;                          // +0x00
    CAIStateGroupBase* (__fastcall* AllocFunc)();        // +0x04
    CAIStateGroupInfo(CCharString name, CAIStateGroupBase* (__fastcall* alloc)());   // 0x008FD21F
    ~CAIStateGroupInfo();                                // 0x008FE93B (declared, defined out of line)
};

// The table's allocator functions (descriptive names: each is named after the first state group that uses it).
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_WANDER();             // 0x00901ED3
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_SLEEP();              // 0x00901F0D
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_GO_TO_BED();          // 0x00901EF7
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_WORK_IN_INN();        // 0x008FEBE1
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_FIND_PERSONAL_SPACE();// 0x00901F31
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_REACT_TO_PLAYER_INTERACTION();// 0x00901F47
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_GUARD_HEARD_YELL_FOR_HELP();// 0x008FF3D8
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_GUARD_REACT_TO_PLAYER_WEAPON();// 0x00902294
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_GUARD_REACT_TO_BODY_FOUND_MESSAGE();// 0x008FF462
CAIStateGroupBase* __fastcall AllocStateGroup_SG_BANDIT_SIT_ROUND_CAMPFIRE(); // 0x009022AA
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_REACT_TO_CRIMINAL();  // 0x00901F6B
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_GUARD_REACT_TO_CRIMINAL();// 0x00901F81
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_GUARD_REACT_TO_BRIBE();// 0x00901F97
CAIStateGroupBase* __fastcall AllocStateGroup_SG_PERFORM_ACTION();            // 0x00902021
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_FACE_HERO_READY_FOR_INTERACTION();// 0x0090209E
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_PUT_OUT_FIRES();      // 0x009020B4
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_WATCH_FIRES();        // 0x009020CA
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_REACT_TO_ALERT();     // 0x008FF178
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_REACT_TO_ENEMY();     // 0x0090210C
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_REACT_TO_CORPSE();    // 0x009020E0
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_REACT_TO_UNCONSCIOUS_PERSON();// 0x009020F6
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_HAS_INFORMATION();    // 0x008FF1B8
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_SIT_ON_SEAT();        // 0x00902D71
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_SIT_AT_WORK();        // 0x0090215C
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_SIT_AT_HOME();        // 0x009021F9
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_SIT_AT_INN();         // 0x00902180
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_SIT_AT_SCHOOL();      // 0x009021A4
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_SIT_OUTSIDE();        // 0x00902D95
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_LISTEN_TO_STORY();    // 0x008FF31A
CAIStateGroupBase* __fastcall AllocStateGroup_SG_SIT_ROUND_CAMPFIRE();        // 0x0090221D
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_CURIOSITY();          // 0x00901FAD
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_GOSSIP();             // 0x008FEE09
CAIStateGroupBase* __fastcall AllocStateGroup_SG_BANDIT_GOSSIP();             // 0x008FEE49
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_MOTHER_PROTECT_FAMILY();// 0x00901FD1
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_KID_ANNOY();          // 0x00901FE7
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_KID_GROUP_TAG();      // 0x00901FFD
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_KID_CALLED_TO_MOTHER();// 0x008FEF4B
CAIStateGroupBase* __fastcall AllocStateGroup_SG_CREATURE_FIND_COVER();       // 0x009034B5
CAIStateGroupBase* __fastcall AllocStateGroup_SG_CHICKEN_PANIC();             // 0x008FF760
CAIStateGroupBase* __fastcall AllocStateGroup_SG_CHICKEN_BEG();               // 0x008FF7A0
CAIStateGroupBase* __fastcall AllocStateGroup_SG_FIREFLY_WANDER();            // 0x008FF80E
CAIStateGroupBase* __fastcall AllocStateGroup_SG_BUTTERFLY_WANDER();          // 0x00902318
CAIStateGroupBase* __fastcall AllocStateGroup_SG_SMALL_BIRD_WANDER();         // 0x008FF84E
CAIStateGroupBase* __fastcall AllocStateGroup_SG_BIRD_FLY_TO_FOOD();          // 0x008FF88E
CAIStateGroupBase* __fastcall AllocStateGroup_SG_BIRD_GROUND_WANDER();        // 0x008FF8CE
CAIStateGroupBase* __fastcall AllocStateGroup_SG_BIRD_STARTLED();             // 0x008FF90E
CAIStateGroupBase* __fastcall AllocStateGroup_SG_GUARD();                     // 0x008FF620
CAIStateGroupBase* __fastcall AllocStateGroup_SG_WANDER();                    // 0x008FF660
CAIStateGroupBase* __fastcall AllocStateGroup_SG_CHICKEN_WANDER();            // 0x008FF6A0
CAIStateGroupBase* __fastcall AllocStateGroup_SG_WANDER_AROUND_INITIAL_POS(); // 0x008FF6E0
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MELEE_COMBAT_NEARBY();       // 0x00902DB9
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MELEE_COMBAT_NEARBY2();      // 0x0090236E
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MELEE_COMBAT_NEARBY_ATTACK_WHEN_VERY_CLOSE();// 0x00902392
CAIStateGroupBase* __fastcall AllocStateGroup_SG_NON_VILLAGE_GUARD_PATROL();  // 0x00902302
CAIStateGroupBase* __fastcall AllocStateGroup_SG_SEEK_ENEMY();                // 0x009022C0
CAIStateGroupBase* __fastcall AllocStateGroup_SG_SEEK_SCRIPT_TARGET();        // 0x009022D6
CAIStateGroupBase* __fastcall AllocStateGroup_SG_STEALTH_SEEK_ENEMY();        // 0x009022EC
CAIStateGroupBase* __fastcall AllocStateGroup_SG_CREATURE_RANGED_COMBAT_USE_PREFERRED_FIRE_POINT();// 0x00902A48
CAIStateGroupBase* __fastcall AllocStateGroup_SG_CREATURE_RANGED_COMBAT_USE_WILL_SHIELD();// 0x00900E6C
CAIStateGroupBase* __fastcall AllocStateGroup_SG_VILLAGE_TASK_TORCH();        // 0x009023B6
CAIStateGroupBase* __fastcall AllocStateGroup_SG_VILLAGE_TASK_PATROL();       // 0x009023CC
CAIStateGroupBase* __fastcall AllocStateGroup_SG_VILLAGE_TASK_SECURITY_SWEEP();// 0x009023E2
CAIStateGroupBase* __fastcall AllocStateGroup_SG_VILLAGE_TASK_PICK_CROPS();   // 0x009023F8
CAIStateGroupBase* __fastcall AllocStateGroup_SG_VILLAGE_TASK_HORN();         // 0x0090240E
CAIStateGroupBase* __fastcall AllocStateGroup_SG_KID_WANDER_NEAR_KIDS();      // 0x008FFB64
CAIStateGroupBase* __fastcall AllocStateGroup_SG_KID_WATCH_HOPSCOTCH();       // 0x00902424
CAIStateGroupBase* __fastcall AllocStateGroup_SG_KID_PLAY_HOPSCOTCH();        // 0x0090243A
CAIStateGroupBase* __fastcall AllocStateGroup_SG_KID_HURT_SELF_RUN_TO_MOTHER();// 0x008FFC38
CAIStateGroupBase* __fastcall AllocStateGroup_SG_KID_REACT_TO_BLOCKAGE();     // 0x00902450
CAIStateGroupBase* __fastcall AllocStateGroup_SG_IDLE_AT_HOME();              // 0x008FFE98
CAIStateGroupBase* __fastcall AllocStateGroup_SG_WORK_AT_SCHOOL();            // 0x00902466
CAIStateGroupBase* __fastcall AllocStateGroup_SG_TELL_STORY();                // 0x0090248A
CAIStateGroupBase* __fastcall AllocStateGroup_SG_WORK_IN_WAREHOUSE();         // 0x009024AE
CAIStateGroupBase* __fastcall AllocStateGroup_SG_WORK_SAWING_WOOD();          // 0x009024C4
CAIStateGroupBase* __fastcall AllocStateGroup_SG_WORK_HAMMER_ANVIL();         // 0x009024DA
CAIStateGroupBase* __fastcall AllocStateGroup_SG_THROW_LOGS_ON_FIRE();        // 0x008FFE58
CAIStateGroupBase* __fastcall AllocStateGroup_SG_STAY_NEAR_SCHOOL();          // 0x008FFE18
CAIStateGroupBase* __fastcall AllocStateGroup_SG_CLEAN_HOUSE();               // 0x008FFEF4
CAIStateGroupBase* __fastcall AllocStateGroup_SG_CLEAN_WORK();                // 0x008FFF34
CAIStateGroupBase* __fastcall AllocStateGroup_SG_SIT_FOR_MEAL();              // 0x009024F0
CAIStateGroupBase* __fastcall AllocStateGroup_SG_PREPARE_MEAL();              // 0x00902506
CAIStateGroupBase* __fastcall AllocStateGroup_SG_GAZE_AT_HOME();              // 0x008FDBDE
CAIStateGroupBase* __fastcall AllocStateGroup_SG_GAZE_AT_WORK();              // 0x008FDC07
CAIStateGroupBase* __fastcall AllocStateGroup_SG_GAZE_OUTSIDE();              // 0x008FDBB5
CAIStateGroupBase* __fastcall AllocStateGroup_SG_WANDER_AROUND_SHOPS();       // 0x00902556
CAIStateGroupBase* __fastcall AllocStateGroup_SG_STAY_NEAR_FOOD_HALL();       // 0x0090256C
CAIStateGroupBase* __fastcall AllocStateGroup_SG_GO_TO_PUB();                 // 0x00902582
CAIStateGroupBase* __fastcall AllocStateGroup_SG_SIT_IN_PUB();                // 0x0090259B
CAIStateGroupBase* __fastcall AllocStateGroup_SG_HOME_CANDLE();               // 0x0090251C
CAIStateGroupBase* __fastcall AllocStateGroup_SG_WORK_CANDLE();               // 0x00902532
CAIStateGroupBase* __fastcall AllocStateGroup_SG_SEEK_PERCEIVED_THING();      // 0x009003D2
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_FOLLOW_PLAYER();      // 0x00902644
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_HAWKING_FROM_STALL(); // 0x00900269
CAIStateGroupBase* __fastcall AllocStateGroup_SG_BUY_FROM_SELLER();           // 0x008FDD23
CAIStateGroupBase* __fastcall AllocStateGroup_SG_SELL_TO_BUYER();             // 0x009025B1
CAIStateGroupBase* __fastcall AllocStateGroup_SG_HANDY_MAN();                 // 0x009002EF
CAIStateGroupBase* __fastcall AllocStateGroup_SG_LOOK_AT_INTERESTING_THINGS();// 0x0090007C
CAIStateGroupBase* __fastcall AllocStateGroup_SG_GAME_MASTER();               // 0x009025C7
CAIStateGroupBase* __fastcall AllocStateGroup_SG_STAND_STILL();               // 0x00900460
CAIStateGroupBase* __fastcall AllocStateGroup_SG_BALVERINE_WANDER();          // 0x009004A0
CAIStateGroupBase* __fastcall AllocStateGroup_SG_BALVERINE_CORNERED();        // 0x0090265A
CAIStateGroupBase* __fastcall AllocStateGroup_SG_BALVERINE_JUMP_AT_FLEEING_ENEMY();// 0x00902670
CAIStateGroupBase* __fastcall AllocStateGroup_SG_NYMPH_FLOAT_AROUND();        // 0x00902686
CAIStateGroupBase* __fastcall AllocStateGroup_SG_NYMPH_ATTACK();              // 0x00900607
CAIStateGroupBase* __fastcall AllocStateGroup_SG_HOVER_WANDER();              // 0x0090295A
CAIStateGroupBase* __fastcall AllocStateGroup_SG_HOVER_WANDER_AROUND_THING(); // 0x00902E5F
CAIStateGroupBase* __fastcall AllocStateGroup_SG_HOVER_ATTACK();              // 0x0090297E
CAIStateGroupBase* __fastcall AllocStateGroup_SG_HOVER_ATTACK_UNLESS_TARGET_FLEEING();// 0x009029A2
CAIStateGroupBase* __fastcall AllocStateGroup_SG_HOVER_KEEP_IN_AREA();        // 0x009029C6
CAIStateGroupBase* __fastcall AllocStateGroup_SG_HOVER_ATTACK_AREA();         // 0x009029EA
CAIStateGroupBase* __fastcall AllocStateGroup_SG_HOVER_HORNET_QUEEN_TAKE_DAMAGE();// 0x00902A0E
CAIStateGroupBase* __fastcall AllocStateGroup_SG_HOVER_HORNET_QUEEN();        // 0x00903351
CAIStateGroupBase* __fastcall AllocStateGroup_SG_HOVER_ALL_AROUND_TEST();     // 0x00900CB1
CAIStateGroupBase* __fastcall AllocStateGroup_SG_EARTH_TROLL();               // 0x00902620
CAIStateGroupBase* __fastcall AllocStateGroup_SG_ROCK_TROLL();                // 0x00902DDD
CAIStateGroupBase* __fastcall AllocStateGroup_SG_ICE_TROLL_ICICLE_ATTACK();   // 0x00902B32
CAIStateGroupBase* __fastcall AllocStateGroup_SG_ICE_TROLL_POUND_GROUND_ATTACK();// 0x00902B48
CAIStateGroupBase* __fastcall AllocStateGroup_SG_ICE_TROLL_ICICLE_ASSAULT();  // 0x00902B5E
CAIStateGroupBase* __fastcall AllocStateGroup_SG_SCORPION_KING();             // 0x00903424
CAIStateGroupBase* __fastcall AllocStateGroup_SG_JACK_OF_BLADES_STAGE1();     // 0x009034D9
CAIStateGroupBase* __fastcall AllocStateGroup_SG_JACK_OF_BLADES_STAGE2();     // 0x00903280
CAIStateGroupBase* __fastcall AllocStateGroup_SG_JACK_OF_BLADES_STAGE3();     // 0x009032BC
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MAZE_TELEPORT();             // 0x00902E01
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MAZE_RANGED_COMBAT();        // 0x009026E7
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MAZE_DEFENSIVE_MELEE_COMBAT();// 0x0090270B
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MAZE_SHIELDED_CHASE();       // 0x0090272F
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MAZE_TUTORIAL_BATTLE();      // 0x00902753
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MAZE_TUTORIAL_BATTLE_TELEPORT();// 0x00902777
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MAZE_TUTORIAL_BATTLE_RESPOND_TO_LIGHTNING();// 0x0090279B
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MAZE_TUTORIAL_BATTLE_USE_SHIELD();// 0x009027BF
CAIStateGroupBase* __fastcall AllocStateGroup_SG_GUILDMASTER();               // 0x009032FC
CAIStateGroupBase* __fastcall AllocStateGroup_SG_BRIAR_ROSE();                // 0x009027E3
CAIStateGroupBase* __fastcall AllocStateGroup_SG_BRIAR_ROSE_WILL_USE();       // 0x009027F9
CAIStateGroupBase* __fastcall AllocStateGroup_SG_VERSUS_ROCK_TROLL();         // 0x00902E75
CAIStateGroupBase* __fastcall AllocStateGroup_SG_VERSUS_SCORPION_KING();      // 0x00902BA2
CAIStateGroupBase* __fastcall AllocStateGroup_SG_CREATURE_RANGED_COMBAT_TWO();// 0x00902A32
CAIStateGroupBase* __fastcall AllocStateGroup_SG_CREATURE_RANGED_COMBAT_USE_COVER();// 0x009033E4
CAIStateGroupBase* __fastcall AllocStateGroup_SG_CREATURE_ADVANCE_IN_COVER(); // 0x009033A4
CAIStateGroupBase* __fastcall AllocStateGroup_SG_CREATURE_RANGED_COMBAT_FIRE_AT_SPECIFIC_TARGETS();// 0x00902A5E
CAIStateGroupBase* __fastcall AllocStateGroup_SG_CREATURE_TEST_TEST_TEST();   // 0x00900EAC
CAIStateGroupBase* __fastcall AllocStateGroup_SG_CREATURE_GO_UNDERGROUND();   // 0x00900FFA
CAIStateGroupBase* __fastcall AllocStateGroup_SG_CREATURE_IDLE_UNDERGROUND(); // 0x0090103A
CAIStateGroupBase* __fastcall AllocStateGroup_SG_CREATURE_SEEK_ENEMY_UNDERGROUND();// 0x00902A8A
CAIStateGroupBase* __fastcall AllocStateGroup_SG_BEETLE_TOGGLE_HOVER();       // 0x009011E6
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_BEHAVIOUR_GREET_CREATURE();// 0x0090280F
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_BEHAVIOUR_SAD();      // 0x00902E25
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_BEHAVIOUR_PODIUM_EMOTE_NEUTRAL();// 0x00902E3B
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_MOTHER_CONSOLE_CHILD();// 0x00900DDE
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_REACT_TO_BURGLARY();  // 0x009028A1
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_REACT_TO_TRESPASSING();// 0x009028B7
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_REACT_TO_NOISE();     // 0x00900B01
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_REACT_TO_APOLOGY();   // 0x00900BAF
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_REACT_TO_CRIME();     // 0x009028CD
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_REACT_TO_CRIME_FROM_SLEEP();// 0x009028E3
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_LEAVE_REGION();       // 0x00900D9E
CAIStateGroupBase* __fastcall AllocStateGroup_SG_REACT_TO_DEAD_LEADER();      // 0x00902A74
CAIStateGroupBase* __fastcall AllocStateGroup_SG_BOASTING_CROWD_MEMBER();     // 0x00902AA0
CAIStateGroupBase* __fastcall AllocStateGroup_SG_OPINION_REACTION_ROOT();     // 0x00901139
CAIStateGroupBase* __fastcall AllocStateGroup_SG_DEED_REACTION_ROOT();        // 0x00902AB6
CAIStateGroupBase* __fastcall AllocStateGroup_SG_DEED_REACTION_ROOT_FROM_SLEEP();// 0x00902ACC
CAIStateGroupBase* __fastcall AllocStateGroup_SG_BANDIT_KING_CHARGE();        // 0x00902AF0
CAIStateGroupBase* __fastcall AllocStateGroup_SG_BANDIT_KING_LUNGE();         // 0x00902B06
CAIStateGroupBase* __fastcall AllocStateGroup_SG_BANDIT_KING_THROW_WEAPONS(); // 0x00902B1C
CAIStateGroupBase* __fastcall AllocStateGroup_SG_JACK_DRAGON();               // 0x0090269C
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_FOLLOW_THING();       // 0x00902BB8
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_WALK_TO_MARKER();     // 0x009014DA
CAIStateGroupBase* __fastcall AllocStateGroup_SG_TAVERN_SERVER();             // 0x00902BE4
CAIStateGroupBase* __fastcall AllocStateGroup_SG_TAVERN_KEEPER();             // 0x00902BFD
CAIStateGroupBase* __fastcall AllocStateGroup_SG_STEAL_STEALABLE_ITEM();      // 0x00901564
CAIStateGroupBase* __fastcall AllocStateGroup_SG_RECOVER_STEALABLE_ITEM();    // 0x00901603
CAIStateGroupBase* __fastcall AllocStateGroup_SG_CARRY_CRATE_AND_FOLLOW();    // 0x00902BCE
CAIStateGroupBase* __fastcall AllocStateGroup_SG_SLEEP_ON_FLOOR();            // 0x00902C13
CAIStateGroupBase* __fastcall AllocStateGroup_SG_WHISPER_RETREAT();           // 0x00902C29
CAIStateGroupBase* __fastcall AllocStateGroup_SG_WHISPER_POTION_COMBAT();     // 0x00903479
CAIStateGroupBase* __fastcall AllocStateGroup_SG_THUNDER();                   // 0x00902C3F
CAIStateGroupBase* __fastcall AllocStateGroup_SG_FREEZE();                    // 0x0090192A
CAIStateGroupBase* __fastcall AllocStateGroup_SG_DRUNK();                     // 0x0090196A
CAIStateGroupBase* __fastcall AllocStateGroup_SG_SERVE_FOOD_AT_HOME();        // 0x00902CB8
CAIStateGroupBase* __fastcall AllocStateGroup_SG_SERVE_FOOD_AT_TAVERN();      // 0x00901A49
CAIStateGroupBase* __fastcall AllocStateGroup_SG_EAT_AT_HOME();               // 0x00902E99
CAIStateGroupBase* __fastcall AllocStateGroup_SG_EAT_AT_TAVERN();             // 0x00902C83
CAIStateGroupBase* __fastcall AllocStateGroup_SG_SCREAMER_PURSUE_TARGET();    // 0x00902CED
CAIStateGroupBase* __fastcall AllocStateGroup_SG_KRAKEN_ATTACK();             // 0x00901AD3
CAIStateGroupBase* __fastcall AllocStateGroup_SG_KRAKEN_TENTACLE_IDLE();      // 0x00901B13
CAIStateGroupBase* __fastcall AllocStateGroup_SG_KRAKEN_TENTACLE_RISE();      // 0x00901B53
CAIStateGroupBase* __fastcall AllocStateGroup_SG_KRAKEN_TENTACLE_ATTACK();    // 0x00901B93
CAIStateGroupBase* __fastcall AllocStateGroup_SG_WIFE_GIVE_YOU_GIFT();        // 0x00901BD3
CAIStateGroupBase* __fastcall AllocStateGroup_SG_OPEN_AND_CLOSE_SHOP();       // 0x00901C13
CAIStateGroupBase* __fastcall AllocStateGroup_SG_SHOPKEEPER_WATCH_PLAYER();   // 0x008FE762
CAIStateGroupBase* __fastcall AllocStateGroup_SG_SETUP_WARES();               // 0x00901C53
CAIStateGroupBase* __fastcall AllocStateGroup_SG_BANDIT_PLAY_DICE();          // 0x00901C93
CAIStateGroupBase* __fastcall AllocStateGroup_SG_BANDIT_PLOT();               // 0x00901CD3
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_GUARD_GREET_PLAYER(); // 0x00901D13
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_GUARD_FOLLOW_PLAYER();// 0x008FED9B
CAIStateGroupBase* __fastcall AllocStateGroup_SG_MINION_GUARD_BRIBED();       // 0x00901D53
CAIStateGroupBase* __fastcall AllocStateGroup_SG_APPRENTICE_HERO();           // 0x00901D93
CAIStateGroupBase* __fastcall AllocStateGroup_SG_SKILL_PRACTICE();            // 0x00901DC5
CAIStateGroupBase* __fastcall AllocStateGroup_SG_ON_HERO_PUSH();              // 0x00901E05
CAIStateGroupBase* __fastcall AllocStateGroup_SG_AVOID_REGION_EXIT();         // 0x00901E45
CAIStateGroupBase* __fastcall AllocStateGroup_SG_SUMMONER_LIGHTNING_ATTACK(); // 0x00902EBD
CAIStateGroupBase* __fastcall AllocStateGroup_SG_SUMMONER_SUMMON();           // 0x00902D5B
CAIStateGroupBase* __fastcall AllocStateGroup_SG_SUMMONER_CHARGE();           // 0x00902F17

class CAIBrain {
public:
    static CAIStateGroupInfo StateGroupInfo[203];        // 0x013BB008
};

// The 203 entries in retail order (names and allocator addresses read from the initializer 0x0121F86F).
CAIStateGroupInfo CAIBrain::StateGroupInfo[203] = {
    CAIStateGroupInfo(CCharString("SG_MINION_WANDER", -1), AllocStateGroup_SG_MINION_WANDER),
    CAIStateGroupInfo(CCharString("SG_MINION_SLEEP", -1), AllocStateGroup_SG_MINION_SLEEP),
    CAIStateGroupInfo(CCharString("SG_MINION_GO_TO_BED", -1), AllocStateGroup_SG_MINION_GO_TO_BED),
    CAIStateGroupInfo(CCharString("SG_MINION_WORK_IN_INN", -1), AllocStateGroup_SG_MINION_WORK_IN_INN),
    CAIStateGroupInfo(CCharString("SG_MINION_FIND_PERSONAL_SPACE", -1), AllocStateGroup_SG_MINION_FIND_PERSONAL_SPACE),
    CAIStateGroupInfo(CCharString("SG_MINION_REACT_TO_PLAYER_INTERACTION", -1), AllocStateGroup_SG_MINION_REACT_TO_PLAYER_INTERACTION),
    CAIStateGroupInfo(CCharString("SG_MINION_GUARD_HEARD_YELL_FOR_HELP", -1), AllocStateGroup_SG_MINION_GUARD_HEARD_YELL_FOR_HELP),
    CAIStateGroupInfo(CCharString("SG_MINION_GUARD_REACT_TO_PLAYER_WEAPON", -1), AllocStateGroup_SG_MINION_GUARD_REACT_TO_PLAYER_WEAPON),
    CAIStateGroupInfo(CCharString("SG_MINION_GUARD_REACT_TO_BODY_FOUND_MESSAGE", -1), AllocStateGroup_SG_MINION_GUARD_REACT_TO_BODY_FOUND_MESSAGE),
    CAIStateGroupInfo(CCharString("SG_BANDIT_SIT_ROUND_CAMPFIRE", -1), AllocStateGroup_SG_BANDIT_SIT_ROUND_CAMPFIRE),
    CAIStateGroupInfo(CCharString("SG_MINION_REACT_TO_CRIMINAL", -1), AllocStateGroup_SG_MINION_REACT_TO_CRIMINAL),
    CAIStateGroupInfo(CCharString("SG_MINION_GUARD_REACT_TO_CRIMINAL", -1), AllocStateGroup_SG_MINION_GUARD_REACT_TO_CRIMINAL),
    CAIStateGroupInfo(CCharString("SG_MINION_GUARD_REACT_TO_BRIBE", -1), AllocStateGroup_SG_MINION_GUARD_REACT_TO_BRIBE),
    CAIStateGroupInfo(CCharString("SG_PERFORM_ACTION", -1), AllocStateGroup_SG_PERFORM_ACTION),
    CAIStateGroupInfo(CCharString("SG_MINION_FACE_HERO_READY_FOR_INTERACTION", -1), AllocStateGroup_SG_MINION_FACE_HERO_READY_FOR_INTERACTION),
    CAIStateGroupInfo(CCharString("SG_MINION_PUT_OUT_FIRES", -1), AllocStateGroup_SG_MINION_PUT_OUT_FIRES),
    CAIStateGroupInfo(CCharString("SG_MINION_WATCH_FIRES", -1), AllocStateGroup_SG_MINION_WATCH_FIRES),
    CAIStateGroupInfo(CCharString("SG_MINION_REACT_TO_ALERT", -1), AllocStateGroup_SG_MINION_REACT_TO_ALERT),
    CAIStateGroupInfo(CCharString("SG_MINION_REACT_TO_ENEMY", -1), AllocStateGroup_SG_MINION_REACT_TO_ENEMY),
    CAIStateGroupInfo(CCharString("SG_MINION_REACT_TO_CORPSE", -1), AllocStateGroup_SG_MINION_REACT_TO_CORPSE),
    CAIStateGroupInfo(CCharString("SG_MINION_REACT_TO_UNCONSCIOUS_PERSON", -1), AllocStateGroup_SG_MINION_REACT_TO_UNCONSCIOUS_PERSON),
    CAIStateGroupInfo(CCharString("SG_MINION_HAS_INFORMATION", -1), AllocStateGroup_SG_MINION_HAS_INFORMATION),
    CAIStateGroupInfo(CCharString("SG_MINION_SIT_ON_SEAT", -1), AllocStateGroup_SG_MINION_SIT_ON_SEAT),
    CAIStateGroupInfo(CCharString("SG_MINION_SIT_AT_WORK", -1), AllocStateGroup_SG_MINION_SIT_AT_WORK),
    CAIStateGroupInfo(CCharString("SG_MINION_SIT_AT_HOME", -1), AllocStateGroup_SG_MINION_SIT_AT_HOME),
    CAIStateGroupInfo(CCharString("SG_MINION_SIT_AT_INN", -1), AllocStateGroup_SG_MINION_SIT_AT_INN),
    CAIStateGroupInfo(CCharString("SG_MINION_SIT_AT_SCHOOL", -1), AllocStateGroup_SG_MINION_SIT_AT_SCHOOL),
    CAIStateGroupInfo(CCharString("SG_MINION_SIT_OUTSIDE", -1), AllocStateGroup_SG_MINION_SIT_OUTSIDE),
    CAIStateGroupInfo(CCharString("SG_MINION_LISTEN_TO_STORY", -1), AllocStateGroup_SG_MINION_LISTEN_TO_STORY),
    CAIStateGroupInfo(CCharString("SG_SIT_ROUND_CAMPFIRE", -1), AllocStateGroup_SG_SIT_ROUND_CAMPFIRE),
    CAIStateGroupInfo(CCharString("SG_MINION_CURIOSITY", -1), AllocStateGroup_SG_MINION_CURIOSITY),
    CAIStateGroupInfo(CCharString("SG_MINION_GOSSIP", -1), AllocStateGroup_SG_MINION_GOSSIP),
    CAIStateGroupInfo(CCharString("SG_BANDIT_GOSSIP", -1), AllocStateGroup_SG_BANDIT_GOSSIP),
    CAIStateGroupInfo(CCharString("SG_MINION_MOTHER_PROTECT_FAMILY", -1), AllocStateGroup_SG_MINION_MOTHER_PROTECT_FAMILY),
    CAIStateGroupInfo(CCharString("SG_MINION_KID_ANNOY", -1), AllocStateGroup_SG_MINION_KID_ANNOY),
    CAIStateGroupInfo(CCharString("SG_MINION_KID_GROUP_TAG", -1), AllocStateGroup_SG_MINION_KID_GROUP_TAG),
    CAIStateGroupInfo(CCharString("SG_MINION_KID_CALLED_TO_MOTHER", -1), AllocStateGroup_SG_MINION_KID_CALLED_TO_MOTHER),
    CAIStateGroupInfo(CCharString("SG_CREATURE_FIND_COVER", -1), AllocStateGroup_SG_CREATURE_FIND_COVER),
    CAIStateGroupInfo(CCharString("SG_CHICKEN_PANIC", -1), AllocStateGroup_SG_CHICKEN_PANIC),
    CAIStateGroupInfo(CCharString("SG_CHICKEN_BEG", -1), AllocStateGroup_SG_CHICKEN_BEG),
    CAIStateGroupInfo(CCharString("SG_FIREFLY_WANDER", -1), AllocStateGroup_SG_FIREFLY_WANDER),
    CAIStateGroupInfo(CCharString("SG_BUTTERFLY_WANDER", -1), AllocStateGroup_SG_BUTTERFLY_WANDER),
    CAIStateGroupInfo(CCharString("SG_SMALL_BIRD_WANDER", -1), AllocStateGroup_SG_SMALL_BIRD_WANDER),
    CAIStateGroupInfo(CCharString("SG_BIRD_FLY_TO_FOOD", -1), AllocStateGroup_SG_BIRD_FLY_TO_FOOD),
    CAIStateGroupInfo(CCharString("SG_BIRD_GROUND_WANDER", -1), AllocStateGroup_SG_BIRD_GROUND_WANDER),
    CAIStateGroupInfo(CCharString("SG_BIRD_STARTLED", -1), AllocStateGroup_SG_BIRD_STARTLED),
    CAIStateGroupInfo(CCharString("SG_GUARD", -1), AllocStateGroup_SG_GUARD),
    CAIStateGroupInfo(CCharString("SG_WANDER", -1), AllocStateGroup_SG_WANDER),
    CAIStateGroupInfo(CCharString("SG_CHICKEN_WANDER", -1), AllocStateGroup_SG_CHICKEN_WANDER),
    CAIStateGroupInfo(CCharString("SG_WANDER_AROUND_INITIAL_POS", -1), AllocStateGroup_SG_WANDER_AROUND_INITIAL_POS),
    CAIStateGroupInfo(CCharString("SG_MELEE_COMBAT_NEARBY", -1), AllocStateGroup_SG_MELEE_COMBAT_NEARBY),
    CAIStateGroupInfo(CCharString("SG_MELEE_COMBAT_NEARBY2", -1), AllocStateGroup_SG_MELEE_COMBAT_NEARBY2),
    CAIStateGroupInfo(CCharString("SG_MELEE_COMBAT_NEARBY_ATTACK_WHEN_VERY_CLOSE", -1), AllocStateGroup_SG_MELEE_COMBAT_NEARBY_ATTACK_WHEN_VERY_CLOSE),
    CAIStateGroupInfo(CCharString("SG_NON_VILLAGE_GUARD_PATROL", -1), AllocStateGroup_SG_NON_VILLAGE_GUARD_PATROL),
    CAIStateGroupInfo(CCharString("SG_SEEK_ENEMY", -1), AllocStateGroup_SG_SEEK_ENEMY),
    CAIStateGroupInfo(CCharString("SG_SEEK_SCRIPT_TARGET", -1), AllocStateGroup_SG_SEEK_SCRIPT_TARGET),
    CAIStateGroupInfo(CCharString("SG_STEALTH_SEEK_ENEMY", -1), AllocStateGroup_SG_STEALTH_SEEK_ENEMY),
    CAIStateGroupInfo(CCharString("SG_CREATURE_RANGED_COMBAT_USE_PREFERRED_FIRE_POINT", -1), AllocStateGroup_SG_CREATURE_RANGED_COMBAT_USE_PREFERRED_FIRE_POINT),
    CAIStateGroupInfo(CCharString("SG_CREATURE_RANGED_COMBAT_USE_WILL_SHIELD", -1), AllocStateGroup_SG_CREATURE_RANGED_COMBAT_USE_WILL_SHIELD),
    CAIStateGroupInfo(CCharString("SG_VILLAGE_TASK_TORCH", -1), AllocStateGroup_SG_VILLAGE_TASK_TORCH),
    CAIStateGroupInfo(CCharString("SG_VILLAGE_TASK_PATROL", -1), AllocStateGroup_SG_VILLAGE_TASK_PATROL),
    CAIStateGroupInfo(CCharString("SG_VILLAGE_TASK_SECURITY_SWEEP", -1), AllocStateGroup_SG_VILLAGE_TASK_SECURITY_SWEEP),
    CAIStateGroupInfo(CCharString("SG_VILLAGE_TASK_PICK_CROPS", -1), AllocStateGroup_SG_VILLAGE_TASK_PICK_CROPS),
    CAIStateGroupInfo(CCharString("SG_VILLAGE_TASK_HORN", -1), AllocStateGroup_SG_VILLAGE_TASK_HORN),
    CAIStateGroupInfo(CCharString("SG_KID_WANDER_NEAR_KIDS", -1), AllocStateGroup_SG_KID_WANDER_NEAR_KIDS),
    CAIStateGroupInfo(CCharString("SG_KID_WATCH_HOPSCOTCH", -1), AllocStateGroup_SG_KID_WATCH_HOPSCOTCH),
    CAIStateGroupInfo(CCharString("SG_KID_PLAY_HOPSCOTCH", -1), AllocStateGroup_SG_KID_PLAY_HOPSCOTCH),
    CAIStateGroupInfo(CCharString("SG_KID_HURT_SELF_RUN_TO_MOTHER", -1), AllocStateGroup_SG_KID_HURT_SELF_RUN_TO_MOTHER),
    CAIStateGroupInfo(CCharString("SG_KID_REACT_TO_BLOCKAGE", -1), AllocStateGroup_SG_KID_REACT_TO_BLOCKAGE),
    CAIStateGroupInfo(CCharString("SG_IDLE_AT_HOME", -1), AllocStateGroup_SG_IDLE_AT_HOME),
    CAIStateGroupInfo(CCharString("SG_WORK_AT_SCHOOL", -1), AllocStateGroup_SG_WORK_AT_SCHOOL),
    CAIStateGroupInfo(CCharString("SG_TELL_STORY", -1), AllocStateGroup_SG_TELL_STORY),
    CAIStateGroupInfo(CCharString("SG_WORK_IN_WAREHOUSE", -1), AllocStateGroup_SG_WORK_IN_WAREHOUSE),
    CAIStateGroupInfo(CCharString("SG_WORK_SAWING_WOOD", -1), AllocStateGroup_SG_WORK_SAWING_WOOD),
    CAIStateGroupInfo(CCharString("SG_WORK_HAMMER_ANVIL", -1), AllocStateGroup_SG_WORK_HAMMER_ANVIL),
    CAIStateGroupInfo(CCharString("SG_THROW_LOGS_ON_FIRE", -1), AllocStateGroup_SG_THROW_LOGS_ON_FIRE),
    CAIStateGroupInfo(CCharString("SG_STAY_NEAR_SCHOOL", -1), AllocStateGroup_SG_STAY_NEAR_SCHOOL),
    CAIStateGroupInfo(CCharString("SG_CLEAN_HOUSE", -1), AllocStateGroup_SG_CLEAN_HOUSE),
    CAIStateGroupInfo(CCharString("SG_CLEAN_WORK", -1), AllocStateGroup_SG_CLEAN_WORK),
    CAIStateGroupInfo(CCharString("SG_SIT_FOR_MEAL", -1), AllocStateGroup_SG_SIT_FOR_MEAL),
    CAIStateGroupInfo(CCharString("SG_PREPARE_MEAL", -1), AllocStateGroup_SG_PREPARE_MEAL),
    CAIStateGroupInfo(CCharString("SG_GAZE_AT_HOME", -1), AllocStateGroup_SG_GAZE_AT_HOME),
    CAIStateGroupInfo(CCharString("SG_GAZE_AT_WORK", -1), AllocStateGroup_SG_GAZE_AT_WORK),
    CAIStateGroupInfo(CCharString("SG_GAZE_OUTSIDE", -1), AllocStateGroup_SG_GAZE_OUTSIDE),
    CAIStateGroupInfo(CCharString("SG_WANDER_AROUND_SHOPS", -1), AllocStateGroup_SG_WANDER_AROUND_SHOPS),
    CAIStateGroupInfo(CCharString("SG_STAY_NEAR_FOOD_HALL", -1), AllocStateGroup_SG_STAY_NEAR_FOOD_HALL),
    CAIStateGroupInfo(CCharString("SG_GO_TO_PUB", -1), AllocStateGroup_SG_GO_TO_PUB),
    CAIStateGroupInfo(CCharString("SG_SIT_IN_PUB", -1), AllocStateGroup_SG_SIT_IN_PUB),
    CAIStateGroupInfo(CCharString("SG_HOME_CANDLE", -1), AllocStateGroup_SG_HOME_CANDLE),
    CAIStateGroupInfo(CCharString("SG_WORK_CANDLE", -1), AllocStateGroup_SG_WORK_CANDLE),
    CAIStateGroupInfo(CCharString("SG_SEEK_PERCEIVED_THING", -1), AllocStateGroup_SG_SEEK_PERCEIVED_THING),
    CAIStateGroupInfo(CCharString("SG_MINION_FOLLOW_PLAYER", -1), AllocStateGroup_SG_MINION_FOLLOW_PLAYER),
    CAIStateGroupInfo(CCharString("SG_MINION_HAWKING_FROM_STALL", -1), AllocStateGroup_SG_MINION_HAWKING_FROM_STALL),
    CAIStateGroupInfo(CCharString("SG_BUY_FROM_SELLER", -1), AllocStateGroup_SG_BUY_FROM_SELLER),
    CAIStateGroupInfo(CCharString("SG_SELL_TO_BUYER", -1), AllocStateGroup_SG_SELL_TO_BUYER),
    CAIStateGroupInfo(CCharString("SG_HANDY_MAN", -1), AllocStateGroup_SG_HANDY_MAN),
    CAIStateGroupInfo(CCharString("SG_LOOK_AT_INTERESTING_THINGS", -1), AllocStateGroup_SG_LOOK_AT_INTERESTING_THINGS),
    CAIStateGroupInfo(CCharString("SG_GAME_MASTER", -1), AllocStateGroup_SG_GAME_MASTER),
    CAIStateGroupInfo(CCharString("SG_STAND_STILL", -1), AllocStateGroup_SG_STAND_STILL),
    CAIStateGroupInfo(CCharString("SG_BALVERINE_WANDER", -1), AllocStateGroup_SG_BALVERINE_WANDER),
    CAIStateGroupInfo(CCharString("SG_BALVERINE_CORNERED", -1), AllocStateGroup_SG_BALVERINE_CORNERED),
    CAIStateGroupInfo(CCharString("SG_BALVERINE_JUMP_AT_FLEEING_ENEMY", -1), AllocStateGroup_SG_BALVERINE_JUMP_AT_FLEEING_ENEMY),
    CAIStateGroupInfo(CCharString("SG_NYMPH_FLOAT_AROUND", -1), AllocStateGroup_SG_NYMPH_FLOAT_AROUND),
    CAIStateGroupInfo(CCharString("SG_NYMPH_ATTACK", -1), AllocStateGroup_SG_NYMPH_ATTACK),
    CAIStateGroupInfo(CCharString("SG_HOVER_WANDER", -1), AllocStateGroup_SG_HOVER_WANDER),
    CAIStateGroupInfo(CCharString("SG_HOVER_WANDER_AROUND_THING", -1), AllocStateGroup_SG_HOVER_WANDER_AROUND_THING),
    CAIStateGroupInfo(CCharString("SG_HOVER_ATTACK", -1), AllocStateGroup_SG_HOVER_ATTACK),
    CAIStateGroupInfo(CCharString("SG_HOVER_ATTACK_UNLESS_TARGET_FLEEING", -1), AllocStateGroup_SG_HOVER_ATTACK_UNLESS_TARGET_FLEEING),
    CAIStateGroupInfo(CCharString("SG_HOVER_KEEP_IN_AREA", -1), AllocStateGroup_SG_HOVER_KEEP_IN_AREA),
    CAIStateGroupInfo(CCharString("SG_HOVER_ATTACK_AREA", -1), AllocStateGroup_SG_HOVER_ATTACK_AREA),
    CAIStateGroupInfo(CCharString("SG_HOVER_HORNET_QUEEN_TAKE_DAMAGE", -1), AllocStateGroup_SG_HOVER_HORNET_QUEEN_TAKE_DAMAGE),
    CAIStateGroupInfo(CCharString("SG_HOVER_HORNET_QUEEN", -1), AllocStateGroup_SG_HOVER_HORNET_QUEEN),
    CAIStateGroupInfo(CCharString("SG_HOVER_ALL_AROUND_TEST", -1), AllocStateGroup_SG_HOVER_ALL_AROUND_TEST),
    CAIStateGroupInfo(CCharString("SG_EARTH_TROLL", -1), AllocStateGroup_SG_EARTH_TROLL),
    CAIStateGroupInfo(CCharString("SG_ROCK_TROLL", -1), AllocStateGroup_SG_ROCK_TROLL),
    CAIStateGroupInfo(CCharString("SG_ICE_TROLL_ICICLE_ATTACK", -1), AllocStateGroup_SG_ICE_TROLL_ICICLE_ATTACK),
    CAIStateGroupInfo(CCharString("SG_ICE_TROLL_POUND_GROUND_ATTACK", -1), AllocStateGroup_SG_ICE_TROLL_POUND_GROUND_ATTACK),
    CAIStateGroupInfo(CCharString("SG_ICE_TROLL_ICICLE_ASSAULT", -1), AllocStateGroup_SG_ICE_TROLL_ICICLE_ASSAULT),
    CAIStateGroupInfo(CCharString("SG_SCORPION_KING", -1), AllocStateGroup_SG_SCORPION_KING),
    CAIStateGroupInfo(CCharString("SG_JACK_OF_BLADES_STAGE1", -1), AllocStateGroup_SG_JACK_OF_BLADES_STAGE1),
    CAIStateGroupInfo(CCharString("SG_JACK_OF_BLADES_STAGE2", -1), AllocStateGroup_SG_JACK_OF_BLADES_STAGE2),
    CAIStateGroupInfo(CCharString("SG_JACK_OF_BLADES_STAGE3", -1), AllocStateGroup_SG_JACK_OF_BLADES_STAGE3),
    CAIStateGroupInfo(CCharString("SG_MAZE_TELEPORT", -1), AllocStateGroup_SG_MAZE_TELEPORT),
    CAIStateGroupInfo(CCharString("SG_MAZE_RANGED_COMBAT", -1), AllocStateGroup_SG_MAZE_RANGED_COMBAT),
    CAIStateGroupInfo(CCharString("SG_MAZE_DEFENSIVE_MELEE_COMBAT", -1), AllocStateGroup_SG_MAZE_DEFENSIVE_MELEE_COMBAT),
    CAIStateGroupInfo(CCharString("SG_MAZE_SHIELDED_CHASE", -1), AllocStateGroup_SG_MAZE_SHIELDED_CHASE),
    CAIStateGroupInfo(CCharString("SG_MAZE_TUTORIAL_BATTLE", -1), AllocStateGroup_SG_MAZE_TUTORIAL_BATTLE),
    CAIStateGroupInfo(CCharString("SG_MAZE_TUTORIAL_BATTLE_TELEPORT", -1), AllocStateGroup_SG_MAZE_TUTORIAL_BATTLE_TELEPORT),
    CAIStateGroupInfo(CCharString("SG_MAZE_TUTORIAL_BATTLE_RESPOND_TO_LIGHTNING", -1), AllocStateGroup_SG_MAZE_TUTORIAL_BATTLE_RESPOND_TO_LIGHTNING),
    CAIStateGroupInfo(CCharString("SG_MAZE_TUTORIAL_BATTLE_USE_SHIELD", -1), AllocStateGroup_SG_MAZE_TUTORIAL_BATTLE_USE_SHIELD),
    CAIStateGroupInfo(CCharString("SG_GUILDMASTER", -1), AllocStateGroup_SG_GUILDMASTER),
    CAIStateGroupInfo(CCharString("SG_BRIAR_ROSE", -1), AllocStateGroup_SG_BRIAR_ROSE),
    CAIStateGroupInfo(CCharString("SG_BRIAR_ROSE_WILL_USE", -1), AllocStateGroup_SG_BRIAR_ROSE_WILL_USE),
    CAIStateGroupInfo(CCharString("SG_VERSUS_ROCK_TROLL", -1), AllocStateGroup_SG_VERSUS_ROCK_TROLL),
    CAIStateGroupInfo(CCharString("SG_VERSUS_SCORPION_KING", -1), AllocStateGroup_SG_VERSUS_SCORPION_KING),
    CAIStateGroupInfo(CCharString("SG_CREATURE_RANGED_COMBAT_TWO", -1), AllocStateGroup_SG_CREATURE_RANGED_COMBAT_TWO),
    CAIStateGroupInfo(CCharString("SG_CREATURE_RANGED_COMBAT_USE_COVER", -1), AllocStateGroup_SG_CREATURE_RANGED_COMBAT_USE_COVER),
    CAIStateGroupInfo(CCharString("SG_CREATURE_ADVANCE_IN_COVER", -1), AllocStateGroup_SG_CREATURE_ADVANCE_IN_COVER),
    CAIStateGroupInfo(CCharString("SG_CREATURE_RANGED_COMBAT_FIRE_AT_SPECIFIC_TARGETS", -1), AllocStateGroup_SG_CREATURE_RANGED_COMBAT_FIRE_AT_SPECIFIC_TARGETS),
    CAIStateGroupInfo(CCharString("SG_CREATURE_TEST_TEST_TEST", -1), AllocStateGroup_SG_CREATURE_TEST_TEST_TEST),
    CAIStateGroupInfo(CCharString("SG_CREATURE_GO_UNDERGROUND", -1), AllocStateGroup_SG_CREATURE_GO_UNDERGROUND),
    CAIStateGroupInfo(CCharString("SG_CREATURE_IDLE_UNDERGROUND", -1), AllocStateGroup_SG_CREATURE_IDLE_UNDERGROUND),
    CAIStateGroupInfo(CCharString("SG_CREATURE_SEEK_ENEMY_UNDERGROUND", -1), AllocStateGroup_SG_CREATURE_SEEK_ENEMY_UNDERGROUND),
    CAIStateGroupInfo(CCharString("SG_BEETLE_TOGGLE_HOVER", -1), AllocStateGroup_SG_BEETLE_TOGGLE_HOVER),
    CAIStateGroupInfo(CCharString("SG_MINION_BEHAVIOUR_GREET_CREATURE", -1), AllocStateGroup_SG_MINION_BEHAVIOUR_GREET_CREATURE),
    CAIStateGroupInfo(CCharString("SG_MINION_BEHAVIOUR_SAD", -1), AllocStateGroup_SG_MINION_BEHAVIOUR_SAD),
    CAIStateGroupInfo(CCharString("SG_MINION_BEHAVIOUR_PODIUM_EMOTE_NEUTRAL", -1), AllocStateGroup_SG_MINION_BEHAVIOUR_PODIUM_EMOTE_NEUTRAL),
    CAIStateGroupInfo(CCharString("SG_MINION_MOTHER_CONSOLE_CHILD", -1), AllocStateGroup_SG_MINION_MOTHER_CONSOLE_CHILD),
    CAIStateGroupInfo(CCharString("SG_MINION_REACT_TO_BURGLARY", -1), AllocStateGroup_SG_MINION_REACT_TO_BURGLARY),
    CAIStateGroupInfo(CCharString("SG_MINION_REACT_TO_TRESPASSING", -1), AllocStateGroup_SG_MINION_REACT_TO_TRESPASSING),
    CAIStateGroupInfo(CCharString("SG_MINION_REACT_TO_NOISE", -1), AllocStateGroup_SG_MINION_REACT_TO_NOISE),
    CAIStateGroupInfo(CCharString("SG_MINION_REACT_TO_APOLOGY", -1), AllocStateGroup_SG_MINION_REACT_TO_APOLOGY),
    CAIStateGroupInfo(CCharString("SG_MINION_REACT_TO_CRIME", -1), AllocStateGroup_SG_MINION_REACT_TO_CRIME),
    CAIStateGroupInfo(CCharString("SG_MINION_REACT_TO_CRIME_FROM_SLEEP", -1), AllocStateGroup_SG_MINION_REACT_TO_CRIME_FROM_SLEEP),
    CAIStateGroupInfo(CCharString("SG_MINION_LEAVE_REGION", -1), AllocStateGroup_SG_MINION_LEAVE_REGION),
    CAIStateGroupInfo(CCharString("SG_REACT_TO_DEAD_LEADER", -1), AllocStateGroup_SG_REACT_TO_DEAD_LEADER),
    CAIStateGroupInfo(CCharString("SG_BOASTING_CROWD_MEMBER", -1), AllocStateGroup_SG_BOASTING_CROWD_MEMBER),
    CAIStateGroupInfo(CCharString("SG_OPINION_REACTION_ROOT", -1), AllocStateGroup_SG_OPINION_REACTION_ROOT),
    CAIStateGroupInfo(CCharString("SG_DEED_REACTION_ROOT", -1), AllocStateGroup_SG_DEED_REACTION_ROOT),
    CAIStateGroupInfo(CCharString("SG_DEED_REACTION_ROOT_FROM_SLEEP", -1), AllocStateGroup_SG_DEED_REACTION_ROOT_FROM_SLEEP),
    CAIStateGroupInfo(CCharString("SG_BANDIT_KING_CHARGE", -1), AllocStateGroup_SG_BANDIT_KING_CHARGE),
    CAIStateGroupInfo(CCharString("SG_BANDIT_KING_LUNGE", -1), AllocStateGroup_SG_BANDIT_KING_LUNGE),
    CAIStateGroupInfo(CCharString("SG_BANDIT_KING_THROW_WEAPONS", -1), AllocStateGroup_SG_BANDIT_KING_THROW_WEAPONS),
    CAIStateGroupInfo(CCharString("SG_JACK_DRAGON", -1), AllocStateGroup_SG_JACK_DRAGON),
    CAIStateGroupInfo(CCharString("SG_MINION_FOLLOW_THING", -1), AllocStateGroup_SG_MINION_FOLLOW_THING),
    CAIStateGroupInfo(CCharString("SG_MINION_WALK_TO_MARKER", -1), AllocStateGroup_SG_MINION_WALK_TO_MARKER),
    CAIStateGroupInfo(CCharString("SG_TAVERN_SERVER", -1), AllocStateGroup_SG_TAVERN_SERVER),
    CAIStateGroupInfo(CCharString("SG_TAVERN_KEEPER", -1), AllocStateGroup_SG_TAVERN_KEEPER),
    CAIStateGroupInfo(CCharString("SG_STEAL_STEALABLE_ITEM", -1), AllocStateGroup_SG_STEAL_STEALABLE_ITEM),
    CAIStateGroupInfo(CCharString("SG_RECOVER_STEALABLE_ITEM", -1), AllocStateGroup_SG_RECOVER_STEALABLE_ITEM),
    CAIStateGroupInfo(CCharString("SG_CARRY_CRATE_AND_FOLLOW", -1), AllocStateGroup_SG_CARRY_CRATE_AND_FOLLOW),
    CAIStateGroupInfo(CCharString("SG_SLEEP_ON_FLOOR", -1), AllocStateGroup_SG_SLEEP_ON_FLOOR),
    CAIStateGroupInfo(CCharString("SG_WHISPER_RETREAT", -1), AllocStateGroup_SG_WHISPER_RETREAT),
    CAIStateGroupInfo(CCharString("SG_WHISPER_POTION_COMBAT", -1), AllocStateGroup_SG_WHISPER_POTION_COMBAT),
    CAIStateGroupInfo(CCharString("SG_THUNDER", -1), AllocStateGroup_SG_THUNDER),
    CAIStateGroupInfo(CCharString("SG_FREEZE", -1), AllocStateGroup_SG_FREEZE),
    CAIStateGroupInfo(CCharString("SG_DRUNK", -1), AllocStateGroup_SG_DRUNK),
    CAIStateGroupInfo(CCharString("SG_SERVE_FOOD_AT_HOME", -1), AllocStateGroup_SG_SERVE_FOOD_AT_HOME),
    CAIStateGroupInfo(CCharString("SG_SERVE_FOOD_AT_TAVERN", -1), AllocStateGroup_SG_SERVE_FOOD_AT_TAVERN),
    CAIStateGroupInfo(CCharString("SG_EAT_AT_HOME", -1), AllocStateGroup_SG_EAT_AT_HOME),
    CAIStateGroupInfo(CCharString("SG_EAT_AT_TAVERN", -1), AllocStateGroup_SG_EAT_AT_TAVERN),
    CAIStateGroupInfo(CCharString("SG_SCREAMER_PURSUE_TARGET", -1), AllocStateGroup_SG_SCREAMER_PURSUE_TARGET),
    CAIStateGroupInfo(CCharString("SG_KRAKEN_ATTACK", -1), AllocStateGroup_SG_KRAKEN_ATTACK),
    CAIStateGroupInfo(CCharString("SG_KRAKEN_TENTACLE_IDLE", -1), AllocStateGroup_SG_KRAKEN_TENTACLE_IDLE),
    CAIStateGroupInfo(CCharString("SG_KRAKEN_TENTACLE_RISE", -1), AllocStateGroup_SG_KRAKEN_TENTACLE_RISE),
    CAIStateGroupInfo(CCharString("SG_KRAKEN_TENTACLE_ATTACK", -1), AllocStateGroup_SG_KRAKEN_TENTACLE_ATTACK),
    CAIStateGroupInfo(CCharString("SG_WIFE_GIVE_YOU_GIFT", -1), AllocStateGroup_SG_WIFE_GIVE_YOU_GIFT),
    CAIStateGroupInfo(CCharString("SG_OPEN_AND_CLOSE_SHOP", -1), AllocStateGroup_SG_OPEN_AND_CLOSE_SHOP),
    CAIStateGroupInfo(CCharString("SG_SHOPKEEPER_WATCH_PLAYER", -1), AllocStateGroup_SG_SHOPKEEPER_WATCH_PLAYER),
    CAIStateGroupInfo(CCharString("SG_SETUP_WARES", -1), AllocStateGroup_SG_SETUP_WARES),
    CAIStateGroupInfo(CCharString("SG_BANDIT_PLAY_DICE", -1), AllocStateGroup_SG_BANDIT_PLAY_DICE),
    CAIStateGroupInfo(CCharString("SG_BANDIT_PLOT", -1), AllocStateGroup_SG_BANDIT_PLOT),
    CAIStateGroupInfo(CCharString("SG_MINION_GUARD_GREET_PLAYER", -1), AllocStateGroup_SG_MINION_GUARD_GREET_PLAYER),
    CAIStateGroupInfo(CCharString("SG_MINION_GUARD_FOLLOW_PLAYER", -1), AllocStateGroup_SG_MINION_GUARD_FOLLOW_PLAYER),
    CAIStateGroupInfo(CCharString("SG_MINION_GUARD_BRIBED", -1), AllocStateGroup_SG_MINION_GUARD_BRIBED),
    CAIStateGroupInfo(CCharString("SG_APPRENTICE_HERO", -1), AllocStateGroup_SG_APPRENTICE_HERO),
    CAIStateGroupInfo(CCharString("SG_SKILL_PRACTICE", -1), AllocStateGroup_SG_SKILL_PRACTICE),
    CAIStateGroupInfo(CCharString("SG_ON_HERO_PUSH", -1), AllocStateGroup_SG_ON_HERO_PUSH),
    CAIStateGroupInfo(CCharString("SG_AVOID_REGION_EXIT", -1), AllocStateGroup_SG_AVOID_REGION_EXIT),
    CAIStateGroupInfo(CCharString("SG_SUMMONER_LIGHTNING_ATTACK", -1), AllocStateGroup_SG_SUMMONER_LIGHTNING_ATTACK),
    CAIStateGroupInfo(CCharString("SG_SUMMONER_SUMMON", -1), AllocStateGroup_SG_SUMMONER_SUMMON),
    CAIStateGroupInfo(CCharString("SG_SUMMONER_CHARGE", -1), AllocStateGroup_SG_SUMMONER_CHARGE),
    CAIStateGroupInfo(CCharString("", -1), 0),
};