// CHandleOwner_00c62500::RemoveHandle(SContext_00c62500* context) -- when the handle at +8 is not negative, remove it from the context's handle table (0x00C62440, a member getter) through virtual slot 22 and mark the handle -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C62500, length 48 bytes.
// Callees: GetHandleTable @ 0x00C62440, the table's slot 22. The chunk label "CItem::GetItemThings" is propagated (the sibling 0x00C62460 PlaceAt is landed); named by its effect.
// Shape: the handle is taken into a local (edi) before the test and pushed from the register after the call; `ret 4`.
struct C3DVector { float X, Y, Z; };
struct CHandleTable_00c62500 {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void Remove(long handle);               // slot 22 (+0x58)
};
struct SContext_00c62500 { void* m_pTarget; };
struct CHandleOwner_00c62500 {                     // the landed CHandleOwner_00c62460 (PlaceAt): the handle at +8, the offset at +0xc
    unsigned char _pad[8];
    long          m_handle;                        // +0x08
    C3DVector     m_offset;                        // +0x0c
    CHandleTable_00c62500* GetHandleTable(SContext_00c62500* context);   // 0x00C62440 (labelled "CRegion::SetMiniMapRegionExitTextOffsetY")
    void RemoveHandle(SContext_00c62500* context);
};

void CHandleOwner_00c62500::RemoveHandle(SContext_00c62500* context)
{
    long handle = m_handle;
    if (handle >= 0) {
        GetHandleTable(context)->Remove(handle);
        m_handle = -1;
    }
}
