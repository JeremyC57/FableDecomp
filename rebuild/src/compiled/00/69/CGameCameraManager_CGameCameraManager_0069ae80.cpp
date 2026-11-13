// CGameCameraManager::CGameCameraManager(const CWorld&) -- build the game camera manager: default camera, member defaults, add the six built-in camera modes to the mode map, look up the null camera-mode def and install the default camera-manager set
// Leaf: ??0CGameCameraManager@@QAE@ABVCWorld@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069AE80, 1923 bytes (to the last null-path jump 0x0069B5FE; `ret 4` at 0x0069B5BE), no EH frame.
// Name: Ego_r.pdb ??0CGameCameraManager@@QAE@ABVCWorld@@@Z (0x006C1E60) + the CGameCameraManager vtable 0x0125C754 stored first.
// Callees: CCamera::CCamera(const C3DVector&, const C3DVector&, const C3DVector&, float, float) 0x00A0C130, CCamera::CCamera(const
//   CCamera&) 0x0049D380, NCameraModes::CTools::CTools(const CGameCameraManager&) 0x00877160, malloc 0x00BFEA0E (STLport list / tree
//   head nodes), CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, operator new 0x00BFEA1A, the RegisteredModes
//   insert_unique 0x0069D900, operator delete 0x00BFE9BC, NCameraModes::CProjectileWeapon::StatRegisterMode 0x00878A50,
//   CCharString::CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CDefinitionManager::GetDef<CCameraModeDef>(const
//   CCharString&, CDefPointer<const CCameraModeDef>*) const 0x00637650, GetDef<CCameraManagerSetDef>(..) const 0x004AB990,
//   CGameCameraManager::SetCameraManagerSetDef 0x0069AC90; CCountedPointer<NCameraModes::CBaseInit>::DeleteData 0x0069BBA0 (stored).
// Globals: DEFAULT_CAMERA_MODE_DEF 0x013BA8D8 (anonymous namespace, both PDBs' dynamic initializer / atexit names), the two bytes
//   0x013BA8D4 / 0x013BA8D5 (descriptive names), strings 0x0124DFC8 "CAMERA_MODE_MODE_NULL", 0x01238F9C "CAMERA_MANAGER_SET_DEFAULT".
// Shape: member initialisers in declaration order (NewAutoMode, SingleCut, MinWorldHeight / MaxWorldHeight, the two list indices,
//   ScriptedCameraModeFromGame and DebugPersistFrame are left alone); the camera-set floats and flags in the body; each mode registered
//   with `RegisteredModes.insert(std::make_pair(T::StatGetModeType(), CCountedPointer<CBaseInit>(T::StatAlloc())))` (the counted
//   pointer temporary, make_pair's copy and the value_type conversion: three releases, in retail order 3, 4, 0xD, 0xE, 0x11, 0xB).
//   Visibility as in retail's TU: STLport's _tree.c insert_unique / _M_insert bodies (FABLE_RETAIL_STLP_MAP +
//   _MAP_ORIGINAL_INSERT_BODY; without them the value_type temporary escapes: `dec [eax] ; mov eax,[slot] ; cmp [eax],ebx` and two
//   dead zero stores per mode), the by-name GetDef<T> body and the same-TU SetCameraManagerSetDef (without them the set-def local gets
//   its own frame word, the frame is 0x28, and its release takes the temporary shape). CamZoomMinHeightFactor is 1.0f / 3.0f
//   (0x3EAAAAAB; 0.333333f gives 0x3EAAAA9F). Matched at grade 3 of pass 1.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCAMERA_CTORS
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_CTOR
#define FABLE_RETAIL_CGAMECAMERAMANAGER_FULL
#define FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_STRING_CAGE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_TYPED_COMPOSITES
#define FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGE_INIT
#include "fable_string.h"
#include "engine/CGameCameraManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/NCameraModes_CBase.h"
#include "engine/NCameraModes_CBaseInit.h"
#include "engine/NCameraModes_CStringAndCage.h"
#include "engine/CCameraManagerSetDef.h"

