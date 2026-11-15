// CParticleSimulation::ReadFromFile(CDataInputStream& is, CMovableResourceMemoryPool& alloc) -- read the timestep settings and the particle count, allocate the arrays from the alloc (fail if that fails), read the positions and copy them into the last and saved positions, read the per-particle alphas and the gravity / wind / dragging / acceleration / damping settings, and mark the bounding box invalid
// Grade: relocation-masked byte match; original Read outer scope restored and verified.
// Leaf: ?ReadFromFile@CParticleSimulation@@QAE_NAAVCDataInputStream@@AAVCMovableResourceMemoryPool@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE1BC0, 1158 bytes (+ padding), `ret 8`.
// Callees: the inlined CDataInputStream::Read (64-bit position range check, memcpy fast path, else ReadWithSrcChunkOverflow
//   0x00993CA0), CParticleSimulation::AllocateArrays 0x00ADF2A0, CParticleSimulation::ReadVectorArray 0x00AE1730 (__fastcall),
//   std::copy<C4DVector*,C4DVector*> 0x00AE61E0 (__fastcall) twice. All landed.
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
#define FABLE_RETAIL_CPARTICLESIMULATION_READFROMFILE
#include "engine/CDataInputStream.h"
#include "engine/CParticleSimulation.h"
namespace std {
template <class _InputIter, class _OutputIter>
_OutputIter FABLE_FASTCALL copy(_InputIter __first, _InputIter __last, _OutputIter __result);   // 0x00AE61E0 <C4DVector*>
}

bool CParticleSimulation::ReadFromFile(CDataInputStream& is, CMovableResourceMemoryPool& alloc)
{
    Timestep = is.ReadFloat();
    TimestepChanged = is.ReadULONG() != 0;
    TimestepMultiplier = is.ReadFloat();
    Size = is.ReadULONG();
    if (!AllocateArrays(alloc))
        return false;
    ReadVectorArray(Positions, Size, is, 0.0f);
    std::copy(Positions, Positions + Size, LastPositions);
    std::copy(Positions, Positions + Size, SavedPositions);
    is.Read(SimulationAlphas, Size * sizeof(float));
    GravityStrength = is.ReadFloat();
    WindStrength = is.ReadFloat();
    DraggingEnable = is.ReadEBOOL();
    DraggingRotational = is.ReadEBOOL();
    DraggingStrength = is.ReadFloat();
    AccelerationEnable = is.ReadEBOOL();
    GlobalDamping = is.ReadFloat();
    BoundingBoxValid = false;
    return true;
}