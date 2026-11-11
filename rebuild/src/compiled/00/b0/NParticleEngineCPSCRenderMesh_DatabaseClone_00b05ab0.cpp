// NParticleEngine::CPSCRenderMesh::DatabaseClone() const -- a database copy of this render-mesh component: a fresh CPSCRenderMesh (Alloc) that copies this one (DatabaseCopy, slot 18)
// Leaf: ?DatabaseClone@CPSCRenderMesh@NParticleEngine@@UBEPAVCPSCBase@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B05AB0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 2 of NParticleEngine::CPSCRenderMesh's vtable 0x0129F96C (the slot holds this body in 1 vtable(s), owner NParticleEngine::CPSCRenderMesh); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts DatabaseClone(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?DatabaseClone@CPSCRenderMesh@NParticleEngine@@UBEPAVCPSCBase@2@XZ; `ret` matches the signature.
// Was: NParticleEngine::CPSCRenderMesh::vfunc_2 (rebuild/src/compiled/00/b0/CThingTrackNode_DrawIsDrawable_00b05ab0.cpp).
// No shared header for NParticleEngine::CPSCBase / CPSCRenderMesh: minimal local definitions on the RTTI bases
// (CPSCRenderMesh : CPSCBase : CParticleUpdateParamCallback<unsigned long,unsigned long> : CBaseClassNonCopyable). The virtuals
// up to DatabaseCopy are declared in TLC slot order (vtable 0x0129F96C aligned with the Xbox 360 Test PDB order, agentFAM03;
// spellings from the Ego_r / FableWin name sets), since this body calls slot 18. Alloc is
// ?Alloc@CPSCRenderMesh@NParticleEngine@@SIPAVCPSCBase@2@XZ (Ego_r.pdb and FableWin.pdb), retail 0x00AF7D80.
#include "engine/CBaseClassNonCopyable.h"
#include "engine/C3DVector.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
#include "fable_string.h"
class CEnginePrimitivePrepareInfo;
class CEnginePrimitiveHandle;
class CSymbolMap;
class CMemoryDataInputStream;
class CDataOutputStream;
class CPersistContext;
namespace NParticleEngine {
enum EPSCType { };
enum EPSCSubType { };
class CParticleSystem;
class CEmitterDatabaseSerialiseInfo;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable {
public:
    virtual bool GetUpdateParam(C3DVector&, K, float, bool, const CParticleUpdateParameters<K, V>&, V);   // slot 1
};
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    virtual CPSCBase* DatabaseClone() const = 0;                                                             // slot 2
    virtual CCharString GetClassTypeName() const = 0;                                                        // slot 3
    virtual EPSCType GetComponentType() const = 0;                                                           // slot 4
    virtual EPSCSubType GetComponentSubType() const = 0;                                                     // slot 5
    virtual CCharString GetGUIName() const = 0;                                                              // slot 6
    virtual void OnCreate();                                                                                 // slot 7
    virtual void StartUpdate(const CParticleUpdateParameters<unsigned long, unsigned long>&);                // slot 8
    virtual void Update(CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&, bool&);   // slot 9
    virtual void CollectUpdateParameters(CParticleUpdateParameters<unsigned long, unsigned long>&, const unsigned long&);   // slot 10
    virtual void TransferUpdateParametersToDialog(const CSymbolMap&);                                        // slot 11
    virtual void UpdateAddParticle(CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&, C3DVector, C3DVector, float);   // slot 12
    virtual void PreparePrimitives(float, const CEnginePrimitivePrepareInfo&, CParticleSystem&,
                                   const CParticleUpdateParameters<unsigned long, unsigned long>&, CEnginePrimitiveHandle&);   // slot 13
    virtual void InterpolateTo(const CPSCBase&, float);                                                      // slot 14
    virtual void ReadBinary(CMemoryDataInputStream&);                                                        // slot 15
    virtual void WriteBinary(CDataOutputStream&) const;                                                      // slot 16
    virtual void DatabaseSerialise(CPersistContext&, const CEmitterDatabaseSerialiseInfo&);                  // slot 17
    virtual void DatabaseCopy(const CPSCBase&);                                                              // slot 18
};
class CPSCRenderMesh : public CPSCBase {
public:
    static CPSCBase* __fastcall Alloc();                                                                     // 0x00AF7D80
    virtual CPSCBase* DatabaseClone() const;                                                                 // slot 2
};
CPSCBase* CPSCRenderMesh::DatabaseClone() const
{
    CPSCBase* clone = Alloc();
    clone->DatabaseCopy(*this);
    return clone;
}
}
