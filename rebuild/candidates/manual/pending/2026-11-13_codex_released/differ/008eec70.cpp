// Mass::Collide(Mass *,Mass *,C2DVector,C2DVector,C2DVector,float,float,float,float) -- resolve the planar collision between two masses.
// Leaf: ?Collide@Mass@@SI_NPAV1@0VC2DVector@@11MMMM@Z
// Grade: DIFFER(1129v1129) -- one scheduler tie: VC7.1 RTM hoists 'mov eax,esp' (the by-value C2DVector argument copy for the tangent ForceToVel call) to just after 'sub esp,8', retail keeps it after the tangent-velocity FPU block; 13 shapes (named tangent/relative-velocity/numerator/denominator locals, Right by reference / not fastcall, GetVel placement and operand order, CopySign inline, ternary clamp, flags) never move it Regraded unchanged on the landed repo headers (f01).
// Flags: /Gr
#define FABLE_RETAIL_C2DVECTOR
#include <math.h>
#include "engine/Mass.h"
inline C2DVector __fastcall Right(C2DVector c){return C2DVector(c.Y,-c.X);}
float __fastcall CopySign(float a,float sign){return sign<0.0f?-(float)fabs(a):(float)fabs(a);}
#pragma auto_inline(off)
C2DVector Mass::ForceToVel(C2DVector pos,C2DVector force)
{
    return force*IMass+Right(pos-Pos)*Right(pos-Pos).Dot(force)*AIMass;
}
void Mass::ApplyImpulse(C2DVector pos,C2DVector force)
{
    AVel+=Right(pos-Pos).Dot(force)*AIMass;
    Vel+=force*IMass;
}
#pragma auto_inline(on)
inline C2DVector Mass::GetVel(C2DVector pos) const
{
    return Vel+Right(pos-Pos)*AVel;
}
void Mass::ApplyMove(C2DVector pos,C2DVector move)
{
    APos+=Right(pos-Pos).Dot(move)*AIMass;
    Pos+=move*IMass;
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
