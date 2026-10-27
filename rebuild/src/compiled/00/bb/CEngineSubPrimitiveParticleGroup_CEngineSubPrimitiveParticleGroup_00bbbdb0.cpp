// CEngineSubPrimitiveParticleGroup::CEngineSubPrimitiveParticleGroup(CEngineInternalPrimitiveBase* parent) -- the sub-primitive base (parent kept, no next sub-primitive) and this class's vtable
// Leaf: ??0CEngineSubPrimitiveParticleGroup@@QAE@PAVCEngineInternalPrimitiveBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00bbbdb0, 34 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): it stores vtable 0x012A5BB0 (RTTI .?AVCEngineSubPrimitiveParticleGroup@@, bound ??_7CEngineSubPrimitiveParticleGroup@@6B@); Ego_r.pdb ??0CEngineSubPrimitiveParticleGroup@@QAE@PAVCEngineInternalPrimitiveBase@@@Z (reloc_bindings spells the parameter as a reference, AAV); callers CEnginePrimitiveManagerParticleGroup::RenderPrimitive 0x00BBBF70 and CEnginePrimitiveManager2DParticleGroup::Render2DPrimitive 0x00BE0CD0 (landed).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0.
// Shape (family L116 = landed family K056, 0x00BB1780): CEngineSubPrimitiveBase's inline constructor (Ego_r.pdb Parent +0x0C,
//   NextSubPrimitive +0x10 cleared); the base's own vptr store is dead and dropped, leaving only 0x012A5BB0.
class CBaseClass {
public:
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();
};
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                             // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};
class CEngineInternalPrimitiveBase { };
class CEnginePrimitiveManager;
class CEngineSubPrimitiveBase : public CBaseClassNonCopyable {   // Ego_r.pdb, 0x18 bytes
public:
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase* parent) : Parent(parent), NextSubPrimitive(0) { }
    unsigned long RenderFlags;                           // +0x04
    unsigned char Alpha;                                 // +0x08
    CEngineInternalPrimitiveBase* Parent;                // +0x0C
    CEngineSubPrimitiveBase* NextSubPrimitive;           // +0x10
    CEnginePrimitiveManager* Manager;                    // +0x14
};
class CEngineSubPrimitiveParticleGroup : public CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitiveParticleGroup(CEngineInternalPrimitiveBase* parent);
};
CEngineSubPrimitiveParticleGroup::CEngineSubPrimitiveParticleGroup(CEngineInternalPrimitiveBase* parent)
    : CEngineSubPrimitiveBase(parent)
{
}