// NScript::CQ_AmbushTradersScript::RefreshSpyAllegiences() -- refill the bandit-ally, convoy-member and sentry lists; if the convoy spy is alive make him and every living bandit ally allies, and then either (once the spy contact is made) turn him, the convoy members (also against the hero, minimap markers removed) and the sentries into enemies, or (before that) make him and the convoy members allies
// Residue: DIFFER(1312v1312) md 10 -- only the nested `Game->EntitySetThingAsEnemyOfThing(Game->GetHero(), ConvoyMembersVector[i])` (0x00F05CF1..0x00F05D11): retail's inner GetHero vtable is in eax (`push eax ; mov eax,[ecx] ; call [eax+118h]`, then the next line's spilled vector start in ecx), ours edx/eax. Probe (c2prio_op --rotation): one expression-temporary rotation step too many on that line -- the `&ConvoyMembersVector[i]` lea (base = the inlined begin() local #27, allocator eax) is rotation-assigned (pointer eax->ecx) where retail's register sequence needs it hint-assigned (c2 0x10775B1F: temp record +0x30 link set); with the same IL, eax for the inner vtable reproduces retail's schedule exactly (WAR edge push->vtable lifts the add).
// Next: find what sets the +0x30 hint link on the lea temp (gdb watch on the temp record while compiling probe wk/micro/m2a.cpp, which reproduces the site in 0.4 s), then the source shape that makes retail's begin() value a tied temp; 31 spellings tried (args/objects/casts/locals/front()/loop index types/inline_depth) are inert or worse; landed 0x00DFC630 shows the plain spelling gives our shape there.
// Inline-budget: #pragma inline_depth(0) at each terminating return -- retail jumps from every terminating return to one out-of-line ~CScriptThing 0x004AA840 call (0x00F05F23) and expands it inline at the normal end (as the landed CMeleeThunder::Main 0x00D58080); without it 1449v1312 md 339.
// Leaf: ?RefreshSpyAllegiences@CQ_AmbushTradersScript@NScript@@QAEXXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_CONVOYSPY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_AmbushTradersScript.h"

inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }

void NScript::CQ_AmbushTradersScript::RefreshSpyAllegiences()
{
    CScriptThing spy = Game->GetThingWithScriptName("AT_ConvoySpy");
    while (BanditsVector.size() != 0)
        BanditsVector.pop_back();
    while (ConvoyMembersVector.size() != 0)
        ConvoyMembersVector.pop_back();
    Game->GetAllThingsWithScriptName("AT_BanditAlly", BanditsVector);
    Game->GetAllThingsWithScriptName("AT_ConvoyMember", ConvoyMembersVector);
    Game->GetAllThingsWithScriptName("AT_Sentry", SentriesVector);
    if (spy.IsAlive()) {
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)   // 0x00F05F23: the terminating returns share the out-of-line ~CScriptThing 0x004AA840 call
                return;
#pragma inline_depth()
            }
        for (unsigned int i = 0; i < BanditsVector.size(); ++i) {
            if (BanditsVector[i].IsAlive()) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)   // 0x00F05F23: the terminating returns share the out-of-line ~CScriptThing 0x004AA840 call
                        return;
#pragma inline_depth()
                    }
                Game->EntitySetThingAsAllyOfThing(spy, BanditsVector[i]);
                Game->EntitySetThingAsAllyOfThing(BanditsVector[i], spy);
            }
        }
        if (SpyContactMade) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)   // 0x00F05F23: the terminating returns share the out-of-line ~CScriptThing 0x004AA840 call
                    return;
#pragma inline_depth()
                }
            for (unsigned int i = 0; i < ConvoyMembersVector.size(); ++i) {
                if (ConvoyMembersVector[i].IsAlive()) {
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)   // 0x00F05F23: the terminating returns share the out-of-line ~CScriptThing 0x004AA840 call
                            return;
#pragma inline_depth()
                        }
                    Game->MiniMapRemoveMarker(ConvoyMembersVector[i]);
                    Game->EntityUnsetThingAsAllyOfThing(spy, ConvoyMembersVector[i]);
                    Game->EntityUnsetThingAsAllyOfThing(ConvoyMembersVector[i], spy);
                    Game->EntitySetThingAsEnemyOfThing(spy, ConvoyMembersVector[i]);
                    Game->EntitySetThingAsEnemyOfThing(ConvoyMembersVector[i], spy);
                    Game->EntitySetThingAsEnemyOfThing(Game->GetHero(), ConvoyMembersVector[i]);
                    Game->EntitySetThingAsEnemyOfThing(ConvoyMembersVector[i], Game->GetHero());
                }
            }
            for (unsigned int i = 0; i < SentriesVector.size(); ++i) {
                if (SentriesVector[i].IsAlive()) {
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)   // 0x00F05F23: the terminating returns share the out-of-line ~CScriptThing 0x004AA840 call
                            return;
#pragma inline_depth()
                        }
                    Game->EntityUnsetThingAsAllyOfThing(spy, SentriesVector[i]);
                    Game->EntityUnsetThingAsAllyOfThing(SentriesVector[i], spy);
                    Game->EntitySetThingAsEnemyOfThing(spy, SentriesVector[i]);
                    Game->EntitySetThingAsEnemyOfThing(SentriesVector[i], spy);
                }
            }
        } else {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)   // 0x00F05F23: the terminating returns share the out-of-line ~CScriptThing 0x004AA840 call
                    return;
#pragma inline_depth()
                }
            for (unsigned int i = 0; i < ConvoyMembersVector.size(); ++i) {
                if (ConvoyMembersVector[i].IsAlive()) {
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)   // 0x00F05F23: the terminating returns share the out-of-line ~CScriptThing 0x004AA840 call
                            return;
#pragma inline_depth()
                        }
                    Game->EntitySetThingAsAllyOfThing(spy, ConvoyMembersVector[i]);
                    Game->EntitySetThingAsAllyOfThing(ConvoyMembersVector[i], spy);
                }
            }
        }
    }
}
