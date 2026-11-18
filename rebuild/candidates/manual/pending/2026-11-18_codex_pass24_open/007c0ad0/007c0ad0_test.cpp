#include <stdio.h>
#include <string>

typedef std::string CCharString;
struct C3DVector {
    float X,Y,Z;
    C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z) {}
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer(T* ptr=0):Object(ptr) {}
    T* operator->() const { return Object; }
};

#ifndef _MSC_VER
#define __fastcall
#endif
struct CRightHandedSet { int Facing; };
struct CMeleeCombatAbilityDef { };
struct CThing;
struct CPlayer { long GetNumber() const { return 4; } };
struct CTCPhysicsBase {
    int Facing;
    void SetRHSet(const CRightHandedSet& set) { Facing=set.Facing; }
};
struct CTCCreatureModeManager {
    int Mode;
    void AddMode(int mode) { Mode=mode; }
};
struct CThingShotDef { long ObjectDef,ShotDefinitionClassName_TablePos; };
namespace NThingShot {
struct CTCShotDefinitionBase {
    unsigned Calls;
    bool Good;
    CTCShotDefinitionBase():Calls(0),Good(false) {}
    void Init(const CDefPointer<const CThingShotDef>& def,CThing* shooter,CThing* weapon,const CThing* target,
        float accuracy,float damage,float buildup,const CDefPointer<const CMeleeCombatAbilityDef>& ability)
    {
        ++Calls;Good=def.Object&&shooter&&weapon&&target&&accuracy==0.5f&&damage==2.0f&&buildup==3.0f&&ability.Object;
    }
};
}
struct CThing {
    C3DVector Position;
    CPlayer Player;
    CTCPhysicsBase Physics;
    CTCCreatureModeManager Modes;
    NThingShot::CTCShotDefinitionBase Shot;
    bool Dead,Bad;
    CCharString Script;
    CThing():Dead(false),Bad(false) { Physics.Facing=0;Modes.Mode=0; }
    const CPlayer& GetPlayer() const { return Player; }
    void GetTC(CTCCreatureModeManager** out) { *out=&Modes; }
    CTCPhysicsBase* GetPhysicsTC() { return &Physics; }
    bool IsDead() const { return Dead; }
    const CCharString& GetScriptName() const { return Script; }
    void SetScriptName(const CCharString& name) { Script=name; }
    NThingShot::CTCShotDefinitionBase* AddTC(const CCharString& name,bool active,int arg)
    { Bad=Bad||name!="ShotTC"||!active||arg!=0;return &Shot; }
};
struct CThingObject {
    static CThing Instance;
    static CThing* Create(long index,const C3DVector& pos,long player,const CRightHandedSet* set,unsigned int uid,
        const CCharString& label)
    {
        Instance.Bad=index!=16||player!=4||!set||uid||!label.empty();
        Instance.Position=pos;Instance.Physics.Facing=set->Facing;return &Instance;
    }
};
CThing CThingObject::Instance;
struct CThingAICreatureInit {
    bool Valid;
    CThingAICreatureInit(int first,int second,bool third,const CCharString& fourth,const CCharString& fifth,bool sixth)
        :Valid(!first&&!second&&!third&&fourth.empty()&&fifth.empty()&&sixth) {}
};
struct CThingAICreature {
    static CThing Instance;
    static CThing* Create(long index,const C3DVector& pos,long player,const CThingAICreatureInit& setup)
    { Instance.Bad=index!=16||player!=4||!setup.Valid;Instance.Position=pos;return &Instance; }
};
CThing CThingAICreature::Instance;
struct CDefName {
    CCharString Text;
    explicit CDefName(const CCharString& name):Text(name) {}
    CCharString GetString() const { return Text; }
};
struct CGameDefinitionManager {
    CThingShotDef Def;
    bool IsCreature;
    CGameDefinitionManager():IsCreature(false) { Def.ObjectDef=16;Def.ShotDefinitionClassName_TablePos=52; }
    void GetDef(long id,CDefPointer<const CThingShotDef>* out) const { out->Object=id==6?&Def:0; }
    CDefName GetDefClassTypeNameFromGlobalIndex(long id) const
    { return CDefName(id==16?(IsCreature?"CREATURE":"OBJECT"):""); }
};
struct CMainGameComponent {
    CGameDefinitionManager Definitions;
    static CMainGameComponent* Get() { static CMainGameComponent game;return &game; }
    const CGameDefinitionManager& PeekDefinitionManager() const { return Definitions; }
};
struct DefTable { CCharString GetString(long id) const { return id==52?"ShotTC":""; } };
DefTable GDefStringTable;
bool g_bShotsEnabled=false;
namespace NThingShot {
void GetCreatureFirePosAndOrientation(const CThing&,C3DVector& position,CRightHandedSet& set)
{ position=C3DVector(7,8,9);set.Facing=8; }

CThing* __fastcall CreateShot(long shot_def_index, CThing* shooter, CThing* weapon, const CThing* target, float accuracy,
                              float damage_multiplier, float buildup_time, const CDefPointer<const CMeleeCombatAbilityDef>& ability,
                              const C3DVector* pos, const CRightHandedSet* orientation)
{
    if (!g_bShotsEnabled)
        return 0;
    CThing* shot = 0;
    C3DVector shot_pos;
    CRightHandedSet shot_orientation;
    if (pos && orientation) {
        shot_pos = *pos;
        shot_orientation = *orientation;
    } else {
        GetCreatureFirePosAndOrientation(*shooter, shot_pos, shot_orientation);
    }
    CDefPointer<const CThingShotDef> shot_def;
    const CGameDefinitionManager& definitions = CMainGameComponent::Get()->PeekDefinitionManager();
    definitions.GetDef(shot_def_index, &shot_def);
    long object_def = shot_def->ObjectDef;
    CCharString class_name = definitions.GetDefClassTypeNameFromGlobalIndex(object_def).GetString();
    if (class_name == "OBJECT") {
        shot = CThingObject::Create(object_def, shot_pos, shooter->GetPlayer().GetNumber(), &shot_orientation, 0, "");
    } else if (class_name == "CREATURE") {
        shot = CThingAICreature::Create(object_def, shot_pos, shooter->GetPlayer().GetNumber(),
                                        CThingAICreatureInit(0, 0, false, "", "", true));
        CTCCreatureModeManager* modes;
        shot->GetTC(&modes);
        modes->AddMode(57);
        shot->GetPhysicsTC()->SetRHSet(shot_orientation);
        CCharString suffix("creature_shot");
        CCharString script_name;
        if (shooter && !shooter->IsDead())
            script_name = shooter->GetScriptName() + "_" + suffix;
        else
            script_name = suffix;
        shot->SetScriptName(script_name);
    }
    CTCShotDefinitionBase* shot_tc =
        (CTCShotDefinitionBase*)shot->AddTC(GDefStringTable.GetString(shot_def->ShotDefinitionClassName_TablePos), true, 0);
    shot_tc->Init(shot_def, shooter, weapon, target, accuracy, damage_multiplier, buildup_time, ability);
    return shot;
}
}

