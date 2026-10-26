// CTCBetting::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- while betting input is on, draw the bet box and the player's money box at their def positions (screen fractions of the render target)
// Retry 10b (getsw sweep, FIXES B-15): the base def is read through an inline getter `GetBaseDef()`; the plain member read gives the parked register/scheduling difference.
// Leaf: DrawPreparePrimitivesForRendering
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E5680, 320 bytes, `ret 4`.
// Callees: CDefClassBase::GetSubDef<CBettingDef> 0x007E5820, CDisplayManager::GetRenderTargetDimensions() 0x009BEDC0 on
//   g_pDisplayDevice 0x013B8390 (C2DExtentsI by hidden pointer, twice per position), CTCBetting::GetBetText 0x007E50C0
//   (descriptive; manifest "CTCSinglePersonOccupiable::IsOfType" is propagated), CTCBetting::GetPlayersMoney 0x007E5570,
//   PeekGuiDef 0x00438570 (__fastcall), DrawStatBoxWithText(const CEnginePrimitivePrepareInfo&, vector<
//   CEnginePrimitiveHandle>&, const C2DVector&, long, const CWideString&, long, unsigned char, bool) 0x00646A46
//   (__fastcall), ~CWideString 0x0099B510; the def's virtual Destroy (slot 1) on release.
// Globals: g_bBettingInputEnabled 0x013BAE1D.
// Layout = Ego_r.pdb CTCBetting with 12-byte retail vectors: BetHandles +0x10, MoneyHandles +0x1C; the betting def's bet
//   box position +0x28, money box position +0x30 (descriptive); the GUI def's stat-box width +0x2E8 (descriptive).
// Shape: the def pointer is a zeroed local released by its inline destructor (the null test folds on the path that read
//   it); each box is one DrawStatBoxWithText call with a C2DVector temporary built from `def x screen size` (Y first:
//   arguments evaluate right to left) and the text a by-value CWideString temporary. The def object is read once into a
//   named local; a same-TU stand-in GetSubDef body lets VC7.1 keep it in esi and reuse the pointer's frame word (the
//   retail 0x20 frame).
// Tried: the def through operator-> at each use (0x24 frame, def copied to two registers), the named def local alone
//   (0x24 frame), an out pointer plus a releasing holder (0x24 frame); the base def through the inline GetBaseDef()
//   accessor fixes the last register rotation.
#include <stddef.h>
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class C2DExtentsI { public:
    long X, Y;
    C2DExtentsI() {}
};
class CDisplayManager { public:
    const C2DExtentsI GetRenderTargetDimensions();   // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayDevice;      // 0x013B8390
extern bool g_bBettingInputEnabled;             // 0x013BAE1D

class CWideString { public:
    void* PStringData;
    ~CWideString();                             // 0x0099B510
};
class CEnginePrimitiveHandle { public: void* _vtable; void* Data; };
struct CHandleVector {                          // STLport vector<CEnginePrimitiveHandle> (12 bytes)
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
};
class CEnginePrimitivePrepareInfo;
void __fastcall DrawStatBoxWithText(const CEnginePrimitivePrepareInfo& info, CHandleVector& handles, const C2DVector& pos,
                                    long width, const CWideString& text, long value, unsigned char alpha, bool money);   // 0x00646A46
class CPlayerGuiDef { public:
    unsigned char _pad00[0x2e8];
    long          StatBoxWidth;                 // +0x2e8
};
CPlayerGuiDef* __fastcall PeekGuiDef();         // 0x00438570

class CBettingDef { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long          RefCount;                     // +0x04
    unsigned char _pad08[0x28 - 0x08];
    float         BetBoxX;                      // +0x28
    float         BetBoxY;                      // +0x2c
    float         MoneyBoxX;                    // +0x30
    float         MoneyBoxY;                    // +0x34
    __forceinline void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CBettingDef>* out);   // 0x007E5820
};
CBettingDef* FindRef_GetSubDef_0();             // stand-in callee of the stand-in body below
// Same-TU stand-in for GetSubDef (the real one is out of line in retail; LTCG knew it): it only writes the out pointer,
// so VC7.1 knows the def pointer's address does not escape the call (the 0x007E4B60 / 0x00702B10 precedent).
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<CBettingDef>* out)
{
    CBettingDef* def = FindRef_GetSubDef_0();
    if (!def)
        return false;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)
struct CThingWithDef {
    unsigned char  _pad00[0x70];
    CDefClassBase* PBaseDef;                    // +0x70
    CDefClassBase* GetBaseDef() const { return PBaseDef; }
};

class CTCBetting {
public:
    void*          __vftable;                   // +0x00 (CTCBase)
    CThingWithDef* ParentThing;                 // +0x04
    unsigned char  _pad08[0x10 - 0x08];
    CHandleVector  BetHandles;                  // +0x10
    CHandleVector  MoneyHandles;                // +0x1c
    CWideString GetBetText();                   // 0x007E50C0
    CWideString GetPlayersMoney();              // 0x007E5570
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

void CTCBetting::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    CDefPointer<CBettingDef> def;
    if (g_bBettingInputEnabled) {
        if (ParentThing->GetBaseDef()->GetSubDef(&def)) {
            CBettingDef* bettingDef = def.Object;
            DrawStatBoxWithText(info, BetHandles,
                                C2DVector(bettingDef->BetBoxX * g_pDisplayDevice->GetRenderTargetDimensions().X,
                                          bettingDef->BetBoxY * g_pDisplayDevice->GetRenderTargetDimensions().Y),
                                PeekGuiDef()->StatBoxWidth, GetBetText(), 0, 0xff, false);
            DrawStatBoxWithText(info, MoneyHandles,
                                C2DVector(bettingDef->MoneyBoxX * g_pDisplayDevice->GetRenderTargetDimensions().X,
                                          bettingDef->MoneyBoxY * g_pDisplayDevice->GetRenderTargetDimensions().Y),
                                PeekGuiDef()->StatBoxWidth, GetPlayersMoney(), 0, 0xff, true);
        }
    }
}