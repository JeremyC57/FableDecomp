// CTCWobble::FrameUpdate() -- while frames of wobble remain, rotate a copy of the parent's right-handed set (Up about Forward by -(direction * angle)), hand it to the physics component and flip the direction; when none remain restore the stored set and remove the component (interface 0xf7)
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E2F00, 199 bytes. __fastcall, `ret`. CTCWobble vtable 0x0123DF34.
// Callees: C3DVector::Rotate(const C3DVector& axis, float angle) 0x00A13C60 (in place); CTCPhysicsBase virtual slot 75
//          (+0x12c, SetFacingAnglesFromRHSet(const CRightHandedSet&) on the thing's physics component at +0x60);
//          CThing::RemoveTC(long) 0x004C9840 (manifest "CBaseObject::OnPropertyChanged" is propagated).
// Shape: FramesOfWobbleRemaining decremented in place; the whole ParentRHSet copied into a frame local (Up through the x87,
//        Forward through integer registers), Up copied again into a C3DVector local (the x87 values forwarded, Z re-read from
//        the copy), rotated about the copied Forward by `-DirectionLastRotated * AngleOfWobble` (spelled with the unary minus on the first factor; fld angle ; fmul direction ;
//        fchs stored into the pushed argument slot), assigned back into the set copy (three dword moves) and passed by reference;
//        DirectionLastRotated negated last. C3DVector carries memberwise user copy/assignment (plain `mov` copies).
// Layout: Ego_r.pdb CTCWobble (0x30): ParentThing +0x04, ParentRHSet +0x0c (Up +0x0c, Forward +0x18), FramesOfWobbleRemaining
//         +0x24, DirectionLastRotated +0x28, AngleOfWobble +0x2c. CThing PhysicsTC +0x60.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    C3DVector& operator=(const C3DVector& other) { X = other.X; Y = other.Y; Z = other.Z; return *this; }
    void Rotate(const C3DVector& axis, float angle);   // 0x00A13C60
};
void* FindRef_C3DVector_Rotate_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void C3DVector::Rotate(const C3DVector& axis, float angle)
{
    void* p = FindRef_C3DVector_Rotate_0();
    if (!p)
        return;
    *(void**)&axis = p;
    
}
#pragma auto_inline(on)
class CRightHandedSet { public:
    C3DVector Up;                                  // +0x00
    C3DVector Forward;                             // +0x0c
};
class CTCPhysicsBase { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual void SetFacingAnglesFromRHSet(const CRightHandedSet& rhSet);   // slot 75 (+0x12c)
};
enum ETCInterfaceType { };
class CThing { public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                     // +0x60
    void RemoveTC(ETCInterfaceType interfaceType);             // 0x004C9840
};
class CTCWobble { public:
    virtual void v00();
    CThing*         ParentThing;                   // +0x04
    unsigned char   _pad_0x08[4];
    CRightHandedSet ParentRHSet;                   // +0x0c
    long            FramesOfWobbleRemaining;       // +0x24
    float           DirectionLastRotated;          // +0x28
    float           AngleOfWobble;                 // +0x2c
    void FrameUpdate();
};
void CTCWobble::FrameUpdate()
{
    if (FramesOfWobbleRemaining) {
        --FramesOfWobbleRemaining;
        CRightHandedSet rhSet = ParentRHSet;
        C3DVector up = rhSet.Up;
        up.Rotate(rhSet.Forward, -DirectionLastRotated * AngleOfWobble);
        rhSet.Up = up;
        ParentThing->PhysicsTC->SetFacingAnglesFromRHSet(rhSet);
        DirectionLastRotated = -DirectionLastRotated;
    } else {
        ParentThing->PhysicsTC->SetFacingAnglesFromRHSet(ParentRHSet);
        ParentThing->RemoveTC((ETCInterfaceType)(0xf7));
    }
}