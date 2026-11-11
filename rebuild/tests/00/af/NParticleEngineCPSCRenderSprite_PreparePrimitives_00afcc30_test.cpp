// Full PreparePrimitives body; controlled engine and resource dependencies.
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <list>
class C3DVector { public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector operator+(const C3DVector&v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}C3DVector operator-(const C3DVector&v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}C3DVector operator*(float v)const{return C3DVector(X*v,Y*v,Z*v);}C3DVector Interpolate(const C3DVector&v,float f)const{return *this+(v-*this)*f;}};
class C2DVector { public:float X,Y;C2DVector(float x=0,float y=0):X(x),Y(y){}};
static void Equal(const C3DVector&a,const C3DVector&b){assert(a.X==b.X&&a.Y==b.Y&&a.Z==b.Z);}
class CRGBColour { public:unsigned char R,G,B,A;CRGBColour(unsigned char r=0,unsigned char g=0,unsigned char b=0,unsigned char a=0):R(r),G(g),B(b),A(a){}};
static CRGBColour GFGetInterpolatedColour(CRGBColour a,CRGBColour b,unsigned long f){assert(f<=255);return CRGBColour((unsigned char)((a.R*(256-f)+b.R*f)/256),(unsigned char)((a.G*(256-f)+b.G*f)/256),(unsigned char)((a.B*(256-f)+b.B*f)/256),(unsigned char)((a.A*(256-f)+b.A*f)/256));}
static long GFFloatToLongNear(float x){float n=(float)floor(x);float f=x-n;return (long)(n+(f>0.5f||(f==0.5f&&((long)n&1))?1.0f:0.0f));}
static long GFFloatToLongFloor(float x){return (long)floor(x);}
static float GFUnpackFloat(unsigned long p,unsigned long bits,float low,float high,unsigned long places){float scale=1;while(places--)scale*=10;return GFFloatToLongNear((p/((float)(1UL<<bits)-1)*(high-low)+low)*scale)/scale;}
enum EEnginePrimitiveBlendOp {BlendOpZero,BlendOpOne};enum EEnginePrimitiveBlendMode {BlendModeZero,BlendModeOne};enum EEngineInterpolationSet {InterpolationZero};
class CEnginePrimitiveHandle { public:int ID;CEnginePrimitiveHandle(int n=0):ID(n){}};
class CEnginePrimitive { public:int Kind;unsigned long Frame,Flags;C3DVector Pos,End;C2DVector Size;float Angle,Width;CRGBColour Colour;EEnginePrimitiveBlendMode Mode;EEnginePrimitiveBlendOp Op;CEnginePrimitive(int k):Kind(k),Frame(0),Flags(0),Angle(0),Width(0),Mode(BlendModeZero),Op(BlendOpZero){}};
struct CEnginePrimitiveParticleSpriteGroup:CEnginePrimitive {CEnginePrimitiveParticleSpriteGroup(long n,unsigned long f,EEnginePrimitiveBlendMode m,EEnginePrimitiveBlendOp o):CEnginePrimitive(1){Frame=n;Flags=f;Mode=m;Op=o;}};
struct CEnginePrimitiveParticleSpriteTrailGroup:CEnginePrimitive {CEnginePrimitiveParticleSpriteTrailGroup(long n,unsigned long f,EEnginePrimitiveBlendMode m,EEnginePrimitiveBlendOp o):CEnginePrimitive(2){Frame=n;Flags=f;Mode=m;Op=o;}};
struct CEnginePrimitiveParticleSprite:CEnginePrimitive {CEnginePrimitiveParticleSprite(int id,unsigned long frame,const C3DVector&p,const C2DVector&s,float a,const CRGBColour&c,EEngineInterpolationSet):CEnginePrimitive(3){assert(id==0);Frame=frame;Pos=p;Size=s;Angle=a;Colour=c;}};
struct CEnginePrimitiveParticleLine:CEnginePrimitive {CEnginePrimitiveParticleLine(int id,const C3DVector&p,const C3DVector&q,const CRGBColour&c,float w,EEngineInterpolationSet):CEnginePrimitive(4){assert(id==0);Pos=p;End=q;Colour=c;Width=w;}};
struct Event {int Parent,Child;bool Added;CEnginePrimitive Primitive;Event(int p,int c,bool a,const CEnginePrimitive&v):Parent(p),Child(c),Added(a),Primitive(v){}};
class CIEngine { public:std::vector<Event> Events;int Next;CIEngine():Next(100){}void SetChildPrimitive(CEnginePrimitiveHandle&h,long&index,CEnginePrimitiveHandle&out,const CEnginePrimitive&p,int n){assert(n==0);if(index<0)index=Next++;out.ID=(int)index;Events.push_back(Event(h.ID,(int)index,false,p));}void SetChildPrimitive(CEnginePrimitiveHandle&h,long index,const CEnginePrimitive&p,int n){assert(n==0);Events.push_back(Event(h.ID,(int)index,false,p));}long AddChildPrimitive(CEnginePrimitiveHandle&h,const CEnginePrimitive&p,int n){assert(n==0);int id=Next++;Events.push_back(Event(h.ID,id,true,p));return id;}};
class CEnginePrimitivePrepareInfo { public:CIEngine*Engine;};
class CGraphicInfoFrame { public:unsigned long GetHeight()const{return 4;}unsigned long GetWidth()const{return 8;}};
class CGraphicInfo { public:CGraphicInfoFrame Frame;const CGraphicInfoFrame*PeekInfoFrame(int i)const{assert(i==0);return &Frame;}};
class CGraphicDataBank { public:CGraphicInfo Graphic;long GetNoFramesInGraphic(long id)const{assert(id==7);return 4;}const CGraphicInfo*PeekGraphic(long id)const{assert(id==7);return &Graphic;}};
namespace NFractal {class CPerlinNoise { public:float Value;mutable int Calls;mutable C2DVector Last;CPerlinNoise():Value(0),Calls(0){}float GetGaussian2DNoise(const C2DVector&p)const{Last=p;++Calls;return Value;}};}
namespace NParticleEngine {
class CParticleEmitterManager { public:NFractal::CPerlinNoise Noise;const NFractal::CPerlinNoise&PeekPerlinNoise()const{return Noise;}};
static CGraphicDataBank Bank;static CParticleEmitterManager Manager;
static long GetTickRate(){return 2;}static float GetOneOverTickRate(){return 0.5f;}static const CGraphicDataBank&GetGraphicBank(){return Bank;}static CParticleEmitterManager&GetParticleEmitterManager(){return Manager;}
static float GFSmoothFadeValue(float x){assert(x>=0&&x<=1);return x;}
template<class A,class B>class CParticleUpdateParameters { public:};
class CParticle { public:bool Active,InterpolatedActive;float Life,Maximum,Angle,AngleChange;C3DVector Positions[4];std::vector<std::vector<long> >IDs;std::vector<int>Parents;int Stores;CParticle():Active(true),InterpolatedActive(true),Life(2),Maximum(4),Angle(0.25f),AngleChange(0.5f),Stores(0){Positions[0]=C3DVector(8,0,0);Positions[1]=C3DVector(4,0,0);Positions[2]=C3DVector(2,0,0);Positions[3]=C3DVector(0,0,0);}float GetInterpolatedLifetimer(float)const{return Life;}bool IsActive()const{return Active;}bool IsActive(float)const{return InterpolatedActive;}float GetMaxLife()const{return Maximum;}float GetFlickerTimeOffset()const{return 2;}float GetFlickerIndex()const{return 3;}float GetAngle()const{return Angle;}float GetAngleChange()const{return AngleChange;}C3DVector GetInterpolatedPos(float f)const{return Positions[1].Interpolate(Positions[0],f);}C3DVector GetRecordedPos(float delay,float f)const{float t=delay+1-f;long index=(long)ceil(t);if(index<1)index=1;else if(index>3)index=3;return Positions[index].Interpolate(Positions[index-1],(float)index-t);}bool GetPrimID(long list,long i,long&id)const{if((unsigned long)list<IDs.size()&&(unsigned long)i<IDs[list].size()){id=IDs[list][i];return id>=0;}return false;}void SetPrimitiveInfo(long list,long i,const CEnginePrimitiveHandle&h,long id){assert(list>=0&&i>=0);if(IDs.size()<=(unsigned long)list)IDs.resize(list+1);if(IDs[list].size()<=(unsigned long)i)IDs[list].resize(i+1,0);IDs[list][i]=id;if(Parents.size()<=(unsigned long)list)Parents.resize(list+1);Parents[list]=h.ID;++Stores;}};
class CParticleSystem { public:typedef std::list<CParticle*>::iterator ParticleIterator;std::list<CParticle*>Particles;float Fade,Alpha,Scale;bool Local;int Clears;CParticleSystem():Fade(1),Alpha(1),Scale(1),Local(false),Clears(0){}float GetFadeAlpha()const{return Fade;}float GetGlobalAlpha()const{return Alpha;}std::list<CParticle*>&GetParticleList(){return Particles;}void TransformPosIfLocal(C3DVector&p)const{if(Local)p=p+C3DVector(10,0,0);}void ScaleParticleDimensions(C2DVector&s)const{s.X*=Scale;s.Y*=Scale;}void ScaleParticleDimensions(float&s)const{s*=Scale;}void ClearParticle(ParticleIterator&it){assert(it!=Particles.end());++Clears;(*it)->IDs.clear();(*it)->Parents.clear();}};
class CPSCRenderSprite { public:long SpriteBankIndex,TrailBankIndex;CRGBColour StartColour,MidColour,EndColour;unsigned long BlendMode,TrailBlendMode,BlendOp,TrailBlendOp,Flags,FadeInEndInteger,FadeOutBeginInteger,TrailLengthInteger,FlickerMinAlphaInteger,FlickerMinSizeInteger,NoCrossedSprites,StartRenderSize,AlphaFadeMinimum,EndRenderSize,FlickerBias,AnimationTimeSecs,SizeFadeMinimum,TrailWidth,FlickerSpeed;bool UseStartColour,UseMidColour,UseEndColour,AlphaFadeEnable,SizeFadeEnable,FlickerEnable,ForceAnimationTime;CEnginePrimitiveHandle PrimHandle,TrailPrimHandle;long ChildPrimIndex,ChildTrailPrimIndex;int Clears;
CPSCRenderSprite():SpriteBankIndex(7),TrailBankIndex(0),StartColour(64,32,16,128),MidColour(64,32,16,128),EndColour(64,32,16,128),BlendMode(1),TrailBlendMode(0),BlendOp(0),TrailBlendOp(1),Flags(0),FadeInEndInteger(0),FadeOutBeginInteger(1000),TrailLengthInteger(2000),FlickerMinAlphaInteger(0),FlickerMinSizeInteger(0),NoCrossedSprites(2),StartRenderSize(2047),AlphaFadeMinimum(0),EndRenderSize(2047),FlickerBias(0),AnimationTimeSecs(16383),SizeFadeMinimum(0),TrailWidth(1023),FlickerSpeed(0),UseStartColour(true),UseMidColour(true),UseEndColour(true),AlphaFadeEnable(false),SizeFadeEnable(false),FlickerEnable(false),ForceAnimationTime(false),PrimHandle(11),TrailPrimHandle(12),ChildPrimIndex(11),ChildTrailPrimIndex(12),Clears(0){}
void ClearAllPrimitives(CIEngine*,CEnginePrimitiveHandle&h){assert(h.ID==1);++Clears;}void PreparePrimitives(const float,const CEnginePrimitivePrepareInfo&,CParticleSystem&,const CParticleUpdateParameters<unsigned long,unsigned long>&,CEnginePrimitiveHandle&);
};
}

