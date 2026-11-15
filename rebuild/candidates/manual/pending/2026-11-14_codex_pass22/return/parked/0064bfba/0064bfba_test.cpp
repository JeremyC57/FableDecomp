#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
enum EHighlightingCursorType { CURSOR_TYPE_NONE=0,CURSOR_TYPE_USABLE=1,CURSOR_TYPE_ENEMY=2,CURSOR_TYPE_ALLY=3,CURSOR_TYPE_HAS_INFORMATION=4,MAX_NO_CURSOR_TYPES=5 };
enum ETargetingType { TARGETING_USABLE=1 };
enum ETCInterfaceType { TCI_TARGETING=8,TCI_TARGETED=9,TCI_ON_ACTION_USE=13,TCI_SMASHABLE=78,TCI_GRAPHIC_APPEARANCE_NEW=91,TCI_HERO_GUIDE=107,TCI_SHOP_KEEPER=144,TCI_BATTLE_CHARGE=168,TCI_ACTION_USE_SCRIPTED_HOOK=194,TCI_TAVERN_GAME_MASTER=210,TCI_WIFE=215 };
enum EPlayerMode { PLAYER_MODE_CONTROL_CREATURE=1,PLAYER_MODE_Z_TARGET=2,PLAYER_MODE_LIGHTNING=40 };
struct C2DVector { float X,Y; C2DVector(){} C2DVector(float x,float y):X(x),Y(y){} };
struct C2DExtentsI { long X,Y; C2DExtentsI(long x,long y):X(x),Y(y){} };
struct C2DBoxF { float TLX,TLY,BRX,BRY;void SetTLPos(const C2DVector&p){TLX=p.X;TLY=p.Y;} };
// fable_compiler.h -- compiler-specific spellings behind portable names.
//
// FABLE_FORCEINLINE: VC7.1's `__forceinline` when the compiler is MSVC (the retail build: the token reaches cl exactly as if it
// were written out, so the bytes are unchanged), plain `inline` elsewhere (same meaning for the program: forceinline is an
// inlining request plus `inline` linkage; GCC/Clang decide inlining themselves). Rows and shared headers spell it this way so a
// GCC/Clang build of the reconstruction does not trip over the MSVC keyword. Never spell `__forceinline` bare in rebuild/
// (tools/decomp_pipeline/check_portable_spellings.py rejects it; an `#ifdef _MSC_VER`-guarded one is tolerated).
#ifndef FABLE_COMPILER_H
#define FABLE_COMPILER_H

#if defined(_MSC_VER)
#define FABLE_FORCEINLINE __forceinline
#else
#define FABLE_FORCEINLINE inline
#endif

#endif // FABLE_COMPILER_H

#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_CRGBCOLOUR_CURSOR_COPY_INLINE_PASS22
class CRGBColour {
public:
    union {
        struct { unsigned char B,G,R,A; };
        unsigned long IntValue;
    };
    CRGBColour() { }
#if defined(FABLE_RETAIL_CRGBCOLOUR) && defined(FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE)
    // Opt-in (chunk 4007, row 0x0061B610; first measured by hand882 on the PC twin 0x0061E6D0): FableWin ??0CRGBColour@@QAE@EEEE@Z is
    // 18 instructions / 44 bytes at /Od = the four-channel constructor forwarding to ?Initialise@CRGBColour@@QAEXEEEE@Z (19i/48b),
    // like the three-channel constructor below; the member-store body is 20i/51b. Same release bytes, one more inline layer.
    CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { Initialise(r,g,b,a); }
#else
    CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; }
#endif
#ifdef FABLE_RETAIL_CRGBCOLOUR
    // Retail opt-in (chunk 724): the PDB's Initialise(r, g, b, a) (?Initialise@CRGBColour@@QAEXEEEE@Z) and the copy constructor
    // built on it: all four channel bytes are read (as Initialise's arguments) before any is stored, the byte-by-byte copy
    // retail emits where a CRGBColour is passed by value (CPSCDecalRenderer::InterpolateTo 0x00B1BDA0 into
    // GFGetInterpolatedColour; GOTCHAS: the landed rows spell the setter `Set`, which neither PDB has).
    void Initialise(unsigned char r,unsigned char g,unsigned char b,unsigned char a) { R=r; G=g; B=b; A=a; }
#ifdef FABLE_RETAIL_CRGBCOLOUR_CURSOR_COPY_INLINE_PASS22
    // Native0064BFBA expands both fade-colour copies; ordinary size budget also expands unrelated native calls.
    FABLE_FORCEINLINE CRGBColour(const CRGBColour& colour) { Initialise(colour.R,colour.G,colour.B,colour.A); }
#else
    CRGBColour(const CRGBColour& colour) { Initialise(colour.R,colour.G,colour.B,colour.A); }
#endif
    // Codex pass 13 (rows 0x00EEDC20 / 0x00E8BEF0): FableWin ??0CRGBColour@@QAE@EEE@Z, inline in retail (R, G, B then A = 0xFF stored
    // in order through Initialise).
    CRGBColour(unsigned char r,unsigned char g,unsigned char b) { Initialise(r,g,b,255); }
