// 00ee91e0
// Leaf: ?Main@CGlobal_DebugCycleThroughSpeechScript@NScript@@UAEXXZ
// Flags: none
// Inline-budget: The native set cleanup expands its tree operations up to clear and base destruction. Both selected 66-byte destructor bodies match, with the primary exit layout still open.
#define FABLE_RETAIL_SPEECH_SET_DTOR26
#define FABLE_RETAIL_SPEECH_CTOR_DECL26
#define FABLE_RETAIL_SPEECH_GLOBAL_NOINLINE26
#define FABLE_RETAIL_SPEECH_EXPAND_INCREMENT26
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_STLP_RB_INCREMENT_SIZE_OWNER_PASS27
#include "fable_compiler.h"
#define FABLE_RETAIL_LEVEL_NOMASTER_ORIGINAL_REFERENCE_PASS29
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_SET_EMPTY
#include "engine/EScriptAIPriority.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_text.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_map.h"
#include "engine/CGlobal_DebugCycleThroughSpeechScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
FABLE_FORCEINLINE CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { return !PImp.IsNull() && PImp->IsAlive(); }
#ifdef _MSC_VER
#pragma inline_depth(2)
#endif
// Inline budget: retail expands set and tree destruction to clear and base destruction
// at the loop interruption, while the standalone destructor bodies both expand clear.
namespace std {
template<> FABLE_FORCEINLINE _Rb_tree<unsigned long,unsigned long,_Identity<unsigned long>,less<unsigned long>,allocator<unsigned long> >::~_Rb_tree() { clear(); }
template<> FABLE_FORCEINLINE set<unsigned long>::~set() {}
}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
void NScript::CGlobal_DebugCycleThroughSpeechScript::Main()
{
    TRetVoid _Fail_Val;
    CScriptThing targeted_thing = Game.GetHero();
    if (targeted_thing.IsAlive()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        std::set<unsigned long> entries;
        Game.DebugGetAllTextEntriesForTargetedThing(entries);
        if (!entries.empty()) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game.FixMovieSequenceCamera(true);
            CScriptGameResourceObjectScriptedThingBase seh_target;
            Game.StartScriptingEntity(targeted_thing, seh_target, SCRIPT_AI_PRIORITY_HIGHEST);
            bool stop = false;
            if (!stop) if (false) { } else for (std::set<unsigned long>::iterator it = entries.begin(); it != entries.end(); ++it) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game.AddScreenTitleMessage(GFIntToCharString(*it), 0.0f, true);
                seh_target.Speak(Game.GetHero(), *it, GROUP_SELECT_NONE, false, true, false);
                while (seh_target.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game.FixMovieSequenceCamera(false);
        }
    }
    Game.DeactivateQuestLater(Game.GetActiveQuestName(), 0);
}
