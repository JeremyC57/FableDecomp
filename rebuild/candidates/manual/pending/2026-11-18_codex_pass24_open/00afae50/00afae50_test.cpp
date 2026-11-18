#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
static void check(bool ok){if(!ok)abort();}
struct C3DVector {
    float X,Y,Z;
    C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
    C3DVector operator+(const C3DVector& b)const{return C3DVector(X+b.X,Y+b.Y,Z+b.Z);}
    C3DVector operator-(const C3DVector& b)const{return C3DVector(X-b.X,Y-b.Y,Z-b.Z);}
    C3DVector operator*(float k)const{return C3DVector(X*k,Y*k,Z*k);}
    float GetMagnitude()const{return (float)sqrt(X*X+Y*Y+Z*Z);}
};
struct C2DVector {float X,Y;};
struct C3DBoundingSphere {
    C3DVector points[4];float radii[4];int n;C3DBoundingSphere():n(0){}
    void AddToBounds(const C3DVector& v,float r){check(n<4);radii[n]=r;points[n++]=v;}
};
struct CGraphicInfoFrame{unsigned int GetHeight()const{return 3;}unsigned int GetWidth()const{return 4;}};
struct CGraphicDataBank {
    mutable int frames,lookups; CGraphicInfoFrame frame;
    unsigned long GetNoFramesInGraphic(unsigned long id)const{check(id==7);++frames;return 8;}
    const CGraphicInfoFrame* PeekGraphicInfoFrame(unsigned long id,unsigned long n)const{check(id==7&&n==0);++lookups;return &frame;}
};
struct Primitive {unsigned int clears;Primitive():clears(0){}void Clear(){++clears;}};
static float GFUnpackFloat(unsigned long packed,unsigned long bits,float minimum,float maximum,unsigned long places){
    check(places==2);
    if(bits==14){check(packed==0&&maximum==100);return 0.125f;}
    if(bits==10){check(minimum==0&&maximum==10);return (float)packed;}
    check(bits==11&&minimum==0&&maximum==20);return (float)packed;
}
const float DELTA_NOTIONAL_ZERO=0.125f;
namespace NParticleEngine {
static CGraphicDataBank bank;
const CGraphicDataBank& GetGraphicBank(){return bank;}
static int ticks; long GetTickRate(){++ticks;return 16;}
template<class K,class V> struct CParticleUpdateParameters{};
struct CParticle{
    bool active,at_time,removed;float life;unsigned long calls;mutable float delay;
    bool IsActive()const{return active;}
    bool IsActive(float t)const{check(t==0.5f);return at_time;}
    C3DVector GetInterpolatedPos(float t)const{check(t==0.5f);return C3DVector(3,0,0);}
    C3DVector GetRecordedPos(float d,float t)const{check(t==0.5f);delay=d;return C3DVector(3,4,0);}
    float GetInterpolatedLifetimer(float t)const{check(t==0.5f);return life;}
    long GetMaxLife()const{return 9;}
};
struct CParticleSystem{
    typedef std::vector<CParticle*>::iterator ParticleIterator;
    std::vector<CParticle*> particles;float time;bool local;int scales,transforms,clears;
    std::vector<CParticle*>& GetParticleList(){return particles;}
    float GetNormalisedTime()const{return time;}
    void ScaleParticleDimensions(C2DVector& s){++scales;s.X*=2;s.Y*=2;}
    void TransformPosIfLocal(C3DVector& p){++transforms;if(local)p.X+=8.0f;}
    void ClearParticle(ParticleIterator& it){++clears;(*it)->removed=true;}
};
struct CPSCRenderSprite{
    long SpriteBankIndex,TrailBankIndex,StartRenderSize,EndRenderSize,AnimationTimeSecs,TrailWidth,TrailLengthInteger;
    bool ForceAnimationTime;Primitive PrimHandle,TrailPrimHandle;
    bool AddToBoundingSphere(C3DBoundingSphere&,const float,CParticleSystem&,const CParticleUpdateParameters<unsigned long,unsigned long>&);
};
bool CPSCRenderSprite::AddToBoundingSphere(C3DBoundingSphere& bsphere, const float interpolate, CParticleSystem& system,
                                           const CParticleUpdateParameters<unsigned long, unsigned long>& params)
{
    if (!SpriteBankIndex && !TrailBankIndex)
        return false;
    float start_render_size = GFUnpackFloat(StartRenderSize, 11, 0.0f, 20.0f, 2);
    float end_render_size = GFUnpackFloat(EndRenderSize, 11, 0.0f, 20.0f, 2);
    float anim_time_secs = GFUnpackFloat(AnimationTimeSecs, 14, 0.1f, 100.0f, 2);
    float trail_width = GFUnpackFloat(TrailWidth, 10, 0.0f, 10.0f, 2);
    float trail_length_f = TrailLengthInteger * 0.001f;
    bool trail_delay_greater_than_max = trail_length_f > 2.0f;
    float trail_stretch = (trail_delay_greater_than_max ? 2.0f : trail_length_f) * 0.5f;
    const float tick_rate=(float)GetTickRate();
    const float anim_step=1.0f/(anim_time_secs*tick_rate);
    const float normalised_time = system.GetNormalisedTime();
    const CGraphicDataBank& bank = GetGraphicBank();
    if (SpriteBankIndex)
        bank.GetNoFramesInGraphic(SpriteBankIndex);
    float anim_percent = normalised_time;
    if (anim_percent < 0.0f) anim_percent = 0.0f;
    else {
        float limit = 1.0f - DELTA_NOTIONAL_ZERO;
        if (anim_percent > limit) anim_percent = limit;
    }
    float aspect_ratio = 0.0f;
    if (SpriteBankIndex) {
        const CGraphicInfoFrame* graphic_frame=bank.PeekGraphicInfoFrame(SpriteBankIndex,0);
        aspect_ratio=(float)graphic_frame->GetHeight()/graphic_frame->GetWidth();
    }
    bool valid = false;
    C3DVector pos;
    C3DVector trail_pos2;
    C3DVector trail_pos2_sub_pos;
    C2DVector size;
    for (CParticleSystem::ParticleIterator it_particle = system.GetParticleList().begin(); it_particle != system.GetParticleList().end(); ++it_particle) {
        const CParticle& particle = **it_particle;
        if (particle.IsActive() && particle.IsActive(interpolate)) {
            pos = particle.GetInterpolatedPos(interpolate);
            system.TransformPosIfLocal(pos);
            if (TrailBankIndex) {

                if (trail_delay_greater_than_max) {
                    trail_pos2 = particle.GetRecordedPos(2.0f, interpolate);
                    system.TransformPosIfLocal(trail_pos2);
                    trail_pos2 = pos + (trail_pos2 - pos) * trail_stretch;
                } else {
                    trail_pos2 = particle.GetRecordedPos(trail_length_f, interpolate);
                    system.TransformPosIfLocal(trail_pos2);
                }

                trail_pos2_sub_pos = trail_pos2 - pos;
                float length = trail_pos2_sub_pos.GetMagnitude();
                if (trail_width > length) length = trail_width;
                bsphere.AddToBounds(pos + trail_pos2_sub_pos * 0.5f, length * 0.5f);
                valid = true;
            }
            if (SpriteBankIndex) {
                if (ForceAnimationTime)
                    anim_percent = (float)fmod((double)(particle.GetInterpolatedLifetimer(interpolate) * anim_step), 1.0);

                size.X = (end_render_size - start_render_size) * anim_percent + start_render_size;
                size.Y = size.X * aspect_ratio;
                system.ScaleParticleDimensions(size);
                bsphere.AddToBounds(pos, (float)sqrt(size.Y * size.Y + size.X * size.X) * 0.5f);
                valid = true;
            }
        } else if (particle.GetInterpolatedLifetimer(interpolate) > particle.GetMaxLife())
            system.ClearParticle(it_particle);
    }
    if (!valid) {
        PrimHandle.Clear();
        TrailPrimHandle.Clear();
    }
    return valid;
}
}

