#include <vector>
#include <algorithm>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
static void check(bool b){if(!b)abort();}
struct Field {unsigned int ordinal;};
class CPersistContext {public:
    unsigned int mode,scalars,flags,defaults,read_values,write_values;int base_calls,vectors;bool loading;
    bool IsBinary() const{return mode==2||mode==3;}
    void Transfer(Field& x){check(x.ordinal==scalars++);}
    void TransferVector(std::vector<int>&){++vectors;}
    void Transfer(const char* name,bool& value,bool fallback);
};
class CThingBaseDef {public: void Transfer(CPersistContext& p){++p.base_calls;}};
class CThingCreatureDef : public CThingBaseDef { public:
    std::vector<int> NavigatorTypes;
    Field StunTimeMin;
    Field StunTimeMax;
    Field StunHealthThreshold;
    Field DazedTimeMin;
    Field DazedTimeMax;
    Field DazedHealthThreshold;
    Field NumSecondsToPersistOnDeath;
    Field MaxSlowWalkingSpeed;
    Field MaxWalkingSpeed;
    Field MaxJoggingSpeed;
    Field MaxRunningSpeed;
    Field MaxPushingSpeed;
    Field MaxFlyingSpeed;
    Field MaxHoverSpeedSlow;
    Field MaxHoverSpeedMedium;
    Field MaxHoverSpeedFast;
    Field SpeedRandomisationFactor;
    Field MaxAcceleration;
    Field HearingRadius;
    Field FOV;
    Field Sex;
    Field CreatureType;
    Field InitialWeaponDef;
    Field PrimaryMeleeWeaponDef;
    Field SecondaryMeleeWeaponDef;
    Field RangedWeaponDef;
    Field InitialSheathedWeapon;
    Field BrainDef;
    Field CreatureInteractionType;
    Field Property;
    Field Damage;
    Field DamageEffectDef;
    Field StunnedParticle;
    Field Graphic;
    Field EyeGraphic;
    Field DefaultOwner;
    Field ControlledMovementType;
    Field CombatTypeDef;
    Field ResetCombatTypeOnUnsheatheDefaultWeapons;
    Field HeroCombatDef;
    Field HeroCombatPCDef;
    Field CombatDiameter;
    Field LeaderTypeDef;
    Field RecoverEvadeHitsPercent;
    Field DeadBodyDecayEffect;
    bool DeferHitsIfBehindHitter:1;
    bool LeaveDeadCreature:1;
    bool AbleToStrafe:1;
    bool CreateDummyOnDeath:1;
    bool ImmuneToAttack:1;
    bool UseMeleeWeaponAsDefault:1;
    bool UnsheatheWeaponsAutomatically:1;
    bool IsWillable:1;
    bool IsIndestructible:1;
    bool IsInvulnerableDuringGenericResponse:1;
    bool IsPushedByExplosions:1;
    void Transfer(CPersistContext&);
};
static const char* expected_names[]={"DeferHitsIfBehindHitter","LeaveDeadCreature","AbleToStrafe","CreateDummyOnDeath","ImmuneToAttack","UseMeleeWeaponAsDefault","UnsheatheWeaponsAutomatically","IsWillable","IsIndestructible","IsInvulnerableDuringGenericResponse","IsPushedByExplosions"};
void CPersistContext::Transfer(const char* name,bool& value,bool fallback){
    check(flags<sizeof(expected_names)/sizeof(*expected_names) && !strcmp(name,expected_names[flags]));
    unsigned int bit=1u<<flags++;if(fallback)defaults|=bit;if(value)read_values|=bit;if(loading)value=(write_values&bit)!=0;
}
void CThingCreatureDef::Transfer(CPersistContext& persist)
{
    CThingBaseDef::Transfer(persist);
    persist.Transfer(StunTimeMin);
    persist.Transfer(StunTimeMax);
    persist.Transfer(StunHealthThreshold);
    persist.Transfer(DazedTimeMin);
    persist.Transfer(DazedTimeMax);
    persist.Transfer(DazedHealthThreshold);
    persist.Transfer(NumSecondsToPersistOnDeath);
    persist.Transfer(MaxSlowWalkingSpeed);
    persist.Transfer(MaxWalkingSpeed);
    persist.Transfer(MaxJoggingSpeed);
    persist.Transfer(MaxRunningSpeed);
    persist.Transfer(MaxPushingSpeed);
    persist.Transfer(MaxFlyingSpeed);
    persist.Transfer(MaxHoverSpeedSlow);
    persist.Transfer(MaxHoverSpeedMedium);
    persist.Transfer(MaxHoverSpeedFast);
    persist.Transfer(SpeedRandomisationFactor);
    persist.Transfer(MaxAcceleration);
    persist.Transfer(HearingRadius);
    persist.Transfer(FOV);
    persist.Transfer(Sex);
    persist.Transfer(CreatureType);
    persist.TransferVector(NavigatorTypes);
    if (!persist.IsBinary()) {
        std::sort(NavigatorTypes.begin(), NavigatorTypes.end());
        NavigatorTypes.erase(std::unique(NavigatorTypes.begin(), NavigatorTypes.end()), NavigatorTypes.end());
    }
    persist.Transfer(InitialWeaponDef);
    persist.Transfer(PrimaryMeleeWeaponDef);
    persist.Transfer(SecondaryMeleeWeaponDef);
    persist.Transfer(RangedWeaponDef);
    persist.Transfer(InitialSheathedWeapon);
    persist.Transfer(BrainDef);
    persist.Transfer(CreatureInteractionType);
    persist.Transfer(Property);
    persist.Transfer(Damage);
    persist.Transfer(DamageEffectDef);
    persist.Transfer(StunnedParticle);
    persist.Transfer(Graphic);
    persist.Transfer(EyeGraphic);
    persist.Transfer(DefaultOwner);
    persist.Transfer(ControlledMovementType);
    persist.Transfer(CombatTypeDef);
    persist.Transfer(ResetCombatTypeOnUnsheatheDefaultWeapons);
    persist.Transfer(HeroCombatDef);
    persist.Transfer(HeroCombatPCDef);
    persist.Transfer(CombatDiameter);
    {
        bool temp = DeferHitsIfBehindHitter;
        persist.Transfer("DeferHitsIfBehindHitter", temp, true);
        DeferHitsIfBehindHitter = temp;
    }
    persist.Transfer(LeaderTypeDef);
    persist.Transfer(RecoverEvadeHitsPercent);
    persist.Transfer(DeadBodyDecayEffect);
    {
        bool temp = LeaveDeadCreature;
        persist.Transfer("LeaveDeadCreature", temp, false);
        LeaveDeadCreature = temp;
    }
    {
        bool temp = AbleToStrafe;
        persist.Transfer("AbleToStrafe", temp, false);
        AbleToStrafe = temp;
    }
    {
        bool temp = CreateDummyOnDeath;
        persist.Transfer("CreateDummyOnDeath", temp, false);
        CreateDummyOnDeath = temp;
    }
    {
        bool temp = ImmuneToAttack;
        persist.Transfer("ImmuneToAttack", temp, false);
        ImmuneToAttack = temp;
    }
    {
        bool temp = UseMeleeWeaponAsDefault;
        persist.Transfer("UseMeleeWeaponAsDefault", temp, true);
        UseMeleeWeaponAsDefault = temp;
    }
    {
        bool temp = UnsheatheWeaponsAutomatically;
        persist.Transfer("UnsheatheWeaponsAutomatically", temp, true);
        UnsheatheWeaponsAutomatically = temp;
    }
    {
        bool temp = IsWillable;
        persist.Transfer("IsWillable", temp, false);
        IsWillable = temp;
    }
    {
        bool temp = IsIndestructible;
        persist.Transfer("IsIndestructible", temp, false);
        IsIndestructible = temp;
    }
    {
        bool temp = IsInvulnerableDuringGenericResponse;
        persist.Transfer("IsInvulnerableDuringGenericResponse", temp, false);
        IsInvulnerableDuringGenericResponse = temp;
    }
    {
        bool temp = IsPushedByExplosions;
        persist.Transfer("IsPushedByExplosions", temp, true);
        IsPushedByExplosions = temp;
    }
}
int main(){for(unsigned int mode=0;mode<4;++mode)for(unsigned int seed=0;seed<32;++seed)for(int load=0;load<2;++load){
    CThingCreatureDef d;CPersistContext p;unsigned int initial=(seed*12345u)&2047;
    p.mode=mode;p.loading=load!=0;p.scalars=p.flags=p.read_values=p.defaults=0;p.write_values=initial^2047;p.base_calls=p.vectors=0;
    const int input[]={3,1,3,2,1}; d.NavigatorTypes.assign(input,input+5);
    d.StunTimeMin.ordinal=0;
    d.StunTimeMax.ordinal=1;
    d.StunHealthThreshold.ordinal=2;
    d.DazedTimeMin.ordinal=3;
    d.DazedTimeMax.ordinal=4;
    d.DazedHealthThreshold.ordinal=5;
    d.NumSecondsToPersistOnDeath.ordinal=6;
    d.MaxSlowWalkingSpeed.ordinal=7;
    d.MaxWalkingSpeed.ordinal=8;
    d.MaxJoggingSpeed.ordinal=9;
    d.MaxRunningSpeed.ordinal=10;
    d.MaxPushingSpeed.ordinal=11;
    d.MaxFlyingSpeed.ordinal=12;
    d.MaxHoverSpeedSlow.ordinal=13;
    d.MaxHoverSpeedMedium.ordinal=14;
    d.MaxHoverSpeedFast.ordinal=15;
    d.SpeedRandomisationFactor.ordinal=16;
    d.MaxAcceleration.ordinal=17;
    d.HearingRadius.ordinal=18;
    d.FOV.ordinal=19;
    d.Sex.ordinal=20;
    d.CreatureType.ordinal=21;
    d.InitialWeaponDef.ordinal=22;
    d.PrimaryMeleeWeaponDef.ordinal=23;
    d.SecondaryMeleeWeaponDef.ordinal=24;
    d.RangedWeaponDef.ordinal=25;
    d.InitialSheathedWeapon.ordinal=26;
    d.BrainDef.ordinal=27;
    d.CreatureInteractionType.ordinal=28;
    d.Property.ordinal=29;
    d.Damage.ordinal=30;
    d.DamageEffectDef.ordinal=31;
    d.StunnedParticle.ordinal=32;
    d.Graphic.ordinal=33;
    d.EyeGraphic.ordinal=34;
    d.DefaultOwner.ordinal=35;
    d.ControlledMovementType.ordinal=36;
    d.CombatTypeDef.ordinal=37;
    d.ResetCombatTypeOnUnsheatheDefaultWeapons.ordinal=38;
    d.HeroCombatDef.ordinal=39;
    d.HeroCombatPCDef.ordinal=40;
    d.CombatDiameter.ordinal=41;
    d.LeaderTypeDef.ordinal=42;
    d.RecoverEvadeHitsPercent.ordinal=43;
    d.DeadBodyDecayEffect.ordinal=44;
    d.DeferHitsIfBehindHitter=(initial&(1u<<0))!=0;
    d.LeaveDeadCreature=(initial&(1u<<1))!=0;
    d.AbleToStrafe=(initial&(1u<<2))!=0;
    d.CreateDummyOnDeath=(initial&(1u<<3))!=0;
    d.ImmuneToAttack=(initial&(1u<<4))!=0;
    d.UseMeleeWeaponAsDefault=(initial&(1u<<5))!=0;
    d.UnsheatheWeaponsAutomatically=(initial&(1u<<6))!=0;
    d.IsWillable=(initial&(1u<<7))!=0;
    d.IsIndestructible=(initial&(1u<<8))!=0;
    d.IsInvulnerableDuringGenericResponse=(initial&(1u<<9))!=0;
    d.IsPushedByExplosions=(initial&(1u<<10))!=0;
    d.Transfer(p);
    check(p.base_calls==1 && p.vectors==1 && p.scalars==45 && p.flags==11 && p.defaults==1121 && p.read_values==initial);
    if(p.IsBinary())check(d.NavigatorTypes.size()==5 && std::equal(d.NavigatorTypes.begin(),d.NavigatorTypes.end(),input));
    else check(d.NavigatorTypes.size()==3 && d.NavigatorTypes[0]==1 && d.NavigatorTypes[1]==2 && d.NavigatorTypes[2]==3);
    unsigned int result=0;
    if(d.DeferHitsIfBehindHitter)result|=1u<<0;
    if(d.LeaveDeadCreature)result|=1u<<1;
    if(d.AbleToStrafe)result|=1u<<2;
    if(d.CreateDummyOnDeath)result|=1u<<3;
    if(d.ImmuneToAttack)result|=1u<<4;
    if(d.UseMeleeWeaponAsDefault)result|=1u<<5;
    if(d.UnsheatheWeaponsAutomatically)result|=1u<<6;
    if(d.IsWillable)result|=1u<<7;
    if(d.IsIndestructible)result|=1u<<8;
    if(d.IsInvulnerableDuringGenericResponse)result|=1u<<9;
    if(d.IsPushedByExplosions)result|=1u<<10;
    check(result==(load?p.write_values:initial));
}puts("00671600_TEST PASS");return 0;}
