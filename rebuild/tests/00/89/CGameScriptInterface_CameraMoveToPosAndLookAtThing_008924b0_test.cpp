#include <cstdio>
class C3DVector;
class CThing {
public:
    unsigned char _pad00[0x6c];
    unsigned long ThingTypeFlags;                        // +0x6c
};
namespace NCameraModes {
class CScriptModuleBase {
public:
    virtual ~CScriptModuleBase();
};
class CScriptModuleGoToLookAt : public CScriptModuleBase {
public:
    unsigned char _pad04[0x6c - 4];
    CScriptModuleGoToLookAt(const C3DVector& pos, const CThing& target);   // 0x00852D10
};
}
template <class T> class CCountedPointer {
public:
    T*    Object;
    long* Count;
    CCountedPointer(T* object);                          // 0x005ED270 (instance shared by folding)
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
class CScriptThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                    // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                        // slot 75 (+0x12c)
};
class CGameScriptInterface {
public:
    virtual void CameraMoveToPosAndLookAtThing(const C3DVector& pos, const CScriptThing& thing, float time) const;
    CWorld* World;                                       // +0x04
};
void CGameScriptInterface::CameraMoveToPosAndLookAtThing(const C3DVector& pos, const CScriptThing& thing, float time) const
{
    if (thing.IsValid() && (thing.GetThing()->ThingTypeFlags & 4))
    {
        CThing* t = thing.GetThing();
        CGameCameraManager* manager;
        World->PCurrentGameCamera->GetCameraManager(&manager);
        manager->SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(new NCameraModes::CScriptModuleGoToLookAt(pos, *t)));
    }
}

void CScriptThing::v000() {}
void CScriptThing::v001() {}
void CScriptThing::v002() {}
void CScriptThing::v003() {}
void CScriptThing::v004() {}
void CScriptThing::v005() {}
void CScriptThing::v006() {}
void CScriptThing::v007() {}
void CScriptThing::v008() {}
void CScriptThing::v009() {}
void CScriptThing::v010() {}
void CScriptThing::v012() {}
void CScriptThing::v013() {}
void CScriptThing::v014() {}
void CScriptThing::v015() {}
void CScriptThing::v016() {}
void CScriptThing::v017() {}
void CScriptThing::v018() {}
void CScriptThing::v019() {}
void CScriptThing::v020() {}
void CScriptThing::v021() {}
void CScriptThing::v022() {}
void CScriptThing::v023() {}
void CScriptThing::v024() {}
void CScriptThing::v025() {}
void CScriptThing::v026() {}
void CScriptThing::v027() {}
void CScriptThing::v028() {}
void CScriptThing::v029() {}
void CScriptThing::v030() {}
void CScriptThing::v031() {}
void CScriptThing::v032() {}
void CScriptThing::v033() {}
void CScriptThing::v034() {}
void CScriptThing::v035() {}
void CScriptThing::v036() {}
void CScriptThing::v037() {}
void CScriptThing::v038() {}
void CScriptThing::v039() {}
void CScriptThing::v040() {}
void CScriptThing::v041() {}
void CScriptThing::v042() {}
void CScriptThing::v043() {}
void CScriptThing::v044() {}
void CScriptThing::v045() {}
void CScriptThing::v046() {}
void CScriptThing::v047() {}
void CScriptThing::v048() {}
void CScriptThing::v049() {}
void CScriptThing::v050() {}
void CScriptThing::v051() {}
void CScriptThing::v052() {}
void CScriptThing::v053() {}
void CScriptThing::v054() {}
void CScriptThing::v055() {}
void CScriptThing::v056() {}
void CScriptThing::v057() {}
void CScriptThing::v058() {}
void CScriptThing::v059() {}
void CScriptThing::v060() {}
void CScriptThing::v061() {}
void CScriptThing::v062() {}
void CScriptThing::v063() {}
void CScriptThing::v064() {}
void CScriptThing::v065() {}
void CScriptThing::v066() {}
void CScriptThing::v067() {}
void CScriptThing::v068() {}
void CScriptThing::v069() {}
void CScriptThing::v070() {}
void CScriptThing::v071() {}
void CScriptThing::v072() {}
void CScriptThing::v073() {}
void CScriptThing::v074() {}
CThing* CScriptThing::GetThing() const { return 0; }
bool CScriptThing::IsValid() const { return false; }
class CTestScriptThing : public CScriptThing { public: CThing* T; bool Valid; mutable int Gets; CThing* GetThing() const { ++Gets; return T; } bool IsValid() const { return Valid; } };
static int g_ctors, g_sets, g_cpdtor; static const C3DVector* g_pos; static const CThing* g_target; static void* g_obj; static CGameCameraManager* g_mgrThis;
NCameraModes::CScriptModuleBase::~CScriptModuleBase() {}
NCameraModes::CScriptModuleGoToLookAt::CScriptModuleGoToLookAt(const C3DVector& pos, const CThing& target) { ++g_ctors; g_pos = &pos; g_target = &target; g_obj = this; }
template <class T> CCountedPointer<T>::CCountedPointer(T* o) { Object = o; Count = 0; }
template <class T> CCountedPointer<T>::CCountedPointer(const CCountedPointer& o) { Object = o.Object; Count = o.Count; }
template <class T> CCountedPointer<T>::~CCountedPointer() { ++g_cpdtor; delete Object; }
void CGameCameraManager::SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> mode) { ++g_sets; g_mgrThis = this; if (mode.Object != g_obj) g_sets += 100; }
static CGameCameraManager g_mgr;
void CAIGameCameraBase::v00() {} void CAIGameCameraBase::v01() {} void CAIGameCameraBase::v02() {} void CAIGameCameraBase::v03() {}
void CAIGameCameraBase::v04() {} void CAIGameCameraBase::v05() {} void CAIGameCameraBase::v06() {} void CAIGameCameraBase::v07() {}
void CAIGameCameraBase::v08() {}
bool CAIGameCameraBase::GetCameraManager(CGameCameraManager** m) { *m = &g_mgr; return true; }
int main()
{
    CThing t; t.ThingTypeFlags = 0;
    CAIGameCameraBase cam; static unsigned char raw[sizeof(CWorld)]; CWorld& w = *(CWorld*)raw; w.PCurrentGameCamera = &cam;
    CGameScriptInterface gsi; gsi.World = &w;
    CTestScriptThing s; s.T = &t; s.Valid = false; s.Gets = 0;
    const C3DVector& pos = *(const C3DVector*)raw;
    t.ThingTypeFlags = 4; gsi.CameraMoveToPosAndLookAtThing(pos, s, 1.0f); bool ok = g_ctors == 0 && s.Gets == 0;
    s.Valid = true; t.ThingTypeFlags = ~4UL; gsi.CameraMoveToPosAndLookAtThing(pos, s, 1.0f); ok = ok && g_ctors == 0 && s.Gets == 1;
    t.ThingTypeFlags = 4; gsi.CameraMoveToPosAndLookAtThing(pos, s, 1.0f);
    ok = ok && g_ctors == 1 && s.Gets == 3 && g_pos == &pos && g_target == &t && g_sets == 1 && g_mgrThis == &g_mgr && g_cpdtor == 1;
    std::printf(ok ? "008924b0_TEST PASS\n" : "008924b0_TEST FAIL\n");
    return ok ? 0 : 1;
}