// CHitLocation::GetClosestPointToPos2D(const C2DVector& position, long flags) -- forward to the hit shape (+4 -> +0x3c) virtual slot 10 with this location's owner word (+0), the position and the flags; the C2DVector result comes back through the hidden return pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00815370, length 38 bytes.
// Shape: the hidden return pointer in esi copied straight through (`mov eax,esi`), `ret 0Ch`.
class C2DVector { public: float X, Y; };
struct CHitShape_00815370 {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual C2DVector GetClosestPointToPos2D(void* owner, const C2DVector& position, long flags);   // slot 10 (+0x28)
};
struct CHitShapeHolder_00815370 {
    unsigned char       _pad[0x3c];
    CHitShape_00815370* m_pShape;                  // +0x3c
};
struct CHitLocation {
    void*                      m_pOwner;           // +0x00
    CHitShapeHolder_00815370*  m_pHolder;          // +0x04
    C2DVector GetClosestPointToPos2D(const C2DVector& position, long flags);
};

C2DVector CHitLocation::GetClosestPointToPos2D(const C2DVector& position, long flags)
{
    return m_pHolder->m_pShape->GetClosestPointToPos2D(m_pOwner, position, flags);
}