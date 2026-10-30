// CNewFrontendGameComponent::Draw(const CEngineInterpolationInfo& info) -- render one front-end frame
// Grade: DIFFER(478v478) -- one spill-order residue in the second inlined SetDesiredState (retail `mov esi,ecx` before `mov [ebp+8],edx`, BookmarkMask read into edx; ours edi). 6 grades.
// Leaf: ?Draw@CNewFrontendGameComponent@@AAEXABVCEngineInterpolationInfo@@@Z
// Retail-asm: 0x0042DFD8 fnstcw / 0x0042DFDF fldcw -- the x87 rounding control is cleared to round-to-nearest in line
//   (`fnstcw [ebp-4] ; and byte ptr [ebp-3],0F3h ; fldcw [ebp-4]`); VC7.1 has no control-word intrinsic (_controlfp is a
//   CRT call), so retail ran these two instructions as inline assembly.
// Retail: Fable.exe @ 0x0042DF9E, 478 bytes, `ret 4`, ebp frame (the inline asm). Names: Ego_r.pdb
// ?Draw@CNewFrontendGameComponent@@AAEXABVCEngineInterpolationInfo@@@Z (private); callees ??0CInterpolationInfo@@QAE@XZ
// 0x00415A60, ?GFGetTime@@YINXZ 0x009E1BC0, ?SetVWindow@CRenderManagerCore@@QAEXABVC2DBoxF@@@Z 0x00A0B560,
// ?GetVVisibleTelevisionScreenArea@CDisplayManager@@QBE?AVC2DBoxF@@XZ 0x009BECE0,
// ?ClearRenderTarget@CRenderManager2D@@QAEXABVCRGBColour@@K@Z 0x009D8CF0, ?BeginRender@CDisplayManager@@QAE_NXZ 0x009BEF20,
// ?GetInstance@CFrontEndManager@NUISystem@@SIPAV12@XZ 0x00595582,
// ?Draw@CFrontEndManager@NUISystem@@QAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@@Z 0x00595222,
// ?GetInstance@CManager@NUISystem@@SIPAV12@XZ 0x0041E5F2, ?RenderErrorMessage@CManager@NUISystem@@QAEXAAVCIEngine@@@Z 0x0041D03C,
// CIEngine::Render (virtual slot 8, +0x20), ?DrawBufferedPolys@CRenderManager2D@@QAEXXZ 0x009D9C80,
// ?Render2DDrawList@CRenderManager2D@@QAEX_N@Z 0x009DA9F0, ?Get@CIME@NUISystem@@SIAAV12@XZ 0x00404A80,
// ?Render@CIME@NUISystem@@QAEXXZ 0x00404C00, ?EndRender@CDisplayManager@@QAEXXZ 0x009BEF50,
// ?SwapScreens@CDisplayManager@@QAE_NXZ 0x009BEEB0. Globals (bound names): g_pRenderManager2D 0x013B8384,
// g_pDisplayManager 0x013B8390, g_CoordinateWidth / g_CoordinateHeight 0x013961E8 / 0x013961F0, the default texture mip-map
// mode 0x01396E68 (descriptive name).
// Layout: CRenderStateManager from the shared header (Ego_r): DeviceState +0x2818 holds CStateInfo FogEnable (+0x12c) and
// TextureMipMapMode[8] (+0x62c); CStateInfo is 0x14 bytes (CurrentState, DesiredState, BookmarkMask, ..., DirtyListFlag +0x10).
// The render-state setter is the inlined bookmark-aware desired-state update (state stack +0x08, update list +0x2008).
// CNewFrontendGameComponent: m_pEngine +0x58, m_fLastRenderTime +0xbc (retail; the landed ctor 0x0042EA8F).
// NUISystem::CManager: m_bHasErrorMessage at retail +0x9c (Ego_r +0xc8).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define CRenderStateManager CRenderStateManagerPdb
#include "engine/CRenderStateManager.h"
#undef CRenderStateManager

class CEnginePrimitiveHandle;
class CEngineInterpolationInfo;

class CInterpolationInfo
{
public:
    CInterpolationInfo();                                   // 0x00415A60
    unsigned char _body[0x34];
};

class C2DBoxF
{
public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF() { }
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};

