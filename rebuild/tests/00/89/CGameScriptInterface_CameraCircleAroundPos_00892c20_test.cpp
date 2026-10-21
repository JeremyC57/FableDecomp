#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
namespace NCameraModes {
class CScriptModuleBase {
public:
    unsigned char _pad04[0x4c - 4];
    CScriptModuleBase();                                 // 0x005ED070
    virtual ~CScriptModuleBase();
};
class CScriptModuleDolly : public CScriptModuleBase {          // vtable 0x01278160
public:
    C3DVector PosAround;                                 // +0x4c
    C3DVector DollyVector;                               // +0x58
    float     TimeFor360Turn;                            // +0x64
    CScriptModuleDolly(const C3DVector& posAround, const C3DVector& dollyVector, float timeFor360Turn)
        : PosAround(posAround), DollyVector(dollyVector), TimeFor360Turn(timeFor360Turn) {}
};
}
struct CCPPointerInfo {
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
    CCPPointerInfo(void (__fastcall* deleteFunc)(void*), void* data) : RefCount(1), DeleteFunc(deleteFunc), Data(data) {}
};
template <class T> class CCountedPointer {
public:
    T*              Object;
    CCPPointerInfo* Info;
    static void __fastcall DeleteObject(void* data);     // 0x005ECF50 (`delete (T*)data`, folded)
    __forceinline CCountedPointer(T* object) : Object(object)
    {
        if (object)
            Info = new CCPPointerInfo(DeleteObject, Object);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other);
    ~CCountedPointer();
};
class CGameCameraManager {
public:
    void SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> mode);   // 0x00699E60
};
class CAIGameCameraBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** manager);   // slot 9 (+0x24)
};
class CWorld {
public:
    unsigned char      _pad00[0x34];
    CAIGameCameraBase* PCurrentGameCamera;               // +0x34
};
class CGameScriptInterface {
public:
    virtual void CameraCircleAroundPos(const C3DVector& pos, const C3DVector& offset, float timeFor360Turn) const;
    CWorld* World;                                       // +0x04
};
void CGameScriptInterface::CameraCircleAroundPos(const C3DVector& pos, const C3DVector& offset, float timeFor360Turn) const
{
    CGameCameraManager* manager;
    World->PCurrentGameCamera->GetCameraManager(&manager);
    manager->SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(
        new NCameraModes::CScriptModuleDolly(pos, offset, timeFor360Turn)));
}

static int g_sets, g_dtors, g_basector; static long g_seenRef;
NCameraModes::CScriptModuleBase::CScriptModuleBase() { ++g_basector; }
NCameraModes::CScriptModuleBase::~CScriptModuleBase() { ++g_dtors; }
template <class T> void __fastcall CCountedPointer<T>::DeleteObject(void* data) { delete (T*)data; }
template <class T> CCountedPointer<T>::CCountedPointer(const CCountedPointer& o) { Object = o.Object; Info = o.Info; if (Info) ++Info->RefCount; }
template <class T> CCountedPointer<T>::~CCountedPointer() { if (Info && --Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
static float g_pos[3]; static float g_vec[3]; static float g_time;
void CGameCameraManager::SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> mode)
{
    ++g_sets; g_seenRef = mode.Info ? mode.Info->RefCount : -1;
    NCameraModes::CScriptModuleDolly* d = static_cast<NCameraModes::CScriptModuleDolly*>(mode.Object);
    g_pos[0] = d->PosAround.X; g_pos[1] = d->PosAround.Y; g_pos[2] = d->PosAround.Z; g_vec[0] = d->DollyVector.X; g_vec[1] = d->DollyVector.Y; g_vec[2] = d->DollyVector.Z; g_time = d->TimeFor360Turn;
}
static CGameCameraManager g_mgr;
void CAIGameCameraBase::v00() {} void CAIGameCameraBase::v01() {} void CAIGameCameraBase::v02() {} void CAIGameCameraBase::v03() {}
void CAIGameCameraBase::v04() {} void CAIGameCameraBase::v05() {} void CAIGameCameraBase::v06() {} void CAIGameCameraBase::v07() {}
void CAIGameCameraBase::v08() {}
bool CAIGameCameraBase::GetCameraManager(CGameCameraManager** m) { *m = &g_mgr; return true; }
int main()
{
    CAIGameCameraBase cam; static unsigned char raw[sizeof(CWorld)]; CWorld& w = *(CWorld*)raw; w.PCurrentGameCamera = &cam;
    CGameScriptInterface gsi; gsi.World = &w;
    static float offRaw[3] = { 1.0f, 2.0f, 3.0f }; const C3DVector& off = *(const C3DVector*)offRaw;
    static float posRaw[3] = { 4.0f, 5.0f, 6.0f }; const C3DVector& pos = *(const C3DVector*)posRaw;
    gsi.CameraCircleAroundPos(pos, off, 7.5f);
    bool ok = g_sets == 1 && g_basector == 1 && g_seenRef == 1 && g_dtors == 1 && g_pos[0] == 4.0f && g_pos[1] == 5.0f && g_pos[2] == 6.0f
           && g_vec[0] == 1.0f && g_vec[1] == 2.0f && g_vec[2] == 3.0f && g_time == 7.5f;
    std::printf(ok ? "00892c20_TEST PASS\n" : "00892c20_TEST FAIL\n");
    return ok ? 0 : 1;
}