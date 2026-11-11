// Full selected serializer body; deterministic persistence dependencies.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <cassert>
static int Splines=0;
class CCharString{ public:std::string Text;CCharString(){}CCharString(const char* p):Text(p){}const char* GetAsCharArray()const{return Text.c_str();}long GetLength()const{return (long)Text.size();}};
static CCharString operator+(const CCharString& a,const CCharString& b){CCharString c;c.Text=a.Text+b.Text;return c;}
static CCharString GFIntToCharString(long n){char p[32];std::sprintf(p,"%ld",n);return CCharString(p);}
class C3DVector{ public:float X,Y,Z;C3DVector():X(0),Y(0),Z(0){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
template<class T>class CArray:public std::vector<T>{};
class CCardinalSpline{
 CArray<C3DVector> ControlPoints;float Tension;bool EndPointsConnected;
public:
 CCardinalSpline():Tension(0.5f),EndPointsConnected(false){++Splines;}~CCardinalSpline(){--Splines;}
 float GetTension()const{return Tension;}void SetTension(float t){Tension=t;}
 long GetControlPointCount()const{return (long)ControlPoints.size();}
 CArray<C3DVector> GetControlPoints()const{return ControlPoints;}
 void SetControlPoints(const CArray<C3DVector>& p){ControlPoints=p;}
};
template<class T>class CCountedPointer{T* Data;CCountedPointer(const CCountedPointer&);CCountedPointer& operator=(const CCountedPointer&);public:CCountedPointer():Data(0){}~CCountedPointer(){delete Data;}bool IsNull()const{return Data==0;}T* operator->()const{return Data;}void Reset(T* p){delete Data;Data=p;}};
class CPersistContext { public:
 bool Saving;std::map<std::string,float> Floats;std::map<std::string,long> Longs;std::map<std::string,unsigned long> ULongs;std::map<std::string,bool> Bools;std::map<std::string,C3DVector> Vectors;std::map<std::string,CCharString> Strings;std::vector<std::string> Names;
 CPersistContext(bool s):Saving(s){}bool IsSaving()const{return Saving;}
 void Transfer(const char* n,float& v,const float& d){Names.push_back(n);if(Saving)Floats[n]=v;else v=Floats.count(n)?Floats[n]:d;}
 void Transfer(const char* n,long& v,const long& d){Names.push_back(n);if(Saving)Longs[n]=v;else v=Longs.count(n)?Longs[n]:d;}
 void Transfer(const char* n,unsigned long& v,const unsigned long& d){Names.push_back(n);if(Saving)ULongs[n]=v;else v=ULongs.count(n)?ULongs[n]:d;}
 void Transfer(const char* n,bool& v,const bool& d){Names.push_back(n);if(Saving)Bools[n]=v;else v=Bools.count(n)?Bools[n]:d;}
 void Transfer(const char* n,C3DVector& v,const C3DVector& d){Names.push_back(n);if(Saving)Vectors[n]=v;else v=Vectors.count(n)?Vectors[n]:d;}
 void Transfer(const char* n,CCharString& v){Names.push_back(n);if(Saving)Strings[n]=v;else v=Strings.count(n)?Strings[n]:CCharString();}
 void Transfer(const char* n,float& v){Transfer(n,v,0.0f);}void Transfer(const char* n,long& v){Transfer(n,v,0L);}
};
static unsigned long CRC(unsigned long seed,const char* p,long n){unsigned int x=(unsigned int)seed;for(long i=0;i<n;++i){x^=(unsigned char)p[i];for(int b=0;b<8;++b)x=(x>>1)^((x&1)?0xedb88320U:0U);}return x;}
static unsigned long MAKE_UPDATE_PARAM_FROM_BASE(unsigned long b,const CCharString& s){return CRC(b,s.GetAsCharArray(),s.GetLength());}
static unsigned long MAKE_VECTOR_PARAM_FROM_BASE(unsigned long b,const CCharString& s){return CRC(b,s.GetAsCharArray(),s.GetLength());}
// Exact body of the pre-existing portable integer-store model (nearest-even).
namespace fable_portable {
inline long X87FistpNear(double v){
 if(!(v>=-2147483648.5&&v<2147483647.5))return -2147483647L-1L;
 long t=(long)v;double r=v-(double)t;
 if(r>0.5)++t;else if(r<-0.5)--t;else if((r==0.5||r==-0.5)&&(t&1L)!=0)t+=(r>0.0)?1L:-1L;
 return t;
}
}
inline long GFFloatToLongNear(float value){return fable_portable::X87FistpNear((double)value);}

#define FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT
#define FABLE_RETAIL_GFUNPACKFLOAT_GROUP_PRODUCT
inline float  GFRoundFloatNear(float value, long places)
{
    float scale = 1.0f;
    for (long i = places; i != 0; --i)
        scale *= 10.0f;
#ifdef FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT
    return GFFloatToLongNear((value * scale)) / scale;
#else
    return GFFloatToLongNear(value * scale) / scale;
#endif
}
inline unsigned long  GFPackFloat(float value, unsigned long numBits, float minValue, float maxValue, unsigned long numDecimalPlaces)
{
    float normalised = (GFRoundFloatNear(value, numDecimalPlaces) - minValue) / (maxValue - minValue);
    return (unsigned long)GFFloatToLongNear(normalised * ((float)(long)(1 << numBits) - 1.0f));
}
inline float  GFUnpackFloat(unsigned long packed, unsigned long numBits, float minValue, float maxValue, unsigned long numDecimalPlaces)
{
    float normalised = packed / ((float)(long)(1 << numBits) - 1.0f);
#ifdef FABLE_RETAIL_GFUNPACKFLOAT_GROUP_PRODUCT
    return GFRoundFloatNear((normalised * (maxValue - minValue)) + minValue, numDecimalPlaces);
#else
    return GFRoundFloatNear(normalised * (maxValue - minValue) + minValue, numDecimalPlaces);
#endif
}

struct CEmitterDatabaseSerialiseInfo{};
struct Emitter{
CCountedPointer<CCardinalSpline> Spline;
    unsigned long EmitterPosParam;                      
    unsigned long DirectionParamName;                   
    long LifeTimer;                                     
    float NextGenerateTime;                             
    unsigned long NoParticlesToStart : 10;              
    unsigned long NoParticlesToStartRand : 10;
    unsigned long EmitterType : 2;
    unsigned long Solid : 1;
    unsigned long UseEmitterLifeSecs : 1;
    unsigned long UseEmitterTimelineSecs : 1;
    unsigned long OrientationXY : 1;
    unsigned long OrientationXZ : 1;
    unsigned long OrientationYZ : 1;
    unsigned long UseCustomDirection : 1;
    unsigned long UseOutwardDirection : 1;
    unsigned long UseParamDirection : 1;
    unsigned long OppositeDirection : 1;
    unsigned long AngularPerturbationInteger : 10;      
    unsigned long CustomDirectionX : 8;
    unsigned long ParticlesPerSecond : 14;
    unsigned long EmitterSize : 10;                     
    unsigned long RadialBias : 10;
    unsigned long MinSpeed : 10;
    unsigned long UseForwardDirection : 1;
    unsigned long UseRandom2DDirection : 1;
    unsigned long CustomDirectionY : 8;                 
    unsigned long CustomDirectionZ : 8;
    unsigned long EmitterTimelineSecs : 15;
    unsigned long UseRandom3DDirection : 1;
    unsigned long EmitterLifeSecs : 15;                 
    unsigned long EmitterStartTime : 15;
    unsigned long MaxSpeed : 10;                        
    unsigned long NonUniformScalingX : 11;
    unsigned long NonUniformScalingY : 11;
    unsigned short NonUniformScalingZ : 11;             
    unsigned long StartedTimer : 1;                     
    unsigned long FinishedGenerating : 1;

Emitter(){
EmitterPosParam=0;
DirectionParamName=0;
LifeTimer=0;
NextGenerateTime=0;
NoParticlesToStart=0;
NoParticlesToStartRand=0;
EmitterType=0;
Solid=0;
UseEmitterLifeSecs=0;
UseEmitterTimelineSecs=0;
OrientationXY=0;
OrientationXZ=0;
OrientationYZ=0;
UseCustomDirection=0;
UseOutwardDirection=0;
UseParamDirection=0;
OppositeDirection=0;
AngularPerturbationInteger=0;
CustomDirectionX=0;
ParticlesPerSecond=0;
EmitterSize=0;
RadialBias=0;
MinSpeed=0;
UseForwardDirection=0;
UseRandom2DDirection=0;
CustomDirectionY=0;
CustomDirectionZ=0;
EmitterTimelineSecs=0;
UseRandom3DDirection=0;
EmitterLifeSecs=0;
EmitterStartTime=0;
MaxSpeed=0;
NonUniformScalingX=0;
NonUniformScalingY=0;
NonUniformScalingZ=0;
StartedTimer=0;
FinishedGenerating=0;
}
void DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&);
};
#define TRANSFER_BOOL_BIT(name, member) { bool value = member; context.Transfer(name, value, false); member = value; }
#define TRANSFER_LONG_BITS(name, member) { long value = member; context.Transfer(name, value, 0L); member = value; }
void Emitter::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo&)
{
    {
        float temp = GFUnpackFloat(EmitterSize, 10, 0.0f, 10.0f, 2);
        context.Transfer("EmitterSize", temp, 1.0f);
        EmitterSize = GFPackFloat(temp, 10, 0.0f, 10.0f, 2);
    }
    {
        float temp = GFUnpackFloat(RadialBias, 10, -5.0f, 5.0f, 2);
        context.Transfer("RadialBias", temp, 0.0f);
        RadialBias = GFPackFloat(temp, 10, -5.0f, 5.0f, 2);
    }
    {
        float temp = GFUnpackFloat(EmitterLifeSecs, 15, 0.0f, 300.0f, 2);
        context.Transfer("EmitterLifeSecs", temp, 10.0f);
        EmitterLifeSecs = GFPackFloat(temp, 15, 0.0f, 300.0f, 2);
    }
    {
        float temp = GFUnpackFloat(EmitterStartTime, 15, 0.0f, 300.0f, 2);
        context.Transfer("EmitterStartTime", temp, 0.0f);
        EmitterStartTime = GFPackFloat(temp, 15, 0.0f, 300.0f, 2);
    }
    {
        float temp = GFUnpackFloat(EmitterTimelineSecs, 15, 0.0f, 300.0f, 2);
        context.Transfer("EmitterTimelineSecs", temp, 30.0f);
        EmitterTimelineSecs = GFPackFloat(temp, 15, 0.0f, 300.0f, 2);
    }
    {
        float temp = GFUnpackFloat(ParticlesPerSecond, 14, 0.0f, 100.0f, 2);
        context.Transfer("ParticlesPerSecond", temp, 3.0f);
        ParticlesPerSecond = GFPackFloat(temp, 14, 0.0f, 100.0f, 2);
    }
    {
        C3DVector temp;
        temp.X = GFUnpackFloat(NonUniformScalingX, 11, -10.0f, 10.0f, 2);
        temp.Y = GFUnpackFloat(NonUniformScalingY, 11, -10.0f, 10.0f, 2);
        temp.Z = GFUnpackFloat(NonUniformScalingZ, 11, -10.0f, 10.0f, 2);
        context.Transfer("NonUniformScaling", temp, C3DVector(1.0f, 1.0f, 1.0f));
        NonUniformScalingX = GFPackFloat(temp.X, 11, -10.0f, 10.0f, 2);
        NonUniformScalingY = GFPackFloat(temp.Y, 11, -10.0f, 10.0f, 2);
        NonUniformScalingZ = GFPackFloat(temp.Z, 11, -10.0f, 10.0f, 2);
    }
    {
        float temp = GFUnpackFloat(MinSpeed, 10, 0.0f, 10.0f, 2);
        context.Transfer("MinSpeed", temp, 0.0f);
        MinSpeed = GFPackFloat(temp, 10, 0.0f, 10.0f, 2);
    }
    {
        float temp = GFUnpackFloat(MaxSpeed, 10, 0.0f, 10.0f, 2);
        context.Transfer("MaxSpeed", temp, 0.0f);
        MaxSpeed = GFPackFloat(temp, 10, 0.0f, 10.0f, 2);
    }
    {
        C3DVector temp;
        temp.X = GFUnpackFloat(CustomDirectionX, 8, -1.0f, 1.0f, 2);
        temp.Y = GFUnpackFloat(CustomDirectionY, 8, -1.0f, 1.0f, 2);
        temp.Z = GFUnpackFloat(CustomDirectionZ, 8, -1.0f, 1.0f, 2);
        context.Transfer("CustomDirection", temp, C3DVector(0.0f, 0.0f, 1.0f));
        CustomDirectionX = GFPackFloat(temp.X, 8, -1.0f, 1.0f, 2);
        CustomDirectionY = GFPackFloat(temp.Y, 8, -1.0f, 1.0f, 2);
        CustomDirectionZ = GFPackFloat(temp.Z, 8, -1.0f, 1.0f, 2);
    }
    TRANSFER_LONG_BITS("EmitterType", EmitterType);
    TRANSFER_BOOL_BIT("Solid", Solid);
    TRANSFER_BOOL_BIT("UseEmitterLifeSecs", UseEmitterLifeSecs);
    TRANSFER_BOOL_BIT("UseEmitterTimelineSecs", UseEmitterTimelineSecs);
    TRANSFER_LONG_BITS("NoParticlesToStart", NoParticlesToStart);
    TRANSFER_LONG_BITS("NoParticlesToStartRand", NoParticlesToStartRand);
    TRANSFER_BOOL_BIT("OrientationXY", OrientationXY);
    TRANSFER_BOOL_BIT("OrientationXZ", OrientationXZ);
    TRANSFER_BOOL_BIT("OrientationYZ", OrientationYZ);
    TRANSFER_BOOL_BIT("UseCustomDirection", UseCustomDirection);
    TRANSFER_BOOL_BIT("UseOutwardDirection", UseOutwardDirection);
    TRANSFER_BOOL_BIT("UseForwardDirection", UseForwardDirection);
    TRANSFER_BOOL_BIT("UseRandom2DDirection", UseRandom2DDirection);
    TRANSFER_BOOL_BIT("UseRandom3DDirection", UseRandom3DDirection);
    TRANSFER_BOOL_BIT("UseParamDirection", UseParamDirection);
    TRANSFER_BOOL_BIT("OppositeDirection", OppositeDirection);
    TRANSFER_LONG_BITS("AngularPerturbationInteger", AngularPerturbationInteger);
    if (context.IsSaving()) {
        context.Transfer("EmitterPosParam", EmitterPosParam, 0UL);
        context.Transfer("DirectionParamName", DirectionParamName, 0UL);
        if (!Spline.IsNull()) {
            float tension = Spline->GetTension();
            context.Transfer("EmitterSplineTension", tension);
            long count = Spline->GetControlPointCount();
            context.Transfer("EmitterSplineUserPointsArraySize_BugFixed", count);
            CArray<C3DVector> points = Spline->GetControlPoints();
            long i = 0;
            if (i < count) {
                C3DVector zero(0.0f, 0.0f, 0.0f);
                do {
                    context.Transfer((CCharString("EmitterSplineUserPointsArray_") + GFIntToCharString(i)).GetAsCharArray(), points[i], zero);
                } while (++i < count);
            }
        }
    } else {
        CCharString name;
        context.Transfer("EmitterPosParam", name);
        EmitterPosParam = MAKE_UPDATE_PARAM_FROM_BASE(0,name);
        context.Transfer("DirectionParamName", name);
        DirectionParamName = MAKE_VECTOR_PARAM_FROM_BASE(0,name);
        if (EmitterType == 3) {
            Spline.Reset(new CCardinalSpline);
            float tension;
            context.Transfer("EmitterSplineTension", tension);
            Spline->SetTension(tension);
            CArray<C3DVector> points;
            long count;
            context.Transfer("EmitterSplineUserPointsArraySize_BugFixed", count);
            if (count != 0) {
                points.resize(count);
                long i = 0;
                if (i < count) {
                    C3DVector zero(0.0f, 0.0f, 0.0f);
                    do {
                        context.Transfer((CCharString("EmitterSplineUserPointsArray_") + GFIntToCharString(i)).GetAsCharArray(), points[i], zero);
                    } while (++i < count);
                }
            } else {
                context.Transfer("EmitterSplineUserPointsArraySize", count);
                if (count > 20)
                    count = 20;
                points.resize(count);
                long i = 0;
                if (i < count) {
                    C3DVector zero(0.0f, 0.0f, 0.0f);
                    do {
                        context.Transfer("EmitterSplineUserPointsArray" + i, points[i], zero);
                    } while (++i < count);
                }
            }
            C3DVector zero(0.0f, 0.0f, 0.0f);
            while (points.size() < 2)
                points.push_back(zero);
            Spline->SetControlPoints(points);
        }
    }
}

