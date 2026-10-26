// std::__unguarded_linear_insert(NGraphicAppearance::CMeshInfo* last, NGraphicAppearance::CMeshInfo value, CSortPrimaryMeshesLast comp) -- STLport insertion step: shift elements up while comp(value, *next) holds, then drop value into the hole
// Leaf: __unguarded_linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C1F10, length 68 bytes, `ret 10h` (__fastcall /Gr algorithm TU: last in
// ecx, the 12-byte value and the 4-byte comparator on the stack).
// Callers: the inlined __linear_insert of __insertion_sort<CMeshInfo*, CSortPrimaryMeshesLast> (0x004C4B23, 0x004C5201; the landed 0x004C5190 names this callee).
// Label: the catalog "Insertion_MoveBackward_AndInsert_Inverted" is behavioural.
// Element (Ego_r.pdb): NGraphicAppearance::CMeshInfo { long Index; float SizeX; uchar PrimaryMesh:1; } -- 12-byte POD.
// Comparator: CSortPrimaryMeshesLast reads only its left operand (`!a.PrimaryMesh`: `not bl ; and bl,1`, as the landed __insertion_sort 0x004C5190),
// so the loop condition is invariant: VC7.1 evaluates it once (the bit, spilled to a dead frame byte) and the
// "true" arm becomes an unconditional backward copy loop (no exit, so esi is used without being saved); the
// "false" arm stores value at last.
struct CMeshInfo {                        // NGraphicAppearance::CMeshInfo (Ego_r.pdb, 12 bytes, POD)
    long Index;                           // +0x00
    float SizeX;                          // +0x04
    unsigned char PrimaryMesh : 1;        // +0x08 bit 0
};

class CSortPrimaryMeshesLast { public:          // 4-byte comparator slot passed by value
    void* m_context;
    bool operator()(const CMeshInfo& a, const CMeshInfo& b) const { return !a.PrimaryMesh; }
};

void __fastcall __unguarded_linear_insert(CMeshInfo* last, CMeshInfo value, CSortPrimaryMeshesLast comp)
{
    CMeshInfo* next = last;
    --next;
    while (comp(value, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}