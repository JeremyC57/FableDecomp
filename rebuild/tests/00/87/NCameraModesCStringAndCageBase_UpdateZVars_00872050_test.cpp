// Behaviour test for NCameraModes::CStringAndCageBase::UpdateZVars (0x00872050): the height offset eases toward (clamped target - z), rate 10 when dropping, 3 otherwise.
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CThing {
public:
    const C3DVector& GetPosition() const;                  // 0x004C73D0
};
class CThingGameObject : public CThing { };
class CGameCameraManager { public:
    unsigned char _pad000[0xf8];
    float         MinWorldHeight;                          // +0xf8
    float         MaxWorldHeight;                          // +0xfc
    unsigned char _pad100[0x152 - 0x100];
    bool          LimitMinMaxWorldHeight;                  // +0x152
};
namespace NCameraModes {
class CTools { unsigned char _body[0x24]; };          // CGameCameraManager::CamTools (+0x78)
static inline void Clamp(float& value, float lo, float hi)
{
    if (value < lo)
        value = lo;
    if (value > hi)
        value = hi;
}
class CBase {
public:
    virtual void v00();
    float               DefaultFOV;                        // +0x04
    unsigned char       _pad08[0x40 - 0x08];
    CGameCameraManager* CameraManager;                     // +0x40
    unsigned char       _pad44[0xa0 - 0x44];
    bool                ViewOccluded;                      // +0xa0
    const CThingGameObject* GetObservedThing() const;      // 0x0084F1B0
    const CTools& GetCamTools() const;                     // 0x0084D080
};
class CStringAndCageBase : public CBase {
public:
    float DesiredHeightOffset;                             // +0xa4
    float DesiredStringLength;                             // +0xa8
    float DesiredCageRadius;                               // +0xac
    float CurrentHeightOffset;                             // +0xb0
    float CurrentStringLength;                             // +0xb4
    float CurrentCageRadius;                               // +0xb8
    void UpdateZVars();
};
void CStringAndCageBase::UpdateZVars()
{
    float z = GetObservedThing()->GetPosition().Z;
    float target = z + DesiredHeightOffset;
    if (CameraManager->LimitMinMaxWorldHeight && !ViewOccluded) {
        float minHeight = CameraManager->MinWorldHeight;
        float maxHeight = CameraManager->MaxWorldHeight;
        Clamp(target, minHeight, maxHeight);
    }
    float offset = target - z;
    float rate = offset < CurrentHeightOffset ? 10.0f : 3.0f;
    float height = (CurrentHeightOffset * rate + offset) / (rate + 1.0f);
    GetCamTools();
    CurrentHeightOffset = height;
}
}
static C3DVector g_pos; static int g_toolsCalls = 0;
const C3DVector& CThing::GetPosition() const { return g_pos; }
static CThingGameObject g_thing;
namespace NCameraModes {
const CThingGameObject* CBase::GetObservedThing() const { return &g_thing; }
static CTools g_tools;
const CTools& CBase::GetCamTools() const { ++g_toolsCalls; return g_tools; }
}
static bool Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }
static float Run(float z, float desired, float current, bool limit, bool occluded, float lo, float hi)
{
    static CGameCameraManager mgr; static unsigned char mem[sizeof(NCameraModes::CStringAndCageBase)];
    NCameraModes::CStringAndCageBase* c = (NCameraModes::CStringAndCageBase*)mem;
    mgr.MinWorldHeight = lo; mgr.MaxWorldHeight = hi; mgr.LimitMinMaxWorldHeight = limit;
    c->CameraManager = &mgr; c->ViewOccluded = occluded; c->DesiredHeightOffset = desired; c->CurrentHeightOffset = current;
    g_pos.X = 1.0f; g_pos.Y = 2.0f; g_pos.Z = z;
    c->UpdateZVars();
    return c->CurrentHeightOffset;
}
int main()
{
    bool ok = Near(Run(10.0f, 2.0f, 0.0f, false, false, 0.0f, 0.0f), 0.5f)        /* offset 2, rate 3: 2/4 */
           && Near(Run(10.0f, 2.0f, 0.0f, true, false, 0.0f, 11.0f), 0.25f)      /* clamped to 11: offset 1 */
           && Near(Run(10.0f, 2.0f, 0.0f, true, true, 0.0f, 11.0f), 0.5f)        /* occluded: no clamp */
           && Near(Run(10.0f, -20.0f, 0.0f, true, false, 5.0f, 50.0f), -5.0f / 11.0f)   /* clamped up to 5, dropping: rate 10 */
           && Near(Run(10.0f, 2.0f, 4.0f, false, false, 0.0f, 0.0f), 42.0f / 11.0f)     /* dropping: (4*10+2)/11 */
           && g_toolsCalls == 5;
    if (!ok) { std::printf("00872050_TEST FAIL\n"); return 1; }
    std::printf("00872050_TEST PASS\n");
    return 0;
}