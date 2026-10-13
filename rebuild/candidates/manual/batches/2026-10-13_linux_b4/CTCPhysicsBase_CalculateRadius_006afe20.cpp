// CTCPhysicsBase::CalculateRadius()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AFE20, oracle length 46 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi] ; call [eax+174h] ; test eax,eax ; je D ; fld [eax+18h] ; pop esi ; ret ; D: mov esi,[esi+38h] ; test esi,esi ; je U ; fld [esi+28h] ; pop esi ; fmul [__real@0.5] ; ret ; U: fld [__real@1.0] ; pop esi ; ret
// engine/CTCPhysicsBase.h (PDB): +0x38 PhysicsDef_Object (CDefPointer<CPhysicsDef>). Virtual slot
// 93 (+0x174) yields the collision shape when one exists, whose radius (+0x18) wins; otherwise half
// of the physics def's size (+0x28 of CPhysicsDef, constant 0.5 at 0x0122F59C); with no def the
// radius defaults to 1.0 (0x0122DED8).
#include "engine/CTCPhysicsBase.h"

struct CCollisionShapeCalls {
    unsigned char _pad_0x00[0x18];
    float         Radius;              // +0x18 (name inferred)
};

struct CPhysicsDefCalls {
    unsigned char _pad_0x00[0x28];
    float         Diameter;            // +0x28 (name inferred: halved to a radius)
};

struct CTCPhysicsVirtuals {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9c();
    virtual void va0(); virtual void va4(); virtual void va8(); virtual void vac();
    virtual void vb0(); virtual void vb4(); virtual void vb8(); virtual void vbc();
    virtual void vc0(); virtual void vc4(); virtual void vc8(); virtual void vcc();
    virtual void vd0(); virtual void vd4(); virtual void vd8(); virtual void vdc();
    virtual void ve0(); virtual void ve4(); virtual void ve8(); virtual void vec();
    virtual void vf0(); virtual void vf4(); virtual void vf8(); virtual void vfc();
    virtual void v100(); virtual void v104(); virtual void v108(); virtual void v10c();
    virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11c();
    virtual void v120(); virtual void v124(); virtual void v128(); virtual void v12c();
    virtual void v130(); virtual void v134(); virtual void v138(); virtual void v13c();
    virtual void v140(); virtual void v144(); virtual void v148(); virtual void v14c();
    virtual void v150(); virtual void v154(); virtual void v158(); virtual void v15c();
    virtual void v160(); virtual void v164(); virtual void v168(); virtual void v16c();
    virtual void v170();
    virtual CCollisionShapeCalls* GetCollisionShape();   // slot 93 (+0x174)
};

struct CTCPhysicsBase_Methods : CTCPhysicsBase {
    float CalculateRadius();
};

float CTCPhysicsBase_Methods::CalculateRadius()
{
    CCollisionShapeCalls* shape = reinterpret_cast<CTCPhysicsVirtuals*>(this)->GetCollisionShape();
    if (shape)
        return shape->Radius;
    CPhysicsDefCalls* def = reinterpret_cast<CPhysicsDefCalls*>(this->PhysicsDef_Object);
    if (def)
        return def->Diameter * 0.5f;
    return 1.0f;
}
