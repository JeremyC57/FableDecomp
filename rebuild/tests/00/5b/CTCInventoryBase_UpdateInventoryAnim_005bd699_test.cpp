// Behaviour test for CTCInventoryBase::UpdateInventoryAnim (0x005BD699): the model and body of final.cpp with the callees
// stubbed. Checks: (1) the X/Z axis updates run first and the last angles / last set are latched from the current ones;
// (2) each axis step is gain / (fps * seconds) (Y uses a gain of 1) and the set is rotated about X, Y, Z by the negated
// steps in that order; (3) the current angles advance by the steps and wrap past 1.0; (4) the def reference count is
// balanced (three temporary copies, each released) and the def is destroyed only when the count reaches zero.
#include <stdio.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif

struct C3DVector { float X, Y, Z; };
const C3DVector g_XAxis = { 1.0f, 0.0f, 0.0f };
const C3DVector g_YAxis = { 0.0f, 1.0f, 0.0f };
const C3DVector g_ZAxis = { 0.0f, 0.0f, 1.0f };
long g_FramesPerSecond = 30;

static int g_rotateCalls = 0; static const C3DVector* g_rotateAxis[8]; static float g_rotateAngle[8];
struct CRightHandedSet {
    C3DVector Forward;
    C3DVector Up;
    void RotateAboutAxis(const C3DVector& axis, float angle)
    {
        if (g_rotateCalls < 8) { g_rotateAxis[g_rotateCalls] = &axis; g_rotateAngle[g_rotateCalls] = angle; }
        g_rotateCalls++;
        Forward.X += angle;   // leave a trace in the set
    }
};

static int g_destroyCalls = 0;
struct CDefPointeeBase {
    virtual void v00() {}
    virtual void Destroy() { g_destroyCalls++; }
    long m_count;
    __forceinline void ReleaseReference() { if (--m_count == 0) Destroy(); }
};

struct CInventoryDef : CDefPointeeBase {
    unsigned char _pad08[0x780];
    float InventoryAnimXRotationSeconds;
    float InventoryAnimYRotationSeconds;
    float InventoryAnimZRotationSeconds;
};

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_count; }
    __forceinline ~CDefPointer()
    {
        if (Object)
            Object->ReleaseReference();
    }
    T* operator->() const { return Object; }
};

static int g_xAxisCalls = 0, g_zAxisCalls = 0;
struct CTCInventoryBase {
    unsigned char   _pad_0x00[0x68];
    CDefPointer<CInventoryDef> PDef;
    unsigned char   _pad_0x6c[0x60];
    CRightHandedSet InventoryAnimCurrentRHSet;
    CRightHandedSet InventoryAnimLastRHSet;
    float InventoryAnimCurrentXRotationAngle;
    float InventoryAnimLastXRotationAngle;
    float InventoryAnimCurrentYRotationAngle;
    float InventoryAnimLastYRotationAngle;
    float InventoryAnimCurrentZRotationAngle;
    float InventoryAnimLastZRotationAngle;
    float InventoryAnimXRotationAngleGain;
    float InventoryAnimYRotationAngleGain;
    float InventoryAnimZRotationAngleGain;

    __forceinline CDefPointer<CInventoryDef> GetPDef() const { return PDef; }
    void UpdateInventoryAnimXAxis() { g_xAxisCalls++; InventoryAnimCurrentXRotationAngle += 0.0f; }
    void UpdateInventoryAnimZAxis() { g_zAxisCalls++; }
    void UpdateInventoryAnim();
};

void CTCInventoryBase::UpdateInventoryAnim()
{
    UpdateInventoryAnimXAxis();
    UpdateInventoryAnimZAxis();
    InventoryAnimLastXRotationAngle = InventoryAnimCurrentXRotationAngle;
    InventoryAnimLastRHSet = InventoryAnimCurrentRHSet;
    InventoryAnimLastYRotationAngle = InventoryAnimCurrentYRotationAngle;
    InventoryAnimLastZRotationAngle = InventoryAnimCurrentZRotationAngle;
    float x = InventoryAnimXRotationAngleGain / (g_FramesPerSecond * GetPDef()->InventoryAnimXRotationSeconds);
    float y = 1.0f / (g_FramesPerSecond * GetPDef()->InventoryAnimYRotationSeconds);
    float z = InventoryAnimZRotationAngleGain / (g_FramesPerSecond * GetPDef()->InventoryAnimZRotationSeconds);
    InventoryAnimCurrentRHSet.RotateAboutAxis(g_XAxis, -x);
    InventoryAnimCurrentRHSet.RotateAboutAxis(g_YAxis, -y);
    InventoryAnimCurrentRHSet.RotateAboutAxis(g_ZAxis, -z);
    x += InventoryAnimCurrentXRotationAngle;
    InventoryAnimCurrentXRotationAngle = x;
    y += InventoryAnimCurrentYRotationAngle;
    InventoryAnimCurrentYRotationAngle = y;
    z += InventoryAnimCurrentZRotationAngle;
    InventoryAnimCurrentZRotationAngle = z;
    if (x > 1.0f)
        InventoryAnimCurrentXRotationAngle = x - 1.0f;
    if (y > 1.0f)
        InventoryAnimCurrentYRotationAngle = y - 1.0f;
    if (z > 1.0f)
        InventoryAnimCurrentZRotationAngle = z - 1.0f;
}

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static bool Near(float a, float b) { float d = a - b; return d < 0.0005f && d > -0.0005f; }

