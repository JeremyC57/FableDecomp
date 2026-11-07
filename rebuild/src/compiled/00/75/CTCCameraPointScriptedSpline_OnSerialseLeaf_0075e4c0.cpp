// CTCCameraPointScriptedSpline::OnSerialseLeaf(CPersistContext& context) -- persist the spline camera: TimeToPlay / Tension, then (save) the valid animation names and every key camera relative to the thing, or (load) rebuild the key cameras from the saved fields
// Leaf: ?OnSerialseLeaf@CTCCameraPointScriptedSpline@@UAEXAAVCPersistContext@@@Z
// Grade: RELOCATION_MATCH (2316 bytes, 0 reloc-masked byte differences, every relocation paired with retail by relpair).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075E4C0, 2316 bytes, thiscall `ret 4`, virtual slot 33 of vtable 0x0126A0BC (slot address
//   0x0126A140; RTTI .?AVCTCCameraPointScriptedSpline@@ : CTCCameraPointDefinitionBase : CTCBase).
// Name (catalog placeholder CTCCameraPointScriptedSpline::vfunc_33): Ego_r.pdb / FableWin.pdb ?OnSerialseLeaf@CTCCameraPointScriptedSpline@@
//   UAEXAAVCPersistContext@@@Z (sic "Serialse", one CPersistContext& argument = `ret 4`); slot 33 of the base vtable 0x01269C84 is the
//   landed CTCCameraPointScripted::OnSerialseLeaf 0x0075D320, and the sibling overrides CTCCameraPointGeneralCase / Track::OnSerialseLeaf
//   0x0075D1C0 / 0x0075D0F0 are landed in the same slot; the body transfers the spline's own fields ("TimeToPlay", "Tension", "ValidAnims",
//   "NumKeyCameras", "KeyCameras[i].*").
// Callees: CPersistContext::Transfer<float> 0x00410620 / <long> 0x00410BE0 / <C3DVector> 0x004859AD / <CCharString> 0x004109A0,
//   TransferVector<CCharString> 0x0049B8D0, CTCBase::GetWorld 0x00686A70, CBankFile::GetEntrySymbol 0x009CC1D0 (world +0x3c mesh bank),
//   vector<CCharString>::_M_insert_overflow 0x00433530, GFIntToCharString 0x0099F830, operator+ 0x0099F690 / 0x0099F600,
//   CCharString::operator const char* 0x0099E4C0 / CCharString() 0x0099E4B0 / copy 0x0099EC30 / operator= 0x0099EFB0 / ~ 0x0099EAE0,
//   CThing::GetPos 0x004C73D0, CKeyCamera(pos, look, fov, roll, shuttle, duration, pause) 0x0075F180 / copy 0x0075F1F0,
//   vector<CKeyCamera>::_M_insert_overflow 0x0075F860, free 0x00BFEA14.
// Layout (Ego_r.pdb CTCCameraPointScriptedSpline 0xdc with retail STLport sizes, sizeof 0xcc): KeyCameras +0x68 (0x38-byte CKeyCamera),
//   SelectedKeyCamera +0x74, MyMessages +0x98 (4-byte list), MessageToBeSent +0x9c, Tension +0xa4, TimeToPlay +0xac, ValidAnimList +0xc0
//   (set<long>, header node pointer); offsets confirmed by the retail reads [edi+0xac] / [edi+0xa4] / [edi+0xc0] / [edi+0x68] / [eax+0x74].
// Shape: the two-argument Transfer is FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS (name converted first, then GetDefaultVal); the
//   Transfer<T> switch bodies sit above the caller under `#pragma auto_inline(off)` (chunk 824) so VC7.1 hoists the zero default
//   temporaries of both key loops into their preheaders; the load arm's AnimationSpeed / RollAngle are explicit three-argument
//   Transfers (their pushes precede the name build, 0x0075EC90 / 0x0075ECC7); the anim index and the world are named locals before
//   GetEntrySymbol, whose result is a named CCharString (the push_back copy re-addresses it with `lea`); the save index is
//   `GFIntToCharString(index++)`.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/C3DVector.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableComponent.h"

CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830

class CBankFile { public:
    CCharString GetEntrySymbol(unsigned long index) const;                             // 0x009CC1D0
};
class CMeshDataBank : public CBankFile { };
class CWorld { public:
    unsigned char _pad00[0x3c];
    CMeshDataBank* PMainMeshBank;                                                      // +0x3c
};

template <> inline CCharString __fastcall CPersistContext::GetDefaultVal<CCharString>() { CCharString value; return value; }
template <> inline float __fastcall CPersistContext::GetDefaultVal<float>() { return 0.0f; }
template <> inline C3DVector __fastcall CPersistContext::GetDefaultVal<C3DVector>() { return C3DVector(0.0f, 0.0f, 0.0f); }

