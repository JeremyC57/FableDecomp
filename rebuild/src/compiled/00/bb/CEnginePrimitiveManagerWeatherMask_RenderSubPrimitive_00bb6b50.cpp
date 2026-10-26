// CEnginePrimitiveManagerWeatherMask::RenderSubPrimitive(CEngineSubPrimitiveBase&, ERenderPassType) -- copies the parent weather-mask primitive's sphere, box, inverse transform and scale into a CWeatherMask and hands it with the primitive's flags to the weather renderer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BB6B50, length 136 bytes (+ int3 padding).
// Signature from the catalog (mangled): ?RenderSubPrimitive@CEnginePrimitiveManagerWeatherMask@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
// (`ret 8`; `this` and the pass type are unused).
// Callees: 0x00B50C80 CEngineWeatherRenderer::AddWeatherMask(const CWeatherMask&, unsigned long) on the global weather
// renderer @ 0x01436E44.
// Layout: CEngineSubPrimitiveBase (PDB, 0x18): Parent at +0xc. CEngineInternalPrimitiveWeatherMask (PDB): BSphere
// +0x48, BBox +0x58, InvTransform +0x70, Scale +0xa0, Flags +0xa4. CWeatherMask (PDB, 0x5c): BSphere +0, BBox +0x10,
// InvTransform +0x28, Scale +0x58 -- the whole 0x5c-byte frame.
// Shape: the mask is built by its member-initialising constructor: the sphere and the box copy memberwise
// (C3DVector has a user copy constructor: one register per dword, ecx/edx alternating), the 0x30-byte matrix is a
// POD block (`rep movsd`), the scale a dword move; the flags argument is loaded before the matrix copy ends.

class C3DVector { public:                                       // memberwise user copy constructor
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
class C3DBoundingSphere { public: C3DVector Center; float Radius; };
class C3DBoundingBox { public: C3DVector Min; C3DVector Max; };
class CMatrix3x4 { public: float m[12]; };                      // XAxis, YAxis, ZAxis, Position: a POD block

class CWeatherMask { public:                                    // 0x5c bytes (PDB)
    C3DBoundingSphere BSphere;                           // +0x00
    C3DBoundingBox    BBox;                              // +0x10
    CMatrix3x4        InvTransform;                      // +0x28
    float             Scale;                             // +0x58
    CWeatherMask(const C3DBoundingSphere& bSphere, const C3DBoundingBox& bBox, const CMatrix3x4& invTransform, const float& scale)
        : BSphere(bSphere), BBox(bBox), InvTransform(invTransform), Scale(scale) { }
};

class CEngineInternalPrimitiveWeatherMask { public:             // CEngineInternalPrimitiveBase (0x48) + the mask data
    unsigned char     _base[0x48];
    C3DBoundingSphere BSphere;                           // +0x48
    C3DBoundingBox    BBox;                              // +0x58
    CMatrix3x4        InvTransform;                      // +0x70
    float             Scale;                             // +0xa0
    unsigned long     Flags;                             // +0xa4
};
class CEngineSubPrimitiveBase { public:                         // PDB, 0x18 bytes
    void*                                vptr;           // +0x00
    unsigned long                        RenderFlags;    // +0x04
    unsigned char                        Alpha;          // +0x08
    CEngineInternalPrimitiveWeatherMask* Parent;         // +0x0c
    CEngineSubPrimitiveBase*             NextSubPrimitive; // +0x10
    void*                                Manager;        // +0x14
};

class CEngineWeatherRenderer { public:
    void AddWeatherMask(const CWeatherMask& mask, unsigned long flags);   // 0x00B50C80
};
void* FindRef_CEngineWeatherRenderer_AddWeatherMask_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void CEngineWeatherRenderer::AddWeatherMask(const CWeatherMask& mask, unsigned long flags)
{
    void* p = FindRef_CEngineWeatherRenderer_AddWeatherMask_0();
    if (!p)
        return;
    if (*(void**)&mask != p)
        return;
}
#pragma auto_inline(on)
extern CEngineWeatherRenderer* g_pEngineWeatherRenderer;  // 0x01436E44

class CEnginePrimitiveManagerWeatherMask { public:
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, long passType);
};

void CEnginePrimitiveManagerWeatherMask::RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, long passType)
{
    CEngineInternalPrimitiveWeatherMask* primitive = subPrimitive.Parent;
    CWeatherMask mask(primitive->BSphere, primitive->BBox, primitive->InvTransform, primitive->Scale);
    g_pEngineWeatherRenderer->AddWeatherMask(mask, primitive->Flags);
}