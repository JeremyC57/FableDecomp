// CTCPhysicsStandard::UpdateCollisionGameInteractions(CThing* other, CPhysicsCollideResponse* response) -- tell both things about a thing-thing collision: the parent thing when this component interacts with things, and the other thing when its physics component interacts with things and it is not dead
// Leaf: UpdateCollisionGameInteractions
// Retail: Fable.exe (TLC, VC7.1) @ 0x00723AC0, 98 bytes.
// Callees: CTCPhysicsBase virtual slot 60 (+0xf0, GetInteractsWithThings: the CTCPhysicsStandard override 0x00724CC0 reads
//   InteractsWithThings, +0x84 bit 6), CThing virtual slot 41 (+0xa4, CThing::OnCollision(ECollideType, C3DVector*, C3DVector*,
//   CThing*) @ 0x004C8DA0 in the CThing vtable 0x01239ABC), collide type 1 = thing.
// Layout: CThing +0x60 = its physics component (CTCPhysicsBase*), +0x91 bit 0 = dead; CPhysicsCollideResponse CollidePos +0x00,
//   CollideNormal +0x0c.
// Shape: the alive test is `(unsigned char)~flags & 1` (8-bit `not al ; test al,1`; without the cast it widens to eax); the parameters are loaded into ebx/edi after the first call.
struct C3DVector { float X, Y, Z; };
struct CPhysicsCollideResponse {
    C3DVector CollidePos;                       // +0x00
    C3DVector CollideNormal;                    // +0x0c
    float     Friction;                         // +0x18
};
struct CTCPhysicsBase;
struct CThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void OnCollision(long collideType, const C3DVector* position, const C3DVector* normal, CThing* other);   // slot 41 (+0xa4)
    unsigned char   _pad04[0x5c];
    CTCPhysicsBase* PPhysics;                   // +0x60
    unsigned char   _pad64[0x2d];
    unsigned char   Flags91;                    // +0x91 bit 0 = dead
};
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual bool GetInteractsWithThings();   // slot 60 (+0xf0)
    CThing* ParentThing;                        // +0x04
};
struct CTCPhysicsStandard : CTCPhysicsBase {
    void UpdateCollisionGameInteractions(CThing* other, CPhysicsCollideResponse* response);
};

void CTCPhysicsStandard::UpdateCollisionGameInteractions(CThing* other, CPhysicsCollideResponse* response)
{
    if (GetInteractsWithThings())
        ParentThing->OnCollision(1, &response->CollidePos, &response->CollideNormal, other);
    if (other->PPhysics->GetInteractsWithThings() && ((unsigned char)~other->Flags91 & 1))
        other->OnCollision(1, &response->CollidePos, &response->CollideNormal, ParentThing);
}
