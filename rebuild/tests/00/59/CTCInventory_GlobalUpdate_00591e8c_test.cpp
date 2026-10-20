// Behaviour test for 00591e8c: body copied from src/00591e8c.cpp (CTCInventory::GlobalUpdate), callees stubbed.
#include <cstdio>
#include <cstring>
static int g_baseCalls = 0, g_rotCalls = 0, g_ctxCalls = 0, g_destroys = 0; static bool g_basePaused = false; static float g_rotAngle = 0.0f; static const void* g_rotAxis = 0;
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct C3DVector { float X, Y, Z; };
long g_framesPerSecond = 10;
C3DVector g_inventoryItemRotationAxis = { 0.0f, -1.0f, 0.0f };
struct CDefBase { virtual void v00() {} virtual void Destroy() { ++g_destroys; } long RefCount; };
struct CInventoryDef : CDefBase { unsigned char _pad_0x08[0x1D4]; float NumberOfSecondsForObjectRotation; unsigned char _pad_0x1e0[0x850]; float MainQuickAccessItemFlashTime; };
struct CInventoryDefPointer {
    CInventoryDef* Object;
    CInventoryDefPointer() : Object(0) { }
    CInventoryDefPointer(const CInventoryDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CInventoryDefPointer() { if (DecRefToZero(Object->RefCount)) Object->Destroy(); }
    CInventoryDef* operator->() const { return Object; }
};
struct CRightHandedSet { float m[6]; void RotateAboutAxis_00a22f80(const C3DVector& axis, float angle) { ++g_rotCalls; g_rotAxis = &axis; g_rotAngle = angle; } };
namespace NInventory {
struct CTCInventoryBase { void* __vftable; unsigned char _pad_0x04[0x64]; CInventoryDefPointer PDef; unsigned char _pad_0x6c[0xC4]; void GlobalUpdate(bool paused) { ++g_baseCalls; g_basePaused = paused; } };
}
struct CTCInventory : NInventory::CTCInventoryBase {
    void* __vftable_130; unsigned char _pad_0x134[0x5C]; CRightHandedSet OldObjectRHSet; float OldRotationAngle; CRightHandedSet CurrentObjectRHSet; float CurrentRotationAngle;
    long ItemRHSetsApplyTo; bool EnableQuickAccessAssign; bool EnableQuickAccessRemove; bool QuickAccessFlashState; long QuickAccessFlashTimer;
    void UpdateContextSensitiveItems() { ++g_ctxCalls; }
    void GlobalUpdate(bool paused);
};
void CTCInventory::GlobalUpdate(bool paused)
{
    NInventory::CTCInventoryBase::GlobalUpdate(paused);
    OldObjectRHSet = CurrentObjectRHSet;
    OldRotationAngle = CurrentRotationAngle;
    float step;
    {
        CInventoryDefPointer def = PDef;
        step = 1.0f / (def->NumberOfSecondsForObjectRotation * g_framesPerSecond);
    }
    CurrentObjectRHSet.RotateAboutAxis_00a22f80(g_inventoryItemRotationAxis, -step);
    CurrentRotationAngle += step;
    if (--QuickAccessFlashTimer <= 0) {
        {
            CInventoryDefPointer def = PDef;
            QuickAccessFlashTimer = (long)(def->MainQuickAccessItemFlashTime * g_framesPerSecond);
        }
        bool& flash = QuickAccessFlashState;
        flash = !flash;
    }
    UpdateContextSensitiveItems();
}

static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static bool near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }
int main()
{
    CInventoryDef def; memset(&def, 0, sizeof(def)); def.RefCount = 1; def.NumberOfSecondsForObjectRotation = 2.0f; def.MainQuickAccessItemFlashTime = 0.5f;
    CTCInventory inv; memset(&inv, 0, sizeof(inv)); inv.PDef.Object = &def;
    for (int i = 0; i < 6; ++i) inv.CurrentObjectRHSet.m[i] = (float)(i + 1);
    inv.CurrentRotationAngle = 0.25f; inv.QuickAccessFlashTimer = 3; inv.QuickAccessFlashState = false;
    // timer still running: rotate only
    inv.GlobalUpdate(true);
    CHECK(g_baseCalls == 1 && g_basePaused == true && g_rotCalls == 1 && g_rotAxis == &g_inventoryItemRotationAxis && g_ctxCalls == 1);
    CHECK(near(g_rotAngle, -0.05f) && near(inv.CurrentRotationAngle, 0.30f) && near(inv.OldRotationAngle, 0.25f) && inv.OldObjectRHSet.m[5] == 6.0f);
    CHECK(inv.QuickAccessFlashTimer == 2 && inv.QuickAccessFlashState == false && def.RefCount == 1 && g_destroys == 0);
    // run the timer out: reload from the def and flip the flash state
    inv.QuickAccessFlashTimer = 1; inv.GlobalUpdate(false);
    CHECK(g_basePaused == false && inv.QuickAccessFlashTimer == 5 && inv.QuickAccessFlashState == true && def.RefCount == 1 && g_destroys == 0 && g_ctxCalls == 2);
    inv.GlobalUpdate(false); CHECK(inv.QuickAccessFlashTimer == 4 && inv.QuickAccessFlashState == true);
    inv.QuickAccessFlashTimer = 0; inv.GlobalUpdate(false); CHECK(inv.QuickAccessFlashTimer == 5 && inv.QuickAccessFlashState == false);
    if (fails == 0) printf("00591e8c_TEST PASS\n"); else printf("00591e8c_TEST FAIL\n");
    def.RefCount = 2;   // the inventory's own PDef releases once more at exit; keep the zeroed-vtable def alive
    return fails;
}