class CRGBColour
{
public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

struct CStateInfo
{
    unsigned long CurrentState;                             // +0x00
    unsigned long DesiredState;                             // +0x04
    unsigned long BookmarkMask;                             // +0x08
    unsigned long DeviceStateID;                            // +0x0c
    bool          DirtyListFlag;                            // +0x10
    unsigned char StateType;                                // +0x11
    unsigned char TextureStage;                             // +0x12
};
struct CStateStackElement
{
    CStateInfo*   State;
    unsigned long Value;
};
struct CDeviceState
{
    unsigned char _pad000[0x12c];
    CStateInfo    FogEnable;                                // +0x12c
    unsigned char _pad140[0x62c - 0x140];
    CStateInfo    TextureMipMapMode[8];                     // +0x62c
};

class CRenderStateManager : public CRenderStateManagerPdb
{
public:
    CDeviceState& GetDeviceState() { return *reinterpret_cast<CDeviceState*>(DeviceState); }
    __forceinline void SetDesiredState(CStateInfo& state, unsigned long value)
    {
        if (state.DesiredState == value)
            return;
        if (!(state.BookmarkMask & CurrentBookmarkMask)) {
            state.BookmarkMask |= CurrentBookmarkMask;
            long& size = StateStackSize;
            reinterpret_cast<CStateStackElement*>(StateStack)[size].State = &state;
            reinterpret_cast<CStateStackElement*>(StateStack)[size].Value = state.DesiredState;
            ++size;
        }
        state.DesiredState = value;
        if (!state.DirtyListFlag) {
            state.DirtyListFlag = true;
            long& count = StateUpdateListSize;
            StateUpdateList[count] = &state;
            ++count;
        }
    }
};

class CRenderManagerCore : public CRenderStateManager
{
public:
    void SetVWindow(const C2DBoxF& box);                    // 0x00A0B560
};

class CRenderManager2D : public CRenderManagerCore
{
public:
    void ClearRenderTarget(const CRGBColour& colour, unsigned long flags);   // 0x009D8CF0
    void DrawBufferedPolys();                               // 0x009D9C80
    void Render2DDrawList(bool flush);                      // 0x009DA9F0
};
extern CRenderManager2D* g_pRenderManager2D;                // 0x013B8384

class CDisplayManager
{
public:
    C2DBoxF GetVVisibleTelevisionScreenArea() const;        // 0x009BECE0
    bool BeginRender();                                     // 0x009BEF20
    void EndRender();                                       // 0x009BEF50
    bool SwapScreens();                                     // 0x009BEEB0
};
extern CDisplayManager* g_pDisplayManager;                  // 0x013B8390

extern double g_CoordinateWidth;                            // 0x013961E8
extern double g_CoordinateHeight;                           // 0x013961F0
extern unsigned long g_DefaultTextureMipMapMode;            // 0x01396E68
double __fastcall GFGetTime();                              // 0x009E1BC0

class CIEngine
{
public:
    virtual void _slot00(); virtual void _slot01(); virtual void _slot02(); virtual void _slot03();
    virtual void _slot04(); virtual void _slot05(); virtual void _slot06(); virtual void _slot07();
    virtual void Render(const CEngineInterpolationInfo& info, unsigned long flags);   // slot 8 (+0x20)
};

template <class T> class CCountedPointer
{
public:
    T* Data;
    long* RefCount;
    T& operator*() const { return *Data; }
    T* operator->() const { return Data; }
};

namespace NUISystem
{
    class CFrontEndManager
    {
    public:
        static CFrontEndManager* __fastcall GetInstance();  // 0x00595582
        void Draw(CIEngine& engine, CEnginePrimitiveHandle* handle);   // 0x00595222
    };
    class CManager
    {
    public:
        static CManager* __fastcall GetInstance();          // 0x0041E5F2
        void RenderErrorMessage(CIEngine& engine);          // 0x0041D03C
        unsigned char _pad00[0x9c];
        bool          m_bHasErrorMessage;                   // +0x9c
    };
    class CIME
    {
    public:
        static CIME& __fastcall Get();                      // 0x00404A80
        void Render();                                      // 0x00404C00
    };
}

class CNewFrontendGameComponent
{
private:
    unsigned char             _pad00[0x58];
    CCountedPointer<CIEngine> m_pEngine;                    // +0x58
    unsigned char             _pad60[0xbc - 0x60];
    float                     m_fLastRenderTime;            // +0xbc
    void Draw(const CEngineInterpolationInfo& info);
};

void CNewFrontendGameComponent::Draw(const CEngineInterpolationInfo& info)
{
    CInterpolationInfo interpolation;
    m_fLastRenderTime = (float)GFGetTime();
    C2DBoxF screen(0.0f, 0.0f, (float)g_CoordinateWidth, (float)g_CoordinateHeight);
    {
        unsigned long control;                              // x87 rounding control -> round to nearest
        __asm fnstcw word ptr control
        reinterpret_cast<unsigned char*>(&control)[1] &= 0xF3;
        __asm fldcw word ptr control
    }
    g_pRenderManager2D->SetDesiredState(g_pRenderManager2D->GetDeviceState().FogEnable, 0);
    g_pRenderManager2D->SetVWindow(screen);
    g_pDisplayManager->GetVVisibleTelevisionScreenArea();
    g_pRenderManager2D->ClearRenderTarget(CRGBColour(0, 0, 0, 0xff), 0);
    g_pDisplayManager->BeginRender();
    NUISystem::CFrontEndManager::GetInstance()->Draw(*m_pEngine, 0);
    if (NUISystem::CManager::GetInstance()->m_bHasErrorMessage)
        NUISystem::CManager::GetInstance()->RenderErrorMessage(*m_pEngine);
    m_pEngine->Render(info, 0);
    g_pRenderManager2D->SetDesiredState(g_pRenderManager2D->GetDeviceState().TextureMipMapMode[0], g_DefaultTextureMipMapMode);
    g_pRenderManager2D->DrawBufferedPolys();
    g_pRenderManager2D->Render2DDrawList(true);
    NUISystem::CIME::Get().Render();
    g_pRenderManager2D->DrawBufferedPolys();
    g_pRenderManager2D->Render2DDrawList(true);
    g_pDisplayManager->EndRender();
    g_pDisplayManager->SwapScreens();
}