int main()
{
    using NThingShot::CreateShot;
    CThing shooter,weapon,target;
    shooter.Script="hero";
    const CMeleeCombatAbilityDef ability;
    C3DVector pos(1,2,3);
    CRightHandedSet set={5};
    if(CreateShot(6,&shooter,&weapon,&target,0.5f,2,3,&ability,&pos,&set))
        return 1;
    g_bShotsEnabled=true;
    CThing* object=CreateShot(6,&shooter,&weapon,&target,0.5f,2,3,&ability,&pos,&set);
    if(object!=&CThingObject::Instance || object->Position.X!=1 || object->Physics.Facing!=5 ||
       object->Bad || object->Shot.Calls!=1 || !object->Shot.Good)
        return 2;
    CMainGameComponent::Get()->Definitions.IsCreature=true;
    CThing* creature=CreateShot(6,&shooter,&weapon,&target,0.5f,2,3,&ability,&pos,0);
    if(creature!=&CThingAICreature::Instance || creature->Position.X!=7 || creature->Physics.Facing!=8 ||
       creature->Modes.Mode!=57 || creature->Script!="hero_creature_shot" || creature->Bad || !creature->Shot.Good)
        return 3;
    shooter.Dead=true;
    creature=CreateShot(6,&shooter,&weapon,&target,0.5f,2,3,&ability,&pos,&set);
    if(creature->Script!="creature_shot" || creature->Physics.Facing!=5 || creature->Shot.Calls!=2)
        return 4;
    puts("007c0ad0_TEST PASS");
    return 0;
}
