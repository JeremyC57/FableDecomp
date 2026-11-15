// CMainGameComponent::`dynamic initializer for 'EventDispatch'' -- build the game-event dispatch table: per event type the CWorld action handler, the CWorld local-action handler and the (all null) CDisplayEngine feedback / confirm handlers
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) @ 0x0121B9B0, 9567 bytes: `push esi`, 1392 dword stores into 0x013B9288..0x013BA847 (87 x 64
//   bytes, every word of every entry), `pop esi ; ret`. Extent proven: the last store 0x0121DF07 is followed by `pop esi ; ret`
//   (0x0121DF0D / 0x0121DF0E) and the next function starts at 0x0121DF0F (landed NPlayerGui initializer at 0x0121DF1C after padding).
// Identity (proven): compiler-emitted dynamic initializer; Ego_r.pdb (0x0100E510) and FableWin.pdb (0x03FE65C0) both name
//   `dynamic initializer for 'CMainGameComponent::EventDispatch''; the table it fills is the one CWorld::GetEventDispatchTableEntry
//   0x0049D8E0 indexes (`shl eax,6 ; add eax,0x013B9288`). The member (protected static const CGameEventDispatch[87]) is spelled
//   from the Anniversary PDB's LF_STMEMBER (third source, the only one that records data members).
// Entries: one per EGameEventType (enumerator names from the Anniversary PDB, comment only). Every handler name is in both
//   Ego_r.pdb and FableWin.pdb (?EA<Name>@CWorld@@QAEXPBVCGameEvent@@@Z / ?LEA<Name>@...) and matches its slot's event name;
//   the retail address of each handler is the slot's stored function word (see NOTES.md for the full address list; sizes agree
//   with the Ego_r donor bodies, landed rows 0x00629240 .. 0x0062D380 agree where they exist).
// Shape: the pointers to member are 16-byte unspecified-inheritance pointers (engine/CGameEventDispatch.h is included before
//   CWorld is defined), so the aggregate needs a dynamic initializer; a null entry stores {0,0,0,-1}, a set one {f,0,0,0}.
#include "engine/CGameEventDispatch.h"
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_EVENTACTIONS
#define FABLE_RETAIL_CWORLD_EVENTDISPATCH
#include "engine/CWorld.h"
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTDISPATCH
#include "engine/CMainGameComponent.h"

const CGameEventDispatch CMainGameComponent::EventDispatch[87] = {
    { NULL, NULL, NULL, NULL },                                                                 //  0 GAME_EVENT_NULL
    { &CWorld::EAUpdateFrame, NULL, NULL, NULL },                                               //  1 GAME_EVENT_UPDATE_FRAME
    { NULL, NULL, NULL, NULL },                                                                 //  2 GAME_EVENT_QUIT
    { NULL, &CWorld::LEASetExclusiveMode, NULL, NULL },                                         //  3 GAME_EVENT_SET_EXCLUSIVE_MODE
    { NULL, &CWorld::LEASetDisplayMode, NULL, NULL },                                           //  4 GAME_EVENT_SET_DISPLAY_MODE
    { NULL, &CWorld::LEASetEditorMode, NULL, NULL },                                            //  5 GAME_EVENT_SET_EDITOR_MODE
    { &CWorld::EAFirstPersonViewStart, NULL, NULL, NULL },                                      //  6 GAME_EVENT_FIRST_PERSON_VIEW_START
    { &CWorld::EACreatureCheat, NULL, NULL, NULL },                                             //  7 GAME_EVENT_CREATURE_CHEAT
    { &CWorld::EAPlayerRespawn, NULL, NULL, NULL },                                             //  8 GAME_EVENT_PLAYER_RESPAWN
    { &CWorld::EASetPauseMode, NULL, NULL, NULL },                                              //  9 GAME_EVENT_SET_PAUSE_MODE
    { &CWorld::EASetSlowMotion, NULL, NULL, NULL },                                             // 10 GAME_EVENT_SET_SLOW_MOTION
    { &CWorld::EASetFreeCameraMode, NULL, NULL, NULL },                                         // 11 GAME_EVENT_SET_FREE_CAMERA_MODE
    { &CWorld::EAUseFreeCamera, NULL, NULL, NULL },                                             // 12 GAME_EVENT_USE_FREE_CAMERA
    { &CWorld::EACreatureBlock, NULL, NULL, NULL },                                             // 13 GAME_EVENT_CONTROLLED_CREATURE_BLOCK
    { &CWorld::EATempWorldEventForWhateverYouWant, NULL, NULL, NULL },                          // 14 GAME_EVENT_TEMP_WORLD_EVENT
    { &CWorld::EAApplyAllScriptedMapBrushes, NULL, NULL, NULL },                                // 15 GAME_EVENT_APPLY_SCRIPTED_MAP_BRUSHES
    { &CWorld::EAMoveCreature, NULL, NULL, NULL },                                              // 16 GAME_EVENT_CREATURE_MOVEMENT
    { &CWorld::EACreatureUseObject, NULL, NULL, NULL },                                         // 17 GAME_EVENT_CREATURE_USE_OBJECT
    { &CWorld::EAControlledCreatureTalk, NULL, NULL, NULL },                                    // 18 GAME_EVENT_CONTROLLED_CREATURE_TALK
    { &CWorld::EAControlledCreatureLearnExpression, NULL, NULL, NULL },                         // 19 GAME_EVENT_CONTROLLED_CREATURE_LEARN_EXPRESSION
    { &CWorld::EAControlledCreatureDropObject, NULL, NULL, NULL },                              // 20 GAME_EVENT_CONTROLLED_CREATURE_DROP_OBJECT
    { &CWorld::EAControlledCreatureZTarget, NULL, NULL, NULL },                                 // 21 GAME_EVENT_CONTROLLED_CREATURE_ZTARGET
    { &CWorld::EAControlledCreatureUseAbility, NULL, NULL, NULL },                              // 22 GAME_EVENT_CONTROLLED_CREATURE_USE_ABILITY
    { &CWorld::EAControlledCreatureStartSneaking, NULL, NULL, NULL },                           // 23 GAME_EVENT_CONTROLLED_CREATURE_START_SNEAK
    { &CWorld::EAControlledCreatureStopSneaking, NULL, NULL, NULL },                            // 24 GAME_EVENT_CONTROLLED_CREATURE_STOP_SNEAK
    { &CWorld::EAClickPastText, NULL, NULL, NULL },                                             // 25 GAME_EVENT_CLICK_PAST_TEXT
    { &CWorld::EAOpenHeroInformationScreen, NULL, NULL, NULL },                                 // 26 GAME_EVENT_OPEN_HERO_INFO_SCREEN
    { &CWorld::EACloseHeroInformationScreen, NULL, NULL, NULL },                                // 27 GAME_EVENT_CLOSE_HERO_INFO_SCREEN
    { &CWorld::EACloseInGameMenu, NULL, NULL, NULL },                                           // 28 GAME_EVENT_CLOSE_IN_GAME_MENU
    { &CWorld::EAAnswerQuestion, NULL, NULL, NULL },                                            // 29 GAME_EVENT_QUESTION_ANSWERED
    { &CWorld::EAControlledCreatureStartThrowObject, NULL, NULL, NULL },                        // 30 GAME_EVENT_CONTROLLED_CREATURE_START_THROW_OBJECT
    { &CWorld::EAControlledCreatureEndThrowObject, NULL, NULL, NULL },                          // 31 GAME_EVENT_CONTROLLED_CREATURE_END_THROW_OBJECT
    { &CWorld::EAControlledCreatureSheatheWeapon, NULL, NULL, NULL },                           // 32 GAME_EVENT_CONTROLLED_CREATURE_SHEATHE_WEAPON
    { &CWorld::EAControlledCreatureUnsheatheMeleeWeapon, NULL, NULL, NULL },                    // 33 GAME_EVENT_CONTROLLED_CREATURE_UNSHEATHE_MELEE_WEAPON
    { &CWorld::EAControlledCreatureUnsheatheRangedWeapon, NULL, NULL, NULL },                   // 34 GAME_EVENT_CONTROLLED_CREATURE_UNSHEATHE_RANGED_WEAPON
    { &CWorld::EAControlledCreaturePlayerInteraction, NULL, NULL, NULL },                       // 35 GAME_EVENT_CONTROLLED_CREATURE_PLAYER_INTERACTION
    { &CWorld::EAControlledCreatureGiveItemToTarget, NULL, NULL, NULL },                        // 36 GAME_EVENT_CONTROLLED_CREATURE_GIVE_ITEM_TO_TARGET
    { &CWorld::EAControlledCreatureUnfreezeControls, NULL, NULL, NULL },                        // 37 GAME_EVENT_CONTROLLED_CREATURE_UNFREEZE_CONTROLS
    { &CWorld::EAControlledCreatureRoll, NULL, NULL, NULL },                                    // 38 GAME_EVENT_CONTROLLED_CREATURE_ROLL
    { &CWorld::EAUseQuickAccessItem, NULL, NULL, NULL },                                        // 39 GAME_EVENT_USE_QUICK_ACCESS_ITEM
    { &CWorld::EAUseQuickAccessItemInCutscene, NULL, NULL, NULL },                              // 40 GAME_EVENT_USE_QUICK_ACCESS_ITEM_IN_CUTSCENE
    { &CWorld::EAChargeQuickAccessItem, NULL, NULL, NULL },                                     // 41 GAME_EVENT_CHARGE_QUICK_ACCESS_ITEM
    { &CWorld::EAPutAway, NULL, NULL, NULL },                                                   // 42 GAME_EVENT_PUT_AWAY
    { &CWorld::EAControlledCreatureStrafe, NULL, NULL, NULL },                                  // 43 GAME_EVENT_CREATURE_STRAFE
    { &CWorld::EAMoveHeroToRegion, NULL, NULL, NULL },                                          // 44 GAME_EVENT_MOVE_HERO_TO_REGION
    { &CWorld::EAFirstPersonTargeting, NULL, NULL, NULL },                                      // 45 GAME_EVENT_FIRST_PERSON_TARGETING
    { &CWorld::EAControlledCreatureDefault, NULL, NULL, NULL },                                 // 46 GAME_EVENT_CONTROLLED_CREATURE_DEFAULT
    { &CWorld::EAExpressionFollow, NULL, NULL, NULL },                                          // 47 GAME_EVENT_EXPRESSION_FOLLOW
    { &CWorld::EAExpressionWait, NULL, NULL, NULL },                                            // 48 GAME_EVENT_EXPRESSION_WAIT
    { &CWorld::EAUseProjectileWeapon, NULL, NULL, NULL },                                       // 49 GAME_EVENT_USE_PROJECTILE_WEAPON
    { &CWorld::EAControlledCreatureJump, NULL, NULL, NULL },                                    // 50 GAME_EVENT_CONTROLLED_CREATURE_JUMP
    { &CWorld::EAMoveSpirit, NULL, NULL, NULL },                                                // 51 GAME_EVENT_SPIRIT_MOVEMENT
    { &CWorld::EAOpenHeroCentreDoor, NULL, NULL, NULL },                                        // 52 GAME_EVENT_OPEN_HERO_CENTRE_DOOR
    { &CWorld::EACloseHeroCentreDoor, NULL, NULL, NULL },                                       // 53 GAME_EVENT_CLOSE_HERO_CENTRE_DOOR
    { &CWorld::EASpiritAttack, NULL, NULL, NULL },                                              // 54 GAME_EVENT_SPIRIT_ATTACK
    { &CWorld::EACreatureSprint, NULL, NULL, NULL },                                            // 55 GAME_EVENT_CREATURE_SPRINT
    { &CWorld::EASpiritReturnToHero, NULL, NULL, NULL },                                        // 56 GAME_EVENT_SPIRIT_RETURN_TO_HERO
    { &CWorld::EASkipCutScene, NULL, NULL, NULL },                                              // 57 GAME_EVENT_SKIP_CUT_SCENE
    { &CWorld::EAUseProjectileWeaponThirdPerson, NULL, NULL, NULL },                            // 58 GAME_EVENT_USE_PROJECTILE_WEAPON_THIRD_PERSON
    { &CWorld::EAChargeUpWillSpell, NULL, NULL, NULL },                                         // 59 GAME_EVENT_CHARGE_UP_WILL_SPELL
    { &CWorld::EALoadGameFromInGameMenu, NULL, NULL, NULL },                                    // 60 GAME_EVENT_LOAD_GAME_FROM_IN_GAME_MENU
    { &CWorld::EAControlledCreatureBlockEnd, NULL, NULL, NULL },                                // 61 GAME_EVENT_CONTROLLED_CREATURE_BLOCK_END
    { &CWorld::EAControlledCreatureDeactivateZTarget, NULL, NULL, NULL },                       // 62 GAME_EVENT_CONTROLLED_CREATURE_DEACTIVATE_ZTARGET
    { &CWorld::EARemoveCurrentMode, NULL, NULL, NULL },                                         // 63 GAME_EVENT_REMOVE_CURRENT_MODE
    { &CWorld::EAControlledCreatureLightning, NULL, NULL, NULL },                               // 64 GAME_EVENT_CONTROLLED_CREATURE_LIGHTNING
    { &CWorld::EAControlledCreatureDeactivateLightning, NULL, NULL, NULL },                     // 65 GAME_EVENT_CONTROLLED_CREATURE_DEACTIVATE_LIGHTNING
    { &CWorld::EACloseLiveGUI, NULL, NULL, NULL },                                              // 66 GAME_EVENT_CLOSE_LIVE_GUI
    { &CWorld::EAClosePhotoCapture, NULL, NULL, NULL },                                         // 67 GAME_EVENT_CLOSE_PHOTO_CAPTURE
    { &CWorld::EATakeTheBloodyScreenshot, NULL, NULL, NULL },                                   // 68 GAME_EVENT_TAKE_THE_BLOODY_SCREENSHOT
    { &CWorld::EAConsoleToggleState, NULL, NULL, NULL },                                        // 69 GAME_EVENT_TOGGLE_CONSOLE
    { &CWorld::EAOpenPCSkillsMenu, NULL, NULL, NULL },                                          // 70 GAME_EVENT_OPEN_PC_SKILLS_MENU
    { &CWorld::EAOpenPCMSNChatMenu, NULL, NULL, NULL },                                         // 71 GAME_EVENT_OPEN_PC_MSN_CHAT_MENU
    { &CWorld::EAOpenPCInventoryMenu, NULL, NULL, NULL },                                       // 72 GAME_EVENT_OPEN_PC_INVENTORY_MENU
    { &CWorld::EAOpenPCOptionsMenu, NULL, NULL, NULL },                                         // 73 GAME_EVENT_OPEN_PC_OPTIONS_MENU
    { &CWorld::EAOpenPCPhotoJournalMenu, NULL, NULL, NULL },                                    // 74 GAME_EVENT_OPEN_PC_PHOTO_JOURNAL_MENU
    { &CWorld::EAOpenPCMapMenu, NULL, NULL, NULL },                                             // 75 GAME_EVENT_OPEN_PC_MAP_MENU
    { &CWorld::EAOpenPCBuyTradingMenu, NULL, NULL, NULL },                                      // 76 GAME_EVENT_OPEN_PC_BUY_TRADING_MENU
    { &CWorld::EAOpenPCSellTradingMenu, NULL, NULL, NULL },                                     // 77 GAME_EVENT_OPEN_PC_SELL_TRADING_MENU
    { &CWorld::EAOpenPCWantedTradingMenu, NULL, NULL, NULL },                                   // 78 GAME_EVENT_OPEN_PC_WANTED_TRADING_MENU
    { &CWorld::EAOpenPCStatusMenu, NULL, NULL, NULL },                                          // 79 GAME_EVENT_OPEN_PC_STATUS_MENU
    { &CWorld::EAOpenPCScoreboard, NULL, NULL, NULL },                                          // 80 GAME_EVENT_OPEN_PC_SCOREBOARD
    { &CWorld::EADeleteMenuComponents, NULL, NULL, NULL },                                      // 81 GAME_EVENT_DELETE_MENU_COMPONENTS
    { &CWorld::EATogglePassiveAggressiveMode, NULL, NULL, NULL },                               // 82 GAME_EVENT_TOGGLE_PASSIVE_AGGRESSIVE_MODE
    { &CWorld::EADeleteTradeActiveMenu, NULL, NULL, NULL },                                     // 83 GAME_EVENT_DELETE_TRADE_ACTIVE_MENU
    { &CWorld::EAToggleKillEverythingMode, NULL, NULL, NULL },                                  // 84 GAME_EVENT_TOGGLE_KILL_EVERYTHING_MODE
    { &CWorld::EAToggleViewHeroMode, NULL, NULL, NULL },                                        // 85 GAME_EVENT_TOGGLE_VIEW_HERO_MODE
    { &CWorld::EACentreCamera, NULL, NULL, NULL },                                              // 86 GAME_EVENT_CENTRE_CAMERA
};
