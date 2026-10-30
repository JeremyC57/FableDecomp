// CTCWobble::OnInitialActivate() -- set the wobble angle, remember the parent's right-handed set, apply the initial half-angle rotation of Up about Forward, then reset the direction and the frame count (half the frame rate, rounded by the fistp helper)
// Grade: IMPROVED(252v257) retry1 -- retail_asm CeilToLong in place; residue: the ParentRHSet assignment shape
// Retail-asm: 0x007E2ECB `fadd [ebp-0Ch]` + 0x007E2ECE `fistp dword ptr [ebp-4]` -- the engine inline-assembly CeilToLong (landed 0x00772720 / 0x006A68F0).
// Leaf: ?OnInitialActivate@CTCWobble
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E2DF0, 257 bytes (+15 int3), `ret`. CTCWobble vtable 0x0123DF34.
// Callees: CTCPhysicsBase virtual slot 72 (+0x120, the current right-handed set by reference) and slot 75 (+0x12c,
//   SetFacingAnglesFromRHSet), C3DVector::Rotate(const C3DVector&, float) @ 0x00A13C60. Globals: the int frame rate
//   @ 0x01375550, 0.5f @ 0x0122F59C. AngleOfWobble = 0.004f (0x3B83126F).
// Layout: the landed sibling CTCWobble::FrameUpdate (0x007E2F00) model.
struct C3DVector {
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
struct CRightHandedSet {
    C3DVector Up;                                  // +0x00
    C3DVector Forward;                             // +0x0c
};
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual const CRightHandedSet& GetRHSet();   // slot 72 (+0x120)
    virtual void v073(); virtual void v074();
    virtual void SetFacingAnglesFromRHSet(const CRightHandedSet& rhSet);   // slot 75 (+0x12c)
};
struct CThing {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                     // +0x60
    void RemoveTC(long interfaceType);             // 0x004C9840
};
struct CTCWobble {
    virtual void v00();
    CThing*         ParentThing;                   // +0x04
    unsigned char   _pad_0x08[4];
    CRightHandedSet ParentRHSet;                   // +0x0c
    long            FramesOfWobbleRemaining;       // +0x24
    float           DirectionLastRotated;          // +0x28
    float           AngleOfWobble;                 // +0x2c
    void OnInitialActivate();
};
extern int g_frameRate;                            // 0x01375550

// the engine's inline-assembly ceil helper (the landed CTCShop::GetMaxStock 0x00772720 / CThingPhysical::ModifyHealth 0x006A68F0 CeilToLong, verbatim)
__forceinline long CeilToLong(float value)
{
    long  result;
    float one;
    float half;
    float back;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fadd  half
        fistp result
        fild  result
        fsub  one
        fstp  back
        mov   eax, value
        cmp   eax, back
        jne   done
        dec   result
    done:
    }
    return result;
}

void CTCWobble::OnInitialActivate()
{
    AngleOfWobble = 0.004f;
    ParentRHSet = ParentThing->PhysicsTC->GetRHSet();
    CRightHandedSet rhSet = ParentRHSet;
    C3DVector up = rhSet.Up;
    up.Rotate(rhSet.Forward, AngleOfWobble * 0.5f);
    rhSet.Up = up;
    ParentThing->PhysicsTC->SetFacingAnglesFromRHSet(rhSet);
    DirectionLastRotated = 1.0f;
    FramesOfWobbleRemaining = CeilToLong(g_frameRate * 0.5f);
}
