#include <stdio.h>
#include <string>

typedef std::string CCharString;
struct C3DVector {
    float X,Y,Z;
    C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z) {}
    C3DVector operator-(const C3DVector& o)const { return C3DVector(X-o.X,Y-o.Y,Z-o.Z); }
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer(T* ptr=0):Object(ptr) {}
    T* operator->() const { return Object; }
};

template<class T> struct CIntelligentPointer {
    T* Ptr;
    CIntelligentPointer(T* ptr=0):Ptr(ptr) {}
    T* GetPItem() const { return Ptr; }
    T* operator->() const { return Ptr; }
};
#define IS_THING_ALIVE(ptr) ((ptr).GetPItem() && !(ptr).GetPItem()->Dead)
struct CRightHandedSet {
    C3DVector Up,Direction;
    CRightHandedSet(const C3DVector& up,const C3DVector& dir):Up(up),Direction(dir) {}
};
struct CThing;
struct CTCDParticleEmitter {
    static CThing* Create(long,const C3DVector&,bool);
    CThing* Parent;
    long Layer;
    C3DVector Direction;
    bool Active,Bad;
    CTCDParticleEmitter():Parent(0),Layer(0),Active(false),Bad(false) {}
    void AttachToThing(CThing& thing,long layer,const CCharString& socket,int index,float age)
    { Parent=&thing;Layer=layer;Bad=!socket.empty()||index||age!=0.0f; }
    void SetEmitterOrientation(const CRightHandedSet& orientation) { Direction=orientation.Direction;Bad=Bad||orientation.Up.Z!=1; }
    void SetAlwaysUpdate(bool active) { Active=active; }
};
struct CThingController {
    C3DVector GetFacingVector() const { return C3DVector(1,2,3); }
};
struct CThing {
    C3DVector Position;
    bool Dead;
    unsigned Sounds;
    CThingController Controller;
    CTCDParticleEmitter Emitter;
    CThing():Dead(false),Sounds(0) {}
    void MakeNoise(const CCharString& category) { if(category=="SIMPLE_NOISE") ++Sounds; }
    const C3DVector& GetPos() const { return Position; }
    bool GetTC(CTCDParticleEmitter** out) { *out=&Emitter;return true; }
    CThingController* GetPhysicsTC() { return &Controller; }
};
typedef CThing CThingCreatureBase;
CThing Effects[3];
CThing* CTCDParticleEmitter::Create(long index,const C3DVector& pos,bool flag)
{
    if(index<0 || index>=3 || flag) return 0;
    Effects[index].Position=pos;
    return &Effects[index];
}
struct CSpecialEffectsDef {
    int ShieldLevel;
    bool Left,Right;
    CSpecialEffectsDef():ShieldLevel(2),Left(true),Right(true) {}
    bool GetEffectFromString(const CCharString& name,long& out) const
    {
        static const char* const shields[]={"SHIELD_EFFECT_01","SHIELD_EFFECT_02","SHIELD_EFFECT_03","SHIELD_EFFECT_04"};
        if(name==shields[ShieldLevel]) { out=0;return true; }
        if(Left&&name=="LEFT_FEET_EFFECT") { out=1;return true; }
        if(Right&&name=="RIGHT_FEET_EFFECT") { out=2;return true; }
        return false;
    }
};
struct CDefClassBase {
    CSpecialEffectsDef Effects;
    bool Available;
    CDefClassBase():Available(false) {}
    bool GetSubDef(CDefPointer<const CSpecialEffectsDef>* out) { out->Object=&Effects;return Available; }
};
struct CTCBattleCharge {
    CThing* ParentThing;
    CIntelligentPointer<CThing> PTarget,ShieldEffect,LeftFeetEffect,RightFeetEffect;
    CDefClassBase Definition;
    CDefClassBase* PDef;
    C3DVector DirectionOverrideVector;
    long SpellLevel;
    bool DirectionOverride,IsZTargetting;
    explicit CTCBattleCharge(CThing& thing):ParentThing(&thing),PDef(&Definition),SpellLevel(2),DirectionOverride(true),IsZTargetting(false) {}
    CThing& GetParentThing() { return *ParentThing; }
    void CreateStartUpEffect();
};

