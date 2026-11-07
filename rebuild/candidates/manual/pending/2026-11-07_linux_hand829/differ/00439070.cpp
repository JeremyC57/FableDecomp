// CPlayerGui::DrawPreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info) -- per-frame HUD build: gamma/alpha from the profile, gui colours to the engine, timers, every NPlayerGui illustrator, the debug TGA sprite
// Grade: DIFFER(2611v2613) -- 125 md lines, 2 bytes short: the fourth colour int temp uses one slot where retail uses a second ([esp+0x18]), so the frame-slot offsets shift by 4 (the PLAYER_GUI_PC CCharString at 0x18 retail vs 0x14 ours); in the TGA-sprite block retail stores the two zero C2DVector temps after the pushes, ours before the calls; handle Reset uses eax in retail vs ecx
// Next idea: give the four ToFloatColour results separate named CRGBFloatColour locals at function scope (not block scope) so each int temp gets its own slot; spell the zero C2DVectors as a named static or as C2DVector() default temporaries inside the AddPrimitive argument list
// Leaf: ?DrawPreparePrimitivesForEngine@CPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00439070, 2613 bytes, `ret 4`.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CRGBColour { public: unsigned char B, G, R, A; };
class CRGBFloatColour { public:
    float R, G, B, A;
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
};
inline CRGBFloatColour ToFloatColour(const CRGBColour& c) { return CRGBFloatColour(c.R * (1.0f / 255.0f), c.G * (1.0f / 255.0f), c.B * (1.0f / 255.0f), c.A * (1.0f / 255.0f)); }
class CPlayerGuiDef { public:
    unsigned char _pad[0x48];
    CRGBColour ColourA;                                 // +0x48
    CRGBColour ColourB;                                 // +0x4c
    CRGBColour ColourC;                                 // +0x50
    unsigned char Colours54[4];                         // +0x54
};
template <class T> class CDefPointer { public: T* Object; };
class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();    // 0x0044C6B0
    bool GetDef(const CCharString& name, CDefPointer<CPlayerGuiDef>* out);   // 0x0043FF30
};
class CInterpolationInfoSet { public: float Time; float Frac; };
class CInterpolationInfo { public:
    CInterpolationInfoSet Current, Last, PausedCurrent, PausedLast, BulletTimeCurrent, BulletTimeLast;
    long WFServerCurrentTime;
};
struct CHandleInfo;
class CEnginePrimitiveHandle { public: void* Data; CHandleInfo* Info; inline void Reset(); };
struct CHandleInfo { virtual void v0(); virtual void Delete(); virtual void v2(); virtual void Release(CEnginePrimitiveHandle*); long RefCount; };
inline void CEnginePrimitiveHandle::Reset()
{
    CHandleInfo* info = Info;
    if (info && info->RefCount == 1)
        info->Release(this);
    info = Info;
    if (info) {
        if (--info->RefCount == 0)
            info->Delete();
        Info = 0;
    }
    Info = 0;
}
class CEnginePrimitive2DSprite;
class CIEngine { public:
    virtual void e00();
    virtual void e01();
    virtual void e02();
    virtual void e03();
    virtual void e04();
    virtual void e05();
    virtual void e06();
    virtual void e07();
    virtual void e08();
    virtual void e09();
    virtual void e10();
    virtual void e11();
    virtual void e12();
    virtual void e13();
    virtual void e14();
    virtual void e15();
    virtual void e16();
    virtual void e17();
    virtual void e18();
    virtual void e19();
    virtual void e20();
    virtual void e21();
    virtual void e22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DSprite* prim, long flags);   // +0x5c
    virtual void e24();
    virtual void e25();
    virtual void e26();
    virtual void e27();
    virtual void e28();
    virtual void e29();
    virtual void e30();
    virtual void e31();
    virtual void e32();
    virtual void e33();
    virtual void e34();
    virtual void e35();
    virtual void e36();
    virtual void e37();
    virtual void e38();
    virtual void e39();
    virtual void e40();
    virtual void e41();
    virtual void e42();
    virtual void e43();
    virtual void e44();
    virtual void e45();
    virtual void e46();
    virtual void e47();
    virtual void e48();
    virtual void e49();
    virtual void e50();
    virtual void e51();
    virtual void e52();
    virtual void e53();
    virtual void e54();
    virtual void e55();
    virtual void e56();
    virtual void e57();
    virtual void e58();
    virtual void e59();
    virtual void e60();
    virtual void e61();
    virtual void e62();
    virtual void e63();
    virtual void e64();
    virtual void e65();
    virtual void e66();
    virtual void SetGuiColours(const void* colours, const CRGBFloatColour& a, const CRGBFloatColour& b, const CRGBFloatColour& c);   // +0x10c
};
class CEnginePrimitivePrepareInfo { public:
    CInterpolationInfo Interpolate;
    CIEngine*          Engine;                          // +0x34
};
class CUserProfileManager { public:
    unsigned char _pad[0xc];
    bool  UseNewProfileValues;                          // +0x0c
    unsigned char _pad0d[0x1c - 0xd];
    float Gamma;                                        // +0x1c
    float HUDAlpha;                                     // +0x20
    unsigned char _pad24[0xdc - 0x24];
    float NewGamma;                                     // +0xdc
    float NewHUDAlpha;                                  // +0xe0
    float GetGamma() const { return UseNewProfileValues ? NewGamma : Gamma; }
    float GetHUDAlpha() const { return UseNewProfileValues ? NewHUDAlpha : HUDAlpha; }
};
template <class T> class CTBaseSingleton { public: static T* __fastcall Get(); };   // 0x0040D2A0
class CFadeInFadeOutBase { public:
    bool Active;
    unsigned char _pad[0x1b];
    float GetFadeInOutAlpha() const;                    // 0x004348D0
};
class CDisplayEngine { public:
    unsigned char _pad[0x70];
    CFadeInFadeOutBase LetterBox;                       // +0x70
    unsigned char _pad8c[0xb4 - 0x8c];
    float GammaRamp;                                    // +0xb4
    unsigned char _padb8[4];
    CFadeInFadeOutBase ScreenFadeOutInfo;               // +0xbc
};
class CMainGameComponent { public: unsigned char _pad[0x28]; CDisplayEngine* DisplayEngine; };
extern CMainGameComponent* PMainGameComponent;          // 0x013B86A0
class CWorldSub { public: virtual void dummy(); };
class CWorld { public:
    unsigned char _pad[0x38];
    struct CWorldMode* Mode;                            // +0x38
    unsigned char _pad3c[0xa4 - 0x3c];
    bool Paused;                                        // +0xa4
};
struct CWorldMode {
    virtual void w000();
    virtual void w001();
    virtual void w002();
    virtual void w003();
    virtual void w004();
    virtual void w005();
    virtual void w006();
    virtual void w007();
    virtual void w008();
    virtual void w009();
    virtual void w010();
    virtual void w011();
    virtual void w012();
    virtual void w013();
    virtual void w014();
    virtual void w015();
    virtual void w016();
    virtual void w017();
    virtual void w018();
    virtual void w019();
    virtual void w020();
    virtual void w021();
    virtual void w022();
    virtual void w023();
    virtual void w024();
    virtual void w025();
    virtual void w026();
    virtual void w027();
    virtual void w028();
    virtual void w029();
    virtual void w030();
    virtual void w031();
    virtual void w032();
    virtual void w033();
    virtual void w034();
    virtual void w035();
    virtual void w036();
    virtual void w037();
    virtual void w038();
    virtual void w039();
    virtual void w040();
    virtual void w041();
    virtual void w042();
    virtual void w043();
    virtual void w044();
    virtual void w045();
    virtual void w046();
    virtual void w047();
    virtual void w048();
    virtual void w049();
    virtual void w050();
    virtual void w051();
    virtual void w052();
    virtual void w053();
    virtual void w054();
    virtual void w055();
    virtual void w056();
    virtual void w057();
    virtual void w058();
    virtual void w059();
    virtual void w060();
    virtual void w061();
    virtual void w062();
    virtual void w063();
    virtual void w064();
    virtual void w065();
    virtual void w066();
    virtual void w067();
    virtual void w068();
    virtual void w069();
    virtual void w070();
    virtual void w071();
    virtual void w072();
    virtual void w073();
    virtual void w074();
    virtual void w075();
    virtual void w076();
    virtual void w077();
    virtual void w078();
    virtual void w079();
    virtual void w080();
    virtual void w081();
    virtual void w082();
    virtual void w083();
    virtual void w084();
    virtual void w085();
    virtual void w086();
    virtual void w087();
    virtual void w088();
    virtual void w089();
    virtual void w090();
    virtual void w091();
    virtual void w092();
    virtual void w093();
    virtual void w094();
    virtual void w095();
    virtual void w096();
    virtual void w097();
    virtual void w098();
    virtual void w099();
    virtual void w100();
    virtual void w101();
    virtual void w102();
    virtual void w103();
    virtual void w104();
    virtual void w105();
    virtual void w106();
    virtual void w107();
    virtual void w108();
    virtual void w109();
    virtual void w110();
    virtual void w111();
    virtual void w112();
    virtual void w113();
    virtual void w114();
    virtual void w115();
    virtual void w116();
    virtual void w117();
    virtual void w118();
    virtual void w119();
    virtual void w120();
    virtual void w121();
    virtual void w122();
    virtual void w123();
    virtual void w124();
    virtual void w125();
    virtual void w126();
    virtual void w127();
    virtual void w128();
    virtual void w129();
    virtual void w130();
    virtual void w131();
    virtual void w132();
    virtual void w133();
    virtual void w134();
    virtual void w135();
    virtual void w136();
    virtual void w137();
    virtual void w138();
    virtual void w139();
    virtual void w140();
    virtual void w141();
    virtual void w142();
    virtual void w143();
    virtual void w144();
    virtual void w145();
    virtual void w146();
    virtual void w147();
    virtual void w148();
    virtual void w149();
    virtual void w150();
    virtual void w151();
    virtual void w152();
    virtual void w153();
    virtual void w154();
    virtual void w155();
    virtual void w156();
    virtual void w157();
    virtual void w158();
    virtual void w159();
    virtual void w160();
    virtual void w161();
    virtual void w162();
    virtual void w163();
    virtual void w164();
    virtual void w165();
    virtual void w166();
    virtual void w167();
    virtual void w168();
    virtual void w169();
    virtual void w170();
    virtual void w171();
    virtual void w172();
    virtual void w173();
    virtual void w174();
    virtual void w175();
    virtual void w176();
    virtual void w177();
    virtual void w178();
    virtual void w179();
    virtual void w180();
    virtual void w181();
    virtual void w182();
    virtual void w183();
    virtual void w184();
    virtual void w185();
    virtual void w186();
    virtual void w187();
    virtual void w188();
    virtual void w189();
    virtual void w190();
    virtual void w191();
    virtual void w192();
    virtual void w193();
    virtual void w194();
    virtual void w195();
    virtual void w196();
    virtual void w197();
    virtual void w198();
    virtual void w199();
    virtual void w200();
    virtual void w201();
    virtual void w202();
    virtual void w203();
    virtual void w204();
    virtual void w205();
    virtual void w206();
    virtual void w207();
    virtual void w208();
    virtual void w209();
    virtual void w210();
    virtual void w211();
    virtual void w212();
    virtual void w213();
    virtual void w214();
    virtual void w215();
    virtual void w216();
    virtual void w217();
    virtual void w218();
    virtual void w219();
    virtual void w220();
    virtual void w221();
    virtual void w222();
    virtual void w223();
    virtual void w224();
    virtual void w225();
    virtual void w226();
    virtual void w227();
    virtual void w228();
    virtual void w229();
    virtual void w230();
    virtual void w231();
    virtual void w232();
    virtual void w233();
    virtual void w234();
    virtual void w235();
    virtual void w236();
    virtual void w237();
    virtual void w238();
    virtual void w239();
    virtual void w240();
    virtual void w241();
    virtual void w242();
    virtual void w243();
    virtual void w244();
    virtual void w245();
    virtual void w246();
    virtual void w247();
    virtual void w248();
    virtual void w249();
    virtual void w250();
    virtual void w251();
    virtual void w252();
    virtual void w253();
    virtual void w254();
    virtual void w255();
    virtual void w256();
    virtual void w257();
    virtual void w258();
    virtual void w259();
    virtual void w260();
    virtual void w261();
    virtual void w262();
    virtual void w263();
    virtual void w264();
    virtual void w265();
    virtual void w266();
    virtual void w267();
    virtual void w268();
    virtual void w269();
    virtual void w270();
    virtual void w271();
    virtual void w272();
    virtual void w273();
    virtual void w274();
    virtual void w275();
    virtual void w276();
    virtual void w277();
    virtual void w278();
    virtual void w279();
    virtual void w280();
    virtual void w281();
    virtual void w282();
    virtual void w283();
    virtual void w284();
    virtual void w285();
    virtual void w286();
    virtual void w287();
    virtual void w288();
    virtual void w289();
    virtual void w290();
    virtual void w291();
    virtual void w292();
    virtual void w293();
    virtual void w294();
    virtual void w295();
    virtual void w296();
    virtual void w297();
    virtual void w298();
    virtual void w299();
    virtual void w300();
    virtual void w301();
    virtual void w302();
    virtual void w303();
    virtual void w304();
    virtual void w305();
    virtual void w306();
    virtual void w307();
    virtual void w308();
    virtual void w309();
    virtual void w310();
    virtual void w311();
    virtual void w312();
    virtual void w313();
    virtual void w314();
    virtual void w315();
    virtual void w316();
    virtual void w317();
    virtual void w318();
    virtual void w319();
    virtual void w320();
    virtual void w321();
    virtual void w322();
    virtual void w323();
    virtual void w324();
    virtual void w325();
    virtual void w326();
    virtual void w327();
    virtual void w328();
    virtual void w329();
    virtual void w330();
    virtual void w331();
    virtual void w332();
    virtual void w333();
    virtual void w334();
    virtual void w335();
    virtual void w336();
    virtual void w337();
    virtual void w338();
    virtual void w339();
    virtual void w340();
    virtual void w341();
    virtual void w342();
    virtual void w343();
    virtual void w344();
    virtual void w345();
    virtual void w346();
    virtual void w347();
    virtual void w348();
    virtual void w349();
    virtual void w350();
    virtual void w351();
    virtual void w352();
    virtual void w353();
    virtual void w354();
    virtual void w355();
    virtual void w356();
    virtual void w357();
    virtual void w358();
    virtual void w359();
    virtual void w360();
    virtual void w361();
    virtual void w362();
    virtual void w363();
    virtual void w364();
    virtual void w365();
    virtual void w366();
    virtual void w367();
    virtual void w368();
    virtual void w369();
    virtual void w370();
    virtual void w371();
    virtual void w372();
    virtual void w373();
    virtual void w374();
    virtual void w375();
    virtual void w376();
    virtual void w377();
    virtual void w378();
    virtual void w379();
    virtual void w380();
    virtual void w381();
    virtual void w382();
    virtual void w383();
    virtual void w384();
    virtual void w385();
    virtual void w386();
    virtual void w387();
    virtual void w388();
    virtual void w389();
    virtual void w390();
    virtual void w391();
    virtual void w392();
    virtual void w393();
    virtual void w394();
    virtual bool IsFrozen();                            // +0x62c (slot 395)
};
class CTCBase { public:
    virtual void t00(); virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04(); virtual void t05();
    virtual void t06(); virtual void t07(); virtual void t08(); virtual void t09(); virtual void t10();
    virtual void Draw(const CEnginePrimitivePrepareInfo& info);   // slot 11 (+0x2c)
};
class CTCQuickAccessMenu : public CTCBase { public: void RequestClose(); };   // 0x00625920
class CThing;
class CBaseIntelligentPointer { public: void* __vftable; void* PData; CThing* GetPItem() const; };   // 0x00A01B50
class CTCWithTarget : public CTCBase { public: unsigned char _pad[8]; CBaseIntelligentPointer Target; };   // +0x0c
struct CTCEntry { long first; CTCBase* second; };
struct CTCMap { CTCEntry* Begin; CTCEntry* End; CTCEntry* Cap; bool Compare; bool Dirty;
    CTCEntry* LowerBound(const long& key);              // 0x0040F020
    CTCEntry* LowerBoundConst(const long& key);         // 0x004365B0
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long PresentBits[9];                       // +0x20
    CTCMap Interfaces;                                  // +0x44
    unsigned char _pad54[0x91 - 0x54];
    unsigned char StatusFlags;                          // +0x91
    bool IsChild() const;                               // 0x00661F70
    __forceinline CTCBase* GetInterface(long interfaceType) { long type = interfaceType; CTCMap* map = &Interfaces; CTCEntry* it = map->LowerBound(type); return (it == map->End || interfaceType < it->first ? map->End : it)->second; }
    __forceinline CTCBase* GetInterfaceC(long interfaceType) { long type = interfaceType; CTCMap* map = &Interfaces; CTCEntry* it = map->LowerBoundConst(type); return (it == map->End || interfaceType < it->first ? map->End : it)->second; }
};
class CPlayer { public:
    long GetCurrentMode() const;                        // 0x00633BE0
    CThing* GetPControlledCreature() const;             // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                           // 0x00449970
    bool IsMultiplayerGameActive() const;               // 0x00449D20
};
class CSubtitleRenderer { public:
    unsigned char _pad[0x7c];
    unsigned long Flags7c;                              // +0x94 in CPlayerGui
    unsigned char _pad80[0xde - 0x80];
    bool Bool_de;                                       // +0xf6
    unsigned char _paddf[0xe5 - 0xdf];
    bool Bool_e5;                                       // +0xfd
    unsigned char _pade6[0x128 - 0xe6];
    void DrawPreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info);   // 0x00490494
};
namespace NPlayerGui {
class CDrawBase { public: virtual void ClearPrimitivesIfNecessary(); unsigned char _pad[0x14]; bool Drawn; };   // +0x18
#define HAS_TC(t, n) ((t)->PresentBits[(n) >> 5] & (1UL << ((n) & 31)))
#define DRAWCLASS(N) class N : public CDrawBase { public: void Draw(const CEnginePrimitivePrepareInfo& info); void ClearEffects(); }
DRAWCLASS(CDrawHealth); DRAWCLASS(CDrawStamina); DRAWCLASS(CDrawCombatMultiplier); DRAWCLASS(CDrawSpiritsHealth);
DRAWCLASS(CDrawScreenFade); DRAWCLASS(CDrawMoney); DRAWCLASS(CDrawRenown); DRAWCLASS(CDrawMorality); DRAWCLASS(CDrawExperience);
DRAWCLASS(CDrawCrime); DRAWCLASS(CDrawReceivedItems); DRAWCLASS(CDrawFishingGame); DRAWCLASS(CDrawStatUpdates); DRAWCLASS(CDrawAutoPickups);
DRAWCLASS(CDrawPerceivers); DRAWCLASS(CDrawDamage); DRAWCLASS(CDrawGuildSeal); DRAWCLASS(CDrawScriptScreenText); DRAWCLASS(CDrawTargetInfo);
DRAWCLASS(CDrawWorldTime); DRAWCLASS(CDrawLetterBoxBlackBars); DRAWCLASS(CDrawBettingHUD); DRAWCLASS(CDrawProjectileTargetingCentre);
DRAWCLASS(CDrawMeleeActionButtonEffects); DRAWCLASS(CDrawSpiritScores); DRAWCLASS(CDrawSpiritTargetingCursor); DRAWCLASS(CDraw2DText);
DRAWCLASS(CDrawMouseCursor); DRAWCLASS(CDrawNPCName);
class CDrawVirtualBase : public CDrawBase { public: virtual void v1(); virtual void Draw(const CEnginePrimitivePrepareInfo& info); void ClearEffects(); };
class CDrawAbilityButtons : public CDrawVirtualBase { };
class CDrawQuickAccessIcons : public CDrawVirtualBase { };
class CDrawMiniMap : public CDrawBase { public: void Update(const CEnginePrimitivePrepareInfo& info); void Draw(const CEnginePrimitivePrepareInfo& info, bool full); };
class CDrawQuestInfo : public CDrawBase { public: long GetMiniGameType(); void Draw(const CEnginePrimitivePrepareInfo& info, bool full); void DrawMiniGameInfo(const CEnginePrimitivePrepareInfo& info); };
class CDrawHighlightingTargetingCursor : public CDrawBase { public: unsigned char _pad[0xc]; CBaseIntelligentPointer Target;   // +0x28
    void Draw(const CEnginePrimitivePrepareInfo& info); void Clear(); };
class CStatBar { public: unsigned char _pad[0xc]; float CurrentValue; };
}
template <class T> class CCountedPointer { public: T* Data; void* Info; T* operator->() const { return Data; } };
class CTexture { public: void* a; void* b; struct C2DExtentsI GetDimensions(unsigned long level) const; };   // 0x009F9DB0
struct C2DExtentsI { unsigned long X, Y; };
struct C2DVector { float X, Y; C2DVector(float x, float y) : X(x), Y(y) { } };
enum EEngine2DLayer { ENGINE_2D_LAYER_DEBUG_TGA = 0x43 };
enum EEnginePrimitiveBlendMode { ENGINE_BLEND_MODE_2 = 2 };
enum EEngineInterpolationSet { ENGINE_INTERPOLATION_SET_0 = 0 };
class CEnginePrimitive2DSprite { public:
    unsigned char _data[0x54];                          // Ego_r.pdb sizeof 84
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DVector& pos, EEngine2DLayer layer, CTexture& texture, EEnginePrimitiveBlendMode blend,
                             EEngineInterpolationSet set, float a, float b, unsigned char r, unsigned char g, unsigned char bl, unsigned char al,
                             unsigned long flags, const C2DVector& uv0, const C2DVector& uv1);   // 0x0043FA20
};
float __fastcall GFVYToAY(float v);                     // 0x009E1E40
float __fastcall GFVXToAX(float v);                     // 0x009E1E10
extern double VIRTUAL_SCREEN_WIDTH;                     // 0x013961E8
extern double VIRTUAL_SCREEN_HEIGHT;                    // 0x013961F0
namespace std {
template <class R, class T> class mem_fun_t { public: explicit mem_fun_t(R (T::*pm)()) : Pm(pm) { } R (T::*Pm)(); };
template <class R, class T> inline mem_fun_t<R, T> mem_fun(R (T::*pm)()) { return mem_fun_t<R, T>(pm); }
template <class I, class F> F __fastcall for_each(I first, I last, F f);   // 0x0043F310 (project default /Gr: __fastcall)
}
using namespace NPlayerGui;