class CCameraModeDef;
class CCameraManagerSetDef;

namespace NCameraModes {
// The five registered modes with no shared header yet: only the nested factory and the two inline statics the constructor expands
// (FableWin.pdb ?StatAlloc@<Mode>@NCameraModes@@SIPAVCBaseInit@2@XZ / ?StatGetModeType@<Mode>@NCameraModes@@SI?AW4EMode@2@XZ;
// ?Alloc@<Mode>Init@<Mode>@NCameraModes@@UBE.. at the init vtables' slot 0). Members not modelled (header debt).
class CFirstPerson {
public:
    class CFirstPersonInit : public CBaseInit {
    public:
        virtual CBase* Alloc(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) const;   // 0x008738F0
    };
    static CBaseInit* FABLE_FASTCALL StatAlloc() { return new CFirstPersonInit; }             // vptr 0x0125C274
    static EMode FABLE_FASTCALL StatGetModeType() { return static_cast<EMode>(4); }
};
class CLookAtFront {
public:
    class CLookAtFrontInit : public CBaseInit {
    public:
        virtual CBase* Alloc(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) const;   // 0x00873930
    };
    static CBaseInit* FABLE_FASTCALL StatAlloc() { return new CLookAtFrontInit; }             // vptr 0x0125C27C
    static EMode FABLE_FASTCALL StatGetModeType() { return static_cast<EMode>(0xd); }
};
class CSACCombat {
public:
    class CSACCombatInit : public CBaseInit {
    public:
        virtual CBase* Alloc(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) const;   // 0x008731B0
    };
    static CBaseInit* FABLE_FASTCALL StatAlloc() { return new CSACCombatInit; }               // vptr 0x0125C26C
    static EMode FABLE_FASTCALL StatGetModeType() { return static_cast<EMode>(0xe); }
};
class CPCMouseLook {
public:
    class CPCMouseLookInit : public CBaseInit {
    public:
        virtual CBase* Alloc(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) const;   // 0x00874770
    };
    static CBaseInit* FABLE_FASTCALL StatAlloc() { return new CPCMouseLookInit; }             // vptr 0x0125C284
    static EMode FABLE_FASTCALL StatGetModeType() { return static_cast<EMode>(0x11); }
};
class CZTargetNew {
public:
    class CZTargetNewInit : public CBaseInit {
    public:
        virtual CBase* Alloc(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def) const;   // 0x00873200
    };
    static CBaseInit* FABLE_FASTCALL StatAlloc() { return new CZTargetNewInit; }              // vptr 0x0125C264
    static EMode FABLE_FASTCALL StatGetModeType() { return static_cast<EMode>(0xb); }
};
class CProjectileWeapon {
public:
    static void FABLE_FASTCALL StatRegisterMode();                       // 0x00878A50 ?StatRegisterMode@CProjectileWeapon@NCameraModes@@SIXXZ
};
}

namespace { extern CDefPointer<const CCameraModeDef> DEFAULT_CAMERA_MODE_DEF; }   // 0x013BA8D8
extern bool g_CameraManagerDebugFlag;                                    // 0x013BA8D5 (descriptive; written only here)
extern bool g_PreloadScriptedCameras;                                    // 0x013BA8D4 (descriptive; SetScriptedCameraMode 0x00699F0F tests it)

