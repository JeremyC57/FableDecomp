// CMapOwner_00b423b0::SetMapVisibility(long visibility) -- for every non-null engine map of the pointer vector at +0x20: SetMapVisibility(&the record at +0x2c, visibility) (0x00B41DB0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B423B0, length 64 bytes.
// Leaf: SetMapVisibility
// Callees: CEngineMap::SetMapVisibility @ 0x00B41DB0. The chunk label (CMeshDataBank::FreeDeviceSpecificResources) is propagated; named by address. Shape: `this` in esi, the index in edi, the argument hoisted into ebx, `ret 4`.
struct SMapRecord_00b423b0 { unsigned char m_body[4]; };
class CEngineMap { public:
    void SetMapVisibility(SMapRecord_00b423b0* record, long visibility);   // 0x00B41DB0
};
struct CRetailVector_00b423b0 {
    CEngineMap** m_first;
    CEngineMap** m_last;
    CEngineMap** m_end;
    unsigned long size() const { return m_last - m_first; }
};

struct CMapOwner_00b423b0 {
    unsigned char          _pad00[0x20];
    CRetailVector_00b423b0 m_maps;                 // +0x20
    SMapRecord_00b423b0    m_record;               // +0x2c
    void SetMapVisibility(long visibility);
};

void CMapOwner_00b423b0::SetMapVisibility(long visibility)
{
    for (unsigned long i = 0; i < m_maps.size(); ++i) {
        if (m_maps.m_first[i])
            m_maps.m_first[i]->SetMapVisibility(&m_record, visibility);
    }
}