class CPlayerGui { public:
    void*                     _vt;                      // +0x00
    CWorld*                   World;                    // +0x04
    CPlayerManager*           PlayerManager;            // +0x08
    void*                     InputInterface;           // +0x0c
    CMainGameComponent*       Component;                // +0x10
    void*                     Graphics;                 // +0x14
    CSubtitleRenderer         SubtitleRenderer;         // +0x18
    CCountedPointer<CDrawHealth> PDrawHealth;           // +0x140
    CCountedPointer<CDrawStamina> PDrawStamina;
    CCountedPointer<CDrawCombatMultiplier> PDrawCombatMultiplier;
    CCountedPointer<CDrawSpiritsHealth> PDrawSpiritsHealth;
    CCountedPointer<CDrawMiniMap> PDrawMiniMap;         // +0x160
    CCountedPointer<CDrawScreenFade> PDrawScreenFade;
    CCountedPointer<CDrawMoney> PDrawMoney;
    CCountedPointer<CDrawRenown> PDrawRenown;
    CCountedPointer<CDrawMorality> PDrawMorality;       // +0x180
    CCountedPointer<CDrawExperience> PDrawExperience;
    CCountedPointer<CDrawCrime> PDrawCrime;
    CCountedPointer<CDrawReceivedItems> PDrawReceivedItems;
    CCountedPointer<CDrawFishingGame> PDrawFishingGame; // +0x1a0
    CCountedPointer<CDrawStatUpdates> PDrawStatUpdates;
    CCountedPointer<CDrawAutoPickups> PDrawAutoPickups;
    CCountedPointer<CDrawPerceivers> PDrawPerceivers;
    CCountedPointer<CDrawDamage> PDrawDamage;           // +0x1c0
    CCountedPointer<CDrawQuestInfo> PDrawQuestInfo;
    CCountedPointer<CDrawGuildSeal> PDrawGuildSeal;
    CCountedPointer<CDrawQuickAccessIcons> PDrawQuickAccessIcons;
    CCountedPointer<CDrawScriptScreenText> PDrawScriptScreenText;   // +0x1e0
    CCountedPointer<CDrawTargetInfo> PDrawTargetInfo;
    CCountedPointer<CDrawHighlightingTargetingCursor> PDrawHighlightingTargetingCursor;
    CCountedPointer<CDrawWorldTime> PDrawWorldTime;
    CCountedPointer<CDrawAbilityButtons> PDrawAbilityButtons;       // +0x200
    CCountedPointer<CDrawLetterBoxBlackBars> PDrawLetterBoxBlackBars;
    CCountedPointer<CDrawBettingHUD> PDrawBettingHUD;
    CCountedPointer<CDrawProjectileTargetingCentre> PDrawProjectileTargetingCentre;
    CCountedPointer<CDrawMeleeActionButtonEffects> PDrawMeleeActionButtonEffects;   // +0x220
    CCountedPointer<CDrawSpiritScores> PDrawSpiritScores;
    CCountedPointer<CDrawSpiritTargetingCursor> PDrawSpiritTargetingCursor;
    CCountedPointer<CDraw2DText> PDraw2DText;
    CCountedPointer<CDrawMouseCursor> PDrawMouseCursor;  // +0x240
    CCountedPointer<CDrawNPCName> PDrawNPCName;          // +0x248
    unsigned char             _pad250[0x294 - 0x250];
    long                      GuiMode;                  // +0x294
    unsigned char             _pad298[0x29d - 0x298];
    bool                      ShowTime;                 // +0x29d
    bool                      ShowMoneyBag;             // +0x29e
    unsigned char             _pad29f;
    float                     Alpha;                    // +0x2a0
    unsigned char             _pad2a4[0x2b1 - 0x2a4];
    bool                      DrawingDebugTGA;          // +0x2b1
    unsigned char             _pad2b2[2];
    CTexture                  DebugTGATexture;          // +0x2b4
    unsigned char             _pad2bc;
    bool                      ForceAllowSubtitleToRender;   // +0x2bd
    bool                      LiveGuiMode;              // +0x2be
    unsigned char             _pad2bf;
    CDrawBase**               IllustratorsBegin;        // +0x2c0
    CDrawBase**               IllustratorsEnd;          // +0x2c4
    unsigned char             _pad2c8[0x2ec - 0x2c8];
    CCountedPointer<CStatBar> PCrime;                   // +0x2ec
    unsigned char             _pad2f4[0x31c - 0x2f4];
    CEnginePrimitiveHandle    DebugTGAPrimitiveHandle;  // +0x31c
    unsigned char             _pad324[0x330 - 0x324];
    bool                      DisplayBottomHalfGUI;     // +0x330
    static CDefPointer<CPlayerGuiDef> PGuiDef;          // 0x013B878C
    static const CPlayerGuiDef& PeekGuiDef()
    {
        if (!PGuiDef.Object) {
            CCharString name("PLAYER_GUI_PC");
            CGameDefinitionManager::Get().GetDef(name, &PGuiDef);
        }
        return *PGuiDef.Object;
    }
    void RenderUpdateGuiTimers(const CInterpolationInfoSet& set);   // 0x004387F0
    void SetMoneyAsDisplaying();                        // 0x004376B0
    void SetCrimeAsDisplaying();                        // 0x00437770
    void DrawPreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info);
};

