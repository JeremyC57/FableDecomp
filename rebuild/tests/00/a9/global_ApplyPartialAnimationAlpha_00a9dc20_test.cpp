#include <stdio.h>
#include <math.h>
static float GFastArcCosTable[1026],GFastCosTable[1025];
static long GFFloatToLongNear(float v) {double i=floor(v);double rem=v-i;return (long)i+(rem>0.5 || (rem==0.5 && ((long)i&1)));}
struct C4DVector {
    float X,Y,Z,W;
    C4DVector Interpolate(const C4DVector& b,float t)const{
        C4DVector r={X+(b.X-X)*t,Y+(b.Y-Y)*t,Z+(b.Z-Z)*t,W+(b.W-W)*t};return r;
    }
};
struct CQuaternion {
    float X,Y,Z,W;
    float Dot(const CQuaternion& b) const;
    void SetSlerp(const CQuaternion& a,const CQuaternion& b,float t);
};
struct C3DKeyframe {
    CQuaternion Rotation;
    C4DVector Position,Scaling;
    void BlendWith(const C3DKeyframe& b,float t);
};
struct C3DAnimationSequence {
    C3DKeyframe keyframe;mutable int called;mutable long last_frame;mutable float last_fraction;
    void GetKeyframeInterpolate(C3DKeyframe& out,long frame,float fraction)const {
        ++called; last_frame=frame;last_fraction=fraction;out=keyframe;
    }
};
inline float GFFastArcCos(float cosine)
{
    cosine = (cosine + 1.0f) * 512.5f;
    long i = GFFloatToLongNear(cosine - 0.5f);
    float frac = cosine - (float)i;
    float a = GFastArcCosTable[i];
    float b = GFastArcCosTable[i + 1];
    return ((1.0f - frac) * a + frac * b) * 0.15915494f;
}
// FableWin ?GFFastSin@@YIMM@Z 37i/121b (this text 35i/120b at /Od; GFFastCos sibling the same +2i/+1b): named table values.
inline float GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i -= 256;
    i &= 0x3ff;
    float a = GFastCosTable[i];
    float b = GFastCosTable[i + 1];
    return (1.0f - frac) * a + frac * b;
}
inline float CQuaternion::Dot(const CQuaternion& q) const { return X * q.X + Y * q.Y + Z * q.Z + W * q.W; }
inline void CQuaternion::SetSlerp(const CQuaternion& from, const CQuaternion& to, float t)
{
    float cosom = from.Dot(to);
    float sign = 1.0f;
    if (cosom < 0.0f) {
        sign = -1.0f;
        cosom = -cosom;
    }
    float scale0, scale1;
    if (1.0f - cosom > 0.05f) {
        float omega = GFFastArcCos(cosom);
        float sinom = (float)sqrt(1.0f - cosom * cosom);
        scale0 = GFFastSin((1.0f - t) * omega) * (1.0f / sinom);
        scale1 = GFFastSin(omega * t) * (1.0f / sinom);
    } else {
        scale0 = 1.0f - t;
        scale1 = t;
    }
    scale1 *= sign;
    X = scale0 * from.X + scale1 * to.X;
    Y = scale0 * from.Y + scale1 * to.Y;
    Z = scale0 * from.Z + scale1 * to.Z;
    W = scale0 * from.W + scale1 * to.W;
}
inline void C3DKeyframe::BlendWith(const C3DKeyframe& other, float t)
{
    CQuaternion rotation;
    rotation.SetSlerp(Rotation, other.Rotation, t);
    Rotation = rotation;
    Position = Position.Interpolate(other.Position, t);
}

static void ApplyPartialAnimationAlpha(C3DKeyframe& dest, bool enable, long frame, float fraction, const C3DAnimationSequence* sequence, float alpha)
{
    if (enable) {
        C3DKeyframe keyframe;
        sequence->GetKeyframeInterpolate(keyframe, frame, fraction);
        dest.BlendWith(keyframe, alpha);
    }
}


int main(){
    const double pi=acos(-1.0);
    for(int n=0;n<1026;++n)GFastArcCosTable[n]=(float)acos(n/512.5-1.0);
    for(int m=0;m<1025;++m)GFastCosTable[m]=(float)cos(2*pi*m/1024.0);
    const float cosines[]={1.0f,0.96875f,0.75f,0.0f,-0.75f,-0.96875f,-1.0f};
    const float weights[]={0.0f,0.25f,0.5f,0.75f,1.0f};
    for(int enabled=0;enabled<2;++enabled)for(int q=0;q<7;++q)
    for(int f=0;f<3;++f)for(int k=0;k<5;++k){
        C3DAnimationSequence sequence;
        CQuaternion origin={0,0,0,1}; C4DVector p={-4,8,-2,1},other={4,-8,6,3},scale={2,4,6,8};
        C3DKeyframe dest;dest.Rotation=origin;dest.Position=p;dest.Scaling=scale;
        float c=cosines[q];double x=sqrt(1.0-(double)c*c);
        CQuaternion end={(float)x,0,0,c};sequence.keyframe.Rotation=end;sequence.keyframe.Position=other;
        sequence.keyframe.Scaling=other;sequence.called=0;sequence.last_frame=-1;sequence.last_fraction=-1.0f;
        long frame=f*11;float frac=f*0.25f;float alpha=weights[k];
        ApplyPartialAnimationAlpha(dest,enabled!=0,frame,frac,enabled?&sequence:0,alpha);
        if(sequence.called!=enabled)return 1;
        if(enabled && (sequence.last_frame!=frame || sequence.last_fraction!=frac))return 2;
        double t=enabled?alpha:0.0;double ax=0,aw=1;
        if(t>0){
            double dot=fabs(c);double sign=c<0?-1:1;
            if(1-dot>0.05){
                double angle=acos(dot),a=sin((1-t)*angle)/sin(angle),b=sin(t*angle)/sin(angle);
                ax=b*sign*x;aw=a+b*sign*c;
            }else{ax=t*sign*x;aw=(1-t)+t*sign*c;}
        }
        if(fabs(dest.Rotation.X-ax)>0.0003 || fabs(dest.Rotation.W-aw)>0.0003 || dest.Rotation.Y || dest.Rotation.Z)return 3;
        if(dest.Position.X!=-4+8*t || dest.Position.Y!=8-16*t || dest.Position.Z!=-2+8*t || dest.Position.W!=1+2*t)return 4;
        if(dest.Scaling.X!=2 || dest.Scaling.Y!=4 || dest.Scaling.Z!=6 || dest.Scaling.W!=8)return 5;
    }
    puts("00a9dc20_TEST PASS");return 0;
}