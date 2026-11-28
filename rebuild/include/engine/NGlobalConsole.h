#ifndef FABLE_ENGINE_NGLOBALCONSOLE_H
#define FABLE_ENGINE_NGLOBALCONSOLE_H
#ifdef FABLE_RETAIL_PROCESS_ROLLING_PASS30
namespace NGlobalConsole { extern bool GCheatModeEnableDoubleTapRoll; }
#endif
#ifdef FABLE_RETAIL_CTCENEMY_ISENEMYOF_PASS30
namespace NGlobalConsole { extern bool GTargetingFugitiveMakeThingsEnemies; }
#endif
#ifdef FABLE_RETAIL_POST_DEED_PASS29
// Both PDBs publish the namespace bool. PostDeed reads its byte at 0x01375740.
namespace NGlobalConsole { extern bool GEnableTutorials; }
#endif
#ifdef FABLE_RETAIL_EXPRESSION_INIT_PASS29
namespace NGlobalConsole { extern bool DebugExpressionsUseSpriteOverrideText; }
#endif
#endif