#pragma auto_inline(off)
template <> void CPersistContext::Transfer<float>(const char* name, float& value, const float& defaultValue)
{
    CheckCRC(name);
    switch (Mode) {
    case MODE_CLEAR:
        value = defaultValue;
        break;
    case MODE_SAVE_BINARY:
        PSaveStream->WriteFloat(value);
        break;
    case MODE_LOAD_BINARY: {
        CPersistTraits<float> traits;
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    }
    case MODE_SAVE_TEXT: {
        CPersistContext_TransferableComponent<float> item(value);
        TransferObjectSaveText(name, item);
        break;
    }
    case MODE_LOAD_TEXT: {
        if (UseDefaults)
            value = defaultValue;
        CPersistContext_TransferableComponent<float> item(value);
        TransferObjectLoadText(name, item);
        break;
    }
    default:
        __assume(0);
    }
}
template <> void CPersistContext::Transfer<C3DVector>(const char* name, C3DVector& value, const C3DVector& defaultValue)
{
    CheckCRC(name);
    switch (Mode) {
    case MODE_CLEAR:
        value = defaultValue;
        break;
    case MODE_SAVE_BINARY:
        PSaveStream->Write3DVector(value);
        break;
    case MODE_SAVE_TEXT: {
        CPersistContext_TransferableComponent<C3DVector> item(value);
        TransferObjectSaveText(name, item);
        break;
    }
    case MODE_LOAD_TEXT: {
        if (UseDefaults)
            value = defaultValue;
        CPersistContext_TransferableComponent<C3DVector> item(value);
        TransferObjectLoadText(name, item);
        break;
    }
    default:
        __assume(0);
    }
}
template <> void CPersistContext::Transfer<long>(const char* name, long& value, const long& defaultValue)
{
    CheckCRC(name);
    switch (Mode) {
    case MODE_CLEAR:
        value = defaultValue;
        break;
    case MODE_SAVE_BINARY:
        PSaveStream->WriteSLONG(value);
        break;
    case MODE_LOAD_BINARY: {
        CPersistTraits<long> traits;
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    }
    case MODE_SAVE_TEXT: {
        CPersistContext_TransferableComponent<long> item(value);
        TransferObjectSaveText(name, item);
        break;
    }
    case MODE_LOAD_TEXT: {
        if (UseDefaults)
            value = defaultValue;
        CPersistContext_TransferableComponent<long> item(value);
        TransferObjectLoadText(name, item);
        break;
    }
    default:
        __assume(0);
    }
}
template <> void CPersistContext::Transfer<CCharString>(const char* name, CCharString& value, const CCharString& defaultValue)
{
    CheckCRC(name);
    switch (Mode) {
    case MODE_CLEAR:
        value = defaultValue;
        break;
    case MODE_SAVE_BINARY:
        PSaveStream->WriteNullTerminatedString(value);
        break;
    case MODE_SAVE_TEXT: {
        CPersistContext_TransferableComponent<CCharString> item(value);
        TransferObjectSaveText(name, item);
        break;
    }
    case MODE_LOAD_TEXT: {
        if (UseDefaults)
            value = defaultValue;
        CPersistContext_TransferableComponent<CCharString> item(value);
        TransferObjectLoadText(name, item);
        break;
    }
    default:
        __assume(0);
    }
}
#pragma auto_inline(on)

class CCardinalSpline;

class CTCCameraPointDefinitionBase : public CTCBase {
public:
    unsigned char _base0c[0x40 - 0x0c];                // CutInto .. Activated (Ego_r members, +0x0c..+0x3c)
};

