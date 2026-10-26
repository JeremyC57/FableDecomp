// CThingTrackNode::GetDrawRecordA() -- returns the first word of the global mesh bank's packed draw record at +0xe6
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00837050, length 12 bytes. No callers and no vtable slot (it follows
// DrawIsDrawable @ 0x00837040 in the same catalogue row, split via the skipped-row ledger, chunk 905); the
// sibling DrawGetDrawSizeX @ 0x00837060 reads the float at +0xee of the same record, so the record is modelled
// exactly as there. The manifest label "CThingTrackNode::Piece_00837050" is the ledger placeholder.
// Callees: the global mesh-bank getter @ 0x004C79A0 (manifest "CAnimateThing::GetMeshBank"; it ignores ecx
// and reads [0x013B8A1C]->+0x20->+0xd0, so it is modelled as a free function).
// Shape: `call ; mov eax,[eax+0E6h] ; ret` -- a dword read at an unaligned offset, hence `#pragma pack(1)`.
#pragma pack(push, 1)
struct CMeshInfoSize {                             // the draw-size record at +0xe6 of the mesh bank
    long  m_a;                                     // +0x00
    long  m_b;                                     // +0x04
    float m_x;                                     // +0x08 (read by 0x00837060)
};
class CMeshDataBank { public:
    unsigned char _pad[0xe6];
    CMeshInfoSize m_size;                          // +0xe6
};
#pragma pack(pop)

CMeshDataBank* GetMeshBank();                      // 0x004C79A0

class CThingTrackNode { public:
    long GetDrawRecordA();
};

long CThingTrackNode::GetDrawRecordA()
{
    return GetMeshBank()->m_size.m_a;
}