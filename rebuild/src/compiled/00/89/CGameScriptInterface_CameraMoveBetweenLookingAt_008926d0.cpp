// CGameScriptInterface::CameraMoveBetweenLookingAt(const C3DVector& from, const C3DVector& to, const CScriptThing& thing, float time, float f) const -- hand the camera manager a scripted move-between camera mode looking at a valid thing
// Leaf: ?CameraMoveBetweenLookingAt@CGameScriptInterface@@UBEXABVC3DVector@@0ABVCScriptThing@@MM@Z
// Retail: Fable.exe @ 0x008926D0, 167 bytes, `ret 0x14`; Ego_r.pdb same symbol.
// Body: if thing.IsValid() (slot 75): t = thing.GetThing() (slot 11); GetCameraManager(&manager) (`manager` in the dead
// `thing` slot); f == -1.0f defaults to 0.2f; manager->SetScriptedCameraMode(CCountedPointer<CScriptModuleBase>(new
// <0x90-byte mode>(*t, from, to, ConvertSecondsToWF(time), f))) -- mode ctor 0x00852D90, CCountedPointer(T*) out of line
// at 0x005ED270 (folded label), SetScriptedCameraMode 0x00699E60.
// Layout: as the matched 0x008924B0.
// Names: mode class name not recovered; descriptive.
// Shape: 0x008924B0 model: out-of-line CCountedPointer(T*) plus a declared copy ctor (in-place argument construction).
// Opt: /O2.
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
class CScriptModuleMoveBetweenLookingAt : public CScriptModuleBase {
public:
    unsigned char _pad04[0x90 - 4];
    CScriptModuleMoveBetweenLookingAt(const CThing& lookAt, const C3DVector& from, const C3DVector& to, long frames, float f);   // 0x00852D90
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
extern long g_WorldFramesPerSecond;                  // 0x01375550
class CMainGameComponent {
public:
    static long ConvertSecondsToWF(float seconds) { return (long)(g_WorldFramesPerSecond * seconds); }
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
    virtual void CameraMoveBetweenLookingAt(const C3DVector& from, const C3DVector& to, const CScriptThing& thing, float time, float f) const;
    CWorld* World;                                       // +0x04
};
void CGameScriptInterface::CameraMoveBetweenLookingAt(const C3DVector& from, const C3DVector& to, const CScriptThing& thing, float time, float f) const
{
    if (thing.IsValid())
    {
        CThing* t = thing.GetThing();
        CGameCameraManager* manager;
        World->PCurrentGameCamera->GetCameraManager(&manager);
        if (f == -1.0f) f = 0.2f;
        manager->SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(
            new NCameraModes::CScriptModuleMoveBetweenLookingAt(*t, from, to, CMainGameComponent::ConvertSecondsToWF(time), f)));
    }
}