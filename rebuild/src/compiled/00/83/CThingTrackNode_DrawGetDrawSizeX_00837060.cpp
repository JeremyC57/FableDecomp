// CThingTrackNode::DrawGetDrawSizeX(float, float*) -- writes the node's draw size along x (the node's scale at +0x9c times the size field of the global mesh bank's draw record at +0xe6) to the out-parameter; the float argument is unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00837060, length 35 bytes (`ret 8`: a float and the out pointer).
// Callees: the global mesh-bank getter @ 0x004C79A0 (manifest "CAnimateThing::GetMeshBank"; it ignores ecx
// and reads [0x013B8A1C]->+0x20->+0xd0, so it is modelled as a free function).
// Shape: `this` is kept in esi across the getter call; the dead `add eax,0E6h` next to `fmul [eax+0EEh]`
// is a named POINTER local to the packed draw record at +0xe6 (the direct `Get()->m_size.m_x` form folds
// the offset and drops the add); `size->m_x * m_scale` loads the scale first (`fld [a] ; fmul [b]` = b * a).
// The record is byte-packed in retail (+0xe6 is not dword aligned): `#pragma pack(1)` describes it.
#pragma pack(push, 1)
struct CMeshInfoSize {                             // the draw-size record at +0xe6 of the mesh bank
    long  m_a;                                     // +0x00 (read alone by the sibling getter 0x00837050)
    long  m_b;                                     // +0x04
    float m_x;                                     // +0x08
};
struct CMeshDataBank {
    unsigned char _pad[0xe6];
    CMeshInfoSize m_size;                          // +0xe6
};
#pragma pack(pop)

CMeshDataBank* GetMeshBank();                      // 0x004C79A0

struct CThingTrackNode {
    void*         m_vtable;                        // +0x00
    unsigned char _pad04[0x98];                    // +0x04
    float         m_scale;                         // +0x9c
    void DrawGetDrawSizeX(float unused, float* out);
};

void CThingTrackNode::DrawGetDrawSizeX(float unused, float* out)
{
    CMeshInfoSize* size = &GetMeshBank()->m_size;
    *out = size->m_x * m_scale;
}