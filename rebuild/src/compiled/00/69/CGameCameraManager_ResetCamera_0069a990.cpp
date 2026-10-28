// CGameCameraManager::ResetCamera() -- re-target the camera on the hero, reset the world height band around the hero, clear the camera flags and the hero's projectile aiming mode, drop the current mode and both script modules, clear the earthquakes, snapshot the camera, push the manual mode, and tell the renderer the camera moved (not in fast mode)
// Leaf: ResetCamera
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069A990, 488 bytes, `ret`.
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, own virtual slots 27/28,
//   CThing::GetPos 0x004C73D0, ResetFlags 0x00699130, the component-map LowerBound 0x0040F020 (TC 0x81),
//   ResetProjectileWeaponMode 0x00789B30, operator delete 0x00BFE9BC, vector<CQuake>::erase 0x0069D180,
//   PushCurrentManualMode 0x006988B0, the mode refresh 0x0069A2A0, CCharString(const char*, long) 0x0099EBF0 /
//   operator+=(const char*) 0x0099F100 / dtor 0x0099EAE0; the renderer's virtual slots 56 and 58; 10.0f 0x01237F0C.
// Layout: local projection with the retail packing of the PDB order (CamTools 0x20 bytes, one-word STLport lists):
//   CurrentMode +0x98, SingleCut +0xb0, AngleChange +0xbc, PCurrentScriptModule +0xd4, PPendingScriptModule +0xdc,
//   EarthQuakes +0xec, MinWorldHeight +0xf8.
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
};
class CCharStringData { public: CBasicStringChar Data; long NoRefs; };
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
    CCharString& operator+=(const char* text);           // 0x0099F100
};
class C3DVector { public: float X, Y, Z; };
struct ComponentEntry { long Type; void* Component; };
struct ComponentMap {
    ComponentEntry* Start;
    ComponentEntry* Finish;
    ComponentEntry* LowerBound(const long& key);         // 0x0040F020
};
class CThing { public:
    unsigned char  Base[0x20];
    unsigned long  UsingComponents[9];                   // +0x20
    ComponentMap   Components;                           // +0x44
    const C3DVector& GetPos() const;                // 0x004C73D0
};
class CThingPlayerCreature : public CThing { };
class CTCHeroRangedAttacks { public: public:
    void ResetProjectileWeaponMode();   // 0x00789B30
};   // 0x00789B30
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };              // 0x00487DC0
class CPlayerManager { public: public:
    CPlayer& GetMainPlayer();   // 0x00449970
};     // 0x00449970
struct CRenderer {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void SetCameraMoved(bool fastMode, bool moved);   // slot 56 (+0xe0)
    virtual void v057();
    virtual void InvalidateCamera();   // slot 58 (+0xe8)
};
class CDisplayEngine { public: unsigned char Data[0x2c]; CRenderer* Renderer; };
class CMainGameComponent { public:
    unsigned char   Data[0x1c];
    CPlayerManager* PlayerManager;                       // +0x1c
    unsigned char   Data2[8];
    CDisplayEngine* DisplayEngine;                       // +0x28
};
extern CMainGameComponent* g_pMainGameComponent;         // 0x013B86A0
class CCPPointerInfo { public: long Count; void (__fastcall* Delete)(void*); void* Object; };
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    void Reset()
    {
        if (Info) {
            --Info->Count;
            if (Info->Count == 0) {
                Info->Delete(Info->Object);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
};
struct CQuake { unsigned char Data[0x14]; };
struct CQuakeVector {
    CQuake* Start;
    CQuake* Finish;
    CQuake* EndOfStorage;
    CQuake* erase(CQuake* first, CQuake* last);          // 0x0069D180
    void clear() { erase(Start, Finish); }
};
class CCamera { public: unsigned char Data[0x38]; };
struct CCameraModeBase;
struct CScriptModuleBase;
class CGameCameraManager { public:
    virtual void v00();   // slot 0 (+0x0)
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026();
    virtual void SetThingToFollow(CThing* thing);   // slot 27 (+0x6c)
    virtual void SetThingToLookAt(CThing* thing);   // slot 28 (+0x70)
    CCamera                             Camera;                  // +0x04
    CCamera                             OldCamera;               // +0x3c
    void*                               World;                   // +0x74
    unsigned char                       CamTools[0x20];          // +0x78
    CCountedPointer<CCameraModeBase>    CurrentMode;             // +0x98
    CCountedPointer<CCameraModeBase>    PendingMode;             // +0xa0
    bool                                NewAutoMode;             // +0xa8
    bool                                ObservedThingInMelee;
    bool                                ObservedThingInBuilding;
    void*                               AvailableCamPoints;      // +0xac
    bool                                SingleCut;               // +0xb0
    bool                                ChangeInControlDirection;
    long                                InsideHeroCount;         // +0xb4
    float                               ControlAngleXY;          // +0xb8
    bool                                AngleChange;             // +0xbc
    bool                                ControlNoControlMovement;
    long                                ControlNoControlMovementStartFrame;   // +0xc0
    float                               RightStickLookAroundAngles[2];        // +0xc4
    float                               RightStickTargetLookAroundAngles[2];  // +0xcc
    CCountedPointer<CScriptModuleBase>  PCurrentScriptModule;    // +0xd4
    CCountedPointer<CScriptModuleBase>  PPendingScriptModule;    // +0xdc
    long                                ScriptedCameraRemoveTimer;   // +0xe4
    bool                                CameraPreload;           // +0xe8
    CQuakeVector                        EarthQuakes;             // +0xec
    float                               MinWorldHeight;          // +0xf8
    float                               MaxWorldHeight;          // +0xfc
    void ResetFlags();                                   // 0x00699130
    void PushCurrentManualMode();                        // 0x006988B0
    void RefreshMode();                                  // 0x0069A2A0
    void ResetCamera();
};
template <class T, long TYPE> __forceinline T* GetComponent(CThing* thing)
{
    T* result;
    if (thing->UsingComponents[TYPE >> 5] & (1u << (TYPE & 31))) {
        ComponentMap* map = &thing->Components;
        ComponentEntry* it = map->LowerBound(TYPE);
        ComponentEntry* end = map->Finish;
        result = (T*)((it == end || it->Type > TYPE) ? end : it)->Component;
    }
    return result;
}

void CGameCameraManager::ResetCamera()
{
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer().GetPControlledCreature();
    SetThingToFollow(hero);
    SetThingToLookAt(hero);
    MinWorldHeight = hero->GetPos().Z;
    MaxWorldHeight = MinWorldHeight + 10.0f;
    ResetFlags();
    if (hero->UsingComponents[4] & 2)
        GetComponent<CTCHeroRangedAttacks, 0x81>(hero)->ResetProjectileWeaponMode();
    CurrentMode.Reset();
    PCurrentScriptModule.Reset();
    PPendingScriptModule.Reset();
    EarthQuakes.clear();
    OldCamera = Camera;
    SingleCut = true;
    AngleChange = true;
    PushCurrentManualMode();
    RefreshMode();
    {
        bool fastMode = false;
        g_pMainGameComponent->DisplayEngine->Renderer->InvalidateCamera();
        CCharString message("telling engine that the camera has moved and");
        if (!fastMode)
            message += " not ";
        message += "using fast mode";
        g_pMainGameComponent->DisplayEngine->Renderer->SetCameraMoved(fastMode, true);
    }
    OldCamera = Camera;
}