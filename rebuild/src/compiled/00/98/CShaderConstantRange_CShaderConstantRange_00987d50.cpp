// CShaderConstantRange::CShaderConstantRange() -- construct an empty shader-constant range (offset 0, count 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00987D50, length 26 bytes (+ int3 padding).
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 (root of the engine's polymorphic classes, vtable 0x01231710).
// Label correction: the manifest calls this "CTargetPoint::CTargetPoint" (propagated over the byte-identical
// shape); the vtable it stores, 0x0129A04C, is CShaderConstantRange's (rtti-vtables.tsv).
// Layout (Ego_r.pdb CShaderConstantRange, 12 bytes): vptr +0, Offset +4, Count +8.
// Shape: base call, both members cleared from one zero register (plain initialisers scheduled before the
// derived vtable store), then the vtable store.
class CBaseClass { public:                                         // vtable 0x01231710
    CBaseClass();                                           // 0x0099A2D0
    virtual void v00();
};

class CShaderConstantRange : public CBaseClass { public:                  // vtable 0x0129A04C
    long          Offset;                                   // +0x04
    unsigned long Count;                                    // +0x08
    CShaderConstantRange();
    virtual void v00();
};

CShaderConstantRange::CShaderConstantRange()
    : Offset(0), Count(0)
{
}