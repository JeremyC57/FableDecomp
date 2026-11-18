#include <cassert>
#include <cstdio>
#include <cstring>

struct C3DVector { float X,Y,Z; };
struct CThing;
struct CTCDParticleEmitter;
struct TestOptions {
    bool Combat,Melee,HasDef,HasEffect,Carrying,LiveWeapon,CreateSucceeds;
    unsigned CombatQueries,MeleeQueries,FpsQueries,SubDefQueries,EffectQueries,PositionQueries,
             CarryQueries,PrimaryQueries,Creates,DriverQueries,Attaches;
    TestOptions():Combat(false),Melee(false),HasDef(false),HasEffect(false),Carrying(false),
        LiveWeapon(false),CreateSucceeds(false),CombatQueries(0),MeleeQueries(0),FpsQueries(0),
        SubDefQueries(0),EffectQueries(0),PositionQueries(0),CarryQueries(0),PrimaryQueries(0),
        Creates(0),DriverQueries(0),Attaches(0) {}
};
static TestOptions Test;
template<class T> class CIntelligentPointer {
    T* Pointer;
public:
    explicit CIntelligentPointer(T* pointer=0):Pointer(pointer) {}
    T* Get() const { return Pointer; }
};
template<class T> class CDefPointer {
    T* Pointer;
public:
    CDefPointer():Pointer(0) {}
    T* operator->() const { return Pointer; }
    void Set(T* pointer) { Pointer=pointer; }
};
struct CSpecialEffectsDef {
    bool GetEffectFromString(const char* name,long& index) const
    {
        assert(std::strcmp(name,"POWER_UP_INDICATOR")==0 && index==0);
        ++Test.EffectQueries;
        index=47;
        return Test.HasEffect;
    }
};
struct CThingBaseDef {
    CSpecialEffectsDef Effects;
    bool GetSubDef(CDefPointer<const CSpecialEffectsDef>* result) const
    {
        ++Test.SubDefQueries;
        if (Test.HasDef)
            result->Set(&Effects);
        return Test.HasDef;
    }
};
struct CTCCombat {
    bool IsInMelee() const { ++Test.MeleeQueries; return Test.Melee; }
};
struct CTCCarrying {
    CThing* Weapon;
    CThing* GetThingInPrimarySlot() const { ++Test.PrimaryQueries; return Weapon; }
};
struct CThing {
    bool Alive;
    CThingBaseDef BaseDef;
    CTCCombat Combat;
    CTCCarrying Carrying;
    CThing():Alive(false) { Carrying.Weapon=0; }
    bool GetTC(CTCCombat** result)
    {
        assert(*result==0);
        ++Test.CombatQueries;
        if (Test.Combat) *result=&Combat;
        return Test.Combat;
    }
    bool GetTC(CTCCarrying** result)
    {
        ++Test.CarryQueries;
        if (Test.Carrying) *result=&Carrying;
        return Test.Carrying;
    }
    const CThingBaseDef& PeekBaseDef() const { return BaseDef; }
    C3DVector GetPos() const
    {
        ++Test.PositionQueries;
        C3DVector position={2,3,5};
        return position;
    }
    void GetTCDriver(CTCDParticleEmitter** result);
};
static CThing Weapon,CreatedEmitter;
static bool IS_THING_ALIVE(const CThing* thing) { return thing && thing->Alive; }
static bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& thing) { return IS_THING_ALIVE(thing.Get()); }
struct CTCDParticleEmitter {
    static CThing* Create(long index,const C3DVector& pos,bool dormant)
    {
        assert(index==47 && pos.X==2 && pos.Y==3 && pos.Z==5 && !dormant);
        ++Test.Creates;
        CreatedEmitter.Alive=Test.CreateSucceeds;
        return Test.CreateSucceeds ? &CreatedEmitter : 0;
    }
    void AttachToThing(CThing& weapon,int slot,const char* name,int flags,float offset)
    {
        assert(&weapon==&Weapon && slot==12 && flags==0 && offset==0.0f);
        assert(std::strcmp(name,"weapon_pos_a")==0);
        ++Test.Attaches;
    }
};
void CThing::GetTCDriver(CTCDParticleEmitter** result)
{
    static CTCDParticleEmitter driver;
    assert(this==&CreatedEmitter && *result==0);
    ++Test.DriverQueries;
    *result=&driver;
}
struct CMainGameComponent {
    static long GetConstantFPS() { ++Test.FpsQueries; return 30; }
};
struct CTCAICreatureWillPowerIndicator {
    CThing Parent;
    CIntelligentPointer<CThing> PEmitter;
    long Timer;
    bool ShowEffect,PoweredUp;
    CThing& GetParentThing() { return Parent; }
    void FrameUpdate();
};

