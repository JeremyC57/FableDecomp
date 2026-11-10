#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#endif
// Required shared-header declarations and arithmetic, copied for a standalone test.
class C2DVector {
public:
    float X,Y;
    C2DVector() { }
    float GetMagnitude() const { return sqrtf(X*X+Y*Y); }
    const C2DVector& operator*=(float scalar) { X*=scalar;Y*=scalar;return *this; }
};
class Mass {
public:
    C2DVector Pos,Vel;
    float IMass,APos,AVel,AIMass;
    Mass();
    virtual ~Mass();
    void BackplaneFriction(float strength,float radius);
};
void Mass::BackplaneFriction(float strength, float radius)
{
    float speed = Vel.GetMagnitude();
    Vel *= speed <= strength ? 0.0f : 1.0f - strength / speed;
    float angular = strength / radius;
    float aspeed = (float)fabs(AVel);
    float scale;
    if (aspeed <= angular) scale = 0.0f; else scale = (1.0f - angular / aspeed);
    AVel *= scale;
}

Mass::Mass() { }
Mass::~Mass() { }
static unsigned int Bits(float value) { unsigned int bits; memcpy(&bits,&value,4); return bits; }
static void Check(float x,float y,float angular,float strength,float radius) {
    Mass mass;
    mass.Pos.X=7.0f;mass.Pos.Y=-8.0f;mass.Vel.X=x;mass.Vel.Y=y;
    mass.IMass=2.0f;mass.APos=9.0f;mass.AVel=angular;mass.AIMass=4.0f;
    float speed=(float)sqrt(x*x+y*y);
    float linearScale=speed<=strength?0.0f:1.0f-strength/speed;
    float angularStrength=strength/radius;
    float angularSpeed=(float)fabs(angular);
    float angularScale=angularSpeed<=angularStrength?0.0f:1.0f-angularStrength/angularSpeed;
    float expectedX=x*linearScale,expectedY=y*linearScale,expectedAngular=angular*angularScale;
    mass.BackplaneFriction(strength,radius);
    assert(fabs(mass.Vel.X-expectedX)<0.00001f);
    assert(fabs(mass.Vel.Y-expectedY)<0.00001f);
    assert(fabs(mass.AVel-expectedAngular)<0.00001f);
    if(expectedX==0.0f)assert(Bits(mass.Vel.X)==Bits(expectedX));
    if(expectedY==0.0f)assert(Bits(mass.Vel.Y)==Bits(expectedY));
    if(expectedAngular==0.0f)assert(Bits(mass.AVel)==Bits(expectedAngular));
    assert(mass.Pos.X==7.0f&&mass.Pos.Y==-8.0f&&mass.IMass==2.0f&&mass.APos==9.0f&&mass.AIMass==4.0f);
}
int main() {
    const float velocities[][2]={{0.0f,0.0f},{3.0f,4.0f},{-3.0f,4.0f},{3.0f,-4.0f},{-3.0f,-4.0f},{0.0f,2.0f},{-2.0f,0.0f}};
    const float strengths[]={0.0f,0.5f,1.0f,2.0f,5.0f,8.0f};
    const float angulars[]={-8.0f,-2.0f,-0.5f,-0.0f,0.0f,0.5f,2.0f,8.0f};
    const float radii[]={0.5f,1.0f,2.0f,4.0f};
    for(int v=0;v<7;++v)for(int s=0;s<6;++s)for(int a=0;a<8;++a)for(int r=0;r<4;++r)
        Check(velocities[v][0],velocities[v][1],angulars[a],strengths[s],radii[r]);
    puts("008eebe0_TEST PASS");return 0;
}