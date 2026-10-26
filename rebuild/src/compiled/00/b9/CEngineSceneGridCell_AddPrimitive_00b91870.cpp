// CEngineSceneGridCell::AddPrimitive(CEngineInternalPrimitiveBase* primitive) -- point the primitive at this cell (+0x20), link it into the cell's list at +4, bump the count at +0x34, and when any of the low three dirty bits at +0x44 is set clear them and invalidate the owning grid at +0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B91870, length 51 bytes.
// Callees: CEngineInternalPrimitiveBase::AddToList @ 0x00B8FDF0 (on the primitive; the list head address), CEngineSceneGridCell::InvalidateAll @ 0x00B91710 (on the owner
// at +0). Shape: the count increment and the flag byte test are interleaved (`mov edx ; mov al ; inc edx ; test al,7 ; mov [count],edx ; je`), the owner null test
// sits between `and al,0F8h` and the byte store.
struct CSceneGridOwner_00b91710 {
    void InvalidateAll();                          // 0x00B91710
};

class CEngineSceneGridCell;

class CEngineInternalPrimitiveBase { public:
    unsigned char          _pad[0x20];
    CEngineSceneGridCell*  m_pCell;                // +0x20
    void AddToList(void* list);                    // 0x00B8FDF0
};

class CEngineSceneGridCell { public:
    CSceneGridOwner_00b91710* m_pOwner;            // +0x00
    unsigned char             m_list[0x30];        // +0x04 .. +0x34 (the primitive list head)
    long                      m_count;             // +0x34
    unsigned char             _pad38[0xc];
    unsigned char             m_dirtyFlags;        // +0x44
    void AddPrimitive(CEngineInternalPrimitiveBase* primitive);
};

void CEngineSceneGridCell::AddPrimitive(CEngineInternalPrimitiveBase* primitive)
{
    primitive->m_pCell = this;
    primitive->AddToList(m_list);
    ++m_count;
    if (m_dirtyFlags & 7) {
        m_dirtyFlags &= ~7;
        if (m_pOwner)
            m_pOwner->InvalidateAll();
    }
}