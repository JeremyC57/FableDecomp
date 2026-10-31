// CPhysicsMesh::GetSingleInstanceCollisionSet() const -- empty the static collision-set vector (finish = start) and return it
// Leaf: ?GetSingleInstanceCollisionSet@CPhysicsMesh@@ABEAAV?$vector@JV?$allocator@J@std@@@std@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6CBB0, length 16 bytes. Callers: CPhysicsMesh::
//   GetPointIntersectionFromToUsingSegmentation 0x00A177A0 (call at 0x00A177A9, ecx = this) and a sibling at 0x00A17AFC.
// Identity (naming wave 12, chunk 620): Ego_r.pdb + FableWin.pdb private const CPhysicsMesh::GetSingleInstanceCollisionSet
//   returning vector<long>&; the static is CPhysicsMesh::SingleInstanceCollisionSet @ 0x013D2D1C (both PDBs name its dynamic
//   initializer and atexit destructor). Was landed as a global AddChildPrimitive (propagated CEngineInternalPrimitiveBase label)
//   over the stand-in globals g_a / g_b.
// Shape: the STLport vector<long> clear() of a POD vector is `_M_finish = _M_start` (mov eax,[s] ; mov [s+4],eax ; mov eax,offset s).
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector { public:   // STLport 4.6 vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void clear() { _M_finish = _M_start; }
};
}
class CPhysicsMesh {
    static std::vector<long> SingleInstanceCollisionSet;   // 0x013D2D1C
    std::vector<long>& GetSingleInstanceCollisionSet() const;
};

std::vector<long>& CPhysicsMesh::GetSingleInstanceCollisionSet() const
{
    SingleInstanceCollisionSet.clear();
    return SingleInstanceCollisionSet;
}
