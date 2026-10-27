// QuadricOptimiserInternals::COptimisedVertex::CGroup::CompareWeight(const CGroup& a, const CGroup& b) -- sort predicate: a's bone weight above b's (heaviest influence first)
// Leaf: ?CompareWeight@CGroup@COptimisedVertex@QuadricOptimiserInternals@@SI_NABV123@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC6B40, 22 bytes, static __fastcall (a in ecx, b in edx).
// Identity: Ego_r.pdb / FableWin.pdb `?CompareWeight@CGroup@COptimisedVertex@QuadricOptimiserInternals@@SI_NABV123@0@Z`, the
//   only `bool (__fastcall*)(const CGroup&, const CGroup&)` named in the sort/heap instantiations over CGroup; its address
//   is pushed as the comparator at 0x00ACA6C3 (the quadric optimiser's bone-weight normalisation, sort 0x00AD7550).
// Layout: Ego_r.pdb CGroup (8 bytes): BoneIndex +0, BoneWeight +4 (float).
namespace QuadricOptimiserInternals {

class COptimisedVertex {
public:
    class CGroup {
    public:
        unsigned long BoneIndex;                        // +0x00
        float         BoneWeight;                       // +0x04
        static bool __fastcall CompareWeight(const CGroup& a, const CGroup& b);
    };
};

bool __fastcall COptimisedVertex::CGroup::CompareWeight(const CGroup& a, const CGroup& b)
{
    return a.BoneWeight > b.BoneWeight;
}

}   // namespace QuadricOptimiserInternals