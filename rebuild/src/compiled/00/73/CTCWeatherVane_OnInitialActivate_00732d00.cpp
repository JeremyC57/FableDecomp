// CTCWeatherVane::OnInitialActivate() -- remember the vane's starting XY angle (the owner's physics/placement angle, 0 without one) plus a half-turn offset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00732D00, length 49 bytes (+ int3 padding).
// Callees: the owner's placement object (CThing +0x60) virtual slot 63 (+0xfc), returning a float angle.
// Floats: 0x0122DEDC = 0.0f, 0x0122F59C = 0.5f. Ego_r.pdb: ?OnInitialActivate@CTCWeatherVane@@UAEXXZ.
// Layout (Ego_r.pdb CTCWeatherVane, 16 bytes): CTCBase 0xc, InitialOffsetXY +0x0c.
// Shape: the placement pointer is a named local and each arm stores `value + 0.5f` itself; the absent case adds to the
// shared 0.0f constant (`fld [0x0122DEDC] ; fadd`, a literal 0.0f + 0.5f would fold). The single-expression ternary
// grades 49v49 with the vptr fetched into eax instead of retail's edx.
struct CThingPlacement {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetAngleXY() const;                       // slot 63 (+0xfc)
};

extern const float g_zero;                                   // 0x0122DEDC (0.0f)

class CThing { public:
    unsigned char    _pad_00[0x60];
    CThingPlacement* m_pPlacement;                          // +0x60
};

struct CTCWeatherVane {
    void*   __vftable;                                      // +0x00
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    float   InitialOffsetXY;                                // +0x0c
    void OnInitialActivate();
};

void CTCWeatherVane::OnInitialActivate()
{
    CThingPlacement* placement = ParentThing->m_pPlacement;
    if (placement)
        InitialOffsetXY = placement->GetAngleXY() + 0.5f;
    else
        InitialOffsetXY = g_zero + 0.5f;
}