class CTCCameraPointScriptedSpline : public CTCCameraPointDefinitionBase {
public:
    class CKeyCamera {
    public:
        float       DeltaIndex;                        // +0x00
        C3DVector   Pos;                               // +0x04
        C3DVector   LookDirection;                     // +0x10
        float       FOV;                               // +0x1c
        float       Roll;                              // +0x20
        float       ShuttleSpeed;                      // +0x24
        float       Duration;                          // +0x28
        float       PauseTime;                         // +0x2c
        CCharString EventName;                         // +0x30
        float       SpeedFraction;                     // +0x34
        CKeyCamera(const C3DVector& pos, const C3DVector& lookDirection, float fov, float roll, float shuttleSpeed, float duration, float pauseTime);   // 0x0075F180
        CKeyCamera(const CKeyCamera& other);           // 0x0075F1F0
        const C3DVector& GetPosition() const { return Pos; }
        const C3DVector& GetLookDirection() const { return LookDirection; }
        float GetFOV() const { return FOV; }
        float GetRoll() const { return Roll; }
        float GetShuttleSpeed() const { return ShuttleSpeed; }
        float GetDuration() const { return Duration; }
        float GetPauseTime() const { return PauseTime; }
        CCharString GetEvent() const { return EventName; }
        float GetAnimationSpeed() const { return SpeedFraction; }
        void SetEvent(const CCharString& name) { EventName = name; }
        void SetAnimationSpeed(float speed) { SpeedFraction = speed; }
    };
    C3DVector EditPreviewPos;                          // +0x40
    C3DVector EditPreviewViewDirection;                // +0x4c
    float EditPreviewFOV;                              // +0x58
    float EditPreviewRoll;                             // +0x5c
    unsigned char PTimingSpline[8];                    // +0x60 CCountedPointer<CCardinalSpline>
    std::vector<CKeyCamera> KeyCameras;                // +0x68
    CKeyCamera* SelectedKeyCamera;                     // +0x74 vector iterator
    unsigned char PPositionSpline[8];                  // +0x78
    unsigned char PViewDirectionSpline[8];             // +0x80
    unsigned char PScalarSpline[8];                    // +0x88
    unsigned char PAnimationSpeedSpline[8];            // +0x90
    unsigned char MyMessages[4];                       // +0x98 list<CCharString>
    CCharString MessageToBeSent;                       // +0x9c
    bool NextCrossInvalid;                             // +0xa0
    float Tension;                                     // +0xa4
    float PauseFor;                                    // +0xa8
    float TimeToPlay;                                  // +0xac
    float EditPreviewIndex;                            // +0xb0
    long NodeToStartOn;                                // +0xb4
    long NodeToEndOn;                                  // +0xb8
    float AnimationSpeed;                              // +0xbc
    std::set<long> ValidAnimList;                      // +0xc0
    virtual void OnSerialseLeaf(CPersistContext& context);
};

void CTCCameraPointScriptedSpline::OnSerialseLeaf(CPersistContext& context)
{
    context.Transfer("TimeToPlay", TimeToPlay, 1.0f);
    context.Transfer("Tension", Tension, 0.8f);
    if (context.IsSaving()) {
        std::vector<CCharString> valid_anims;
        for (std::set<long>::iterator it = ValidAnimList.begin(); it != ValidAnimList.end(); ++it)
        {
            long anim = *it;
            CWorld& world = GetWorld();
            CCharString name = world.PMainMeshBank->GetEntrySymbol(anim);
            valid_anims.push_back(name);
        }
        context.TransferVector("ValidAnims", valid_anims);
        long num_key_cameras = KeyCameras.size();
        context.Transfer("NumKeyCameras", num_key_cameras, 0L);
        long index = 0;
        for (std::vector<CKeyCamera>::iterator key = KeyCameras.begin(); key != KeyCameras.end(); ++key) {
            CCharString prefix = "KeyCameras[" + GFIntToCharString(index++) + "].";
            float fov = key->GetFOV();
            float shuttle_speed = key->GetShuttleSpeed();
            C3DVector position = key->GetPosition() - ParentThing->GetPos();
            C3DVector look_direction = key->GetLookDirection();
            float duration = key->GetDuration();
            float pause_time = key->GetPauseTime();
            CCharString event = key->GetEvent();
            float animation_speed = key->GetAnimationSpeed();
            float roll = key->GetRoll();
            context.Transfer(prefix + "Position", position);
            context.Transfer(prefix + "LookDirection", look_direction);
            context.Transfer(prefix + "FOV", fov);
            context.Transfer(prefix + "ShuttleSpeed", shuttle_speed);
            context.Transfer(prefix + "Duration", duration);
            context.Transfer(prefix + "PauseTime", pause_time);
            context.Transfer(prefix + "Event", event);
            context.Transfer(prefix + "AnimationSpeed", animation_speed);
            context.Transfer(prefix + "RollAngle", roll);
        }
    } else {
        long num_key_cameras;
        context.Transfer("NumKeyCameras", num_key_cameras, 0L);
        for (long i = 0; i < num_key_cameras; ++i) {
            CCharString prefix = "KeyCameras[" + GFIntToCharString(i) + "].";
            C3DVector position;
            context.Transfer(prefix + "Position", position);
            C3DVector look_direction;
            context.Transfer(prefix + "LookDirection", look_direction);
            float fov;
            context.Transfer(prefix + "FOV", fov);
            float shuttle_speed;
            context.Transfer(prefix + "ShuttleSpeed", shuttle_speed);
            float duration;
            context.Transfer(prefix + "Duration", duration);
            float pause_time;
            context.Transfer(prefix + "PauseTime", pause_time);
            CCharString event;
            context.Transfer(prefix + "Event", event);
            float animation_speed;
            context.Transfer(prefix + "AnimationSpeed", animation_speed, 0.0f);
            float roll;
            context.Transfer(prefix + "RollAngle", roll, 0.0f);
            CKeyCamera key(position, look_direction, fov, roll, shuttle_speed, duration, pause_time);
            key.SetEvent(event);
            key.SetAnimationSpeed(animation_speed);
            KeyCameras.push_back(key);
            SelectedKeyCamera = KeyCameras.end();
        }
    }
}