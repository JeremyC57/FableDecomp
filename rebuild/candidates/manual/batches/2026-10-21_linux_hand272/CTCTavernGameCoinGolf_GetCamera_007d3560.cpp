// CTCTavernGameCoinGolf::GetCamera(C3DVector& camPos, C3DVector& lookAt) -- overview camera orbiting the table by the Z angle; once the coin is live, a chase camera behind and above the coin, pitched and turned by the player's angles
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D3560, 416 bytes (+ int3 padding), `ret 8`.
// Callees: C3DVector rotate-about-Z 0x00A14040 (landed spelling, 0x007A9960) and the two-angle rotate 0x00A14260;
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 (inline IS_THING_ALIVE + the position fetch); CThing::GetPosition 0x004C73D0.
// Constants: 0.05f/1.75f immediates, 0.1f 0x0122EE58, 0.9f 0x0126F3C8, 0.25f 0x0123078C, 0.2f 0x012305A0, 0.025f 0x0126F42C.
// Layout: ZAngle +0x148 / XAngle +0x14c (PDB +0x160/+0x164: retail CTCTavernGame base is 0x18 smaller); +0x108 coin record
//   pointer whose thing pointer sits at the odd offset +0x25 (pack(1)); +0x10c table centre (descriptive names).
// Shape: C3DVector with a user copy constructor (memberwise register copies); the offset is added in place then copied
//   out; the coin camera is a temporary `C3DVector(0, y, 0)` assigned to camPos before the two rotations.
struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    void operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; }
    void RotateAboutZ(float angle);                 // 0x00A14040
    void Rotate(float zAngle, float xAngle);        // 0x00A14260
};
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;           // 0x004C73D0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +4
    CThing* GetPItem() const;                       // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
#pragma pack(push, 1)
struct CCoinGolfCoin {                              // descriptive: the coin record, its thing pointer at the odd offset +0x25
    unsigned char _pad00[0x25];
    CBaseIntelligentPointer Thing;                  // +0x25
};
#pragma pack(pop)
struct CTCTavernGameCoinGolf {
    virtual void v00();
    unsigned char _pad04[0x108 - 0x04];
    CCoinGolfCoin* PCoin;                           // +0x108
    C3DVector TableCentre;                          // +0x10c (descriptive)
    unsigned char _pad118[0x148 - 0x118];
    float ZAngle;                                   // +0x148 (PDB +0x160)
    float XAngle;                                   // +0x14c (PDB +0x164)
    void GetCamera(C3DVector& camPos, C3DVector& lookAt);
};

void CTCTavernGameCoinGolf::GetCamera(C3DVector& camPos, C3DVector& lookAt)
{
    C3DVector centre(TableCentre);
    C3DVector offset(0.0f, 0.05f, 1.75f);
    offset.RotateAboutZ(ZAngle);
    offset += TableCentre;
    camPos = offset;
    lookAt = centre;
    if (PCoin && IS_THING_ALIVE(PCoin->Thing)) {
        lookAt = PCoin->Thing.GetPItem()->GetPosition();
        lookAt.Z += 0.1f;
        camPos = C3DVector(0.0f, XAngle * 0.9f + 0.25f, 0.0f);
        camPos.Rotate(0.0f, XAngle * 0.2f + 0.025f);
        camPos.Rotate(ZAngle, 0.0f);
        camPos += lookAt;
    }
}
