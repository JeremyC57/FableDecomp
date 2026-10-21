// Behaviour test for CTCTavernGameCoinGolf::GetCamera (0x007d3560): overview camera vs coin chase camera.
#include <cstdio>
#include <cmath>
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

static int g_rotZ = 0, g_rot2 = 0;
static float g_lastA = 0, g_lastB = 0;
void C3DVector::RotateAboutZ(float angle) { ++g_rotZ; float c = std::cos(angle), s = std::sin(angle); float x = X * c - Y * s; Y = X * s + Y * c; X = x; }
void C3DVector::Rotate(float zAngle, float xAngle) { ++g_rot2; g_lastA = zAngle; g_lastB = xAngle; if (zAngle != 0.0f) RotateAboutZ(zAngle); --g_rotZ; }
static C3DVector g_coinPos(5.0f, 6.0f, 7.0f);
const C3DVector& CThing::GetPosition() const { return g_coinPos; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::v00() { }
void CTCTavernGameCoinGolf::v00() { }
static int fails = 0;
static void check(bool c, const char* what) { if (!c) { std::printf("FAIL %s\n", what); ++fails; } }
static bool near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }
int main()
{
    CTCTavernGameCoinGolf g;
    g.PCoin = 0; g.TableCentre = C3DVector(1.0f, 2.0f, 3.0f); g.ZAngle = 0.0f; g.XAngle = 1.0f;
    C3DVector cam(0, 0, 0), look(0, 0, 0);
    g.GetCamera(cam, look);
    check(near(cam.X, 1.0f) && near(cam.Y, 2.05f) && near(cam.Z, 4.75f), "overview cam");
    check(look.X == 1.0f && look.Y == 2.0f && look.Z == 3.0f && g_rot2 == 0, "overview look");
    static CCoinGolfCoin coin; CThing thing; thing.m_flags91 = 0;
    coin.Thing.m_pItem = &thing; g.PCoin = &coin;
    g.GetCamera(cam, look);
    check(near(look.Z, 7.1f) && look.X == 5.0f, "coin look");
    check(g_rot2 == 2 && g_lastA == 0.0f && g_lastB == 0.0f, "two rotations");
    check(near(cam.X, 5.0f) && near(cam.Y, 6.0f + 1.15f) && near(cam.Z, 7.1f), "chase cam");
    thing.m_flags91 = 1; g_rot2 = 0;
    g.GetCamera(cam, look);
    check(g_rot2 == 0 && look.X == 1.0f, "dead coin -> overview");
    if (fails == 0) std::printf("007d3560_TEST PASS\n");
    return fails;
}