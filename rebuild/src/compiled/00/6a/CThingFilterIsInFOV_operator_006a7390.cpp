// CThingFilter_IsInFOV::operator()(CThing* thing) -- fetch the centre (and radius) of the thing's physics bounding info and test that point against the field of view (the C3DVector overload)
// Leaf: ??RCThingFilter_IsInFOV@@QBE_NPAVCThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7390, length 47 bytes (+ int3 padding).
// Callees: CTCPhysicsBase::GetBoundingInfo @ 0x006AF640 (`__fastcall(C3DVector* centre, float* radius)`),
// CThingFilter_IsInFOV::operator()(const C3DVector&) @ 0x006A7270.
// Layout: PDB CThingFilter_IsInFOV { C3DVector FromPos; C3DVector Direction; float CosFOV; float FOV; bool AngleTest3D; };
// retail CThing PhysicsTC +0x60 (no null test here).
// Shape: the centre is a 12-byte frame local; the unused radius out-value is homed in the dead `thing` slot
// (`lea eax,[esp+14h]`).
class C3DVector { public:
    float x, y, z;
};

class CTCPhysicsBase { public:
    void GetBoundingInfo(C3DVector* centre, float* radius);   // 0x006AF640
};

class CThing { public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                      // +0x60
};

class CThingFilter_IsInFOV { public:
    C3DVector FromPos;                              // +0x00
    C3DVector Direction;                            // +0x0c
    float     CosFOV;                               // +0x18
    float     FOV;                                  // +0x1c
    bool      AngleTest3D;                          // +0x20
    bool operator()(const C3DVector& pos) const;    // 0x006A7270
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsInFOV::operator()(CThing* thing) const
{
    C3DVector centre;
    float radius;
    thing->PhysicsTC->GetBoundingInfo(&centre, &radius);
    return (*this)(centre);
}