namespace NParticleEngine {
void CPSCRenderSprite::PreparePrimitives(const float interpolate, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                         const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& system_group)
{
    if (!SpriteBankIndex && !TrailBankIndex) {
        ClearAllPrimitives(info.Engine, system_group);
        return;
    }
    const float alpha_fade = GFUnpackFloat(AlphaFadeMinimum, 7, 0.0f, 1.0f, 2);
    const float size_fade = GFUnpackFloat(SizeFadeMinimum, 7, 0.0f, 1.0f, 2);
    const float anim_time_secs = GFUnpackFloat(AnimationTimeSecs, 14, 0.1f, 100.0f, 2);
    const float start_render_size = GFUnpackFloat(StartRenderSize, 11, 0.0f, 20.0f, 2);
    const float end_render_size = GFUnpackFloat(EndRenderSize, 11, 0.0f, 20.0f, 2);
    const float flicker_speed = GFUnpackFloat(FlickerSpeed, 12, 0.0f, 30.0f, 2);
    const float this_flicker_bias = GFUnpackFloat(FlickerBias, 8, -1.0f, 1.0f, 2);
    const float trail_width = GFUnpackFloat(TrailWidth, 10, 0.0f, 10.0f, 2);
    const float tick_rate=(float)GetTickRate();
    const float one_on_tick_rate = GetOneOverTickRate();
    const CGraphicDataBank& bank = GetGraphicBank();
    const NFractal::CPerlinNoise& perlin_noise = GetParticleEmitterManager().PeekPerlinNoise();
    const float trail_length_f = TrailLengthInteger * 0.001f;
    const float trail_max_delay=2.0f;
    const float one_on_trail_max_delay=1.0f/trail_max_delay;
    const bool trail_delay_greater_than_max=trail_length_f>trail_max_delay;
    const float trail_delay=trail_delay_greater_than_max?trail_max_delay:trail_length_f;
    const float trail_stretch=trail_delay*one_on_trail_max_delay;
    const float fade_in_end = FadeInEndInteger * 0.001f;
    const float fade_out_begin = FadeOutBeginInteger * 0.001f;
    const float min_alpha = FlickerMinAlphaInteger * 0.001f;
    const float min_size = FlickerMinSizeInteger * 0.001f;
    const float flicker_scale = (float)fabs(this_flicker_bias) + 0.5f;
    const float flicker_bias = this_flicker_bias + 0.5f;
    const float flicker_over_tick = one_on_tick_rate * flicker_speed;
    const float one_on_amin_time_secs = 1.0f / anim_time_secs;
    const float one_on_num_crossed_sprites = 1.0f / NoCrossedSprites;
    const float one_over_fade_in_end = 1.0f / fade_in_end;
    const float one_over_one_sub_fade_out_begin = 1.0f / (1.0f - fade_out_begin);
    const float one_sub_min_alpha = 1.0f - min_alpha;
    const float one_sub_min_size = 1.0f - min_size;
    const float fade_alpha = system.GetFadeAlpha();
    float alpha_fade_min, one_sub_alpha_fade_min;
    if (AlphaFadeEnable) {
        one_sub_alpha_fade_min = 1.0f - alpha_fade;
        alpha_fade_min = alpha_fade;
    } else {
        one_sub_alpha_fade_min = 0.0f;
        alpha_fade_min = 1.0f;
    }
    float size_fade_min, one_sub_size_fade_min;
    if (SizeFadeEnable) {
        one_sub_size_fade_min = 1.0f - size_fade;
        size_fade_min = size_fade;
    } else {
        one_sub_size_fade_min = 0.0f;
        size_fade_min = 1.0f;
    }
    const float crossed_sprite_full_angle=((Flags&2)||(Flags&4))?1.0f:0.5f;
    long particle_prim_handle_index=0;
    long particle_trail_prim_handle_index = 0;
    long no_frames_in_graphic = 0;
    float no_frames_in_graphic_f = 0.0f;
    const CGraphicInfoFrame* graphic_frame=0;
    float base_size_y = 0.0f;
    EEnginePrimitiveBlendOp blend_op = (EEnginePrimitiveBlendOp)BlendOp;
    EEnginePrimitiveBlendMode blend_mode = (EEnginePrimitiveBlendMode)BlendMode;
    EEnginePrimitiveBlendOp trail_blend_op = (EEnginePrimitiveBlendOp)TrailBlendOp;
    EEnginePrimitiveBlendMode trail_blend_mode = (EEnginePrimitiveBlendMode)TrailBlendMode;
    if (SpriteBankIndex) {
        no_frames_in_graphic = bank.GetNoFramesInGraphic(SpriteBankIndex);
        no_frames_in_graphic_f = (float)no_frames_in_graphic;
        graphic_frame = bank.PeekGraphic(SpriteBankIndex)->PeekInfoFrame(0);
        base_size_y = (float)graphic_frame->GetHeight() / graphic_frame->GetWidth();
        info.Engine->SetChildPrimitive(system_group, ChildPrimIndex, PrimHandle,
            CEnginePrimitiveParticleSpriteGroup(SpriteBankIndex, Flags, (EEnginePrimitiveBlendMode)blend_mode,
                                                (EEnginePrimitiveBlendOp)blend_op), 0);
        particle_trail_prim_handle_index = 1;
    }
    const long no_frames_in_graphic_sub_one = no_frames_in_graphic - 1;
    if (TrailBankIndex)
        info.Engine->SetChildPrimitive(system_group, ChildTrailPrimIndex, TrailPrimHandle,
            CEnginePrimitiveParticleSpriteTrailGroup(TrailBankIndex, Flags, (EEnginePrimitiveBlendMode)trail_blend_mode,
                                                     (EEnginePrimitiveBlendOp)trail_blend_op), 0);
    CRGBColour colours[3];
    colours[0] = UseStartColour ? StartColour : UseMidColour ? MidColour : UseEndColour ? EndColour : CRGBColour(255, 255, 255, 255);
    colours[2] = UseEndColour ? EndColour : UseMidColour ? MidColour : UseStartColour ? StartColour : CRGBColour(255, 255, 255, 255);
    colours[1] = UseMidColour ? MidColour : GFGetInterpolatedColour(colours[0], colours[2], 128);
    colours[0].A = (unsigned char)(colours[0].A * fade_alpha);
    colours[1].A = (unsigned char)(colours[1].A * fade_alpha);
    colours[2].A = (unsigned char)(colours[2].A * fade_alpha);
    CRGBColour colour;
    C3DVector pos,trail_pos_2;
    C2DVector noise_lookup;

    long child_prim_id;
    const unsigned long colour_intervals=512;
    for (CParticleSystem::ParticleIterator it_particle = system.GetParticleList().begin(); it_particle != system.GetParticleList().end(); ++it_particle) {
        CParticle& particle = **it_particle;
        const float life_timer = particle.GetInterpolatedLifetimer(interpolate);
        if (particle.IsActive() && particle.IsActive(interpolate) && particle.GetMaxLife()) {
            const float normalised_time = life_timer/particle.GetMaxLife()<0.0f?0.0f:life_timer/particle.GetMaxLife()>1.0f?1.0f:life_timer/particle.GetMaxLife();
            unsigned long colour_time = GFFloatToLongNear(normalised_time * (colour_intervals-1));
            unsigned long colour_index = colour_time >> 8;
            unsigned long colour_interp=colour_time&0xff;
            colour = GFGetInterpolatedColour(colours[colour_index], colours[colour_index + 1], colour_interp);
            float fade_value = system.GetGlobalAlpha();
            if (normalised_time < fade_in_end)
                fade_value *= GFSmoothFadeValue(normalised_time * one_over_fade_in_end);
            if (normalised_time > fade_out_begin)
                fade_value *= GFSmoothFadeValue((1.0f - normalised_time) * one_over_one_sub_fade_out_begin);
            float colour_fade = fade_value * one_sub_alpha_fade_min + alpha_fade_min;
            float scale = fade_value * one_sub_size_fade_min + size_fade_min;
            if (FlickerEnable) {
                noise_lookup.X = life_timer * flicker_over_tick + particle.GetFlickerTimeOffset();
                noise_lookup.Y = particle.GetFlickerIndex();
                float val=perlin_noise.GetGaussian2DNoise(noise_lookup);
                val=val*flicker_scale+flicker_bias;
                if(val<0.0f)val=0.0f;else if(val>1.0f)val=1.0f;
                colour_fade *= one_sub_min_alpha * val + min_alpha;
                scale *= one_sub_min_size * val + min_size;
            }
            colour.A = (unsigned char)GFFloatToLongNear(colour.A * colour_fade);
            if (colour.A) {
                pos = particle.GetInterpolatedPos(interpolate);
                system.TransformPosIfLocal(pos);
                if (SpriteBankIndex) {
                    float anim_percent = normalised_time;
                    // NativeAFD7ED multiplies the two reciprocal rates before the lifetime.
                    if (ForceAnimationTime)
                        anim_percent = (float)fmod((double)((one_on_amin_time_secs * one_on_tick_rate) * life_timer), 1.0);
                    const float sprite_angle = particle.GetAngleChange() * interpolate + particle.GetAngle();
                    const long frame = GFFloatToLongFloor(anim_percent * no_frames_in_graphic_f) < no_frames_in_graphic_sub_one ? GFFloatToLongFloor(anim_percent * no_frames_in_graphic_f) : no_frames_in_graphic_sub_one;
                    C2DVector size;
                    size.X = ((end_render_size - start_render_size) * anim_percent + start_render_size) * scale;
                    size.Y = size.X * base_size_y;
                    system.ScaleParticleDimensions(size);
                    if (Flags & 0x40) {
                        for (long c_sprite = 0; c_sprite < NoCrossedSprites; ++c_sprite) {
                            const float angle = (c_sprite + 0.5f) * crossed_sprite_full_angle * one_on_num_crossed_sprites + sprite_angle;

                            if (particle.GetPrimID(particle_prim_handle_index, c_sprite, child_prim_id))
                                info.Engine->SetChildPrimitive(PrimHandle, child_prim_id,
                                    CEnginePrimitiveParticleSprite(0, frame, pos, size, angle, colour, (EEngineInterpolationSet)0), 0);
                            else {
                                // NativeAFDA2C uses the existing result slot for both lookup and creation.
                                child_prim_id = info.Engine->AddChildPrimitive(PrimHandle,
                                    CEnginePrimitiveParticleSprite(0, frame, pos, size, angle, colour, (EEngineInterpolationSet)0), 0);
                                particle.SetPrimitiveInfo(particle_prim_handle_index, c_sprite, PrimHandle, child_prim_id);
                            }
                        }
                    } else {

                        if (particle.GetPrimID(particle_prim_handle_index, 0, child_prim_id))
                            info.Engine->SetChildPrimitive(PrimHandle, child_prim_id,
                                CEnginePrimitiveParticleSprite(0, frame, pos, size, sprite_angle, colour, (EEngineInterpolationSet)0), 0);
                        else {
                            child_prim_id = info.Engine->AddChildPrimitive(PrimHandle,
                                CEnginePrimitiveParticleSprite(0, frame, pos, size, sprite_angle, colour, (EEngineInterpolationSet)0), 0);
                            particle.SetPrimitiveInfo(particle_prim_handle_index, 0, PrimHandle, child_prim_id);
                        }
                    }
                }
                if (TrailBankIndex) {
                    trail_pos_2 = particle.GetRecordedPos(trail_delay, interpolate);
                    if (trail_delay_greater_than_max)
                        trail_pos_2 = pos + (trail_pos_2 - pos) * trail_stretch;
                    system.TransformPosIfLocal(trail_pos_2);
                    float width = scale * trail_width;
                    system.ScaleParticleDimensions(width);

                    if (particle.GetPrimID(particle_trail_prim_handle_index, 0, child_prim_id))
                        info.Engine->SetChildPrimitive(TrailPrimHandle, child_prim_id,
                            CEnginePrimitiveParticleLine(0, pos, trail_pos_2, colour, width, (EEngineInterpolationSet)0), 0);
                    else {
                        child_prim_id = info.Engine->AddChildPrimitive(TrailPrimHandle,
                            CEnginePrimitiveParticleLine(0, pos, trail_pos_2, colour, width, (EEngineInterpolationSet)0), 0);
                        particle.SetPrimitiveInfo(particle_trail_prim_handle_index, 0, TrailPrimHandle, child_prim_id);
                    }
                }
            }
        } else if (life_timer > particle.GetMaxLife())
            system.ClearParticle(it_particle);
    }
}

}
using namespace NParticleEngine;
struct Fixture {CPSCRenderSprite Renderer;CIEngine Engine;CParticleSystem System;CParticle Particle;CParticleUpdateParameters<unsigned long,unsigned long>Params;CEnginePrimitiveHandle Group;Fixture():Group(1){Manager=CParticleEmitterManager();System.Particles.push_back(&Particle);}void Run(float f=0.5f){CEnginePrimitivePrepareInfo info;info.Engine=&Engine;Renderer.PreparePrimitives(f,info,System,Params,Group);}int Count(int kind)const{int n=0;for(unsigned int i=0;i<Engine.Events.size();++i)if(Engine.Events[i].Primitive.Kind==kind)++n;return n;}const Event&Find(int kind,int index=0)const{for(unsigned int i=0;i<Engine.Events.size();++i)if(Engine.Events[i].Primitive.Kind==kind){if(index--==0)return Engine.Events[i];}assert(false);return Engine.Events[0];}};
static int Cases;
static void Basic(){
 {Fixture f;f.Renderer.SpriteBankIndex=0;f.Run();assert(f.Renderer.Clears==1&&f.Engine.Events.empty()&&f.Particle.Stores==0);++Cases;}
 for(int mode=0;mode<5;++mode){Fixture f;if(mode==1)f.Renderer.SpriteBankIndex=0;if(mode==1||mode==2||mode==4)f.Renderer.TrailBankIndex=9;if(mode>=3)f.Renderer.Flags=0x40;f.Run();int sprites=mode==1?0:mode>=3?2:1;int lines=(mode==1||mode==2||mode==4)?1:0;assert(f.Count(1)==(sprites?1:0)&&f.Count(2)==lines&&f.Count(3)==sprites&&f.Count(4)==lines);assert(f.Particle.Stores==sprites+lines);if(sprites){const Event&e=f.Find(3);Equal(e.Primitive.Pos,C3DVector(6,0,0));assert(e.Primitive.Size.X==20&&e.Primitive.Size.Y==10&&e.Primitive.Frame==2&&e.Primitive.Colour.A==128&&e.Added);assert(e.Primitive.Angle==(mode>=3?0.625f:0.5f));assert(f.Particle.Parents[0]==11);assert(f.Find(1).Primitive.Frame==7&&f.Find(1).Primitive.Mode==BlendModeOne);}if(lines){const Event&e=f.Find(4);assert(e.Parent==12&&e.Added&&e.Primitive.Width==10);Equal(e.Primitive.End,C3DVector(1,0,0));assert(f.Particle.Parents[mode==1?0:1]==12);assert(f.Find(2).Primitive.Frame==9&&f.Find(2).Primitive.Op==BlendOpOne);}int stores=f.Particle.Stores;f.Engine.Events.clear();f.Run();assert(f.Particle.Stores==stores);for(unsigned int i=0;i<f.Engine.Events.size();++i)assert(!f.Engine.Events[i].Added);++Cases;}
 for(int flag=2;flag<=4;flag+=2){Fixture f;f.Renderer.Flags=0x40|flag;f.Run();assert(f.Find(3,0).Primitive.Angle==0.75f&&f.Find(3,1).Primitive.Angle==1.25f);++Cases;}
 {Fixture f;f.Renderer.Flags=0x40;f.Renderer.NoCrossedSprites=0;f.Run();assert(f.Count(3)==0&&f.Particle.Stores==0);++Cases;}
}
static void Lifetime(){
 for(int k=0;k<5;++k){Fixture f;if(k==0)f.Particle.Active=false;if(k==1)f.Particle.InterpolatedActive=false;if(k==2)f.Particle.Maximum=0;if(k==3){f.Particle.Active=false;f.Particle.Life=8;}if(k==4){f.Particle.Active=false;f.Particle.Life=4;}f.Run();assert(f.Count(3)==0&&f.Particle.Stores==0&&f.System.Clears==((k==2||k==3)?1:0));++Cases;}
 {Fixture f;f.Particle.Life=-2;f.Run();assert(f.Find(3).Primitive.Frame==0);++Cases;}
 {Fixture f;f.Particle.Life=8;f.Run();assert(f.Find(3).Primitive.Frame==3&&f.System.Clears==0);++Cases;}
 {Fixture f;f.System.Fade=0;f.Run();assert(f.Count(3)==0&&f.Particle.Stores==0);++Cases;}
 {Fixture f;f.System.Fade=0.5f;f.Run();assert(f.Find(3).Primitive.Colour.A==64);++Cases;}
 {Fixture f;f.Renderer.ForceAnimationTime=true;f.Particle.Life=100;f.Run();assert(f.Find(3).Primitive.Frame==2);++Cases;}
}
static void Modulation(){
 for(int mode=0;mode<4;++mode){Fixture f;f.Renderer.AlphaFadeEnable=(mode&1)!=0;f.Renderer.SizeFadeEnable=(mode&2)!=0;f.System.Alpha=0.5f;f.Run();assert(f.Find(3).Primitive.Colour.A==((mode&1)?64:128));assert(f.Find(3).Primitive.Size.X==((mode&2)?10:20));++Cases;}
 for(int direction=0;direction<2;++direction){Fixture f;f.Renderer.AlphaFadeEnable=true;f.Renderer.SizeFadeEnable=true;if(direction==0)f.Renderer.FadeInEndInteger=1000;else f.Renderer.FadeOutBeginInteger=0;f.Run();assert(f.Find(3).Primitive.Colour.A==64&&f.Find(3).Primitive.Size.X==10);++Cases;}
 for(int mode=0;mode<4;++mode){Fixture f;f.Renderer.FlickerEnable=true;Manager.Noise.Value=mode==0?-2.0f:mode==1?2.0f:0.5f;if(mode==3){f.Renderer.FlickerMinAlphaInteger=1000;f.Renderer.FlickerMinSizeInteger=1000;}f.Run();assert(Manager.Noise.Calls==1&&Manager.Noise.Last.X==2&&Manager.Noise.Last.Y==3);if(mode==0)assert(f.Count(3)==0);else {float factor=mode==2?0.25f:1.0f;assert(f.Find(3).Primitive.Colour.A==(unsigned char)(128*factor)&&f.Find(3).Primitive.Size.X==20*factor);}++Cases;}
 for(int mask=0;mask<8;++mask){Fixture f;f.Renderer.UseStartColour=(mask&1)!=0;f.Renderer.UseMidColour=(mask&2)!=0;f.Renderer.UseEndColour=(mask&4)!=0;f.Run();const CRGBColour&c=f.Find(3).Primitive.Colour;assert(c.A==(mask?128:255)&&c.R==(mask?64:255)&&c.G==(mask?32:255)&&c.B==(mask?16:255));++Cases;}
}
static void GeometryAndIteration(){
 {Fixture f;f.Renderer.TrailBankIndex=9;f.System.Local=true;f.System.Scale=2;f.Run();Equal(f.Find(3).Primitive.Pos,C3DVector(16,0,0));assert(f.Find(3).Primitive.Size.X==40&&f.Find(3).Primitive.Size.Y==20);Equal(f.Find(4).Primitive.Pos,C3DVector(16,0,0));Equal(f.Find(4).Primitive.End,C3DVector(11,0,0));assert(f.Find(4).Primitive.Width==20);++Cases;}
 {Fixture f;f.Renderer.TrailBankIndex=9;f.Renderer.TrailLengthInteger=4000;f.Run();Equal(f.Find(4).Primitive.End,C3DVector(1,0,0));++Cases;}
 {Fixture f;f.Renderer.TrailBankIndex=9;f.Renderer.TrailLengthInteger=0;f.Run();Equal(f.Find(4).Primitive.End,C3DVector(6,0,0));++Cases;}
 {Fixture f;f.Particle.IDs.resize(1);f.Particle.IDs[0].push_back(-1);f.Run();assert(f.Find(3).Added&&f.Particle.Stores==1);++Cases;}
 {Fixture f;CParticle another;f.Particle.Active=false;f.Particle.Life=8;f.System.Particles.push_back(&another);f.Run();assert(f.System.Clears==1&&f.Count(3)==1&&another.Stores==1&&f.Particle.Stores==0);++Cases;}
 {Fixture f;f.System.Particles.clear();f.Run();assert(f.Count(1)==1&&f.Count(3)==0);++Cases;}
}
int main(){Basic();Lifetime();Modulation();GeometryAndIteration();assert(Cases==43);puts("00afcc30_TEST PASS");return 0;}