// CTCTavernGame::DrawBetAndMoney(const CEnginePrimitivePrepareInfo&, float, float, float, float, float, float) -- draw the bet box, the money box (money flag set while the game state is 5) and, when both its coordinates are non-zero, a third box whose width comes from the tavern-game def
// Leaf: DrawBetAndMoney
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EA400, length 592 bytes, `ret 0x1c`.
// Callees: CDefClassBase::GetSubDef<CTavernGameDef> @ 0x005ED4A0, CDisplayManager::GetRenderTargetDimensions @ 0x009BEDC0 on
// g_pDisplayDevice 0x013B8390, PeekGuiDef @ 0x00438570, DrawStatBoxWithText @ 0x00646A46 (__fastcall), ~CWideString @ 0x0099B510,
// the game's text virtuals at vtable +0xbc / +0xc0 / +0xc4 (CWideString by hidden pointer).
// Layout: retail CTCTavernGame with 12-byte STLport vectors (see differ/005ea650.cpp): three handle vectors +0x5c / +0x68 / +0x74
// (the DrawStatBoxWithText edx argument), the game state +0xa8 (compared with 5); the tavern-game def's third-box width +0x10c.
// Shape: the landed CTCBetting::DrawPreparePrimitivesForRendering (0x007E5680) form -- a zeroed CDefPointer released by its
// inline null-tested destructor, each box one call with a C2DVector temporary of `coordinate x screen size`; the money box
// is an if/else of two full calls (tail-merged) and a same-TU stand-in GetSubDef body keeps the def pointer in ebx.
#include <stddef.h>
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C2DExtentsI {
    long X, Y;
    C2DExtentsI() {}
};
struct CDisplayManager {
    const C2DExtentsI GetRenderTargetDimensions();   // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayDevice;      // 0x013B8390

struct CWideString {
    void* PStringData;
    ~CWideString();                             // 0x0099B510
};
struct CEnginePrimitiveHandle { void* _vtable; void* Data; };
struct CHandleVector {
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
};
struct CEnginePrimitivePrepareInfo;
void __fastcall DrawStatBoxWithText(const CEnginePrimitivePrepareInfo& info, CHandleVector& handles, const C2DVector& pos,
                                    long width, const CWideString& text, long value, unsigned char alpha, bool money);   // 0x00646A46
struct CPlayerGuiDef {
    unsigned char _pad00[0x2e8];
    long          StatBoxWidth;                 // +0x2e8
};
CPlayerGuiDef* __fastcall PeekGuiDef();         // 0x00438570

struct CTavernGameDef {
    virtual void v00();
    virtual void Destroy();
    long          RefCount;
    unsigned char _pad08[0x10c - 0x08];
    long          ThirdBoxWidth;                // +0x10c
    __forceinline void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CTavernGameDef>* out);   // 0x005ED4A0
};
CTavernGameDef* FindRef_GetSubDef_0();
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<CTavernGameDef>* out)
{
    CTavernGameDef* def = FindRef_GetSubDef_0();
    if (!def)
        return false;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)
struct CThingWithDef {
    unsigned char  _pad00[0x70];
    CDefClassBase* PBaseDef;                    // +0x70
};

class CTCTavernGame {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46();
    virtual CWideString GetBetText();           // +0xbc
    virtual CWideString GetMoneyText();         // +0xc0
    virtual CWideString GetThirdText();         // +0xc4
    CThingWithDef* ParentThing;                 // +0x04
    unsigned char  _pad08[0x5c - 0x08];
    CHandleVector  BetHandles;                  // +0x5c
    CHandleVector  MoneyHandles;                // +0x68
    CHandleVector  ThirdHandles;                // +0x74
    unsigned char  _pad80[0xa8 - 0x80];
    long           State;                       // +0xa8
    void DrawBetAndMoney(const CEnginePrimitivePrepareInfo& info, float betX, float betY, float moneyX, float moneyY,
                         float thirdX, float thirdY);
};

void CTCTavernGame::DrawBetAndMoney(const CEnginePrimitivePrepareInfo& info, float betX, float betY, float moneyX,
                                    float moneyY, float thirdX, float thirdY)
{
    CDefPointer<CTavernGameDef> def;
    ParentThing->PBaseDef->GetSubDef(&def);
    DrawStatBoxWithText(info, BetHandles,
                        C2DVector(betX * g_pDisplayDevice->GetRenderTargetDimensions().X,
                                  betY * g_pDisplayDevice->GetRenderTargetDimensions().Y),
                        PeekGuiDef()->StatBoxWidth, GetBetText(), 0, 0xff, false);
    if (State == 5)
        DrawStatBoxWithText(info, MoneyHandles,
                            C2DVector(moneyX * g_pDisplayDevice->GetRenderTargetDimensions().X,
                                      moneyY * g_pDisplayDevice->GetRenderTargetDimensions().Y),
                            PeekGuiDef()->StatBoxWidth, GetMoneyText(), 0, 0xff, true);
    else
        DrawStatBoxWithText(info, MoneyHandles,
                            C2DVector(moneyX * g_pDisplayDevice->GetRenderTargetDimensions().X,
                                      moneyY * g_pDisplayDevice->GetRenderTargetDimensions().Y),
                            PeekGuiDef()->StatBoxWidth, GetMoneyText(), 0, 0xff, false);
    if (thirdX != 0.0f && thirdY != 0.0f)
        DrawStatBoxWithText(info, ThirdHandles,
                            C2DVector(thirdX * g_pDisplayDevice->GetRenderTargetDimensions().X,
                                      thirdY * g_pDisplayDevice->GetRenderTargetDimensions().Y),
                            def->ThirdBoxWidth, GetThirdText(), 0, 0xff, false);
}
