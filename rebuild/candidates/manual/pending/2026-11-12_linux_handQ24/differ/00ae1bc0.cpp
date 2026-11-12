// CParticleSimulation::ReadFromFile(CDataInputStream& in, CMovableResourceMemoryPool& pool) -- read the timestep settings and the particle count, allocate the arrays from the pool (fail if that fails), read the positions and copy them into the last and saved positions, read the per-particle alphas and the gravity / wind / dragging / acceleration / damping settings, and mark the bounding box invalid
// Grade: DIFFER(1158v1158) -- Q24 port onto the shared engine/CDataInputStream.h (FABLE_RETAIL_CDATAINPUTSTREAM: ReadFloat / ReadSLONG / ReadULONG / ReadEBOOL / Read) and engine/CParticleSimulation.h (overlay FABLE_RETAIL_CPARTICLESIMULATION_READFROMFILE), PDB signature (references, global class -- not NParticleEngine::). Same single residue as the Q-draft: the first 4-byte read after the alpha-array read (GravityStrength, 0x00AE1DD4, the join after the array read) loads StreamPos as `mov eax,[ebx+4]; lea ecx,[eax+4]` in retail, `mov ecx,[ebx+4]; add ecx,4` here; every other byte matches. Toy-tried (no grade): a named byte count, Read straight into the member, GetParticleCount(), sizeof(float)*Size, a gravity temp, `if (Size)` guard, a FABLE_FORCEINLINE ReadArray<T> helper, /Gr, /Ow, /G6 (all unchanged), /Oa (fixes the join but rewrites the frame).
// Leaf: ?ReadFromFile@CParticleSimulation@@QAE_NAAVCDataInputStream@@AAVCMovableResourceMemoryPool@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE1BC0, 1158 bytes (+ padding), `ret 8`.
// Callees: the inlined CDataInputStream::Read (64-bit position range check, memcpy fast path, else ReadWithSrcChunkOverflow
//   0x00993CA0), CParticleSimulation::AllocateArrays 0x00ADF2A0, CParticleSimulation::ReadVectorArray 0x00AE1730 (__fastcall),
//   std::copy<C4DVector*,C4DVector*> 0x00AE61E0 (__fastcall) twice. All landed.
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CPARTICLESIMULATION_READFROMFILE
#include "engine/CDataInputStream.h"
#include "engine/CParticleSimulation.h"
namespace std {
template <class _InputIter, class _OutputIter>
_OutputIter __fastcall copy(_InputIter __first, _InputIter __last, _OutputIter __result);   // 0x00AE61E0 <C4DVector*>
}

bool CParticleSimulation::ReadFromFile(CDataInputStream& in, CMovableResourceMemoryPool& pool)
{
    Timestep = in.ReadFloat();
    TimestepChanged = in.ReadSLONG() != 0;
    TimestepMultiplier = in.ReadFloat();
    Size = in.ReadULONG();
    if (!AllocateArrays(pool))
        return false;
    ReadVectorArray(Positions, Size, in, 0.0f);
    std::copy(Positions, Positions + Size, LastPositions);
    std::copy(Positions, Positions + Size, SavedPositions);
    in.Read(SimulationAlphas, Size * sizeof(float));
    GravityStrength = in.ReadFloat();
    WindStrength = in.ReadFloat();
    DraggingEnable = in.ReadEBOOL();
    DraggingRotational = in.ReadEBOOL();
    DraggingStrength = in.ReadFloat();
    AccelerationEnable = in.ReadEBOOL();
    GlobalDamping = in.ReadFloat();
    BoundingBoxValid = false;
    return true;
}