void CPlayerGui::DrawPreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info)
{
    const CInterpolationInfoSet& set = World->Paused ? info.Interpolate.PausedCurrent : info.Interpolate.Current;
    static long frameCount;
    if (frameCount <= 2 && ++frameCount == 2)
        PMainGameComponent->DisplayEngine->GammaRamp = CTBaseSingleton<CUserProfileManager>::Get()->GetGamma();
    Alpha = CTBaseSingleton<CUserProfileManager>::Get()->GetHUDAlpha();
    {
        CRGBFloatColour colourC = ToFloatColour(PeekGuiDef().ColourC);
        CRGBFloatColour colourB = ToFloatColour(PeekGuiDef().ColourB);
        CRGBFloatColour colourA = ToFloatColour(PeekGuiDef().ColourA);
        info.Engine->SetGuiColours(PeekGuiDef().Colours54, colourA, colourB, colourC);
    }
    RenderUpdateGuiTimers(set);
    CDrawBase** end = IllustratorsEnd;
    for (CDrawBase** it = IllustratorsBegin; it != end; ++it)
        (*it)->Drawn = false;
    PDrawMiniMap->Update(info);
    if (GuiMode != 1 || ForceAllowSubtitleToRender)
        SubtitleRenderer.DrawPreparePrimitivesForEngine(info);
    if (GuiMode != 1) {
        PDrawScreenFade->Draw(info);
        if (Component->DisplayEngine->LetterBox.Active)
            PDrawLetterBoxBlackBars->Draw(info);
        bool drawHud = true;
        if ((PDrawScreenFade->Drawn && Component->DisplayEngine->ScreenFadeOutInfo.Active
             && (unsigned char)(Component->DisplayEngine->ScreenFadeOutInfo.GetFadeInOutAlpha() * 255.0f) == 0xff)
            || SubtitleRenderer.Bool_de || World->Mode->IsFrozen()
            || (SubtitleRenderer.Bool_e5 && (SubtitleRenderer.Flags7c & 0x28))
            || (Component->DisplayEngine->LetterBox.Active && SubtitleRenderer.Bool_e5)
            || PlayerManager->GetMainPlayer().GetCurrentMode() == 0x10)
            drawHud = false;
        CThing* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
        if (hero && !(hero->StatusFlags & 1)) {
            if (PlayerManager->GetMainPlayer().GetCurrentMode() != 0x10) {
                PDrawFishingGame->Draw(info);
                PDrawReceivedItems->Draw(info);
                if (PDrawQuestInfo->GetMiniGameType() == 7) {
                    PDrawQuickAccessIcons->Draw(info);
                    PDrawQuestInfo->Draw(info, true);
                }
                CThing* h = PlayerManager->GetMainPlayer().GetPControlledCreature();
                if (HAS_TC(h, 0xf6)) {
                    CTCQuickAccessMenu* menu = (CTCQuickAccessMenu*)h->GetInterface(0xf6);
                    if (!SubtitleRenderer.Bool_e5 && !World->Mode->IsFrozen() && !PDrawFishingGame->Drawn && drawHud)
                        menu->Draw(info);
                    else
                        menu->RequestClose();
                }
                if (!SubtitleRenderer.Bool_e5) {
                    if (!SubtitleRenderer.Bool_de && !World->Mode->IsFrozen() && !PDrawFishingGame->Drawn
                        && (PDrawQuestInfo->GetMiniGameType() == 0 || PDrawQuestInfo->GetMiniGameType() == 6))
                        PDrawMiniMap->Draw(info, drawHud);
                    if (!Component->DisplayEngine->ScreenFadeOutInfo.Active) {
                        if (PDrawQuestInfo->GetMiniGameType() != 0) {
                            if (PDrawQuestInfo->GetMiniGameType() == 7)
                                PDrawQuestInfo->Draw(info, true);
                            else
                                PDrawQuestInfo->DrawMiniGameInfo(info);
                        }
                        else if (!SubtitleRenderer.Bool_de && !World->Mode->IsFrozen() && !PDrawFishingGame->Drawn)
                            PDrawQuestInfo->Draw(info, false);
                    }
                }
            }
            if (drawHud) {
                if (LiveGuiMode)
                    PDrawMouseCursor->Draw(info);
                if (DisplayBottomHalfGUI) {
                    PDrawNPCName->Draw(info);
                    if (DisplayBottomHalfGUI && PCrime->CurrentValue > 0.0f && !SubtitleRenderer.Bool_e5)
                        SetCrimeAsDisplaying();
                }
                PDrawHealth->Draw(info);
                if (!hero->IsChild())
                    PDrawStamina->Draw(info);
                if (DisplayBottomHalfGUI) {
                    PDrawExperience->Draw(info);
                    PDrawStatUpdates->Draw(info);
                    PDrawAutoPickups->Draw(info);
                    PDrawCombatMultiplier->Draw(info);
                    PDrawProjectileTargetingCentre->Draw(info);
                }
                if (PDrawMiniMap->Drawn) {
                    if (ShowTime)
                        PDrawWorldTime->Draw(info);
                    PDrawPerceivers->Draw(info);
                }
                if (!SubtitleRenderer.Bool_e5 && DisplayBottomHalfGUI) {
                    if (!PDrawProjectileTargetingCentre->Drawn)
                        PDrawMoney->Draw(info);
                    PDrawRenown->Draw(info);
                    PDrawMorality->Draw(info);
                    PDrawCrime->Draw(info);
                    PDrawScriptScreenText->Draw(info);
                    PDrawAbilityButtons->Draw(info);
                    PDrawMeleeActionButtonEffects->Draw(info);
                    PDrawDamage->Draw(info);
                    if (!PDrawProjectileTargetingCentre->Drawn) {
                        PDrawGuildSeal->Draw(info);
                        PDrawQuickAccessIcons->Draw(info);
                    }
                    PDrawTargetInfo->Draw(info);
                    PDrawHighlightingTargetingCursor->Draw(info);
                    if (PlayerManager->IsMultiplayerGameActive()) {
                        PDrawSpiritScores->Draw(info);
                        PDrawSpiritsHealth->Draw(info);
                        PDrawSpiritTargetingCursor->Draw(info);
                    }
                }
                if (ShowMoneyBag)
                    SetMoneyAsDisplaying();
                else {
                    CThing* target = PDrawHighlightingTargetingCursor->Target.GetPItem();
                    if (target && !(target->StatusFlags & 1) && HAS_TC(target, 0x90))
                        SetMoneyAsDisplaying();
                    else if (HAS_TC(hero, 0x75)) {
                        CThing* t = ((CTCWithTarget*)hero->GetInterfaceC(0x75))->Target.GetPItem();
                        if (t && HAS_TC(t, 0x53))
                            SetMoneyAsDisplaying();
                    }
                }
            }
            else if (PDrawQuestInfo->GetMiniGameType() == 3 || PDrawQuestInfo->GetMiniGameType() == 4 || PDrawQuestInfo->GetMiniGameType() == 5)
                PDrawHighlightingTargetingCursor->Draw(info);
            else
                PDrawHighlightingTargetingCursor->Clear();
        }
    }
    PDraw2DText->Draw(info);
    if (!PDrawHealth->Drawn) PDrawHealth->ClearEffects();
    if (!PDrawStamina->Drawn) PDrawStamina->ClearEffects();
    if (!PDrawAbilityButtons->Drawn) PDrawAbilityButtons->ClearEffects();
    if (!PDrawQuickAccessIcons->Drawn) PDrawQuickAccessIcons->ClearEffects();
    if (!PDrawMeleeActionButtonEffects->Drawn) PDrawMeleeActionButtonEffects->ClearEffects();
    if (DrawingDebugTGA) {
        unsigned long height = DebugTGATexture.GetDimensions(0).Y;
        unsigned long width = DebugTGATexture.GetDimensions(0).X;
        CEnginePrimitive2DSprite sprite(0, C2DVector((GFVXToAX((float)VIRTUAL_SCREEN_WIDTH) - (float)width) * 0.5f, (GFVYToAY((float)VIRTUAL_SCREEN_HEIGHT) - (float)height) * 0.5f),
                                        ENGINE_2D_LAYER_DEBUG_TGA, DebugTGATexture, ENGINE_BLEND_MODE_2, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, 0xff, 0xff, 0xff, 0xff, 2,
                                        C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine->AddPrimitive(&DebugTGAPrimitiveHandle, &sprite, 0x40);
    }
    else
        DebugTGAPrimitiveHandle.Reset();
    std::for_each(IllustratorsBegin, IllustratorsEnd, std::mem_fun(&CDrawBase::ClearPrimitivesIfNecessary));
}
