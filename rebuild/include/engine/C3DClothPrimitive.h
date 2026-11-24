#ifndef FABLE_ENGINE_C3DCLOTHPRIMITIVE_H
#define FABLE_ENGINE_C3DCLOTHPRIMITIVE_H
// NEW header (agentC6; rows 0x00BB3E80 RenderClothPrimitive / 0x00BB30A0 Render2DClothPrimitive -- the model chunk 2060 / agentC2 kept
// local in the row). Ego_r.pdb C3DClothPrimitive, 0x14 bytes, polymorphic (vfptr +0x00): ParticleProgram +0x04
// (CCountedPointer<CParticleProgram>), PrimitiveIndex +0x0C, MaterialIndex +0x10 (struct_layouts_egor.tsv; retail reads +0x04 / +0x10
// at 0x00BB3E9x and 0x00BB30AB / 0x00BB3BC2). Inline accessors: FableWin.pdb ?GetParticleProgram@C3DClothPrimitive@@QBEPAVCParticleProgram@@XZ
// and ?GetMaterialIndex@C3DClothPrimitive@@QBEKXZ. The vtable is not modelled (no virtual is called through it here).
#include "rebuild_abi.h"
#include "engine/CCountedPointer.h"
class CParticleProgram;
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_LOADBINARY_PASS27
#include "engine/CBaseClassNonCopyable.h"
class C3DClothPrimitive : public CBaseClassNonCopyable {
#else
class C3DClothPrimitive {
#endif
public:
#ifdef FABLE_RETAIL_C3DCLOTHPRIMITIVE_CTOR
    // Opt-in (acctC wfE8, row 0x00A91640 C3DPrimitive2::InitialiseFromMeshFileData; moved here from the row's local model at landing gn):
    // the loader builds each cloth primitive with `new C3DClothPrimitive(chunk)` (operator new(0x14) at 0x00A92260, ctor 0x00AC3E00).
    // Declarations only (both out of line in retail); the destructor is declared non-virtual as the row's matched model had it.
    C3DClothPrimitive(const class C3DMeshFileClothChunk* chunk);                    // 0x00AC3E00
    ~C3DClothPrimitive();
#endif
#ifdef FABLE_RETAIL_C3DCLOTHPRIMITIVE_GETPARTICLEPROGRAM_GET
    // Opt-in (agentC8, rows 0x00BB30A0 / 0x00BB3E80): FableWin.pdb's debug ?GetParticleProgram@C3DClothPrimitive@@QBEPAVCParticleProgram@@XZ
    // is 10 instructions / 22 bytes = a call of the smart pointer's Get() (?Get@?$CCountedPointer@VCParticleProgram@@@@QBEPAVCParticleProgram@@XZ,
    // also in FableWin.pdb); the member read is 9 / 17. Same release bytes.
    CParticleProgram* GetParticleProgram() const { return ParticleProgram.Get(); }
#else
    CParticleProgram* GetParticleProgram() const { return ParticleProgram.Data; }   // FableWin ?GetParticleProgram@C3DClothPrimitive@@QBEPAVCParticleProgram@@XZ
#endif
    unsigned long GetMaterialIndex() const { return MaterialIndex; }                // FableWin ?GetMaterialIndex@C3DClothPrimitive@@QBEKXZ
#ifdef FABLE_RETAIL_C3DCLOTHPRIMITIVE_GETPRIMITIVEINDEX
    // Opt-in (agentNF41, row 0x00BA77B0): FableWin.pdb ?GetPrimitiveIndex@C3DClothPrimitive@@QBEKXZ (debug only: inline in release, a read
    // of PrimitiveIndex +0x0C). No layout change.
    unsigned long GetPrimitiveIndex() const { return PrimitiveIndex; }
#endif
#ifdef FABLE_RETAIL_C3DCLOTHPRIMITIVE_REMAPBONEINDICES
    // NF70 opt-in (round 7, row 0x00AC3AF0): Ego_r.pdb 0x00C4B610 / FableWin.pdb 0x03496150
    // ?RemapBoneIndices@C3DClothPrimitive@@QAEXAAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@@Z: forwards to the particle
    // program's RemapBoneIndices (retail `mov ecx,[ecx+4] ; jmp`). Declaration only (needs stlport/fable_map.h); no layout change.
    void RemapBoneIndices(std::map<unsigned long, unsigned long>& remap);
#endif
#ifdef FABLE_RETAIL_C3DCLOTHPRIMITIVE_VIRTUAL_DTOR
    // Retail opt-in (NF197, 2026-11-20, row 0x00A93A60 CCountedPointer<C3DClothPrimitive>::DeleteData): the vptr as the class's own --
    // vtable slot 0 is the scalar deleting destructor (the deleter's `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]`), as
    // FABLE_RETAIL_CPARTICLESIMULATIONRENDERDATA_VIRTUAL_DTOR. Same layout. Not combinable with FABLE_RETAIL_C3DCLOTHPRIMITIVE_CTOR
    // (which declares the destructor non-virtual).
    virtual ~C3DClothPrimitive();
#ifdef FABLE_RETAIL_C3DPRIMITIVE2_LOADBINARY_PASS27
    // FableWin 032FE500 initializes the base, vtable and ParticleProgram only.
    C3DClothPrimitive() { }
    bool LoadBinary(class CMemoryDataInputStream&, class CMovableResourceMemoryPool*); // 00AC3BD0
#endif
#else
    void* _vftable;
#endif
    CCountedPointer<CParticleProgram> ParticleProgram;                              // +0x04
    unsigned long PrimitiveIndex;                                                   // +0x0C
    unsigned long MaterialIndex;                                                    // +0x10
};
FABLE_STATIC_ASSERT_AT(20, sizeof(C3DClothPrimitive) == 0x14);
#endif
