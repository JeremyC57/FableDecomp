// Leaf: AdjustValueToProfile
// Grade: RELOCATION_MATCH 996/996 on the corrected span 0x00548B50+996 (code + embedded jump table; bounds audit) -- catalog row 1616 B held a second function at 0x00548F40; really NUISystem::CTextSlider::AdjustValueToProfile (catalog: CSlider)
// NUISystem::CTextSlider::AdjustValueToProfile() -- read the text slider's selected child from the user-profile setting its default action names; when it changed, flag a move, stamp the input time, light the accept button (observe the accept event, full alpha) and play the front-end text-slider sound
// Retail: Fable.exe (TLC, VC7.1) @ 0x00548B50, 682 bytes, plain `ret`; two jump tables (0x2B..0x2F; 0x54..0x143 via a byte index table).
// Callees: CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0 and the profile getters (each `UseNewProfileValues ? <x>NewProfile : <x>`;
//   manifest labels them IsCameraResetting / PeekConnectedToTrackNode): see the declarations; TutorialSystemEnabled and NewProfileToUseWASD
//   are read inline. CFrontEndManager::GetInstance 0x0041E5F2, its PlaySound (+0xa0) and GetDef (+0xa8, CDefPointer by value).
// Label correction: catalog class "CSlider" -- the layout (SelectedChild +0x15c, PreviouslySelectedChild +0x160, Move +0x168,
//   LastInputTime +0x180, PDefAction +0x188, PAcceptButton +0x198) is NUISystem::CTextSlider (landed Initialise 0x00549230).
// Model: the landed CSlider::AdjustValueToProfile 0x0054A6F0 (button / front-end manager / def pointer); profile field names from Ego_r.pdb.
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
    unsigned char _pad08[0x1a0 - 8];
    CSoundName    TextSliderSound;               // +0x1a0 (descriptive)
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
    unsigned char _pad0d[0x17 - 0xd];
    bool  TutorialSystemEnabled;                 // +0x17
    unsigned char _pad18[0x78 - 0x18];
    bool  NewProfileToUseWASD;                   // +0x78
    unsigned char _pad79[0xd7 - 0x79];
    bool  TutorialSystemEnabledNewProfile;       // +0xd7
    static CUserProfileManager* Get();           // 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get
    bool IsCameraXYInverted() const;             // 0x00548590
    bool IsCameraYZInverted() const;             // 0x005485B0
    bool IsVibrationEnabled() const;             // 0x00548570
    bool IsUsingHeadPhones() const;              // 0x00548630
    bool IsSubtitlingEnabled() const;            // 0x0048DB35
    bool IsGuildMasterEnabled() const;           // 0x005485F0
    bool IsHUDTooltipEnabled() const;            // 0x00548610
    bool IsShowingBuddyNames() const;            // 0x00548650
    unsigned long GetResolution() const;         // 0x005486B0
    unsigned long GetRefreshRate() const;        // 0x005486D0
    unsigned long GetAntialiasing() const;       // 0x00548670
    unsigned long GetScreenAspectRatio() const;  // 0x005486F0
    bool IsVerticalSyncEnabled() const;          // 0x00548710
    bool IsCameraResetting() const;              // 0x00548690
    bool IsCameraPCBowYZInverted() const;        // 0x005485D0
    bool IsShowTargetingStatus() const;          // 0x00548740
    bool IsTutorialSystemEnabled() const { return UseNewProfileValues ? TutorialSystemEnabledNewProfile : TutorialSystemEnabled; }
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
namespace NUISystem {
class CTextSlider { public:
    void* __vftable;
    unsigned char _pad04[0x30 - 4];
    float m_time;                                // +0x30 frame time (draggable base)
    unsigned char _pad34[0x15c - 0x34];
    unsigned long SelectedChild;                 // +0x15c
    unsigned long PreviouslySelectedChild;       // +0x160
    unsigned long StartSelectedChild;            // +0x164
    bool Move;                                   // +0x168
    unsigned char _pad169[0x180 - 0x169];
    float LastInputTime;                         // +0x180
    float InputDelay;                            // +0x184
    long PDefAction;                             // +0x188 EActionType
    CComponent* PLeftArrow;                      // +0x18c
    CComponent* PRightArrow;                     // +0x190
    unsigned long OriginalSelectedChild;         // +0x194
    CComponent* PAcceptButton;                   // +0x198
    void AdjustValueToProfile();
};

void CTextSlider::AdjustValueToProfile()
{
    PreviouslySelectedChild = SelectedChild;
    if (PDefAction != 0) {
        switch (PDefAction) {
        case 0x2b:  SelectedChild = CUserProfileManager::Get()->IsCameraXYInverted(); break;
        case 0x2c:  SelectedChild = CUserProfileManager::Get()->IsCameraYZInverted(); break;
        case 0x2d:  SelectedChild = CUserProfileManager::Get()->IsVibrationEnabled(); break;
        case 0x2e:  SelectedChild = CUserProfileManager::Get()->IsUsingHeadPhones(); break;
        case 0x2f:  SelectedChild = CUserProfileManager::Get()->IsSubtitlingEnabled(); break;
        case 0x53:  SelectedChild = CUserProfileManager::Get()->IsTutorialSystemEnabled(); break;
        case 0x5d:  SelectedChild = CUserProfileManager::Get()->IsGuildMasterEnabled(); break;
        case 0x5e:  SelectedChild = CUserProfileManager::Get()->IsHUDTooltipEnabled(); break;
        case 0x54:  SelectedChild = CUserProfileManager::Get()->IsShowingBuddyNames(); break;
        case 0x139: SelectedChild = CUserProfileManager::Get()->NewProfileToUseWASD != 0; break;
        case 0x102: SelectedChild = CUserProfileManager::Get()->GetResolution(); break;
        case 0x13d: SelectedChild = CUserProfileManager::Get()->GetRefreshRate(); break;
        case 0x109: SelectedChild = CUserProfileManager::Get()->GetAntialiasing(); break;
        case 0x12c: SelectedChild = CUserProfileManager::Get()->GetScreenAspectRatio(); break;
        case 0x106: SelectedChild = CUserProfileManager::Get()->IsVerticalSyncEnabled(); break;
        case 0x140: SelectedChild = CUserProfileManager::Get()->IsCameraResetting(); break;
        case 0x13f: SelectedChild = CUserProfileManager::Get()->IsCameraPCBowYZInverted(); break;
        case 0x143: SelectedChild = CUserProfileManager::Get()->IsShowTargetingStatus(); break;
        }
    }
    if (PreviouslySelectedChild != SelectedChild) {
        Move = true;
        LastInputTime = m_time;
        if (PAcceptButton && !PAcceptButton->IsObservingEvent(0x19)) {
            PAcceptButton->ObserveEvent(0x19);
            CRGBColour colour = PAcceptButton->GetColour();
            colour.A = 0xff;
            PAcceptButton->ChangeColour(colour, 0.2f, false);
        }
        CFrontEndManager::GetInstance()->PlaySound(CFrontEndManager::GetInstance()->GetDef()->TextSliderSound, true);
    }
}
}