// CHandleOwner_00c62460::PlaceAt(SContext* context) -- when the handle at +8 is not negative, add this owner's offset (+0xc) to the context target's position (+0x14 of the object at +0; C3DVector operator+ @ 0x00C257E0, `__fastcall` with the hidden result in ecx) and hand the handle and the position to the context's table (0x00C62440, a member getter) through virtual slot 27
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C62460, length 96 bytes.
// The chunk label "CItem::GetItemThings" is propagated; named by its effect. Shape: a 0x18-byte frame (the sum temporary at +8, the named position at +0x14), `ret 4`.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }   // a constructor: the `__fastcall` hidden result pointer travels in ecx
};
C3DVector __fastcall Add_00c257e0(const C3DVector& a, const C3DVector& b);   // 0x00C257E0 (labelled "CQuadric::operator+=")
struct CHandleTable_00c62460 {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026();
    virtual void Place(long handle, const C3DVector* position);   // slot 27 (+0x6c)
};
struct STarget_00c62460 {
    unsigned char _pad[0x14];
    C3DVector     m_position;                      // +0x14
};
struct SContext_00c62460 {
    STarget_00c62460* m_pTarget;                   // +0x00
};
struct CHandleOwner_00c62460 {
    unsigned char _pad[8];
    long          m_handle;                        // +0x08
    C3DVector     m_offset;                        // +0x0c
    CHandleTable_00c62460* GetHandleTable(SContext_00c62460* context);   // 0x00C62440 (labelled "CRegion::SetMiniMapRegionExitTextOffsetY")
    void PlaceAt(SContext_00c62460* context);
};

void CHandleOwner_00c62460::PlaceAt(SContext_00c62460* context)
{
    if (m_handle >= 0) {
        C3DVector sum = Add_00c257e0(m_offset, context->m_pTarget->m_position);
        C3DVector position(sum);                   // the sum then its copy (both on the frame)
        GetHandleTable(context)->Place(m_handle, &position);
    }
}