void CTCBattleCharge::CreateStartUpEffect()
{
    CThingCreatureBase& parent_creature = static_cast<CThingCreatureBase&>(GetParentThing());
    parent_creature.MakeNoise("SIMPLE_NOISE");
    long effect = 0;
    CDefPointer<const CSpecialEffectsDef> pspecial_effects;
    if (PDef->GetSubDef(&pspecial_effects)) {
        C3DVector movement_direction;
        if (DirectionOverride && !IsZTargetting)
            movement_direction = DirectionOverrideVector;
        else if (IS_THING_ALIVE(PTarget))
            movement_direction = PTarget->GetPos() - parent_creature.GetPos();
        else
            movement_direction = parent_creature.GetPhysicsTC()->GetFacingVector();
        CRightHandedSet rhset(C3DVector(0.0f, 0.0f, 1.0f), movement_direction);
        CCharString shield_effect_name("SHIELD_EFFECT_01");
        switch (SpellLevel) {
        case 1: shield_effect_name = "SHIELD_EFFECT_02"; break;
        case 2: shield_effect_name = "SHIELD_EFFECT_03"; break;
        case 3: shield_effect_name = "SHIELD_EFFECT_04"; break;
        }
        if (pspecial_effects->GetEffectFromString(shield_effect_name, effect)) {
            ShieldEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect, parent_creature.GetPos(), false));
            if (IS_THING_ALIVE(ShieldEffect)) {
                CTCDParticleEmitter* particle_driver;
                ShieldEffect->GetTC(&particle_driver);
                particle_driver->AttachToThing(parent_creature, 0x28, "", 0, 0.0f);
                particle_driver->SetEmitterOrientation(rhset);
                particle_driver->SetAlwaysUpdate(true);
            }
        }
        if (pspecial_effects->GetEffectFromString("LEFT_FEET_EFFECT", effect)) {
            LeftFeetEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect, GetParentThing().GetPos(), false));
            if (IS_THING_ALIVE(LeftFeetEffect)) {
                CTCDParticleEmitter* particle_driver;
                LeftFeetEffect->GetTC(&particle_driver);
                particle_driver->AttachToThing(parent_creature, 8, "", 0, 0.0f);
                particle_driver->SetEmitterOrientation(rhset);
                particle_driver->SetAlwaysUpdate(true);
            }
        }
        if (pspecial_effects->GetEffectFromString("RIGHT_FEET_EFFECT", effect)) {
            RightFeetEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect, GetParentThing().GetPos(), false));
            if (IS_THING_ALIVE(RightFeetEffect)) {
                CTCDParticleEmitter* particle_driver;
                RightFeetEffect->GetTC(&particle_driver);
                particle_driver->AttachToThing(parent_creature, 8, "", 0, 0.0f);
                particle_driver->SetEmitterOrientation(rhset);
                particle_driver->SetAlwaysUpdate(true);
            }
        }
    }
}

int main()
{
    CThing creature,target;
    creature.Position=C3DVector(5,4,3);
    target.Position=C3DVector(8,9,10);
    CTCBattleCharge charge(creature);
    charge.CreateStartUpEffect();
    if(creature.Sounds!=1 || charge.ShieldEffect.GetPItem())
        return 1;
    charge.Definition.Available=true;
    charge.DirectionOverrideVector=C3DVector(9,8,7);
    charge.CreateStartUpEffect();
    if(creature.Sounds!=2 || charge.ShieldEffect.GetPItem()!=&Effects[0] ||
        charge.LeftFeetEffect.GetPItem()!=&Effects[1] || charge.RightFeetEffect.GetPItem()!=&Effects[2])
        return 2;
    for(unsigned i=0;i!=3;++i)
        if(Effects[i].Emitter.Parent!=&creature || Effects[i].Emitter.Layer!=(i==0?0x28:8) ||
           Effects[i].Emitter.Direction.X!=9 || !Effects[i].Emitter.Active || Effects[i].Emitter.Bad || Effects[i].Position.X!=5)
            return 3;
    charge.IsZTargetting=true;
    charge.PTarget=CIntelligentPointer<CThing>(&target);
    charge.CreateStartUpEffect();
    if(Effects[0].Emitter.Direction.X!=3 || Effects[0].Emitter.Direction.Y!=5 || Effects[0].Emitter.Direction.Z!=7)
        return 4;
    target.Dead=true;
    charge.CreateStartUpEffect();
    if(Effects[0].Emitter.Direction.X!=1 || Effects[0].Emitter.Direction.Y!=2 || Effects[0].Emitter.Direction.Z!=3)
        return 5;
    puts("007a76f0_TEST PASS");
    return 0;
}
