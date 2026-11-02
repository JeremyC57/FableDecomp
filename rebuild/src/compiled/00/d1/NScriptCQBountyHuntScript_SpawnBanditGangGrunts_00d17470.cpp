// NScript::CQ_BountyHuntScript::SpawnBanditGangGrunts(CCharString) -- spawn the configured bandit gang grunts.
// Leaf: ?SpawnBanditGangGrunts@CQ_BountyHuntScript@NScript@@QAEXVCCharString@@@Z
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#include <stdlib.h>
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "fable_string.h"
class CScriptThing;
#pragma auto_inline(off)
template<> CCountedPointer<CScriptThing>& CCountedPointer<CScriptThing>::operator=(const CCountedPointer<CScriptThing>& other)
{
    ShareData(other.Data,other.Info);
    return *this;
}
#pragma auto_inline(on)
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_BountyHuntScript.h"
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp=other.PImp; return *this; }
inline bool CScriptThing::IsAlive() const { return !PImp.IsNull() && PImp->IsAlive(); }
inline const C3DVector& CScriptThing::GetPos() const { if(PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline void CScriptThing::SetDataString(const CCharString& data) { if(!PImp.IsNull()) PImp->SetDataString(data); }
void NScript::CQ_BountyHuntScript::SpawnBanditGangGrunts(CCharString spawnName)
{
    std::vector<CScriptThing> spawnPoints;
    long numPoints=Game->GetAllThingsWithScriptName(spawnName,spawnPoints);
    CScriptThing spawnMkr;
    CScriptThing grunt;
    for(long idx=0;idx<numPoints;++idx) {
        spawnMkr=spawnPoints[idx];
        if(spawnMkr.IsAlive()) {
            if(IsActiveThreadTerminating()) return;
            int choice=rand()%4;
            if(choice==0) {
                if(IsActiveThreadTerminating()) return;
                grunt=Game->CreateCreature("CREATURE_BANDIT_GRUNT_LEVEL3_A",spawnMkr.GetPos(),"BH_GangGruntSpawn",false);
            } else if(choice==1) {
                if(IsActiveThreadTerminating()) return;
                grunt=Game->CreateCreature("CREATURE_BANDIT_GRUNT_LEVEL3_B",spawnMkr.GetPos(),"BH_GangGruntSpawn",false);
            } else if(choice==2) {
                if(IsActiveThreadTerminating()) return;
                grunt=Game->CreateCreature("CREATURE_BANDIT_GRUNT_LEVEL3_C",spawnMkr.GetPos(),"BH_GangGruntSpawn",false);
            } else {
                if(IsActiveThreadTerminating()) return;
                grunt=Game->CreateCreature("CREATURE_BANDIT_GRUNT_LEVEL3_D",spawnMkr.GetPos(),"BH_GangGruntSpawn",false);
            }
            while(!grunt.IsAlive()) {
                Game->NewScriptFrame();
                if(IsActiveThreadTerminating()) return;
            }
            if(IsActiveThreadTerminating()) return;
            CCharString num=spawnMkr.GetDataString();
            grunt.SetDataString(spawnMkr.GetDataString());
            Game->EntityAttachToScript(grunt,Game->GetActiveQuestName());
        }
    }
}