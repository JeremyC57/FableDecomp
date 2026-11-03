#ifndef FABLE_ENGINE_MASS_H
#define FABLE_ENGINE_MASS_H
#include "engine/C2DVector.h"
// Layout: original Ego_r Mass TPI30d33 and native constructor008ee9b0.
// Full size0x24; public Pos+4,Vel+c,IMass+14,APos+18,AVel+1c,AIMass+20.
class Mass {
public:
    C2DVector Pos,Vel;
    float IMass,APos,AVel,AIMass;
    Mass();
    virtual ~Mass();
    C2DVector GetVel(C2DVector pos) const;
    void ApplyImpulse(C2DVector pos,C2DVector force);
    void ApplyMove(C2DVector pos,C2DVector move);
    C2DVector ForceToVel(C2DVector pos,C2DVector force);
    static bool __fastcall Collide(Mass* a,Mass* b,C2DVector a_contact,C2DVector b_contact,C2DVector normal,float restitution_coefficient,float sliding_friction_coefficient,float seperate,float time_delta);
};
#endif
