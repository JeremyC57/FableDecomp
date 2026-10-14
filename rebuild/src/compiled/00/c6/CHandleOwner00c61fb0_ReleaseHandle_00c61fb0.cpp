// CHandleOwner_00c61fb0::ReleaseHandle(long context) -- when the handle at +0xc is not negative, release it through the context's object (0x00C61CF0, `__stdcall`) virtual slot 22 and set the handle to -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C61FB0, length 41 bytes.
// The chunk label "CItem::GetItemThings" is propagated; named by its effect. Shape: `test edi,edi ; jl`, `push eax ; call` (a `__stdcall` getter), `ret 4`.
struct CHandleTable_00c61fb0 {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021();
    virtual void Release(long handle);             // slot 22 (+0x58)
};
CHandleTable_00c61fb0* __stdcall GetHandleTable_00c61cf0(long context);   // 0x00C61CF0 (labelled "CRegion::SetMiniMapRegionExitTextOffsetY" / "CTools::GetWorld", propagated)
struct CHandleOwner_00c61fb0 {
    unsigned char _pad[0xc];
    long          m_handle;                        // +0xc
    void ReleaseHandle(long context);
};

void CHandleOwner_00c61fb0::ReleaseHandle(long context)
{
    long handle = m_handle;
    if (handle >= 0) {
        GetHandleTable_00c61cf0(context)->Release(handle);
        m_handle = -1;
    }
}