#elif defined(FABLE_RETAIL_CRGBCOLOUR_DWORD_COPY)
    // Retail opt-in (chunk 1018, row 0x00405220 TransferIMEColour): the PDB copy constructor ??0CRGBColour@@QAE@ABV0@@Z copies the
    // colour as one dword in that TU (`mov ecx,[esp+18h] ; mov [esp+10h],ecx` at 0x00405251 / 0x0040525A).
    CRGBColour(const CRGBColour& colour) : IntValue(colour.IntValue) { }
#elif defined(FABLE_RETAIL_CRGBCOLOUR_FORCEINLINE_COPY)
    // Opt-in (chunk 2051, row 0x0042EC7C CNewFrontendGameComponent::Run): the default copy body, forced. In a size TU c2 refuses this
    // plain inline with no score (the by-value CXMVPlayer::SetClearColour argument was a call) where retail copies the four bytes in
    // R, G, B, A order in place (0x0042ED68); GOTCHAS chunk 268: "a by-value CRGBColour argument built in its slot needs a
    // __forceinline user copy ctor".
    FABLE_FORCEINLINE CRGBColour(const CRGBColour& colour) { R=colour.R; G=colour.G; B=colour.B; A=colour.A; }
#else
    CRGBColour(const CRGBColour& colour) { R=colour.R; G=colour.G; B=colour.B; A=colour.A; }
#endif
#ifdef FABLE_RETAIL_CRGBCOLOUR_GETASINT
    // Opt-in (chunk 2032, row 0x00BA6FF0): FableWin.pdb ?GetAsInt@CRGBColour@@QBEKXZ (no Ego_r.pdb or Fable.exe copy): the packed dword
    // read in place (0x00BA7732 `mov edx,[esp+18h] ; push edx` hands the silhouette colour to DrawAlphaBoundingBox's unsigned long).
    unsigned long GetAsInt() const { return IntValue; }
#endif
#ifdef FABLE_RETAIL_CRGBCOLOUR_GETD3DCOLOR
    // Retail opt-in (chunk 2018, row 0x00B2B180): Ego_r.pdb / FableWin.pdb ?GetD3DCOLOR@CRGBColour@@QBEKXZ, inline in retail (the B, G, R,
    // A bytes are a D3DCOLOR dword: 0x00B2B4BA `mov edx,[eax+8]` stores the blend's DiffuseColour into a vertex as one dword).
    // The dword read is the body: FableWin.pdb's debug COMDAT 0x02CCFDA0 fills one 0x20-byte slot, like GetAsInt 0x02284290 and GetR
    // 0x01D73670 (a four-channel packing is over 50 bytes at /Od). The channel packing in CRenderManager2D::Draw2DLine 0x009DD96E /
    // Draw2DTexture 0x009DC6ED is those rows' own expression over GetA / GetR / GetG / GetB (landing fz, 2026-11-13).
    unsigned long GetD3DCOLOR() const { return IntValue; }
#endif
    unsigned char GetR() const { return R; }
    unsigned char GetG() const { return G; }
    unsigned char GetB() const { return B; }
    unsigned char GetA() const { return A; }
};
namespace NGraphicAppearance {
enum EAppearanceMeshEffectPriority { MESH_EFFECT_PRIORITY_TARGETING=3 };
struct CTCGraphicAppearance { static ETCInterfaceType StatGetInterfaceType(){return TCI_GRAPHIC_APPEARANCE_NEW;} void ClearHighlighted(void*key){ClearMeshEffect(key);} long Cleared,Highlighted;float WorldWidth,ScreenWidth;CRGBColour Colour;void*Owner;CTCGraphicAppearance():Cleared(0),Highlighted(0),WorldWidth(0),ScreenWidth(0),Owner(0){}
 void ClearMeshEffect(void*p){assert(p);++Cleared;Owner=p;}
 void SetAsHighlighted(float w,float s,const CRGBColour&c,bool a,bool b,EAppearanceMeshEffectPriority p,void*o){assert(a&&b&&p==MESH_EFFECT_PRIORITY_TARGETING&&o);++Highlighted;WorldWidth=w;ScreenWidth=s;Colour=c;Owner=o;}
};
}
using namespace NGraphicAppearance;
struct CDoorDef{};template<class T>struct CDefPointer { T*Data;CDefPointer():Data(0){} };
struct CDefClassBase {bool Door;CDefClassBase():Door(false){}bool GetSubDef(CDefPointer<const CDoorDef>*p)const{static CDoorDef d;p->Data=Door?&d:0;return Door;} };
struct CThing {
 void*Interfaces[256];bool Alive,Vulnerable;long Type;std::string DefName;CDefClassBase Def;
 CThing():Alive(true),Vulnerable(false),Type(0),DefName("ordinary"){memset(Interfaces,0,sizeof Interfaces);}
 template<class T>bool GetTC(ETCInterfaceType k,T**p){*p=(T*)Interfaces[k];return *p!=0;}
 template<class T>bool PeekTC(ETCInterfaceType k,const T**p)const{*p=(const T*)Interfaces[k];return *p!=0;}
 template<class T>bool GetTC(T**p){return GetTC(T::StatGetInterfaceType(),p);}
 template<class T>bool PeekTC(const T**p)const{return PeekTC(T::StatGetInterfaceType(),p);}
 bool IsDead()const{return !Alive;}bool HasTC(ETCInterfaceType k)const{return Interfaces[k]!=0;}long GetType()const{return Type;}
 const CDefClassBase*GetPBaseDef()const{return &Def;}const std::string&GetDefName()const{return DefName;}
 CTCGraphicAppearance*GetTCGraphicAppearance(){return (CTCGraphicAppearance*)Interfaces[TCI_GRAPHIC_APPEARANCE_NEW];}
};
struct CThingAICreature: CThing {bool Information;CThingAICreature():Information(false){Type=1;}bool GetHasInformation()const{return Information;} };
struct CThingPlayerCreature: CThing {};
template<class T>struct CIntelligentPointer {T*Data;CIntelligentPointer():Data(0){}T*Get()const{return Data;}T*operator->()const{return Data;}T&operator*()const{return *Data;}CIntelligentPointer&operator=(T*p){Data=p;return *this;}};
template<class T>bool operator!=(T*p,const CIntelligentPointer<T>&q){return p!=q.Data;}
template<class T>bool operator==(const CIntelligentPointer<T>&q,const T*p){return p==q.Data;}
#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_FASTCALL
#endif
template<class T> inline bool FABLE_FASTCALL operator!=(const CIntelligentPointer<T>& pointer,int) {
    return pointer.Get()!=0;
}
#define FABLE_RETAIL_IS_THING_ALIVE
#ifndef FABLE_ENGINE_THINGALIVE_H
#define FABLE_ENGINE_THINGALIVE_H