int main(){
    using namespace NParticleEngine;
    for(int banks=0;banks<4;++banks)for(int particle_mode=0;particle_mode<4;++particle_mode)for(int scene=0;scene<12;++scene){
        CPSCRenderSprite sprite;C3DBoundingSphere sphere;CParticleSystem system;CParticleUpdateParameters<unsigned long,unsigned long> params;
        bool have_sprite=(banks&1)!=0,have_trail=(banks&2)!=0;
        sprite.SpriteBankIndex=have_sprite?7:0;sprite.TrailBankIndex=have_trail?8:0;
        sprite.StartRenderSize=2;sprite.EndRenderSize=6;sprite.AnimationTimeSecs=0;sprite.TrailWidth=(scene&1)?6:2;
        sprite.TrailLengthInteger=(scene&2)?4000:1000;sprite.ForceAnimationTime=(scene&4)!=0;
        const float times[]={-1.0f,0.5f,2.0f};system.time=times[scene%3];system.local=(scene&1)!=0;
        system.clears=system.scales=system.transforms=0;ticks=bank.frames=bank.lookups=0;
        CParticle p; p.active=particle_mode!=2;p.at_time=particle_mode==1;p.life=particle_mode==2?10.0f:1.0f;p.removed=false;p.delay=-1.0f;
        if(particle_mode)system.particles.push_back(&p);
        const bool expected=banks && particle_mode==1;
        check(sprite.AddToBoundingSphere(sphere,0.5f,system,params)==expected);
        check(ticks==(banks?1:0) && bank.frames==(have_sprite?1:0) && bank.lookups==bank.frames);
        check(p.removed==(bool)(banks && particle_mode==2) && system.clears==(int)p.removed);
        check(sprite.PrimHandle.clears==(unsigned)(banks&&!expected)&&sprite.TrailPrimHandle.clears==sprite.PrimHandle.clears);
        check(system.scales==(expected&&have_sprite) && system.transforms==(expected?1+(have_trail?1:0):0));
        check(sphere.n==(expected?(int)have_trail+(int)have_sprite:0));
        if(expected){
            const float x=system.local?11.0f:3.0f;
            if(have_trail){check(p.delay==(sprite.TrailLengthInteger>2000?2.0f:1.0f) && sphere.points[0].X==x && sphere.points[0].Y==2.0f && sphere.radii[0]==(sprite.TrailWidth>4?3.0f:2.0f));}
            if(have_sprite){int i=have_trail?1:0;float phase=sprite.ForceAnimationTime?0.5f:scene%3==0?0.0f:scene%3==1?0.5f:0.875f;
              check(sphere.points[i].X==x && sphere.points[i].Y==0 && sphere.points[i].Z==0 && sphere.radii[i]==1.25f*(2.0f+4.0f*phase));}
        }
    }
    puts("00afae50_TEST PASS");return 0;
}
