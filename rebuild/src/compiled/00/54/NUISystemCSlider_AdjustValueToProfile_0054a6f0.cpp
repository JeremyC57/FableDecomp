// NUISystem::CSlider::AdjustValueToProfile() -- read the slider's value from the user profile setting its default action
//   names; when it moved, flag a move, light the accept button (observe the accept event, full alpha) and play the
//   front-end slider sound.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054A6F0, 672 bytes (+ int3 padding), plain `ret`.
// Leaf: AdjustValueToProfile
// Callees: CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0 and the profile getters (each `UseNewProfileValues ?
//   <x>NewProfile : <x>`; manifest labels them all GetCameraSensitivity): Gamma 0x00437470, HUDAlpha 0x00437490,
//   SoundVolume 0x0047CFC2, DialogueVolume 0x0047CFD3, MusicVolume 0x0054A4F0, CameraSensitivity 0x0054A510,
//   TextureDetail 0x0054A530, EffectsDetail 0x0054A570, MeshDetail 0x0054A590; ShadowDetail (+0x40 / +0xfc) is inline.
//   CFrontEndManager::GetInstance 0x0041E5F2, its PlaySound (+0xa0) and GetDef (+0xa8, CDefPointer by value).
// Layout: as the landed CSlider rows (Ego_r.pdb minus 0x30): Value +0x1A4, Move +0x1AC, OldValue +0x1AD, PDefAction +0x1D4,
//   PAcceptButton +0x1E8. CUserProfileManager retail offsets = Ego_r.pdb with 12-byte vectors (UseNewProfileValues +0xc).
//   The def model is the landed CMenuEntry::GoForward 0x0053D720 one; the slider sound is the def's +0x19c member.
// Switch: 0x103 is the split pivot; 0x28..0x58 go through byte index 0x0054A904 + table 0x0054A8EC = { 0x0054A782 Gamma,
//   0x0054A741 SoundVolume, 0x0054A763 MusicVolume, 0x0054A774 HUDAlpha, 0x0054A752 DialogueVolume, default } and
//   0x10a..0x13e through byte index 0x0054A94C + table 0x0054A938 = { 0x0054A7C1 TextureDetail, 0x0054A7DD MeshDetail,
//   0x0054A7EB EffectsDetail, 0x0054A7CF CameraSensitivity, default }. Each getter's profile field was checked against
//   the PDB layout (Gamma +0x1c, HUDAlpha +0x20, Sound +0x24, Music +0x28, Dialogue +0x2c, Texture +0x3c, Mesh +0x44,
//   Effects +0x70, Camera +0x7c). The arms sit in the bytes as Sound, Dialogue, Music, HUDAlpha, Gamma, Shadow, Texture,
//   Camera, Mesh, Effects, so the cases are written in that order (an earlier version listed them by value: same
//   behaviour, but every jump-table slot pointed at an arm calling a different getter than retail's).
#include <math.h>
class C2DVector { public: float X, Y; };
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(const CRGBColour& c) : B(c.B), G(c.G), R(c.R), A(c.A) {}
};
class CCPPointerInfo { public: long Count; };
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};
template <class T> struct CRetailVector {        // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};
struct IPrimary_0054a990 { virtual void v00(); };
struct CObserver {
    virtual void a00(); virtual void a01();
    virtual bool IsObservingEvent(long eventType);       // slot 2 (+0x08)
    virtual void ObserveEvent(long eventType);           // slot 3 (+0x0c)
    unsigned char _pad04[0x10];
};
struct ISecondaryB_0054a990 { virtual void b00(); };
struct CComponentPrimary_0054a990 {
    virtual void c00(); virtual void c01(); virtual void c02(); virtual void c03(); virtual void c04(); virtual void c05();
    virtual void c06(); virtual void c07(); virtual void c08(); virtual void c09(); virtual void c10(); virtual void c11();
    virtual void c12(); virtual void c13();
    virtual CRGBColour GetColour() const;                // slot 14 (+0x38)
    virtual void c15(); virtual void c16(); virtual void c17(); virtual void c18(); virtual void c19(); virtual void c20();
    virtual void c21(); virtual void c22(); virtual void c23(); virtual void c24(); virtual void c25(); virtual void c26();
    virtual void c27(); virtual void c28(); virtual void c29(); virtual void c30(); virtual void c31(); virtual void c32();
    virtual void c33(); virtual void c34(); virtual void c35(); virtual void c36(); virtual void c37();
    virtual void ChangeColour(const CRGBColour& colour, float time, bool linear);   // slot 38 (+0x98)
};
struct CComponent : CComponentPrimary_0054a990, CObserver {
};
struct CChangingStateComponent : IPrimary_0054a990, CObserver, ISecondaryB_0054a990 {
    unsigned char _pad1c[0x87 - 0x1c];
    bool Selectable;                                     // +0x87
    unsigned char _pad88[0x188 - 0x88];
    void ChangeState(unsigned long state);               // 0x0052CF40
};
namespace NUISystem { class CAction; }
struct CRefCounted {                             // reference count base at +4 of a def
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                      // slot 1
    CRefCounted Ref;                             // +0x04
    void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};
struct CSoundName { long Value; };
struct CFrontEndManagerDef : CDefBase {
    unsigned char _pad08[0x19c - 8];
    CSoundName    SliderSound;                   // +0x19c (descriptive)
};
template <class T> class CDefPointer { public:
    T* Data;
    ~CDefPointer()
    {
        if (Data)
            Data->ReleaseTemporary();
    }
    T* operator->() const { return Data; }
};
class CUserProfileManager { public:                     // retail = Ego_r.pdb with 12-byte vectors
    unsigned char _pad00[0xc];
    bool  UseNewProfileValues;                   // +0x0c
    unsigned char _pad0d[0x40 - 0xd];
    float ShadowDetail;                          // +0x40
    unsigned char _pad44[0xfc - 0x44];
    float ShadowDetailNewProfile;                // +0xfc
    static CUserProfileManager* Get();           // 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get
    float GetGamma() const;                      // 0x00437470
    float GetHUDAlpha() const;                   // 0x00437490
    float GetSoundVolume() const;                // 0x0047CFC2
    float GetDialogueVolume() const;             // 0x0047CFD3
    float GetMusicVolume() const;                // 0x0054A4F0
    float GetCameraSensitivity() const;          // 0x0054A510
    float GetTextureDetail() const;              // 0x0054A530
    float GetEffectsDetail() const;              // 0x0054A570
    float GetMeshDetail() const;                 // 0x0054A590
    float GetShadowDetail() const { return UseNewProfileValues ? ShadowDetailNewProfile : ShadowDetail; }
};
struct CFrontEndManager {
    virtual void v000(); virtual void v001();
    virtual void AddObserver(CObserver* observer);       // slot 2 (+0x08)
    virtual void v003(); virtual void v004();
    virtual void RemoveObserver(CObserver* observer);    // slot 5 (+0x14)
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual void v011(); virtual void v012(); virtual void v013();
    virtual void ExecuteAction(const CCountedPointer<NUISystem::CAction>& action);   // slot 14 (+0x38)
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032();
    virtual long GetLastInputResult();                   // slot 33 (+0x84)
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void PlaySound(const CSoundName& sound, bool a);   // slot 40 (+0xa0)
    virtual void v041();
    virtual CDefPointer<CFrontEndManagerDef> GetDef();   // slot 42 (+0xa8)
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
struct CSliderValue { unsigned char _pad[8]; float Value; };
struct CSliderTarget { unsigned char _pad[8]; CSliderValue* Value; };
struct CSliderTargetRef { CSliderTarget* Target; };
namespace NUISystem {
class CAction { public:
    virtual void v00();
    CSliderTargetRef* PTarget;                           // +0x04
};
static __forceinline bool Differs(const C2DVector& a, const C2DVector& b)
{
    return !(fabs(a.X - b.X) <= 0.0001f) || !(fabs(a.Y - b.Y) <= 0.0001f);
}
#pragma pack(push, 1)
class CSlider : public CChangingStateComponent { public:
    unsigned char _pad188[0x18c - 0x188];
    C2DVector     Min;                           // +0x18c
    C2DVector     Max;                           // +0x194
    C2DVector     Step;                          // +0x19c
    C2DVector     Value;                         // +0x1a4
    bool          Move;                          // +0x1ac
    C2DVector     OldValue;                      // +0x1ad
    C2DVector     StartValue;                    // +0x1b5
    unsigned char _pad1bd[3];
    CRetailVector<CCountedPointer<CAction> > Actions;   // +0x1c0
    unsigned char _pad1cc[0x1d4 - 0x1cc];
    long          PDefAction;                    // +0x1d4 EActionType
    unsigned char _pad1d8[0x1e0 - 0x1d8];
    C2DVector     PreviousValue;                 // +0x1e0
    CComponent*   PAcceptButton;                 // +0x1e8
    virtual void d00(); virtual void d01(); virtual void d02(); virtual void d03(); virtual void d04(); virtual void d05();
    virtual void d06(); virtual void d07(); virtual void d08(); virtual void d09(); virtual void d10(); virtual void d11();
    virtual void d12(); virtual void d13(); virtual void d14(); virtual void d15(); virtual void d16(); virtual void d17();
    virtual void d18(); virtual void d19(); virtual void d20(); virtual void d21(); virtual void d22(); virtual void d23();
    virtual void d24(); virtual void d25(); virtual void d26(); virtual void d27(); virtual void d28(); virtual void d29();
    virtual void d30(); virtual void d31(); virtual void d32(); virtual void d33(); virtual void d34(); virtual void d35();
    virtual void d36(); virtual void d37(); virtual void d38(); virtual void d39(); virtual void d40(); virtual void d41();
    virtual void d42(); virtual void d43(); virtual void d44(); virtual void d45(); virtual void d46(); virtual void d47();
    virtual void d48(); virtual void d49(); virtual void d50(); virtual void d51(); virtual void d52(); virtual void d53();
    virtual void d54(); virtual void d55(); virtual void d56(); virtual void d57(); virtual void d58(); virtual void d59();
    virtual void d60(); virtual void d61(); virtual void d62(); virtual void d63(); virtual void d64(); virtual void d65();
    virtual void d66(); virtual void d67(); virtual void d68(); virtual void d69(); virtual void d70(); virtual void d71();
    virtual void d72(); virtual void d73(); virtual void d74(); virtual void d75(); virtual void d76(); virtual void d77();
    virtual void d78(); virtual void d79(); virtual void d80(); virtual void d81(); virtual void d82(); virtual void d83();
    virtual void d84(); virtual void d85(); virtual void d86(); virtual void d87(); virtual void d88(); virtual void d89();
    virtual void d90(); virtual void d91(); virtual void d92(); virtual void d93(); virtual void d94(); virtual void d95();
    virtual void d96(); virtual void d97(); virtual void d98(); virtual void d99(); virtual void d100(); virtual void d101();
    virtual void d102(); virtual void d103(); virtual void d104(); virtual void d105(); virtual void d106(); virtual void d107();
    virtual void d108(); virtual void d109(); virtual void d110(); virtual void d111(); virtual void d112(); virtual void d113();
    virtual void d114(); virtual void d115(); virtual void d116(); virtual void d117(); virtual void d118(); virtual void d119();
    virtual void d120(); virtual void d121(); virtual void d122(); virtual void d123(); virtual void d124(); virtual void d125();
    virtual void d126(); virtual void d127(); virtual void d128(); virtual void d129(); virtual void d130(); virtual void d131();
    virtual void d132(); virtual void d133(); virtual void d134(); virtual void d135(); virtual void d136(); virtual void d137();
    virtual void d138(); virtual void d139(); virtual void d140(); virtual void d141(); virtual void d142(); virtual void d143();
    virtual void d144(); virtual void d145(); virtual void d146(); virtual void d147(); virtual void d148();
    virtual void UpdateValue();                  // slot 150 (+0x258)
    void ChangeState(unsigned long state);
    void AdjustValueToProfile();
};
#pragma pack(pop)

void CSlider::AdjustValueToProfile()
{
    OldValue = Value;
    if (PDefAction != 0) {
        switch (PDefAction) {
        case 0x29:  Value.X = CUserProfileManager::Get()->GetSoundVolume(); break;
        case 0x58:  Value.X = CUserProfileManager::Get()->GetDialogueVolume(); break;
        case 0x2a:  Value.X = CUserProfileManager::Get()->GetMusicVolume(); break;
        case 0x4e:  Value.X = CUserProfileManager::Get()->GetHUDAlpha(); break;
        case 0x28:  Value.X = CUserProfileManager::Get()->GetGamma(); break;
        case 0x103: Value.X = CUserProfileManager::Get()->GetShadowDetail(); break;
        case 0x10a: Value.X = CUserProfileManager::Get()->GetTextureDetail(); break;
        case 0x13e: Value.X = CUserProfileManager::Get()->GetCameraSensitivity(); break;
        case 0x12a: Value.X = CUserProfileManager::Get()->GetMeshDetail(); break;
        case 0x12b: Value.X = CUserProfileManager::Get()->GetEffectsDetail(); break;
        }
    }
    if (Differs(OldValue, Value)) {
        Move = true;
        if (PAcceptButton && !PAcceptButton->IsObservingEvent(0x19)) {
            PAcceptButton->ObserveEvent(0x19);
            CRGBColour colour = PAcceptButton->GetColour();
            colour.A = 0xff;
            PAcceptButton->ChangeColour(colour, 0.2f, false);
        }
        CFrontEndManager::GetInstance()->PlaySound(CFrontEndManager::GetInstance()->GetDef()->SliderSound, true);
    }
}
}