// Retail opt-in (Codex pass 17): original IS_THING_ALIVE<T>(const T&),
// FableWin instantiation 01CCA150 called by the donor IsRunnable.
#ifdef FABLE_RETAIL_IS_THING_ALIVE
#ifdef FABLE_RETAIL_IS_THING_DEAD_DECL_ONLY
// Retail opt-in (Codex pass22,row00927F30): original externally provided instantiation.
template<class T> bool FABLE_FASTCALL IS_THING_DEAD(const T&);
#else
template<class T> inline bool FABLE_FASTCALL IS_THING_DEAD(const T& thing) {return thing==0||thing->IsDead();}
#endif
#if defined(FABLE_RETAIL_IS_THING_ALIVE_NOINLINE) && defined(_MSC_VER)
// Per-TU opt-in (chunk 1021, row 0x0076D2F0 CTCEnemy::AreThingsEnemies): the same body kept out of line, as that TU calls the
// IS_THING_ALIVE<const CThingPlayerCreature*> COMDAT 0x0043DBF0 (`lea ecx,[esp+1Ch] ; call 0x0043DBF0`, 0x0076D3C2). The visible
// body tells VC7.1 the callee keeps no reference to the caller's pointer local, so its frame word is shared, as retail does.
template<class T> __declspec(noinline) bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing) {
#else
template<class T> inline bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing) {
#endif
    if(thing!=0) {
        if(!thing->IsDead()) return true;
    }
    return false;
}
#endif
#endif