int main()
{
    CInventoryDef def; def.m_count = 1;
    def.InventoryAnimXRotationSeconds = 2.0f;    // x step = 6 / (30 * 2) = 0.1
    def.InventoryAnimYRotationSeconds = 0.5f;    // y step = 1 / (30 * 0.5) = 0.0667
    def.InventoryAnimZRotationSeconds = 1.0f;    // z step = 3 / (30 * 1) = 0.1
    CTCInventoryBase inv;
    inv.PDef.Object = &def; def.m_count = 2;
    inv.InventoryAnimXRotationAngleGain = 6.0f; inv.InventoryAnimYRotationAngleGain = 99.0f; inv.InventoryAnimZRotationAngleGain = 3.0f;
    inv.InventoryAnimCurrentXRotationAngle = 0.95f; inv.InventoryAnimCurrentYRotationAngle = 0.5f; inv.InventoryAnimCurrentZRotationAngle = 0.25f;
    inv.InventoryAnimLastXRotationAngle = inv.InventoryAnimLastYRotationAngle = inv.InventoryAnimLastZRotationAngle = -7.0f;
    inv.InventoryAnimCurrentRHSet.Forward.X = 1.0f; inv.InventoryAnimCurrentRHSet.Forward.Y = 2.0f; inv.InventoryAnimCurrentRHSet.Forward.Z = 3.0f;
    inv.InventoryAnimCurrentRHSet.Up.X = 4.0f; inv.InventoryAnimCurrentRHSet.Up.Y = 5.0f; inv.InventoryAnimCurrentRHSet.Up.Z = 6.0f;
    inv.InventoryAnimLastRHSet.Forward.X = -1.0f; inv.InventoryAnimLastRHSet.Up.Z = -1.0f;

    inv.UpdateInventoryAnim();

    // (1) axis updates and latching of the previous state
    CHECK(g_xAxisCalls == 1 && g_zAxisCalls == 1);
    CHECK(Near(inv.InventoryAnimLastXRotationAngle, 0.95f) && Near(inv.InventoryAnimLastYRotationAngle, 0.5f) && Near(inv.InventoryAnimLastZRotationAngle, 0.25f));
    CHECK(Near(inv.InventoryAnimLastRHSet.Forward.X, 1.0f) && Near(inv.InventoryAnimLastRHSet.Forward.Z, 3.0f) && Near(inv.InventoryAnimLastRHSet.Up.Z, 6.0f));
    // (2) rotations about X, Y, Z by the negated steps (Y gain is the constant 1, not the member)
    CHECK(g_rotateCalls == 3);
    CHECK(g_rotateAxis[0] == &g_XAxis && g_rotateAxis[1] == &g_YAxis && g_rotateAxis[2] == &g_ZAxis);
    CHECK(Near(g_rotateAngle[0], -0.1f) && Near(g_rotateAngle[1], -1.0f / 15.0f) && Near(g_rotateAngle[2], -0.1f));
    // (3) angle advance with wrap: 0.95 + 0.1 = 1.05 -> 0.05; 0.5 + 0.0667; 0.25 + 0.1
    CHECK(Near(inv.InventoryAnimCurrentXRotationAngle, 0.05f));
    CHECK(Near(inv.InventoryAnimCurrentYRotationAngle, 0.5f + 1.0f / 15.0f));
    CHECK(Near(inv.InventoryAnimCurrentZRotationAngle, 0.35f));
    // (4) def reference count balanced, def not destroyed
    CHECK(def.m_count == 2 && g_destroyCalls == 0);

    // a def held only by the temporaries would be destroyed by the last release: count 0 -> each temp inc/dec passes through 0
    def.m_count = 0;
    inv.UpdateInventoryAnim();
    CHECK(g_destroyCalls == 3 && def.m_count == 0 && g_rotateCalls == 6);

    char line[64];
    sprintf(line, "005bd699_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}