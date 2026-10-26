// CThing::AddToRenderList() -- (propagated label) physics damping step: when the object's virtual test (slot 49) passes, scale its velocity vector at +0x28 by (1 - air resistance)
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E5FB0, 52 bytes. Callees: virtual slot 49 (+0xc4) on `this`,
// CTCPhysicsBase::GetAirResistance @ 0x006AFDE0 on `this`; the 1.0f constant @ 0x0122DED8.
// Identity: the catalog's "CThing::AddToRenderList" is a propagated name; the body calls
// CTCPhysicsBase::GetAirResistance on `this`, so `this` is a physics-component class (no xrefs: a vtable slot).
// Shape: `call ; fsubr [1.0]` is `1.0f - GetAirResistance()`; the factor stays on the x87 stack and each
// component is `fld st(0) ; fmul [m] ; fstp [m]`, the last one consuming it: an inline C3DVector `*=` float.
class C3DVector { public:
    float X, Y, Z;
    void operator*=(float f)
    {
        X *= f;
        Y *= f;
        Z *= f;
    }
};

class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48();
    virtual bool IsMoving();                         // slot 49 (+0xc4)
    unsigned char _pad04[0x28 - 0x04];
    C3DVector     Velocity;                          // +0x28
    float GetAirResistance();                        // 0x006AFDE0
};

class CThing : public CTCPhysicsBase { public:
    void AddToRenderList();
};

void CThing::AddToRenderList()
{
    if (IsMoving())
        Velocity *= 1.0f - GetAirResistance();
}