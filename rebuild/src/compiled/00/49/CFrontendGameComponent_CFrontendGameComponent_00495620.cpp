// CFrontendGameComponent::~CFrontendGameComponent() -- destroy the debug front end's members (splash texture, world/save/profile name lists and their selections, main font, temporary graphics) and the game-component base
// Leaf: ??1CFrontendGameComponent (Ego_r.pdb ??1CFrontendGameComponent@@UAE@XZ)
// Retail: Fable.exe @ 0x00495620, 344 bytes, ends in a tail jump to CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300.
// Body: vtables 0x01238768 / 0x0123875C (this class at +0 / +4); ~CTexture 0x009F9F70 (SplashScreen); ~CWideString
// 0x0099B510 (WorldToLoad, SaveToLoad and every vector element); free 0x00BFEA14 (each vector's storage); the counted
// pointers' inline release (destroy function, operator delete 0x00BFE9BC, then Data and Info zeroed); base vtables
// 0x0122ECB0 / 0x0122ECA4.
// Names: the manifest's NHeroInformationScreens CBase destructor label is stale (Ego_r.pdb ??1CFrontendGameComponent).
// Layout: retail CFrontendGameComponent: PTempGraphics +0x1DFC, MainFont +0x1E04, ProfileNames +0x1E0C,
// ProfileNamesOffset +0x1E18, SaveNames +0x1E1C, SaveNamesOffset +0x1E28, SaveToLoad +0x1E2C, WorldNames +0x1E30,
// WorldNamesOffset +0x1E3C, WorldToLoad +0x1E40, CursorHighlight +0x1E44, TimeLastMovementHappened +0x1E48,
// StartedRepeatingMovement +0x1E50, CurrentMovement +0x1E54, SplashScreen +0x1E58 (Ego_r.pdb less 4 per STLport vector);
// the game-component base model of the landed ~CNewFrontendGameComponent 0x0042F50B.
// Shape: the vector<CWideString> destructor is inline: STLport _Destroy(first, last) over parameter copies (finish
// cached in ebx), then free of the reloaded start; the counted
// pointer is the CText model (Release, then Data = Info = 0).
// Opt: /O2 (16-aligned entry).
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();                                      // 0x0099A300
};
struct CGameComponentInterface {
    virtual void v00();
};
class CGameComponent : public CBaseClassNonCopyable, public CGameComponentInterface { public:   // vtables 0x0122ECB0 / 0x0122ECA4
    bool m_bShutDown;                                      // +0x08
    virtual __forceinline ~CGameComponent() { }
};
class CCPPointerInfo { public:
    long  References;
    void  (__fastcall* Destroy)(void* object);
    void* Object;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    void Release()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
    }
    ~CCountedPointer() { Release(); Data = 0; Info = 0; }
};
class CWideString { public:
    void* PStringData;
    ~CWideString();                                        // 0x0099B510
};
struct CWideStringVector {                                 // STLport vector<CWideString>
    CWideString* _M_start;
    CWideString* _M_finish;
    CWideString* _M_end_of_storage;
    static __forceinline void _Destroy(CWideString* first, CWideString* last)
    {
        for (; first != last; ++first)
            first->~CWideString();
    }
    ~CWideStringVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
class CTexture { public:
    unsigned char _data[8];
    ~CTexture();                                           // 0x009F9F70
};
class CGraphicDataBank; class CFontBank;
enum ECursorMovement { CURSOR_MOVEMENT_NONE = 0 };
struct CFrontendGameComponent : CGameComponent {           // vtables 0x01238768 / 0x0123875C
    unsigned char                     _pad00c[0x1dfc - 0x0c];
    CCountedPointer<CGraphicDataBank> PTempGraphics;              // +0x1dfc
    CCountedPointer<const CFontBank>  MainFont;                   // +0x1e04
    CWideStringVector                 ProfileNames;               // +0x1e0c
    long                              ProfileNamesOffset;         // +0x1e18
    CWideStringVector                 SaveNames;                  // +0x1e1c
    long                              SaveNamesOffset;            // +0x1e28
    CWideString                       SaveToLoad;                 // +0x1e2c
    CWideStringVector                 WorldNames;                 // +0x1e30
    long                              WorldNamesOffset;           // +0x1e3c
    CWideString                       WorldToLoad;                // +0x1e40
    long                              CursorHighlight;            // +0x1e44
    double                            TimeLastMovementHappened;   // +0x1e48
    bool                              StartedRepeatingMovement;   // +0x1e50
    ECursorMovement                   CurrentMovement;            // +0x1e54
    CTexture                          SplashScreen;               // +0x1e58
    virtual ~CFrontendGameComponent();
};

CFrontendGameComponent::~CFrontendGameComponent()
{
}