// The out-of-line set-def installer that precedes the constructor in the retail TU (0x0069AC90, landed): its body is visible here, as
// in retail's TU, so VC7.1 knows the call only reads the constructor's camera-manager-set local (no capture: the local shares the
// dead parameter word and is released as `mov ecx,[esp+38h] ; dec [ecx+4] ; jne`, 0x0069B5A3).
#ifdef _MSC_VER
#pragma auto_inline(off)
__declspec(noinline)
#endif
void CGameCameraManager::SetCameraManagerSetDef(const CDefPointer<const CCameraManagerSetDef>& def)
{
    PCameraManagerSet = def;
    CDefPointer<const CCameraManagerDef> manager_def;
    World.PeekDefinitionManager().GetDef(def->CameraManagerPCMain, &manager_def);
    SetCameraManagerDef(manager_def);
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif

CGameCameraManager::CGameCameraManager(const CWorld& world)
    : Camera(C3DVector(0.0f, 0.0f, 0.0f), C3DVector(1.0f, 0.0f, 0.0f), C3DVector(0.0f, 0.0f, 1.0f), 0.19444f, 0.0f),
      OldCamera(Camera),
      World(world),
      CamTools(*this),
      ObservedThingInMelee(false),
      ObservedThingInBuilding(false),
      ChangeInControlDirection(true),
      InsideHeroCount(0),
      ControlAngleXY(0.0f),
      AngleChange(false),
      ControlNoControlMovement(true),
      ControlNoControlMovementStartFrame(0),
      RightStickLookAroundAngles(0.0f, 0.0f),
      RightStickTargetLookAroundAngles(0.0f, 0.0f),
      ScriptedCameraRemoveTimer(-1),
      CameraPreload(false),
      CameraPreloadUseFastMode(false),
      ScriptHasPreloadedCamera(false),
      UsingCameraManually(false),
      BlockModeChange(false),
      ScriptedCameraPreloadFlag(false),
      DrawDebugGraphics(0)
{
    CamRSLookAroundDeltaTheta = 0.03f;
    CamZoomMinHeightFactor = 1.0f / 3.0f;
    AutoFOVZoomInCombat = true;
    CamAutoGoBehind = false;
    LimitMinMaxWorldHeight = false;
    g_CameraManagerDebugFlag = false;
    g_PreloadScriptedCameras = false;

    RegisteredModes.insert(std::make_pair(NCameraModes::CStringAndCage::StatGetModeType(), CCountedPointer<NCameraModes::CBaseInit>(NCameraModes::CStringAndCage::StatAlloc())));
    RegisteredModes.insert(std::make_pair(NCameraModes::CFirstPerson::StatGetModeType(), CCountedPointer<NCameraModes::CBaseInit>(NCameraModes::CFirstPerson::StatAlloc())));
    RegisteredModes.insert(std::make_pair(NCameraModes::CLookAtFront::StatGetModeType(), CCountedPointer<NCameraModes::CBaseInit>(NCameraModes::CLookAtFront::StatAlloc())));
    RegisteredModes.insert(std::make_pair(NCameraModes::CSACCombat::StatGetModeType(), CCountedPointer<NCameraModes::CBaseInit>(NCameraModes::CSACCombat::StatAlloc())));
    RegisteredModes.insert(std::make_pair(NCameraModes::CPCMouseLook::StatGetModeType(), CCountedPointer<NCameraModes::CBaseInit>(NCameraModes::CPCMouseLook::StatAlloc())));
    RegisteredModes.insert(std::make_pair(NCameraModes::CZTargetNew::StatGetModeType(), CCountedPointer<NCameraModes::CBaseInit>(NCameraModes::CZTargetNew::StatAlloc())));

    NCameraModes::CProjectileWeapon::StatRegisterMode();

    const CGameDefinitionManager& definition_manager = world.PeekDefinitionManager();
    definition_manager.GetDef("CAMERA_MODE_MODE_NULL", &DEFAULT_CAMERA_MODE_DEF);
    CDefPointer<const CCameraManagerSetDef> camera_manager_set;
    definition_manager.GetDef("CAMERA_MANAGER_SET_DEFAULT", &camera_manager_set);
    CameraListIndex = InitialModeCameraListIndex = CameraList.end();
    SpecialCameraListIndex = SpecialCameraList.end();
    SetCameraManagerSetDef(camera_manager_set);
}