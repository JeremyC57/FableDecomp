// Behaviour test for CWorld::InitMeshBank (0x0049E620): the opened bank replaces the old one (old released, new
// referenced once by the world after the temporary dies), the animation manager is taken from it, the bank becomes
// the global appearance bank, both progress texts are shown and the result is true.
#include <cstdio>
#include <cstring>
struct CCharString { const char* Text; CCharString(const char* t) : Text(t) {} };
static const char* g_progress[4]; static int g_np = 0;
static void DisplayProgress(const CCharString& t, float, bool, bool) { g_progress[g_np++] = t.Text; }
static int g_deleted = 0;
struct CCPPointerInfo { long RefCount; void* Data; };
template <class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    void Release() { if (Info) { --Info->RefCount; if (Info->RefCount == 0) ++g_deleted; } }
    ~CCountedPointer() { Release(); Data = 0; Info = 0; }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data; CCPPointerInfo* info = other.Info;
        if (Info != info) { Release(); Data = data; Info = info; if (info) ++info->RefCount; }
        return *this;
    }
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
};
struct C3DAnimationManager { int id; };
struct CMeshDataBank { C3DAnimationManager* AnimationManager; };
static CMeshDataBank g_bank; static CCPPointerInfo g_info; static C3DAnimationManager g_anim; static const char* g_opened;
struct CMeshBankManager {
    CCountedPointer<CMeshDataBank> OpenMeshBank(const CCharString& name, bool)
    {
        g_opened = name.Text; CCountedPointer<CMeshDataBank> p; p.Data = &g_bank; p.Info = &g_info; ++g_info.RefCount; return p;
    }
};
static CMeshBankManager g_mgr;
struct CSystemManager { CMeshBankManager* MeshBankManager; CMeshBankManager& GetMeshBankManager() { return *MeshBankManager; } };
static CSystemManager g_sys = { &g_mgr };
static CSystemManager* GFGetSystemManager() { return &g_sys; }
static CMeshDataBank* g_globalBank = 0;
struct CTCGraphicAppearance { static void SetMeshBank(CMeshDataBank& b) { g_globalBank = &b; } };
struct CWorld {
    CCountedPointer<CMeshDataBank> PMainMeshBank;
    C3DAnimationManager* PAnimationManager;
    bool InitMeshBank()
    {
        { CCharString text("Opening Mesh Bank"); DisplayProgress(text, -1.0f, false, false); }
        PMainMeshBank = GFGetSystemManager()->GetMeshBankManager().OpenMeshBank("MBANK_ALLMESHES", false);
        PAnimationManager = PMainMeshBank->AnimationManager;
        { CCharString text("Setting Mesh Bank"); DisplayProgress(text, -1.0f, false, false); }
        CTCGraphicAppearance::SetMeshBank(*PMainMeshBank);
        return true;
    }
};
int main()
{
    g_bank.AnimationManager = &g_anim; g_info.RefCount = 0;
    CMeshDataBank oldBank; CCPPointerInfo oldInfo = { 1, 0 };
    CWorld w; w.PMainMeshBank.Data = &oldBank; w.PMainMeshBank.Info = &oldInfo; w.PAnimationManager = 0;
    bool r = w.InitMeshBank();
    bool ok = r && g_deleted == 1 && oldInfo.RefCount == 0 && w.PMainMeshBank.Data == &g_bank && g_info.RefCount == 1
        && w.PAnimationManager == &g_anim && g_globalBank == &g_bank && g_np == 2
        && std::strcmp(g_progress[0], "Opening Mesh Bank") == 0 && std::strcmp(g_progress[1], "Setting Mesh Bank") == 0
        && std::strcmp(g_opened, "MBANK_ALLMESHES") == 0;
    w.PMainMeshBank.Info = 0;
    if (!ok) { std::printf("0049e620_TEST FAIL\n"); return 1; }
    std::printf("0049e620_TEST PASS\n");
    return 0;
}
