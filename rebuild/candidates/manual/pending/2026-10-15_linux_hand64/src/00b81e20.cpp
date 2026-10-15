// CEnginePrimitiveSimulationInfo::ClearSimulations() -- empties the Simulations array (the VC6-era `erase(begin(), end())`: the empty tail moved onto the first element, every old element destroyed, the end pointer rewritten)
// Leaf: ClearSimulations
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B81E20, 50 bytes.
// Callees: `__fastcall _Move(first, last, dest, tag, bool)` @ 0x00B82EC0 and `__fastcall _Destroy_range(first, last,
// allocator tag)` @ 0x00B83580 for CCountedPointer<CEngineSimulationBase> (8-byte elements; both labelled with other
// instantiations' names).
// Label: "CParticleProgram::ClearGroups" is propagated. The only caller is ~CEngineInternalPrimitiveMeshBase
// (0x00BEBC50), which runs it on its SimulationInfo (CIVCountedPointer<CEnginePrimitiveSimulationInfo>, +0x64)
// before releasing it; CEnginePrimitiveSimulationInfo is vptr + intrusive count + CArray<CCountedPointer<
// CEngineSimulationBase>> Simulations at +8 (PDB), the 12-byte retail array. Leaf named by role.
// Shape: VC6 `erase(first, last)` with no empty-range test (GOTCHAS wave 24n); both empty tag temporaries live in
// the high byte of the `push ecx` frame slot; last is read before first (that puts first in eax and the tag's
// `lea` in edx, as agent 54's CLipSyncDataBank::Close 0x00A42DA0).
struct CEngineSimulationBase;
struct SCountBlock;

struct CSimulationPointer {                          // CCountedPointer<CEngineSimulationBase>, 8 bytes
    CEngineSimulationBase* Object;
    SCountBlock*           Block;
};

struct SMoveTag { SMoveTag() { } };
struct SAllocatorTag { SAllocatorTag() { } };

CSimulationPointer* __fastcall Move_00b82ec0(CSimulationPointer* first, CSimulationPointer* last, CSimulationPointer* dest,
                                             const SMoveTag& tag, bool overlap);                          // 0x00B82EC0
void __fastcall DestroyRange_00b83580(CSimulationPointer* first, CSimulationPointer* last, const SAllocatorTag& allocator);   // 0x00B83580

struct CEnginePrimitiveSimulationInfo {
    void*               m_vtable;                    // +0x00
    long                m_refCount;                  // +0x04 (CIVCountedPointer intrusive count)
    CSimulationPointer* m_simulationsFirst;          // +0x08 Simulations (CArray)
    CSimulationPointer* m_simulationsLast;           // +0x0c
    CSimulationPointer* m_simulationsEnd;            // +0x10
    void ClearSimulations();
};

void CEnginePrimitiveSimulationInfo::ClearSimulations()
{
    CSimulationPointer* last = m_simulationsLast;
    CSimulationPointer* first = m_simulationsFirst;
    CSimulationPointer* kept = Move_00b82ec0(last, last, first, SMoveTag(), false);
    DestroyRange_00b83580(kept, m_simulationsLast, SAllocatorTag());
    m_simulationsLast = kept;
}
