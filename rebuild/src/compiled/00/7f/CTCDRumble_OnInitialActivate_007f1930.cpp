// CTCDRumble::OnInitialActivate() -- take the rumble def from the thing's def, start a camera earthquake at the thing, then rumble the hero's controller (scaled by distance unless the rumble is global) and time the rumble's life from its length
// Leaf: OnInitialActivate
//
// Retail 0x007F1930, 391 bytes (plain `ret`; vtable 0x012408BC slot 6, the CTCBase OnInitialActivate slot -- the catalog's
// "CTCDRumble::vfunc_6").
// Callees: CDefClassBase::GetSubDef<const CRumbleDef>(CDefPointer*) @ 0x007F1EE0, CTCBase::GetWorld @ 0x00686A70, the current game
// camera's virtual slot 9 (+0x24, fetch its camera manager), CGameCameraManager::SetEarthquakeAt(const C3DVector&, float, float,
// bool) @ 0x00699560, CThing::GetPosition @ 0x004C73D0, CTCBase::GetPlayerManager @ 0x00686D40, CPlayerManager::GetMainPlayer @
// 0x00449970, CPlayer::GetHeroThing @ 0x00487DC0, the distance falloff exp(k * |a - b|^2) @ 0x007F1840 (`__fastcall`, two vectors),
// CThing::GetJoystickDeviceNumber @ 0x004C7CA0, the joystick device's virtual slot 17 (+0x44, start a rumble), `_ftol` @ 0x00BFEA70,
// the defs' virtual slot 1 on release. Globals: g_pInputManager 0x013B8388 (joystick table +0x2c, 8-byte entries),
// g_FramesPerSecond 0x01375550.
// Layout = Ego_r.pdb CTCDRumble (0x20): PDef +0x0c, Strength +0x10, Length +0x14, GlobalRumble +0x18, LifeTimer +0x1c; CWorld
// PCurrentGameCamera +0x34; CInitRumble {MaxLevel, Smoothness, AttackTime, DecayTime, Cumulative}; CThing PDef +0x70, Dead bit 0
// of +0x91.
// Shape: the joystick device is a named value local read BEFORE the CInitRumble is built (retail loads it between the
// scale fmul and the init stores); building init first, or binding the device as const&, reorders the tail.
// Shape: the def pointer assignment is the inline CDefPointer::operator= (release + clear, then add a reference); the decay and
// attack times are locals computed before the hero fetch; the falloff scale defaults to 1 in the def's dead frame slot.
class C3DVector { public: float X, Y, Z; };

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
    mutable long m_refCount;                            // +0x04
    void Release() const { if (--m_refCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && --Object->m_refCount == 0) Object->Destroy(); }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                Object->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_refCount;
        }
        return *this;
    }
};
class CRumbleDef : public CDefPointeeBase { public: };
class CThingBaseDef : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<const CRumbleDef>* out) const;   // 0x007F1EE0
};
void* FindSubDef(const CThingBaseDef* def);
void ReleaseDef(void* def);
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef (retail's body is out of line): CDefPointer::operator= on the out pointer, so VC7.1 knows the
// out pointer's address does not escape the call and keeps the fetched def in a register.
bool CThingBaseDef::GetSubDef(CDefPointer<const CRumbleDef>* out) const
{
    void* def = FindSubDef(this);
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThing {
public:
    unsigned char _pad00[0x70];
    CThingBaseDef* PDef;                                // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Dead : 1;                             // +0x91 bit 0
    const C3DVector& GetPosition() const;               // 0x004C73D0
    long GetJoystickDeviceNumber() const;               // 0x004C7CA0
};
class CPlayer { public:
    CThing* GetHeroThing();                             // 0x00487DC0
};
class CPlayerManager { public:
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};
class CGameCameraManager { public:
    void SetEarthquakeAt(const C3DVector& position, float strength, float length, bool global);   // 0x00699560
};
class CAIGameCameraBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetGameCameraManager(CGameCameraManager** out);   // slot 9 (+0x24)
};
class CWorld { public:
    unsigned char      _pad00[0x34];
    CAIGameCameraBase* PCurrentGameCamera;              // +0x34
    bool GetGameCameraManager(CGameCameraManager** out) { return PCurrentGameCamera->GetGameCameraManager(out); }
};
struct CInitRumble {
    float MaxLevel;                                     // +0x00
    float Smoothness;                                   // +0x04
    float AttackTime;                                   // +0x08
    float DecayTime;                                    // +0x0c
    bool  Cumulative;                                   // +0x10
    CInitRumble(float maxLevel, float smoothness, float attackTime, float decayTime, bool cumulative)
        : MaxLevel(maxLevel), Smoothness(smoothness), AttackTime(attackTime), DecayTime(decayTime), Cumulative(cumulative) { }
};
struct CJoystickDevice {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void StartRumble(long type, const CInitRumble* init, bool replace);   // slot 17 (+0x44)
};
struct CJoystickEntry {
    CJoystickDevice* Device;
    long             Index;
};
class CInputManager { public:
    unsigned char   _pad00[0x2c];
    CJoystickEntry* Joysticks;                          // +0x2c
};
extern CInputManager* g_pInputManager;                  // 0x013B8388
extern long g_FramesPerSecond;                          // 0x01375550
float __fastcall RumbleDistanceFalloff(const C3DVector& from, const C3DVector& to);   // 0x007F1840

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
    CWorld& GetWorld() const;   // 0x00686A70
};
class CTCDRumble : public CTCBase {
public:
    CDefPointer<const CRumbleDef> PDef;                 // +0x0c
    float Strength;                                     // +0x10
    float Length;                                       // +0x14
    bool  GlobalRumble;                                 // +0x18
    long  LifeTimer;                                    // +0x1c
    void OnInitialActivate();
};

void CTCDRumble::OnInitialActivate()
{
    CDefPointer<const CRumbleDef> def;
    ParentThing->PDef->GetSubDef(&def);
    PDef = def;
    CGameCameraManager* cameraManager;
    if (GetWorld().GetGameCameraManager(&cameraManager)) {
        CThing* thing = ParentThing;
        cameraManager->SetEarthquakeAt(thing->GetPosition(), Strength, Length, GlobalRumble);
    }
    float decayTime = Length * 0.8f;
    float attackTime = Length * 0.2f;
    CThing* hero = GetPlayerManager().GetMainPlayer().GetHeroThing();
    if (hero && !hero->Dead) {
        float scale = 1.0f;
        if (!GlobalRumble) {
            CThing* thing = ParentThing;
            scale = RumbleDistanceFalloff(thing->GetPosition(), hero->GetPosition());
        }
        long joystick = hero->GetJoystickDeviceNumber();
        CJoystickDevice* device = g_pInputManager->Joysticks[joystick].Device;
        CInitRumble init(scale * Strength, 0.5f, attackTime, decayTime, false);
        device->StartRumble(4, &init, false);
        LifeTimer = (long)(g_FramesPerSecond * Length);
        return;
    }
    LifeTimer = -1;
}