struct CTCTargetingBase {static ETCInterfaceType StatGetInterfaceType(){return TCI_TARGETING;}CThing*Target;bool Usable,InRange;CTCTargetingBase():Target(0),Usable(false),InRange(false){}CThing*GetPTargetedThing()const{return Target;}bool IsTargetedThingOfType(ETargetingType t)const{assert(t==TARGETING_USABLE);return Usable;}bool IsTargetedThingInRange(long x)const{assert(x==1);return InRange;} };
namespace NTargeting { bool IsThingVulnerableToMeleeStrikesFromThing(const CThing&a,const CThing&){return a.Vulnerable;} }
struct CTCShop {bool Available;CTCShop():Available(false){}bool IsShopKeeperAvailable()const{return Available;}};
struct CTCShopKeeper {static ETCInterfaceType StatGetInterfaceType(){return TCI_SHOP_KEEPER;}CTCShop*Shop;CTCShopKeeper():Shop(0){}CTCShop*GetShopTC(){return Shop;}};
struct CTCTavernGameMaster {static ETCInterfaceType StatGetInterfaceType(){return TCI_TAVERN_GAME_MASTER;}void*Game;CTCTavernGameMaster():Game(0){}void*GetGame(){return Game;}};
struct CTCWife {static ETCInterfaceType StatGetInterfaceType(){return TCI_WIFE;}bool Married,Love,Sex;CTCWife():Married(false),Love(false),Sex(false){}bool IsMarriedToThePlayer()const{return Married;}bool IsInLoveWithHero()const{return Love;}bool IsOfferingSex()const{return Sex;}};
struct CTCBattleCharge {static ETCInterfaceType StatGetInterfaceType(){return TCI_BATTLE_CHARGE;}CThing*Target;bool Active;CTCBattleCharge():Target(0),Active(false){}bool IsBattleChargeZTargetting()const{return Active;}CThing*GetPTarget()const{return Target;}};
struct CTCActionUseBase {static ETCInterfaceType StatGetInterfaceType(){return TCI_ON_ACTION_USE;}bool Usable;CTCActionUseBase():Usable(true){}bool IsUsable()const{return Usable;}};
struct CTCActionUseScriptedHook {static ETCInterfaceType StatGetInterfaceType(){return TCI_ACTION_USE_SCRIPTED_HOOK;}bool Usable;CTCActionUseScriptedHook():Usable(true){}bool IsUsable()const{return Usable;}};
struct CTCTargeted {static ETCInterfaceType StatGetInterfaceType(){return TCI_TARGETED;}bool Glow;CTCTargeted():Glow(true){}bool IsToUseTargetGlow()const{return Glow;}};
struct CPlayer {CThingPlayerCreature*Hero;EPlayerMode Mode;bool ZMode;CPlayer():Hero(0),Mode(PLAYER_MODE_CONTROL_CREATURE),ZMode(false){}const CThingPlayerCreature*PeekPControlledCreature()const{return Hero;}EPlayerMode GetCurrentMode()const{return Mode;}bool HasMode(EPlayerMode m)const{assert(m==PLAYER_MODE_Z_TARGET);return ZMode;} };
struct CPlayerManager {CPlayer Player;CPlayer&GetMainPlayer(){return Player;}};
struct CWorld {bool MapTableShowQuestCardsOnUsed;CWorld():MapTableShowQuestCardsOnUsed(false){}bool IsMapTableToShowQuestCardsWhenUsed()const{return MapTableShowQuestCardsOnUsed;}};
struct CPlayerGuiDef {
 CRGBColour HighlightStartColour[5],HighlightEndColour[5];float HighlightStartWidthWorldSpace[5],HighlightEndWidthWorldSpace[5],HighlightStartWidthScreenSpace[5],HighlightEndWidthScreenSpace[5];long HighlightFadeSteps[5];
 unsigned long TargetingBorderLeftGraphic,TargetingBorderRightGraphic;C2DVector TargetingBorderLeftPos,TargetingBorderRightPos;
};
static CPlayerGuiDef Gui;
struct CPlayerGui {static const CPlayerGuiDef&PeekGuiDef(){return Gui;}};
struct CUserProfileManager {bool UseNewProfileValues,ShowTargetingStatusNewProfile,ShowTargetingStatus;static CUserProfileManager&Get(){static CUserProfileManager p;return p;}bool GetShowTargetingStatus()const{return UseNewProfileValues?ShowTargetingStatusNewProfile:ShowTargetingStatus;} };
struct CDisplayManager {long Width,Height;C2DExtentsI GetRenderTargetDimensions(){return C2DExtentsI(Width,Height);}};
static CDisplayManager Display;static CDisplayManager*GPDisplayManager=&Display;
struct CGraphicDataBank {unsigned long Width,Height;unsigned long GetGraphicMaxWidth(unsigned long x)const{assert(x==Gui.TargetingBorderLeftGraphic);return Width;}unsigned long GetGraphicMaxHeight(unsigned long x)const{assert(x==Gui.TargetingBorderLeftGraphic);return Height;}};
enum EEngine2DLayer {ENGINE_2D_LAYER_0=0x43};enum EEnginePrimitiveBlendMode {ENGINE_PRIMITIVE_BLEND_ALPHA=2};enum EEngineInterpolationSet {ENGINE_INTERPOLATION_SET_ALWAYSUPDATE=0};
struct CEnginePrimitive2DSprite {C2DBoxF Box;unsigned long Graphic;unsigned char Alpha;
 CEnginePrimitive2DSprite(unsigned long id,const C2DBoxF&box,EEngine2DLayer layer,unsigned long graphic,unsigned long frame,EEnginePrimitiveBlendMode blend,EEngineInterpolationSet interpolation,float a,float b,unsigned char alpha,unsigned char red,unsigned char green,unsigned char blue,unsigned long flags):Box(box),Graphic(graphic),Alpha(alpha){assert(id==0&&layer==ENGINE_2D_LAYER_0&&frame==0&&blend==ENGINE_PRIMITIVE_BLEND_ALPHA&&interpolation==0&&a==0&&b==0&&red==255&&green==255&&blue==255&&flags==2);}
};
struct CEnginePrimitiveHandle {long Clears;CEnginePrimitiveHandle():Clears(0){}void Clear(){++Clears;}};
struct CIEngine {long Calls;C2DBoxF Boxes[2];unsigned long Graphics[2];unsigned char Alphas[2];CEnginePrimitiveHandle*Handles[2];CIEngine():Calls(0){}void SetPersistentPrimitive(CEnginePrimitiveHandle&h,const CEnginePrimitive2DSprite&s,unsigned long flags){assert(Calls<2&&flags==0xc0);Handles[Calls]=&h;Boxes[Calls]=s.Box;Graphics[Calls]=s.Graphic;Alphas[Calls]=s.Alpha;++Calls;}};
struct CEnginePrimitivePrepareInfo {CIEngine&Engine;CEnginePrimitivePrepareInfo(CIEngine&e):Engine(e){}};
namespace NPlayerGui {
C2DVector GFAnchorToTopLeft(const C2DVector&p){return p;}
C2DVector GFAnchorToTopRight(const C2DVector&p){return C2DVector(Display.Width+p.X-640.0f,p.Y);}
struct CDrawHighlightingTargetingCursor {
 CPlayerManager&PlayerManager;CWorld&World;const CGraphicDataBank&Bank;bool Drawing;std::vector<CEnginePrimitiveHandle>PrimitiveHandles;CIntelligentPointer<CThing>PTargetedThing;EHighlightingCursorType CursorType;CRGBColour CurrentColour;float CurrentWidthWorldSpace,CurrentWidthScreenSpace;long CurrentFadeStep;unsigned char Alpha;
 CDrawHighlightingTargetingCursor(CPlayerManager&m,CWorld&w,const CGraphicDataBank&b):PlayerManager(m),World(w),Bank(b),Drawing(false),PrimitiveHandles(2),CursorType(CURSOR_TYPE_NONE),CurrentWidthWorldSpace(0),CurrentWidthScreenSpace(0),CurrentFadeStep(0),Alpha(128){memset(&CurrentColour,0,sizeof CurrentColour);}
 void SetAsDrawing(bool drawing){Drawing=drawing;}CPlayer&GetPlayer(){return PlayerManager.GetMainPlayer();}unsigned char ScaleAlpha(unsigned char a)const{assert(a==255);return Alpha;}void Draw(const CEnginePrimitivePrepareInfo&);
};
}
// BEGIN_AUTHORED_BODY
void NPlayerGui::CDrawHighlightingTargetingCursor::Draw(const CEnginePrimitivePrepareInfo& info)
{
    const CThingPlayerCreature* pcreature = GetPlayer().PeekPControlledCreature();
    const CTCTargetingBase* ptc_targeting;
    pcreature->PeekTC(&ptc_targeting);
    const CPlayerGuiDef& player_gui_def = CPlayerGui::PeekGuiDef();
    if (ptc_targeting->GetPTargetedThing() != PTargetedThing) {
        if (IS_THING_ALIVE(PTargetedThing) && PTargetedThing->GetTCGraphicAppearance() != 0)
            PTargetedThing->GetTCGraphicAppearance()->ClearHighlighted(this);
        PTargetedThing = ptc_targeting->GetPTargetedThing();
        CursorType = CURSOR_TYPE_NONE;
    }
    if (PTargetedThing.Get() != 0) {
        SetAsDrawing(true);
        CDefPointer<const CDoorDef> pdoor_def;
        EHighlightingCursorType cursor_type = CURSOR_TYPE_NONE;
        if (ptc_targeting->IsTargetedThingOfType(TARGETING_USABLE)) {
            if (ptc_targeting->IsTargetedThingInRange(1))
                cursor_type = CURSOR_TYPE_USABLE;
        } else if (NTargeting::IsThingVulnerableToMeleeStrikesFromThing(*PTargetedThing, *pcreature)) {
            cursor_type = CURSOR_TYPE_ENEMY;
        } else if (!PTargetedThing->GetPBaseDef()->GetSubDef(&pdoor_def) && PTargetedThing->HasTC(TCI_SMASHABLE)) {
            cursor_type = CURSOR_TYPE_ENEMY;
        } else {
            cursor_type = CURSOR_TYPE_ALLY;
            if (PTargetedThing->GetType() == 1) {
                CThingAICreature* pai_creature = (CThingAICreature*)PTargetedThing.Get();
                CTCShopKeeper* tc_shopkeeper;
                CTCTavernGameMaster* tc_taverngamemaster;
                const CTCWife* ptc_wife;
                if (pai_creature->GetHasInformation()
                    || (pai_creature->GetTC(&tc_shopkeeper) && tc_shopkeeper->GetShopTC() != 0
                        && tc_shopkeeper->GetShopTC()->IsShopKeeperAvailable())
                    || (pai_creature->GetTC(&tc_taverngamemaster) && tc_taverngamemaster->GetGame() != 0)
                    || (pai_creature->PeekTC(&ptc_wife)
                        && ((!ptc_wife->IsMarriedToThePlayer() && ptc_wife->IsInLoveWithHero())
                            || (ptc_wife->IsMarriedToThePlayer() && ptc_wife->IsOfferingSex()))))
                    cursor_type = CURSOR_TYPE_HAS_INFORMATION;
            }
        }
        if (IS_THING_ALIVE(PTargetedThing)) {
            const CTCBattleCharge* ptc_battlecharge;
            if (PTargetedThing->HasTC(TCI_HERO_GUIDE)) {
                if (World.IsMapTableToShowQuestCardsWhenUsed())
                    cursor_type = CURSOR_TYPE_USABLE;
                else
                    cursor_type = CURSOR_TYPE_NONE;
            } else if (PTargetedThing->GetDefName() == "CREATURE_DEMON_DOOR_FACE_01") {
                cursor_type = CURSOR_TYPE_USABLE;
            } else if (PTargetedThing->GetDefName() == "CREATURE_NW_DEMON_DOOR_FACE_01") {
                cursor_type = CURSOR_TYPE_USABLE;
            } else {
                if (GetPlayer().PeekPControlledCreature()->PeekTC(&ptc_battlecharge) && ptc_battlecharge->IsBattleChargeZTargetting()
                    && PTargetedThing == ptc_battlecharge->GetPTarget())
                    cursor_type = CURSOR_TYPE_ENEMY;
            }
        }
        if (cursor_type == CURSOR_TYPE_USABLE) {
            const CTCActionUseBase* ptc_action_use;
            if (PTargetedThing->PeekTC(&ptc_action_use)) {
                if (!ptc_action_use->IsUsable())
                    cursor_type = CURSOR_TYPE_NONE;
            } else {
                const CTCActionUseScriptedHook* ptc_action_use_hook = 0;
                if (PTargetedThing->PeekTC(&ptc_action_use_hook)) {
                    if (!ptc_action_use_hook->IsUsable())
                        cursor_type = CURSOR_TYPE_NONE;
                }
            }
        }
        if (cursor_type != CursorType) {
            CursorType = cursor_type;
            CurrentColour = player_gui_def.HighlightStartColour[cursor_type];
            CurrentWidthWorldSpace = player_gui_def.HighlightStartWidthWorldSpace[cursor_type];
            CurrentWidthScreenSpace = player_gui_def.HighlightStartWidthScreenSpace[cursor_type];
            CurrentFadeStep = 0;
        }
        if (CurrentFadeStep < player_gui_def.HighlightFadeSteps[CursorType]) {
            CurrentWidthWorldSpace += (player_gui_def.HighlightEndWidthWorldSpace[CursorType] - player_gui_def.HighlightStartWidthWorldSpace[CursorType])
                                      / (float)player_gui_def.HighlightFadeSteps[CursorType];
            CurrentWidthScreenSpace += (player_gui_def.HighlightEndWidthScreenSpace[CursorType] - player_gui_def.HighlightStartWidthScreenSpace[CursorType])
                                       / (float)player_gui_def.HighlightFadeSteps[CursorType];
            CRGBColour start_col = player_gui_def.HighlightStartColour[CursorType];
            CRGBColour end_col = player_gui_def.HighlightEndColour[CursorType];
            long fade_steps = player_gui_def.HighlightFadeSteps[CursorType];
            CurrentColour.R += (unsigned char)((end_col.R - start_col.R) / fade_steps);
            CurrentColour.G += (unsigned char)((end_col.G - start_col.G) / fade_steps);
            CurrentColour.B += (unsigned char)((end_col.B - start_col.B) / fade_steps);
            CurrentColour.A += (unsigned char)((end_col.A - start_col.A) / fade_steps);
            CTCGraphicAppearance* ptc_appearance;
            if (PTargetedThing->GetTC(&ptc_appearance)) {
                const CTCTargeted* ptc_targeted;
                if (!PTargetedThing->PeekTC(&ptc_targeted) || ptc_targeted->IsToUseTargetGlow())
                    ptc_appearance->SetAsHighlighted(CurrentWidthWorldSpace, CurrentWidthScreenSpace, CurrentColour, true, true,
                                                 MESH_EFFECT_PRIORITY_TARGETING, this);
            }
            ++CurrentFadeStep;
        }
        bool added_primitives = false;
        if (CUserProfileManager::Get().GetShowTargetingStatus() && CursorType == CURSOR_TYPE_ENEMY
            && (GetPlayer().GetCurrentMode() == PLAYER_MODE_Z_TARGET
                || (GetPlayer().GetCurrentMode() == PLAYER_MODE_LIGHTNING
                    && GetPlayer().HasMode(PLAYER_MODE_Z_TARGET)))) {
            const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
            float graphic_width = (float)(Bank.GetGraphicMaxWidth(gui_def.TargetingBorderLeftGraphic) * GPDisplayManager->GetRenderTargetDimensions().X)
                          / 640.0f;
            float graphic_height = (float)(Bank.GetGraphicMaxHeight(gui_def.TargetingBorderLeftGraphic) * GPDisplayManager->GetRenderTargetDimensions().Y)
                           / 480.0f;
            C2DBoxF indicator_box;
            indicator_box.SetTLPos(NPlayerGui::GFAnchorToTopLeft(gui_def.TargetingBorderLeftPos));
            indicator_box.BRX = indicator_box.TLX + graphic_width;
            indicator_box.TLY = ((float)GPDisplayManager->GetRenderTargetDimensions().Y - graphic_height) * 0.5f;
            indicator_box.BRY = indicator_box.TLY + graphic_height;
            CEnginePrimitive2DSprite left_sprite(0, indicator_box, ENGINE_2D_LAYER_0, gui_def.TargetingBorderLeftGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, (EEngineInterpolationSet)0, 0.0f, 0.0f, ScaleAlpha(0xff),
                                          0xff, 0xff, 0xff, 2);
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[0], left_sprite, 0xc0);
            indicator_box.SetTLPos(NPlayerGui::GFAnchorToTopRight(gui_def.TargetingBorderRightPos));
            indicator_box.BRX = (float)GPDisplayManager->GetRenderTargetDimensions().X - gui_def.TargetingBorderLeftPos.X;
            indicator_box.TLX = indicator_box.BRX - graphic_width;
            indicator_box.TLY = ((float)GPDisplayManager->GetRenderTargetDimensions().Y - graphic_height) * 0.5f;
            indicator_box.BRY = indicator_box.TLY + graphic_height;
            CEnginePrimitive2DSprite right_sprite(0, indicator_box, ENGINE_2D_LAYER_0, gui_def.TargetingBorderRightGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, (EEngineInterpolationSet)0, 0.0f, 0.0f, ScaleAlpha(0xff),
                                           0xff, 0xff, 0xff, 2);
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[1], right_sprite, 0xc0);
            added_primitives = true;
        }
        if (!added_primitives) {
            std::for_each(PrimitiveHandles.begin(), PrimitiveHandles.end(), std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
        }
    }
}
// END_AUTHORED_BODY
// END_AUTHORED_BODY
static bool Near(float a,float b){return fabs((double)a-b)<0.0001;}
int main(){
 for(int c=0;c<4096;++c){
  for(int i=0;i<5;++i){
   Gui.HighlightStartColour[i].R=(unsigned char)(96+i*8);Gui.HighlightStartColour[i].G=(unsigned char)(112+i*8);Gui.HighlightStartColour[i].B=(unsigned char)(128+i*8);Gui.HighlightStartColour[i].A=(unsigned char)(144+i*8);
   int delta=(c&16)?-16:16;Gui.HighlightEndColour[i].R=(unsigned char)(Gui.HighlightStartColour[i].R+delta);Gui.HighlightEndColour[i].G=(unsigned char)(Gui.HighlightStartColour[i].G+delta);Gui.HighlightEndColour[i].B=(unsigned char)(Gui.HighlightStartColour[i].B+delta);Gui.HighlightEndColour[i].A=(unsigned char)(Gui.HighlightStartColour[i].A+delta);
   Gui.HighlightStartWidthWorldSpace[i]=2.0f+i;Gui.HighlightEndWidthWorldSpace[i]=6.0f+i;Gui.HighlightStartWidthScreenSpace[i]=4.0f+i;Gui.HighlightEndWidthScreenSpace[i]=12.0f+i;Gui.HighlightFadeSteps[i]=4;
  }
  Gui.TargetingBorderLeftGraphic=11;Gui.TargetingBorderRightGraphic=12;Gui.TargetingBorderLeftPos=C2DVector(12,20);Gui.TargetingBorderRightPos=C2DVector(-30,40);
  Display.Width=(c&128)?1280:640;Display.Height=(c&256)?960:480;CGraphicDataBank bank;bank.Width=16;bank.Height=24;
  CThingAICreature target,old;CThingPlayerCreature hero;CTCTargetingBase targeting;CTCBattleCharge charge;CTCShop shop;CTCShopKeeper keeper;CTCTavernGameMaster tavern;CTCWife wife;CTCActionUseBase action;CTCActionUseScriptedHook hook;CTCTargeted targeted;CTCGraphicAppearance appearance,oldappearance;
  CPlayerManager manager;manager.Player.Hero=&hero;manager.Player.Mode=(c/16)%3==0?PLAYER_MODE_CONTROL_CREATURE:((c/16)%3==1?PLAYER_MODE_Z_TARGET:PLAYER_MODE_LIGHTNING);manager.Player.ZMode=(c&128)!=0;hero.Interfaces[TCI_TARGETING]=&targeting;hero.Interfaces[TCI_BATTLE_CHARGE]=&charge;
  CWorld world;world.MapTableShowQuestCardsOnUsed=(c&256)!=0;int shape=c%16;bool present=shape!=0;targeting.Target=present?&target:0;EHighlightingCursorType wanted=CURSOR_TYPE_ALLY;
  switch(shape){
   case 0:wanted=CURSOR_TYPE_NONE;break;
   case 1:targeting.Usable=true;wanted=CURSOR_TYPE_NONE;break;
   case 2:targeting.Usable=targeting.InRange=true;wanted=CURSOR_TYPE_USABLE;break;
   case 3:target.Vulnerable=true;wanted=CURSOR_TYPE_ENEMY;break;
   case 4:target.Interfaces[TCI_SMASHABLE]=&target;wanted=CURSOR_TYPE_ENEMY;break;
   case 5:target.Interfaces[TCI_SMASHABLE]=&target;target.Def.Door=true;break;
   case 6:target.Information=true;wanted=CURSOR_TYPE_HAS_INFORMATION;break;
   case 7:shop.Available=true;keeper.Shop=&shop;target.Interfaces[TCI_SHOP_KEEPER]=&keeper;wanted=CURSOR_TYPE_HAS_INFORMATION;break;
   case 8:tavern.Game=&target;target.Interfaces[TCI_TAVERN_GAME_MASTER]=&tavern;wanted=CURSOR_TYPE_HAS_INFORMATION;break;
   case 9:wife.Love=true;target.Interfaces[TCI_WIFE]=&wife;wanted=CURSOR_TYPE_HAS_INFORMATION;break;
   case 10:wife.Married=wife.Sex=true;target.Interfaces[TCI_WIFE]=&wife;wanted=CURSOR_TYPE_HAS_INFORMATION;break;
   case 11:target.Type=0;break;
   case 12:target.Interfaces[TCI_HERO_GUIDE]=&target;wanted=world.MapTableShowQuestCardsOnUsed?CURSOR_TYPE_USABLE:CURSOR_TYPE_NONE;break;
   case 13:target.DefName=(c&512)?"CREATURE_DEMON_DOOR_FACE_01":"CREATURE_NW_DEMON_DOOR_FACE_01";wanted=CURSOR_TYPE_USABLE;break;
   case 14:charge.Active=(c&1024)!=0;charge.Target=&target;wanted=charge.Active?CURSOR_TYPE_ENEMY:CURSOR_TYPE_ALLY;break;
   case 15:targeting.Usable=targeting.InRange=true;target.Interfaces[TCI_ACTION_USE_SCRIPTED_HOOK]=&hook;wanted=CURSOR_TYPE_USABLE;break;
  }
  int use=(c/16)%4;if(use==1){action.Usable=false;target.Interfaces[TCI_ON_ACTION_USE]=&action;}else if(use==2){hook.Usable=false;target.Interfaces[TCI_ACTION_USE_SCRIPTED_HOOK]=&hook;}else if(use==3){target.Interfaces[TCI_ON_ACTION_USE]=&action;hook.Usable=false;target.Interfaces[TCI_ACTION_USE_SCRIPTED_HOOK]=&hook;}
  if(wanted==CURSOR_TYPE_USABLE&&(use==1||use==2))wanted=CURSOR_TYPE_NONE;
  bool have_appearance=(c&512)==0;bool targeted_present=(c&2048)!=0;targeted.Glow=(c&1024)==0;
  if(have_appearance)target.Interfaces[TCI_GRAPHIC_APPEARANCE_NEW]=&appearance;if(targeted_present)target.Interfaces[TCI_TARGETED]=&targeted;
  old.Alive=(c&256)==0;bool old_has_appearance=(c&1024)==0;if(old_has_appearance)old.Interfaces[TCI_GRAPHIC_APPEARANCE_NEW]=&oldappearance;
  NPlayerGui::CDrawHighlightingTargetingCursor draw(manager,world,bank);draw.Drawing=(c&64)!=0;draw.PTargetedThing.Data=(c&32)?targeting.Target:&old;
  draw.CursorType=(EHighlightingCursorType)((c/64)%5);draw.CurrentFadeStep=(c/256)%6;draw.CurrentWidthWorldSpace=20;draw.CurrentWidthScreenSpace=30;draw.CurrentColour.R=80;draw.CurrentColour.G=88;draw.CurrentColour.B=96;draw.CurrentColour.A=104;
  const bool changed=draw.PTargetedThing.Data!=targeting.Target;const bool initial_drawing=draw.Drawing;const long initial_fade=draw.CurrentFadeStep;const EHighlightingCursorType old_cursor=draw.CursorType;
  EHighlightingCursorType before=changed?CURSOR_TYPE_NONE:old_cursor;bool reset=present&&before!=wanted;long fade=reset?0:initial_fade;float ww=reset?Gui.HighlightStartWidthWorldSpace[wanted]:20.0f;float sw=reset?Gui.HighlightStartWidthScreenSpace[wanted]:30.0f;CRGBColour colour=reset?Gui.HighlightStartColour[wanted]:draw.CurrentColour;
  bool fading=present&&fade<4;if(fading){++fade;ww+=1.0f;sw+=2.0f;int delta=(c&16)?-4:4;colour.R=(unsigned char)((colour.R+delta+256)%256);colour.G=(unsigned char)((colour.G+delta+256)%256);colour.B=(unsigned char)((colour.B+delta+256)%256);colour.A=(unsigned char)((colour.A+delta+256)%256);}
  CUserProfileManager&p=CUserProfileManager::Get();p.UseNewProfileValues=(c&64)!=0;p.ShowTargetingStatusNewProfile=(c&128)!=0;p.ShowTargetingStatus=(c&256)!=0;bool show=p.UseNewProfileValues?p.ShowTargetingStatusNewProfile:p.ShowTargetingStatus;
  bool graphics=present&&wanted==CURSOR_TYPE_ENEMY&&show&&(manager.Player.Mode==PLAYER_MODE_Z_TARGET||(manager.Player.Mode==PLAYER_MODE_LIGHTNING&&manager.Player.ZMode));
  CIEngine engine;CEnginePrimitivePrepareInfo info(engine);draw.Draw(info);
  assert(draw.PTargetedThing.Data==targeting.Target);assert(oldappearance.Cleared==(changed&&old.Alive&&old_has_appearance?1:0));assert(draw.Drawing==(present?true:initial_drawing));assert(draw.CursorType==(present?wanted:before));
  assert(draw.CurrentFadeStep==fade&&Near(draw.CurrentWidthWorldSpace,ww)&&Near(draw.CurrentWidthScreenSpace,sw));assert(draw.CurrentColour.R==colour.R&&draw.CurrentColour.G==colour.G&&draw.CurrentColour.B==colour.B&&draw.CurrentColour.A==colour.A);
  long highlighted=fading&&have_appearance&&(!targeted_present||targeted.Glow)?1:0;assert(appearance.Highlighted==highlighted);if(highlighted){assert(appearance.Owner==&draw&&Near(appearance.WorldWidth,ww)&&Near(appearance.ScreenWidth,sw));assert(memcmp(&appearance.Colour,&colour,sizeof colour)==0);}
  assert(engine.Calls==(graphics?2:0));for(int i=0;i<2;++i)assert(draw.PrimitiveHandles[i].Clears==(present&&!graphics?1:0));
  if(graphics){float width=16.0f*(Display.Width/640),height=24.0f*(Display.Height/480),top=(Display.Height-height)*0.5f;for(int i=0;i<2;++i){assert(engine.Handles[i]==&draw.PrimitiveHandles[i]&&engine.Graphics[i]==(unsigned long)(11+i)&&engine.Alphas[i]==128);assert(Near(engine.Boxes[i].TLY,top)&&Near(engine.Boxes[i].BRY,top+height));}assert(Near(engine.Boxes[0].TLX,12)&&Near(engine.Boxes[0].BRX,12+width)&&Near(engine.Boxes[1].BRX,Display.Width-12.0f)&&Near(engine.Boxes[1].TLX,Display.Width-12.0f-width));}
 }
 puts("0064bfba_TEST PASS");return 0;
}
