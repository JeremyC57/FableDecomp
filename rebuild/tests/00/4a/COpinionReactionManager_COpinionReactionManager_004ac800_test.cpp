// Behaviour test for 0x004AC800: the opinion reaction manager destroys its debug creature, opinion-source list (clear, then header), opinion source, reaction list, then releases its definition.
#include <cstdio>
#include <cstdlib>
static int g_log[32]; static int g_n = 0;
struct CDefPointeeBase { virtual ~CDefPointeeBase() {} virtual void DeleteThis() { g_log[g_n++] = 50; } unsigned long RefCount; };
template<class T> struct CDefPointer {
    CDefPointeeBase* Object;
    ~CDefPointer() { CDefPointeeBase* object = Object; if (object) { if (--object->RefCount == 0) object->DeleteThis(); Object = 0; } }
};
struct CBaseIntelligentPointer { virtual ~CBaseIntelligentPointer() { g_log[g_n++] = id; } int id; };
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer { };
struct CReactionMatchList { void* _M_node; ~CReactionMatchList() { g_log[g_n++] = 30; } };
struct COpinionSourceList {
    void* _M_node;
    void clear() { g_log[g_n++] = 20; }
    ~COpinionSourceList() { clear(); if (_M_node) { g_log[g_n++] = 21; std::free(_M_node); } }
};
class CWorld; class CThing; class CThingAICreature; class COpinionReactionManagerDef;
class COpinionReactionManager {
public:
    CDefPointer<const COpinionReactionManagerDef> PDef; CWorld* World; bool ReactionFlagsTemp[79];
    CReactionMatchList Reactions; long AttitudeCondition[79]; long TargetingCondition[79]; unsigned long PreReactionDelaySeconds[79];
    bool BlockFurtherReactionsFlags[79]; bool AllowSpeechOnNonPureAISpeaker[79]; bool AllowWhileCarrying[79];
    bool AllowWhileFollowingPlayer[79]; bool FurtherReactionsBlocked; unsigned long ReactionReenableFrame; bool AttitudeMask[79];
    long DeedReactionSpeechPlaying[92];
    CIntelligentPointer<CThing> PThingOpinionSource; COpinionSourceList OpinionSourcesList;
    CIntelligentPointer<const CThingAICreature> DebugCreature;
    ~COpinionReactionManager() { }
};
int main()
{
    CDefPointeeBase def; def.RefCount = 1;
    COpinionReactionManager* m = new COpinionReactionManager;
    m->PDef.Object = &def;
    m->PThingOpinionSource.id = 1;
    m->DebugCreature.id = 3;
    m->OpinionSourcesList._M_node = std::malloc(16);
    delete m;
    // debug creature (3), list clear (20), header (21), opinion source (1), reaction list (30), def deleted (50)
    bool ok = g_n == 6 && g_log[0] == 3 && g_log[1] == 20 && g_log[2] == 21 && g_log[3] == 1 && g_log[4] == 30 && g_log[5] == 50
           && def.RefCount == 0;
    if (!ok) { std::printf("004ac800_TEST FAIL\n"); return 1; }
    std::printf("004ac800_TEST PASS\n");
    return 0;
}