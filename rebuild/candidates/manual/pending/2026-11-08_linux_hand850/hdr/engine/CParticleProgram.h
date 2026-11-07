#ifdef FABLE_RETAIL_CPARTICLEPROGRAM
#ifndef FABLE_ENGINE_CPARTICLEPROGRAM_H
#define FABLE_ENGINE_CPARTICLEPROGRAM_H
// Retail branch (pass 12 header wave; rows 0x00AE0070 ApplyTransform, 0x00AE0710 CalculateAveragePatchSize, 0x00AE2E20
// OptimiseSpringOrder): the polymorphic retail class, RTTI .?AVCParticleProgram@@ : CMovableResource. Members are the Ego_r TPI
// names (struct_layouts_egor.tsv) at retail offsets (12-byte STLport CArray, 0x10 CVectorMap): InitialSimulation +0x08,
// ExportParticles +0x88, RenderTris +0x98, RenderQuads +0xA4, RenderVertices +0xB0, NonSimCount +0xBC, NonSimPositions +0xC0,
// IndexedTextureCoords +0xC4, TempVertexPositions +0xD0, ParticleIndices +0xDC, VertexIndices +0xE8, BezierEnable +0xF4,
// AveragePatchSize +0xF8, SimGroups +0xFC, NonSimGroups +0x108, Constraints +0x114 (CMemoryDataOutputStream, 0x18),
// LoopStack +0x12C, MemoryHandle +0x138, MemoryPool +0x13C; sizeof 0x140 (the generated default below agrees).
// 0x00AE0070 reads Constraints.PeekData / GetLength and InitialSimulation; 0x00AE0710 RenderTris / RenderQuads (strides 6 / 8) and
// stores AveragePatchSize +0xF8. Methods: Ego_r / FableWin ?ApplyTransform@CParticleProgram@@QAEXABVCMatrix3x4@@@Z,
// ?CalculateAveragePatchSize@CParticleProgram@@AAEXABVCParticleSimulation@@@Z (private),
// ?OptimiseSpringOrder@CParticleProgram@@QBEXAAV?$CArray@VCParticleDistanceConstraint@@@@@Z, ?PeekInitialSimulation@...QBE,
// ?GetInitialSimulation@...QAE (inline in retail).
#include "rebuild_abi.h"
#ifndef FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMOVABLERESOURCE
#endif
#ifndef FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#endif
#ifndef FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM
#endif
#include "engine/CParticleSimulation.h"
#include "engine/CMemoryDataOutputStream.h"
#include "engine/CArray.h"
#include "engine/C3DTriangle2.h"
#include "engine/C3DQuad2.h"
class CMatrix3x4;
class CMatrix4x4;
class C3DGroup2;
class C3DAnimationBlendState;
class C3DAnimationManager;
class C3DMesh2;
class CParticleContext;
class CParticleDistanceConstraint;
#ifdef FABLE_RETAIL_CPARTICLEPROGRAM_READ
// Nested opt-in (chunk 850, row 0x00AE3C30 CParticleProgram::ReadFromFile): the typed members the reader fills (Ego_r TPI types at the
// retail offsets above: ExportParticles CVectorMap<CCharString,unsigned long> 0x10, RenderVertices CArray<C3DClothRenderVertex> (4-byte
// elements, `sar 2`), IndexedTextureCoords CArray<C2DVector> (`sar 3`), ParticleIndices / VertexIndices CArray<long>, SimGroups /
// NonSimGroups CArray<C3DGroup2>), and the private helpers it calls: Ego_r.pdb ?AllocateArrays@CParticleProgram@@AAE_NKAAVCMovable
// ResourceMemoryPool@@@Z (0x00ADF460: stores MemoryPool +0x13C, allocates NonSimPositions +0xC0 through MemoryHandle +0x138) and
// ?ReadGroups@CParticleProgram@@CIXAAVCDataInputStream@@AAV?$CArray@VC3DGroup2@@@@@Z (0x00AE3A60, landed). Needs FABLE_RETAIL_CCHARSTRING.
#include "engine/CVectorMap.h"
#include "engine/C2DVector.h"
#include "fable_string.h"
class CDataInputStream;
class C3DGroup2;
class C3DClothRenderVertex {
public:
    C3DClothRenderVertex() { }                               // Ego_r.pdb ??0C3DClothRenderVertex@@QAE@XZ
    unsigned short PositionIndex;                            // +0x00
    unsigned short TexCoordIndex;                            // +0x02
};
#endif
class CParticleProgram : public CMovableResource {
public:
    CParticleProgram();
    virtual ~CParticleProgram();
    virtual void RelocateData(long,void*,void*,long);
    const CParticleSimulation& PeekInitialSimulation() const { return InitialSimulation; }
    CParticleSimulation& GetInitialSimulation() { return InitialSimulation; }
    void ApplyTransform(const CMatrix3x4&);
    void OptimiseSpringOrder(CArray<CParticleDistanceConstraint>&) const;
    // Chunk 748 (FableWin.pdb spellings; the skinned set-up / cling rows 0x00AE29D0 / 0x00AE1320 and their callees):
    bool SetupSimulation(CParticleSimulation& simulation, const C3DAnimationBlendState& blend_state, C3DAnimationManager& animation_manager,
                         const C3DVector* position, const C3DMesh2& mesh, CMovableResourceMemoryPool& pool, const CParticleContext& context) const;
private:
    static CMatrix4x4* __fastcall GetMatrixBuffer(unsigned long count);   // 0x00AE04E0 ?GetMatrixBuffer@CParticleProgram@@CIPAVCMatrix4x4@@K@Z
    static C4DVector* __fastcall GetVectorBuffer(unsigned long count);    // 0x00AE0540 ?GetVectorBuffer@CParticleProgram@@CIPAVC4DVector@@K@Z
    void TransformVertices(C4DVector* out, const C4DVector* in, unsigned long count, const CMatrix4x4* matrices,
                           const CArray<C3DGroup2>& groups) const;        // 0x00AE0E60
    void ApplyConstraints(CParticleSimulation& simulation, long iterations, float time_delta) const;   // 0x00AE2510
    void ClingToMesh(CParticleSimulation& simulation, const C3DAnimationBlendState& blend_state, C3DAnimationManager& animation_manager,
                     const C3DVector* position, const C3DMesh2& mesh) const;   // 0x00AE1320
    void CalculateAveragePatchSize(const CParticleSimulation&);
#ifdef FABLE_RETAIL_CPARTICLEPROGRAM_READ
    bool AllocateArrays(unsigned long count, CMovableResourceMemoryPool& pool);                 // 0x00ADF460
    static void __fastcall ReadGroups(CDataInputStream& stream, CArray<C3DGroup2>& groups);   // 0x00AE3A60
public:
    bool ReadFromFile(CDataInputStream& in, CMovableResourceMemoryPool& pool);                 // 0x00AE3C30
private:
#endif
    CParticleSimulation InitialSimulation;                   // +0x08
#ifdef FABLE_RETAIL_CPARTICLEPROGRAM_READ
    CVectorMap<CCharString,unsigned long> ExportParticles;   // +0x88
#else
    unsigned char ExportParticles[16];                       // +0x88 CVectorMap<CCharString,unsigned long,CKeyPairCompareLess<CCharString,unsigned long> >
#endif
    CArray<C3DTriangle2> RenderTris;                         // +0x98
    CArray<C3DQuad2> RenderQuads;                            // +0xA4
#ifdef FABLE_RETAIL_CPARTICLEPROGRAM_READ
    CArray<C3DClothRenderVertex> RenderVertices;             // +0xB0
#else
    unsigned char RenderVertices[12];                        // +0xB0 CArray<C3DClothRenderVertex>
#endif
    unsigned long NonSimCount;                               // +0xBC
    C4DVector* NonSimPositions;                              // +0xC0
#ifdef FABLE_RETAIL_CPARTICLEPROGRAM_READ
    CArray<C2DVector> IndexedTextureCoords;                  // +0xC4
    unsigned char TempVertexPositions[12];                   // +0xD0 CArray<C4DVector>
    CArray<long> ParticleIndices;                            // +0xDC
    CArray<long> VertexIndices;                              // +0xE8
#else
    unsigned char IndexedTextureCoords[12];                  // +0xC4 CArray<C2DVector>
    unsigned char TempVertexPositions[12];                   // +0xD0 CArray<C4DVector>
    unsigned char ParticleIndices[12];                       // +0xDC CArray<long>
    unsigned char VertexIndices[12];                         // +0xE8 CArray<long>
#endif
    bool BezierEnable;                                       // +0xF4
    float AveragePatchSize;                                  // +0xF8
#ifdef FABLE_RETAIL_CPARTICLEPROGRAM_READ
    CArray<C3DGroup2> SimGroups;                             // +0xFC
    const CArray<C3DGroup2>& PeekSimGroups() const { return SimGroups; }
    CArray<C3DGroup2> NonSimGroups;                          // +0x108
#else
    unsigned char SimGroups[12];                             // +0xFC CArray<C3DGroup2>
    const CArray<C3DGroup2>& PeekSimGroups() const { return *reinterpret_cast<const CArray<C3DGroup2>*>(SimGroups); }   // chunk 748
    unsigned char NonSimGroups[12];                          // +0x108 CArray<C3DGroup2>
#endif
    CMemoryDataOutputStream Constraints;                     // +0x114
    unsigned char LoopStack[12];                             // +0x12C CArray<CParticleProgram::CLoopBegin>
    long MemoryHandle;                                       // +0x138
    CMovableResourceMemoryPool* MemoryPool;                  // +0x13C
};
FABLE_STATIC_ASSERT(sizeof(CParticleProgram)==0x140);
#endif // FABLE_ENGINE_CPARTICLEPROGRAM_H
#else // generated flattened default (unchanged)
#ifndef FABLE_ENGINE_CPARTICLEPROGRAM_H
#define FABLE_ENGINE_CPARTICLEPROGRAM_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CParticleProgram = 0x170.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CParticleProgram = 0x140.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class C4DVector;
class CMovableResourceMemoryPool;

