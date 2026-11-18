#include <stdio.h>
#include <map>
#include <string>
#include <vector>

typedef std::string CCharString;
struct C3DVector { float X,Y,Z; };
struct CThing;
template<class T> struct CIntelligentPointer {
    T* Ptr;
    mutable unsigned GetCalls;
    CIntelligentPointer(T* p=0):Ptr(p),GetCalls(0) {}
    T* Get() const { ++GetCalls;return Ptr; }
    T* operator->() const { return Ptr; }
};
#define IS_THING_ALIVE(pointer) ((pointer).Get() && !(pointer).Get()->Dead)
struct CTCSoundPlayer {
    unsigned Calls;
    CTCSoundPlayer():Calls(0) {}
    long PlayAttachedCriteria(const CCharString& name,int a,int b,int c,int d)
    { return name.empty()||a||b||c||d!=-1?-1:(long)++Calls; }
};
struct CTCDParticleEmitter {
    static CThing* Create(long,const C3DVector&,bool);
    CThing* Host;
    bool Valid;
    CTCDParticleEmitter():Host(0),Valid(false) {}
    void AttachToThing(CThing& host,int layer,const CCharString& socket,int index,float t)
    { Host=&host;Valid=layer==0x28&&socket=="tip"&&!index&&t==0; }
};
struct CThing {
    bool Sound,Projectile,Dead;
    C3DVector Position;
    CTCSoundPlayer Audio;
    CTCDParticleEmitter Emitter;
    CThing():Sound(false),Projectile(false),Dead(false) { Position.X=Position.Y=Position.Z=0; }
    bool HasTC(int id) const { return id==0x10?Sound:id==0x77?Projectile:id==0x4b; }
    bool GetTC(CTCSoundPlayer** out) { *out=&Audio;return Sound; }
    bool GetTC(CTCDParticleEmitter** out) { *out=&Emitter;return true; }
    const C3DVector& GetPos() const { return Position; }
};
CThing Effects[4];
CThing* CTCDParticleEmitter::Create(long id,const C3DVector& pos,bool flag)
{
    if(id<1||id>3||flag) return 0;
    Effects[id].Dead=false;Effects[id].Position=pos;return &Effects[id];
}
enum EObjectAugmentationType { OBJECT_AUGMENTATION_TYPE_FLAME=4,OBJECT_AUGMENTATION_TYPE_LIGHTNING=8 };
enum { TCI_SHOT_DEFINITION=0x77 };
struct CObjectAugmentationSoundSet {
    bool Played;
    long Handle;
    CCharString Sound;
    CObjectAugmentationSoundSet():Played(false),Handle(0),Sound("augment") {}
};
struct ParticleAndEndPoints {
    long PeriodicFrames,ParticleEffectID,ParticleEffectIDToBlendTo;
    CCharString StartPoint,EndPoint;
    ParticleAndEndPoints():PeriodicFrames(7),ParticleEffectID(11),ParticleEffectIDToBlendTo(4),StartPoint("start"),EndPoint("end") {}
};
struct AttackParticleAndEndPoints { CIntelligentPointer<CThing> ParticleEffect; };
struct AugmentDef {
    long FireAndLightningProjectileTrailEffect,FireProjectileTrailEffect,LightningProjectileTrailEffect;
    CCharString WeaponTrailAttachmentPoint;
    AugmentDef():FireAndLightningProjectileTrailEffect(3),FireProjectileTrailEffect(1),
         LightningProjectileTrailEffect(2),WeaponTrailAttachmentPoint("tip") {}
};
struct CTCObjectAugmentations {
    std::map<EObjectAugmentationType,CObjectAugmentationSoundSet> InitSounds,LoopingSounds;
    CThing* ParentThing;
    CIntelligentPointer<CThing> ProjectileTrailEffect;
    AugmentDef Definition;
    AugmentDef* PDef;
    bool InStrikeAction;
    long CurrentRunningAugEffectIndex,CurPeriod;
    unsigned long Augments,Starts;
    std::vector<CIntelligentPointer<CThing> > CurrentRunningAugEffect;
    std::vector<std::vector<ParticleAndEndPoints> > ParticleEffects;
    std::vector<std::vector<AttackParticleAndEndPoints> > AttackParticleEffects;
    explicit CTCObjectAugmentations(CThing& parent):ParentThing(&parent),PDef(&Definition),InStrikeAction(false),
        CurrentRunningAugEffectIndex(1),CurPeriod(0),Augments(12),Starts(0) {}
    bool HasCombinationOfAugmentations(unsigned mask) const { return (Augments&mask)==mask; }
    bool HasAugmentationOfType(unsigned mask) const { return (Augments&mask)!=0; }
    void AttemptToStartParticle(CIntelligentPointer<CThing>&,long type,long frames,const CCharString& start,const CCharString& end)
    { if(type==11&&frames==4&&start=="start"&&end=="end") ++Starts; }
    CThing& GetParentThing() { return *ParentThing; }
    void FrameUpdate();
};