void CTCAICreatureWillPowerIndicator::FrameUpdate()
{
    CTCCombat* ptc_combat = 0;
    if (!GetParentThing().GetTC(&ptc_combat) || !ptc_combat->IsInMelee() || PoweredUp)
        return;
    if ((float)Timer <= (float)CMainGameComponent::GetConstantFPS() && !IS_THING_ALIVE(PEmitter) && ShowEffect) {
        CDefPointer<const CSpecialEffectsDef> pspecial_fx;
        long fx_index = 0;
        if (GetParentThing().PeekBaseDef().GetSubDef(&pspecial_fx) && pspecial_fx->GetEffectFromString("POWER_UP_INDICATOR", fx_index)) {
            CTCCarrying* ptc_carrying;
            C3DVector weapon_pos = GetParentThing().GetPos();
            if (GetParentThing().GetTC(&ptc_carrying)) {
                CThing* pweapon = ptc_carrying->GetThingInPrimarySlot();
                if (IS_THING_ALIVE(pweapon)) {
                    CThing* pemitter = CTCDParticleEmitter::Create(fx_index, weapon_pos, false);
                    if (pemitter) {
                        PEmitter = CIntelligentPointer<CThing>(pemitter);
                        CTCDParticleEmitter* particle_driver = 0;
                        pemitter->GetTCDriver(&particle_driver);
                        if (pweapon)
                            particle_driver->AttachToThing(*pweapon, 0xc, "weapon_pos_a", 0, 0.0f);
                    }
                }
            }
        }
    }
    if (Timer <= 0)
        PoweredUp = true;
    else
        --Timer;
}

int main()
{
    const long times[]={-1,0,1,30,31,100};
    for (unsigned i=0; i<sizeof times/sizeof *times; ++i) {
        for (unsigned mask=0; mask<1024; ++mask) {
            Test=TestOptions();
            Test.Combat=(mask & 1)!=0;
            Test.Melee=(mask & 2)!=0;
            Test.HasDef=(mask & 32)!=0;
            Test.HasEffect=(mask & 64)!=0;
            Test.Carrying=(mask & 128)!=0;
            Test.LiveWeapon=(mask & 256)!=0;
            Test.CreateSucceeds=(mask & 512)!=0;
            CTCAICreatureWillPowerIndicator indicator;
            CThing active_driver;
            active_driver.Alive=true;
            const bool active=(mask & 8)!=0;
            if (active)
                indicator.PEmitter=CIntelligentPointer<CThing>(&active_driver);
            Weapon.Alive=Test.LiveWeapon;
            indicator.Parent.Carrying.Weapon=&Weapon;
            indicator.ShowEffect=(mask & 16)!=0;
            indicator.PoweredUp=(mask & 4)!=0;
            indicator.Timer=times[i];
            const bool moving=Test.Combat && Test.Melee && !indicator.PoweredUp;
            const bool subdef=moving && times[i]<=30 && !active && indicator.ShowEffect;
            const bool effect=subdef && Test.HasDef;
            const bool carrying=effect && Test.HasEffect;
            const bool primary=carrying && Test.Carrying;
            const bool create=primary && Test.LiveWeapon;
            const bool attach=create && Test.CreateSucceeds;
            const bool power=indicator.PoweredUp || (moving && times[i]<=0);

            indicator.FrameUpdate();

            assert(Test.CombatQueries==1 && Test.MeleeQueries==static_cast<unsigned>(Test.Combat) && Test.FpsQueries==static_cast<unsigned>(moving));
            assert(Test.SubDefQueries==static_cast<unsigned>(subdef) && Test.EffectQueries==static_cast<unsigned>(effect));
            assert(Test.PositionQueries==static_cast<unsigned>(carrying) && Test.CarryQueries==static_cast<unsigned>(carrying) && Test.PrimaryQueries==static_cast<unsigned>(primary));
            assert(Test.Creates==static_cast<unsigned>(create) && Test.DriverQueries==static_cast<unsigned>(attach) && Test.Attaches==static_cast<unsigned>(attach));
            assert(indicator.PEmitter.Get()==(attach ? &CreatedEmitter : active ? &active_driver : 0));
            assert(indicator.PoweredUp==power);
            assert(indicator.Timer==times[i]-(moving && times[i]>0 ? 1 : 0));
        }
    }
    std::puts("007e0f20_TEST PASS");
    return 0;
}