#pragma pack(push, 1)
class CParticleProgram {
public:
    void*                       __vftable;                   // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char               _pad_0x04[0x4];              // +0x04
    unsigned char               InitialSimulation[0x80];     // +0x08 CParticleSimulation
    unsigned char               ExportParticles[0x10];       // +0x88 CVectorMap<CCharString,unsigned_long,CKeyPairCompareLess<CCharString,unsigned_long>_>
    unsigned char               RenderTris[0xc];             // +0x98 CArray<C3DTriangle2>
    unsigned char               RenderQuads[0xc];            // +0xa4 CArray<C3DQuad2>
    unsigned char               RenderVertices[0xc];         // +0xb0 CArray<C3DClothRenderVertex>
    unsigned long               NonSimCount;                 // +0xbc
    C4DVector*                  NonSimPositions;             // +0xc0
    unsigned char               IndexedTextureCoords[0xc];   // +0xc4 CArray<C2DVector>
    unsigned char               TempVertexPositions[0xc];    // +0xd0 CArray<C4DVector>
    unsigned char               ParticleIndices[0xc];        // +0xdc CArray<long>
    unsigned char               VertexIndices[0xc];          // +0xe8 CArray<long>
    bool                        BezierEnable;                // +0xf4
    unsigned char               _pad_0xf5[0x3];              // +0xf5
    float                       AveragePatchSize;            // +0xf8
    unsigned char               SimGroups[0xc];              // +0xfc CArray<C3DGroup2>
    unsigned char               NonSimGroups[0xc];           // +0x108 CArray<C3DGroup2>
    unsigned char               Constraints[0x18];           // +0x114 CMemoryDataOutputStream
    unsigned char               LoopStack[0xc];              // +0x12c CArray<CParticleProgram::CLoopBegin>
    long                        MemoryHandle;                // +0x138
    CMovableResourceMemoryPool* MemoryPool;                  // +0x13c
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CParticleProgram) == 0x140);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, InitialSimulation) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, ExportParticles) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, RenderTris) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, RenderQuads) == 0xa4);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, RenderVertices) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, NonSimCount) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, NonSimPositions) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, IndexedTextureCoords) == 0xc4);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, TempVertexPositions) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, ParticleIndices) == 0xdc);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, VertexIndices) == 0xe8);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, BezierEnable) == 0xf4);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, AveragePatchSize) == 0xf8);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, SimGroups) == 0xfc);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, NonSimGroups) == 0x108);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, Constraints) == 0x114);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, LoopStack) == 0x12c);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, MemoryHandle) == 0x138);
FABLE_STATIC_ASSERT(offsetof(CParticleProgram, MemoryPool) == 0x13c);

#endif // FABLE_ENGINE_CPARTICLEPROGRAM_H
#endif // FABLE_RETAIL_CPARTICLEPROGRAM
