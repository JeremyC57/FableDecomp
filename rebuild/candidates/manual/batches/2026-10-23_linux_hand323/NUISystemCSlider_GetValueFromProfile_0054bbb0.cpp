// NUISystem::CSlider::GetValueFromProfile() -- load the slider value (and, for the detail settings, the maximum) from the user-profile setting its default action names, then slide the thumb straight to the value (no tween) and move the default state's position with it
// Leaf: GetValueFromProfile
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054BBB0, 1021 bytes + two jump tables, plain `ret`.
// Callees: CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0; profile getters Gamma 0x00437470, HUDAlpha 0x00437490, SoundVolume 0x0047CFC2,
//   DialogueVolume 0x0047CFD3, MusicVolume 0x0054A4F0, CameraSensitivity 0x0054A510, TextureDetail 0x0054A530, EffectsDetail 0x0054A570,
//   MeshDetail 0x0054A590 (ShadowDetail inline), maxima 0x00408550 (shadow) / 0x00408500 (texture) / 0x004085A0 (mesh) / 0x004084B0 (effects);
//   ChangePositionDelta (slot 32, +0x80), STLport map<unsigned long, CUIState>::operator[] 0x0042D5B1. Pool 0.0f 0x0122DEDC.
// Model: the landed CSlider::Update 0x0054BA20 and AdjustValueToProfile 0x0054A6F0.
#include <math.h>
struct C2DVector {
    float X, Y;
    C2DVector& operator+=(const C2DVector& v) { X += v.X; Y += v.Y; return *this; }
};
struct CUIState {
    unsigned long StateChangeFlag;   // +0x00
    long          StateChangeType;   // +0x04
    C2DVector     Position;          // +0x08
};
struct CStateMap {                   // STLport map<unsigned long, CUIState> (12 bytes)
    void* Header; unsigned long NodeCount; unsigned long Compare;
    CUIState& operator[](const unsigned long& key);   // 0x0042D5B1
};
struct CChangingStateComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31();
    virtual void ChangePositionDelta(const C2DVector& delta, float time, bool linear);   // slot 32 (+0x80), 0x0052EA30
    unsigned char _pad04[0x20];      // +0x04
    CStateMap     States;            // +0x24
    float         Time;              // +0x30
    C2DVector     Position;          // +0x34
    C2DVector     TargetPosition;    // +0x3c
    void Update(float dt);           // 0x0052C7E0
};
#pragma pack(push, 1)
struct CSlider : CChangingStateComponent {
    unsigned char _pad44[0x184 - 0x44];
    C2DVector Dimensions;            // +0x184
    C2DVector Min;                   // +0x18c
    C2DVector Max;                   // +0x194
    C2DVector Step;                  // +0x19c
    C2DVector Value;                 // +0x1a4
    bool      Move;                  // +0x1ac
    C2DVector OldValue;              // +0x1ad
    C2DVector StartValue;            // +0x1b5
    unsigned char _pad1bd[0x1d4 - 0x1bd];
    long      PDefAction;            // +0x1d4 EActionType
    void SetThumbToValue();
    void GetValueFromProfile();
};
#pragma pack(pop)

struct CUserProfileManager {
    unsigned char _pad00[0xc];
    bool  UseNewProfileValues;                   // +0x0c
    unsigned char _pad0d[0x40 - 0xd];
    float ShadowDetail;                          // +0x40
    unsigned char _pad44[0xfc - 0x44];
    float ShadowDetailNewProfile;                // +0xfc
    static CUserProfileManager* Get();           // 0x0040D2A0
    float GetGamma() const;                      // 0x00437470
    float GetHUDAlpha() const;                   // 0x00437490
    float GetSoundVolume() const;                // 0x0047CFC2
    float GetDialogueVolume() const;             // 0x0047CFD3
    float GetMusicVolume() const;                // 0x0054A4F0
    float GetCameraSensitivity() const;          // 0x0054A510
    float GetTextureDetail() const;              // 0x0054A530
    float GetEffectsDetail() const;              // 0x0054A570
    float GetMeshDetail() const;                 // 0x0054A590
    float GetMaxShadowDetail() const;            // 0x00408550
    float GetMaxTextureDetail() const;           // 0x00408500
    float GetMaxMeshDetail() const;              // 0x004085A0
    float GetMaxEffectsDetail() const;           // 0x004084B0
    float GetShadowDetail() const { return UseNewProfileValues ? ShadowDetailNewProfile : ShadowDetail; }
};
static inline float SliderPixels(float value, float minimum, float dimension, float range)
{
    float offset = (value - minimum) * dimension;
    return offset / range;
}
__forceinline void CSlider::SetThumbToValue()
{
    C2DVector delta;
    float rangeX = Max.X - Min.X;
    if (rangeX != 0.0f)
        delta.X = SliderPixels(Value.X, Min.X, Dimensions.X, rangeX) - Position.X;
    else
        delta.X = 0.0f;
    float rangeY = Max.Y - Min.Y;
    if (rangeY != 0.0f)
        delta.Y = SliderPixels(Value.Y, Min.Y, Dimensions.Y, rangeY) - Position.Y;
    else
        delta.Y = 0.0f;
    ChangePositionDelta(delta, 0.0f, false);
    unsigned long key = 0;
    States[key].Position += delta;
}
void CSlider::GetValueFromProfile()
{
    if (PDefAction != 0) {
        switch (PDefAction) {
        case 0x29:
            Value.X = OldValue.X = StartValue.X = CUserProfileManager::Get()->GetSoundVolume();
            SetThumbToValue();
            break;
        case 0x58:
            Value.X = OldValue.X = StartValue.X = CUserProfileManager::Get()->GetDialogueVolume();
            SetThumbToValue();
            break;
        case 0x2a:
            Value.X = OldValue.X = StartValue.X = CUserProfileManager::Get()->GetMusicVolume();
            SetThumbToValue();
            break;
        case 0x4e:
            Value.X = OldValue.X = StartValue.X = CUserProfileManager::Get()->GetHUDAlpha();
            SetThumbToValue();
            break;
        case 0x28:
            Value.X = OldValue.X = StartValue.X = CUserProfileManager::Get()->GetGamma();
            SetThumbToValue();
            break;
        case 0x103:
            Value.X = OldValue.X = StartValue.X = CUserProfileManager::Get()->GetShadowDetail();
            Max.X = CUserProfileManager::Get()->GetMaxShadowDetail();
            SetThumbToValue();
            break;
        case 0x10a:
            Value.X = OldValue.X = StartValue.X = CUserProfileManager::Get()->GetTextureDetail();
            Max.X = CUserProfileManager::Get()->GetMaxTextureDetail();
            SetThumbToValue();
            break;
        case 0x13e:
            Value.X = OldValue.X = StartValue.X = CUserProfileManager::Get()->GetCameraSensitivity();
            SetThumbToValue();
            break;
        case 0x12a:
            Value.X = OldValue.X = StartValue.X = CUserProfileManager::Get()->GetMeshDetail();
            Max.X = CUserProfileManager::Get()->GetMaxMeshDetail();
            SetThumbToValue();
            break;
        case 0x12b:
            Value.X = OldValue.X = StartValue.X = CUserProfileManager::Get()->GetEffectsDetail();
            Max.X = CUserProfileManager::Get()->GetMaxEffectsDetail();
            SetThumbToValue();
            break;
        }
    }
}
