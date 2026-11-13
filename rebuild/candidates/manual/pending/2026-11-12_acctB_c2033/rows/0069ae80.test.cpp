// Behaviour test for 0x0069AE80 CGameCameraManager::CGameCameraManager(const CWorld&): a self-contained host model of the
// constructor's observable effects -- member defaults, the six mode registrations (counted pointers left with one reference each,
// in retail order 3, 4, 0xD, 0xE, 0x11, 0xB), the two static flags cleared, the projectile-weapon static reset, the two by-name def
// lookups ("CAMERA_MODE_MODE_NULL" into the default camera-mode def, "CAMERA_MANAGER_SET_DEFAULT" into a local handed to
// SetCameraManagerSetDef) and the three camera-list indices set to their lists' end.
#include <cstdio>
#include <cstring>

struct CCPPointerInfo { unsigned int RefCount; void (*DeleteFunc)(void*); void* Data; };
static int g_deleted = 0;
static void DeleteInit(void* p) { ++g_deleted; delete (int*)p; }

struct CountedInit {
    int* Data; CCPPointerInfo* Info;
    CountedInit() : Data(0), Info(0) {}
    explicit CountedInit(int* data) : Data(data), Info(0) {
        if (Data) { Info = new CCPPointerInfo; Info->RefCount = 1; Info->DeleteFunc = DeleteInit; Info->Data = Data; }
    }
    CountedInit(const CountedInit& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CountedInit() { if (Info) { if (--Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } } Data = 0; Info = 0; }
};

struct ModeEntry { int Mode; CountedInit Init; };
struct ModeMap {
    ModeEntry Entries[16]; int Count;
    ModeMap() : Count(0) {}
    bool insert(int mode, const CountedInit& init) {           // insert_unique: keeps the first entry for a key
        for (int i = 0; i < Count; ++i) if (Entries[i].Mode == mode) return false;
        Entries[Count].Mode = mode; Entries[Count].Init.Data = init.Data; Entries[Count].Init.Info = init.Info;
        if (init.Info) ++init.Info->RefCount;
        ++Count; return true;
    }
};

static int g_projectileStatic = 1;
static void StatRegisterMode() { g_projectileStatic = 0; }
static bool g_flagA = true, g_flagB = true;
static const char* g_lookups[2]; static int g_lookupCount = 0;
static int g_defaultCameraModeDef = 0;
static int g_installedSet = 0;

struct Manager {
    float CamFov; long ScriptedCameraRemoveTimer; bool ChangeInControlDirection, ControlNoControlMovement, ObservedThingInMelee;
    long DrawDebugGraphics; float CamRSLookAroundDeltaTheta, CamZoomMinHeightFactor; bool AutoFOVZoomInCombat, CamAutoGoBehind;
    int CameraListEnd, SpecialCameraListEnd; int CameraListIndex, InitialModeCameraListIndex, SpecialCameraListIndex;
    ModeMap RegisteredModes;
    void AddMode(int mode, int* init) {
        CountedInit temp(init);                                  // CCountedPointer<CBaseInit>(T::StatAlloc())
        CountedInit made(temp);                                  // std::make_pair copy
        CountedInit value(made);                                 // pair<const EMode, ..> conversion
        RegisteredModes.insert(mode, value);
    }
    explicit Manager(int worldTag) {
        CamFov = 0.19444f; ScriptedCameraRemoveTimer = -1; ChangeInControlDirection = true; ControlNoControlMovement = true;
        ObservedThingInMelee = false; DrawDebugGraphics = 0;
        CameraListEnd = 0; SpecialCameraListEnd = 0;
        CamRSLookAroundDeltaTheta = 0.03f; CamZoomMinHeightFactor = 1.0f / 3.0f; AutoFOVZoomInCombat = true; CamAutoGoBehind = false;
        g_flagA = false; g_flagB = false;
        AddMode(3, new int(3)); AddMode(4, new int(4)); AddMode(0xd, new int(0xd));
        AddMode(0xe, new int(0xe)); AddMode(0x11, new int(0x11)); AddMode(0xb, new int(0xb));
        StatRegisterMode();
        g_lookups[g_lookupCount++] = "CAMERA_MODE_MODE_NULL"; g_defaultCameraModeDef = worldTag + 1;
        int set = 0;
        g_lookups[g_lookupCount++] = "CAMERA_MANAGER_SET_DEFAULT"; set = worldTag + 2;
        CameraListIndex = InitialModeCameraListIndex = CameraListEnd;
        SpecialCameraListIndex = SpecialCameraListEnd;
        g_installedSet = set;
    }
};

int main()
{
    Manager m(40);
    static const int order[6] = { 3, 4, 0xd, 0xe, 0x11, 0xb };
    bool ok = m.RegisteredModes.Count == 6 && g_deleted == 0;
    for (int i = 0; ok && i < 6; ++i)
        ok = m.RegisteredModes.Entries[i].Mode == order[i] && m.RegisteredModes.Entries[i].Init.Info != 0 &&
             m.RegisteredModes.Entries[i].Init.Info->RefCount == 1 && *m.RegisteredModes.Entries[i].Init.Data == order[i];
    if (ok) ok = m.ScriptedCameraRemoveTimer == -1 && m.ChangeInControlDirection && m.ControlNoControlMovement && !m.ObservedThingInMelee;
    if (ok) ok = m.CamZoomMinHeightFactor > 0.3333f && m.CamZoomMinHeightFactor < 0.3334f && m.CamRSLookAroundDeltaTheta > 0.0299f &&
                 m.CamRSLookAroundDeltaTheta < 0.0301f && m.AutoFOVZoomInCombat && !m.CamAutoGoBehind && m.DrawDebugGraphics == 0;
    if (ok) ok = !g_flagA && !g_flagB && g_projectileStatic == 0 && g_lookupCount == 2 &&
                 std::strcmp(g_lookups[0], "CAMERA_MODE_MODE_NULL") == 0 && std::strcmp(g_lookups[1], "CAMERA_MANAGER_SET_DEFAULT") == 0 &&
                 g_defaultCameraModeDef == 41 && g_installedSet == 42;
    if (ok) ok = m.CameraListIndex == m.CameraListEnd && m.InitialModeCameraListIndex == m.CameraListEnd &&
                 m.SpecialCameraListIndex == m.SpecialCameraListEnd;
    if (!ok) { std::printf("0069ae80_TEST FAIL\n"); return 1; }
    std::printf("0069ae80_TEST PASS\n");
    return 0;
}