void CTCObjectAugmentations::FrameUpdate()
{
    typedef std::map<EObjectAugmentationType, CObjectAugmentationSoundSet> SoundMap;
    for (SoundMap::iterator it_init_sounds = InitSounds.begin(); it_init_sounds != InitSounds.end(); ++it_init_sounds) {
        if (!it_init_sounds->second.Played) {
            CTCSoundPlayer* sound_player;
            if (GetParentThing().GetTC(&sound_player)) {
                it_init_sounds->second.Handle = sound_player->PlayAttachedCriteria(it_init_sounds->second.Sound, false, false, false, -1);
                it_init_sounds->second.Played = true;
            }
        }
    }
    for (SoundMap::iterator it_looping_sounds = LoopingSounds.begin(); it_looping_sounds != LoopingSounds.end(); ++it_looping_sounds) {
        if (!it_looping_sounds->second.Played) {
            CTCSoundPlayer* sound_player;
            if (GetParentThing().GetTC(&sound_player)) {
                it_looping_sounds->second.Handle = sound_player->PlayAttachedCriteria(it_looping_sounds->second.Sound, false, false, false, -1);
                it_looping_sounds->second.Played = true;
            }
        }
    }

    if (GetParentThing().HasTC(TCI_SHOT_DEFINITION) && !IS_THING_ALIVE(ProjectileTrailEffect)) {
        long effect = 0;
        if (HasCombinationOfAugmentations(static_cast<EObjectAugmentationType>(OBJECT_AUGMENTATION_TYPE_FLAME | OBJECT_AUGMENTATION_TYPE_LIGHTNING)))
            effect = PDef->FireAndLightningProjectileTrailEffect;
        else if (HasAugmentationOfType(OBJECT_AUGMENTATION_TYPE_FLAME))
            effect = PDef->FireProjectileTrailEffect;
        else if (HasAugmentationOfType(OBJECT_AUGMENTATION_TYPE_LIGHTNING))
            effect = PDef->LightningProjectileTrailEffect;
        if (effect) {
            ProjectileTrailEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect, GetParentThing().GetPos(), false));
            if (IS_THING_ALIVE(ProjectileTrailEffect)) {
                CTCDParticleEmitter* particle_driver;
                ProjectileTrailEffect->GetTC(&particle_driver);
                particle_driver->AttachToThing(GetParentThing(), 0x28, PDef->WeaponTrailAttachmentPoint, 0, 0.0f);
            }
        }
    }

    if (!InStrikeAction) {
        bool all_dead = true;
        for (long i = 0; i < static_cast<long>(CurrentRunningAugEffect.size()); ++i) {
            if (IS_THING_ALIVE(CurrentRunningAugEffect[i]))
                all_dead = false;
        }
        if (all_dead) {
            if (CurPeriod > 0)
                --CurPeriod;
            else {
                long num_particles = static_cast<long>(ParticleEffects.size());
                if (num_particles > 0) {
                    CurrentRunningAugEffectIndex = (CurrentRunningAugEffectIndex + 1) % num_particles;
                    CurPeriod = ParticleEffects[CurrentRunningAugEffectIndex][0].PeriodicFrames;
                    CurrentRunningAugEffect.clear();
                    long num = static_cast<long>(ParticleEffects[CurrentRunningAugEffectIndex].size());
                    CurrentRunningAugEffect.resize(num);
                    for (long i = 0; i < num; ++i) {
                        ParticleAndEndPoints& particle = ParticleEffects[CurrentRunningAugEffectIndex][i];
                        AttemptToStartParticle(CurrentRunningAugEffect[i], particle.ParticleEffectID, particle.ParticleEffectIDToBlendTo,
                                               particle.StartPoint, particle.EndPoint);
                    }
                }
            }
        }
    }

    std::vector<std::vector<AttackParticleAndEndPoints> >::iterator bit = AttackParticleEffects.begin();
    const std::vector<std::vector<AttackParticleAndEndPoints> >::iterator bendit = AttackParticleEffects.end();
    for (; bit != bendit; ++bit) {
        std::vector<AttackParticleAndEndPoints>& apeps = *bit;
        for (long i = 0; i < static_cast<long>(apeps.size()); ++i) {
            AttackParticleAndEndPoints& apep = apeps[i];
            if (apep.ParticleEffect.Get())
                apep.ParticleEffect.Get();
        }
    }
}

int main()
{
    CThing thing,attack;
    thing.Projectile=true;
    CTCObjectAugmentations a(thing);
    a.InitSounds[OBJECT_AUGMENTATION_TYPE_FLAME];a.LoopingSounds[OBJECT_AUGMENTATION_TYPE_LIGHTNING];
    a.ParticleEffects.resize(2);a.ParticleEffects[0].resize(1);a.ParticleEffects[1].resize(1);
    a.AttackParticleEffects.resize(1);a.AttackParticleEffects[0].resize(1);
    a.AttackParticleEffects[0][0].ParticleEffect.Ptr=&attack;
    a.FrameUpdate();
    if(a.Starts!=1||a.CurPeriod!=7||a.CurrentRunningAugEffectIndex!=0||thing.Audio.Calls||
        a.ProjectileTrailEffect.Ptr!=&Effects[3]||Effects[3].Emitter.Host!=&thing||!Effects[3].Emitter.Valid||
        a.AttackParticleEffects[0][0].ParticleEffect.GetCalls!=2)
        return 1;
    thing.Sound=true;
    a.FrameUpdate();
    if(thing.Audio.Calls!=2||!a.InitSounds[OBJECT_AUGMENTATION_TYPE_FLAME].Played||!a.LoopingSounds[OBJECT_AUGMENTATION_TYPE_LIGHTNING].Played||a.CurPeriod!=6)
        return 2;
    a.ProjectileTrailEffect.Ptr->Dead=true;a.Augments=4;a.InStrikeAction=true;
    a.FrameUpdate();
    if(thing.Audio.Calls!=2||a.CurPeriod!=6||a.ProjectileTrailEffect.Ptr!=&Effects[1])
        return 3;
    a.ProjectileTrailEffect.Ptr->Dead=true;a.Augments=8;a.InStrikeAction=false;
    a.CurrentRunningAugEffect[0].Ptr=&attack;
    a.FrameUpdate();
    if(a.CurPeriod!=6||a.ProjectileTrailEffect.Ptr!=&Effects[2]||a.Starts!=1)
        return 4;
    puts("00766dd0_TEST PASS");
    return 0;
}
