#define FABLE_RETAIL_C2DVECTOR
#include "engine/C2DVector.h"
#include <cassert>
#include <cmath>
#include <cstdio>

#ifndef _MSC_VER
#define __fastcall
#endif
#include <math.h>

struct Mass
{
    C2DVector Pos, Vel;
    float IMass, APos, AVel, AIMass;
    C2DVector GetVel(C2DVector pos) const;
    void ApplyImpulse(C2DVector pos,C2DVector force);
    void ApplyMove(C2DVector pos,C2DVector move);
    C2DVector ForceToVel(C2DVector pos,C2DVector force);
    static bool __fastcall Collide(Mass*,Mass*,C2DVector,C2DVector,C2DVector,float,float,float,float);
};

// The selected implementation is reproduced verbatim for this standalone fixture.
inline C2DVector __fastcall Right(C2DVector c){return C2DVector(c.Y,-c.X);}
float __fastcall CopySign(float a,float sign){return sign<0.0f?-(float)fabs(a):(float)fabs(a);}
C2DVector Mass::GetVel(C2DVector pos) const
{
    return Vel+Right(pos-Pos)*AVel;
}
void Mass::ApplyImpulse(C2DVector pos,C2DVector force)
{
    AVel+=Right(pos-Pos).Dot(force)*AIMass;
    Vel+=force*IMass;
}
void Mass::ApplyMove(C2DVector pos,C2DVector move)
{
    APos+=Right(pos-Pos).Dot(move)*AIMass;
    Pos+=move*IMass;
}
C2DVector Mass::ForceToVel(C2DVector pos,C2DVector force)
{
    return force*IMass+Right(pos-Pos)*Right(pos-Pos).Dot(force)*AIMass;
}
bool __fastcall Mass::Collide(Mass* a,Mass* b,C2DVector a_contact,C2DVector b_contact,C2DVector normal,float restitution_coefficient,float sliding_friction_coefficient,float seperate,float time_delta)
{
    float div1=normal.Dot(a->ForceToVel(a_contact,normal)+b->ForceToVel(b_contact,normal));
    float move=((b_contact-a_contact).Dot(normal)-seperate)/div1;
    a->ApplyMove(a_contact,normal*move);
    b->ApplyMove(b_contact,-normal*move);
    float bounce_impulse=(b->GetVel(b_contact)-a->GetVel(a_contact)).Dot(normal)*(1.0f+restitution_coefficient)/div1;
    float verlet_impulse=move/time_delta;
    if(verlet_impulse>bounce_impulse)bounce_impulse=verlet_impulse;
    if(bounce_impulse<0.0f)return false;
    a->ApplyImpulse(a_contact,normal*bounce_impulse);
    b->ApplyImpulse(b_contact,-normal*bounce_impulse);
    float sliding_friction_impulse=Right(normal).Dot(b->GetVel(b_contact)-a->GetVel(a_contact))/Right(normal).Dot(a->ForceToVel(a_contact,Right(normal))+b->ForceToVel(b_contact,Right(normal)));
    float max_sliding_friction_impulse=CopySign(bounce_impulse*sliding_friction_coefficient,sliding_friction_impulse);
    if(fabs(max_sliding_friction_impulse)>fabs(sliding_friction_impulse))max_sliding_friction_impulse=sliding_friction_impulse;
    a->ApplyImpulse(a_contact,Right(normal)*max_sliding_friction_impulse);
    b->ApplyImpulse(b_contact,-Right(normal)*max_sliding_friction_impulse);
    return true;
}

int main()
{
    for (int separation = -2; separation <= 2; ++separation) {
        for (int y = -3; y <= 3; ++y) {
            for (int x = -3; x <= 3; ++x) {
                for (int friction = 0; friction <= 2; ++friction) {
                    Mass a, b;
                    a.Pos = C2DVector(0, 0); b.Pos = C2DVector(0, 4);
                    a.Vel = C2DVector(0, 0); b.Vel = C2DVector(float(x), float(y));
                    a.IMass = 1; b.IMass = 1;
                    a.APos = b.APos = 0;
                    a.AVel = b.AVel = 0;
                    a.AIMass = b.AIMass = 0;
                    const bool actual = Mass::Collide(&a, &b, a.Pos, b.Pos, C2DVector(0, 1),
                         0.5f, 0.5f * friction, float(4 + separation), 1.0f);
                    const float moved = -float(separation) * 0.5f;
                    const float impulse = 0.75f * y > moved ? 0.75f * y : moved;
                    assert(actual == (impulse >= 0));
                    assert(a.Pos.Y == moved && b.Pos.Y == 4.0f - moved);
                    float slide = 0;
                    if (actual) {
                        const float limit = impulse * 0.5f * friction;
                        const float requested = 0.5f * x;
                        slide = requested < -limit ? -limit : requested > limit ? limit : requested;
                    }
                    assert(a.Vel.Y == (actual ? impulse : 0));
                    assert(b.Vel.Y == (actual ? y - impulse : y));
                    assert(a.Vel.X == slide && b.Vel.X == x - slide);
                    assert(a.APos == 0 && a.AVel == 0 && b.APos == 0 && b.AVel == 0);
                }
            }
        }
    }
    std::puts("008eec70_TEST PASS");
    return 0;
}