static bool Eq(C3DVector a,C3DVector b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
static bool Ordered(const CPersistContext& c){const char* names[]={"EmitterSize","RadialBias","EmitterLifeSecs","EmitterStartTime","EmitterTimelineSecs","ParticlesPerSecond","NonUniformScaling","MinSpeed","MaxSpeed","CustomDirection","EmitterType","Solid","UseEmitterLifeSecs","UseEmitterTimelineSecs","NoParticlesToStart","NoParticlesToStartRand","OrientationXY","OrientationXZ","OrientationYZ","UseCustomDirection","UseOutwardDirection","UseForwardDirection","UseRandom2DDirection","UseRandom3DDirection","UseParamDirection","OppositeDirection","AngularPerturbationInteger","EmitterPosParam","DirectionParamName"};if(c.Names.size()<29)return false;for(int i=0;i<29;++i)if(c.Names[i]!=names[i])return false;return true;}
static void Endpoints(CPersistContext& c){c.Floats["EmitterSize"]=10;c.Floats["RadialBias"]=-5;c.Floats["EmitterLifeSecs"]=300;c.Floats["EmitterStartTime"]=300;c.Floats["EmitterTimelineSecs"]=300;c.Floats["ParticlesPerSecond"]=100;c.Vectors["NonUniformScaling"]=C3DVector(10,-10,10);c.Floats["MinSpeed"]=0;c.Floats["MaxSpeed"]=10;c.Vectors["CustomDirection"]=C3DVector(-1,1,0);}
int main(){bool ok=true;CEmitterDatabaseSerialiseInfo info;
 {Emitter e;CPersistContext c(false);e.DatabaseSerialise(c,info);ok=ok&&Ordered(c)&&e.Spline.IsNull()&&e.EmitterPosParam==0&&e.DirectionParamName==0&&e.EmitterSize==102&&e.RadialBias==512&&e.EmitterLifeSecs==1092&&e.EmitterTimelineSecs==3277&&e.ParticlesPerSecond==491&&e.NonUniformScalingX==1126&&e.CustomDirectionZ==255&&!e.Solid;}
 {Emitter e;CPersistContext c(false);Endpoints(c);c.Longs["EmitterType"]=2;c.Longs["NoParticlesToStart"]=1025;c.Longs["NoParticlesToStartRand"]=1023;c.Longs["AngularPerturbationInteger"]=511;c.Bools["Solid"]=true;c.Bools["UseEmitterTimelineSecs"]=true;c.Bools["OrientationXZ"]=true;c.Bools["OppositeDirection"]=true;c.Strings["EmitterPosParam"]=CCharString("Centre");c.Strings["DirectionParamName"]=CCharString("Forward");e.DatabaseSerialise(c,info);ok=ok&&Ordered(c)&&e.EmitterSize==1023&&e.RadialBias==0&&e.EmitterLifeSecs==32767&&e.EmitterStartTime==32767&&e.EmitterTimelineSecs==32767&&e.ParticlesPerSecond==16383&&e.NonUniformScalingX==2047&&e.NonUniformScalingY==0&&e.NonUniformScalingZ==2047&&e.MinSpeed==0&&e.MaxSpeed==1023&&e.CustomDirectionX==0&&e.CustomDirectionY==255&&e.CustomDirectionZ==128&&e.NoParticlesToStart==1&&e.NoParticlesToStartRand==1023&&e.AngularPerturbationInteger==511&&e.EmitterType==2&&e.Solid&&e.UseEmitterTimelineSecs&&e.OrientationXZ&&e.OppositeDirection&&e.EmitterPosParam==CRC(0,"Centre",6)&&e.DirectionParamName==CRC(0,"Forward",7);CPersistContext save(true);e.DatabaseSerialise(save,info);ok=ok&&Ordered(save)&&save.Floats["EmitterSize"]==10&&save.Floats["RadialBias"]==-5&&save.Floats["EmitterLifeSecs"]==300&&Eq(save.Vectors["NonUniformScaling"],C3DVector(10,-10,10))&&save.Bools["OrientationXZ"]&&save.ULongs["EmitterPosParam"]==e.EmitterPosParam;}
 for(int count=0;count<4;++count){Emitter e;CPersistContext c(false);c.Longs["EmitterType"]=3;c.Longs["EmitterSplineUserPointsArraySize_BugFixed"]=count;c.Floats["EmitterSplineTension"]=0.25f;for(int i=0;i<count;++i){char n[64];std::sprintf(n,"EmitterSplineUserPointsArray_%d",i);c.Vectors[n]=C3DVector((float)i,2,4);}e.DatabaseSerialise(c,info);CArray<C3DVector> p=e.Spline->GetControlPoints();ok=ok&&Ordered(c)&&e.Spline->GetTension()==0.25f&&p.size()==(unsigned)(count<2?2:count);for(int i=0;i<count;++i)ok=ok&&Eq(p[i],C3DVector((float)i,2,4));for(unsigned i=count;i<p.size();++i)ok=ok&&Eq(p[i],C3DVector());CPersistContext save(true);e.DatabaseSerialise(save,info);ok=ok&&save.Longs["EmitterSplineUserPointsArraySize_BugFixed"]==(long)p.size()&&save.Floats["EmitterSplineTension"]==0.25f&&save.Vectors.count("EmitterSplineUserPointsArray_0")==1;}
 for(int count=1;count<=25;count+=12){Emitter e;CPersistContext c(false);c.Longs["EmitterType"]=3;c.Longs["EmitterSplineUserPointsArraySize"]=count;const char* key="EmitterSplineUserPointsArray";for(int i=0;i<count&&i<20;++i)c.Vectors[key+i]=C3DVector((float)i,8,16);e.DatabaseSerialise(c,info);CArray<C3DVector> p=e.Spline->GetControlPoints();unsigned n=count>20?20:count;ok=ok&&p.size()==(n<2?2:n);for(unsigned i=0;i<n;++i)ok=ok&&Eq(p[i],C3DVector((float)i,8,16));}
 {Emitter e;CPersistContext a(false);a.Longs["EmitterType"]=3;e.DatabaseSerialise(a,info);ok=ok&&Splines==1;CPersistContext b(false);b.Longs["EmitterType"]=3;b.Longs["EmitterSplineUserPointsArraySize_BugFixed"]=1;b.Vectors["EmitterSplineUserPointsArray_0"]=C3DVector(4,8,16);e.DatabaseSerialise(b,info);ok=ok&&Splines==1&&Eq(e.Spline->GetControlPoints()[0],C3DVector(4,8,16));}
 ok=ok&&Splines==0&&GFFloatToLongNear(0.5f)==0&&GFFloatToLongNear(1.5f)==2&&GFFloatToLongNear(2.5f)==2&&GFFloatToLongNear(-1.5f)==-2&&GFFloatToLongNear(-2.5f)==-2;
 std::printf(ok?"00b13530_TEST PASS\n":"00b13530_TEST FAIL\n");return ok?0:1;
}