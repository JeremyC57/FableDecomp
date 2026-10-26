// Behaviour test for 00646dbd: src/00646dbd.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// NPlayerGui::CAutoPickupBar::DrawStatBarOnLeft(...)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NPlayerGui::CAutoPickupBar::DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos, const C2DVector& size, const CRGBColour& colour, unsigned char alpha, bool flash) -- advance the auto-pickup queue by the frame time (show the front pickup for the display time, then wait and drop it), point the bar at the front pickup's inventory graphic and count, then draw it as a stat bar
// Leaf: DrawStatBarOnLeft
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00646DBD, 381 bytes (size TU, frame pointer), `ret 1Ch`; Ego_r.pdb
//   ?DrawStatBarOnLeft@CAutoPickupBar@NPlayerGui@@UAEXABVCEnginePrimitivePrepareInfo@@ABVCGraphicDataBank@@ABVC2DVector@@2ABVCRGBColour@@E_N@Z.
// Callees: list<CAutoPickup>::pop_front 0x00655AA4 (catalog owner label propagated), CMainGameComponent::PeekDefinitionManager
//   0x00415FBC (catalog "GFGetPreMainMemoryUsed"), CDefinitionManager::GetDef<CObjectDef> 0x0042B040 and
//   CDefClassBase::GetSubDef<CInventoryItemDef> 0x005BADE1 (catalog labels propagated; same-TU out-only stand-ins),
//   CStatBar::DrawStatBarOnLeft 0x00646762; the defs' slot 1.
// Layout: as the landed sibling CAutoPickupBar::AddObject 0x00647511 (state +0x28, pickups list +0x68, CurrentDefIndex +0x6c,
//   WaitingTimeLeft +0x78; the info's display/wait times +0x520/+0x524); CStatBar shown value +0x10, GraphicFrame +0x50;
//   CInventoryItemDef Graphic +0x25 (packed); g_pMainGameComponent 0x013B86A0 (+0x24 state, paused byte +0xa4).
// Shape: the countdowns as the landed CMultiStatBar::DrawStatBarOnLeft 0x00646C44 (`t -= frameTime / period`, tested <= 0),
//   the paused test repeated per statement; the defs held by CDefPointer locals.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CRGBColour { public: unsigned char B, G, R, A; };
class C2DVector { public: float X, Y; };
class CGraphicDataBank;
class CEnginePrimitivePrepareInfo { public:
    float FrameTime;                                        // +0x00
};

class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags);                        // slot 0
    virtual void Delete();                                  // slot 1
    unsigned long IVRefCount;                               // +4
};
#pragma pack(push, 1)
class CInventoryItemDef : public CIVCountedPointeeBase { public:
    unsigned char _pad08[0x25 - 0x08];
    long          Graphic;                                  // +0x25 (CEngineGraphic)
};
#pragma pack(pop)
template <class T> class CDefPointer { public:
    T* Object;
    __forceinline CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer()
    {
        if (Object && --Object->IVRefCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
struct CObjectDef : CIVCountedPointeeBase {
    void GetSubDef(CDefPointer<CInventoryItemDef>* out) const;   // 0x005BADE1
};
void* FindDef_CObjectDef_GetSubDef();   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) void CObjectDef::GetSubDef(CDefPointer<CInventoryItemDef>* out) const
{
    void* def = FindDef_CObjectDef_GetSubDef();
    if (!def)
        return;
    *(void**)out = def;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CObjectDef>* out) const;   // 0x0042B040
};
void* FindDef_CDefinitionManager_GetDef();   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CObjectDef>* out) const
{
    void* def = FindDef_CDefinitionManager_GetDef();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CGameState {
    unsigned char _pad00[0xa4];
    bool          Paused;                                   // +0xa4
};
class CMainGameComponent { public:
    unsigned char _pad00[0x24];
    CGameState*   GameState;                                // +0x24
    const CDefinitionManager& PeekDefinitionManager() const;   // 0x00415FBC
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0

namespace NPlayerGui {

struct CStatBarInfo {
    unsigned char _pad[0x520];
    float PickupDisplayTime;                                // +0x520
    float PickupWaitTime;                                   // +0x524
};

struct CAutoPickup {
    long  DefIndex;                                         // +0
    long  Count;                                            // +4
    float TimeRemaining;                                    // +8
};

struct CRetailList_AutoPickup {                             // retail list<CAutoPickup>: the sentinel pointer
    struct SNode {
        SNode*      next;
        SNode*      prev;
        CAutoPickup value;
    };
    SNode* Node;
    bool empty() const { return Node->next == Node; }
    CAutoPickup& front() { return Node->next->value; }
    void pop_front();                                       // 0x00655AA4
};

class CStatBar { public:                                           // vtable pointer +0x00
    CStatBarInfo* m_pInfo;                                  // +0x04
    unsigned char _pad_0x08[0x10 - 0x08];
    float         Value;                                    // +0x10 (the amount the bar shows)
    unsigned char _pad_0x14[0x28 - 0x14];
    long          m_state;                                  // +0x28 (0 showing, 1 waiting, 2 updating, 3 done)
    unsigned char _pad_0x2c[0x50 - 0x2c];
    unsigned long GraphicFrame;                             // +0x50
    unsigned char _pad_0x54[0x68 - 0x54];
    virtual void DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos,
                                   const C2DVector& size, const CRGBColour& colour, unsigned char alpha, bool flash);   // 0x00646762
};

class CAutoPickupBar : public CStatBar { public:
    CRetailList_AutoPickup Pickups;                         // +0x68
    long  CurrentDefIndex;                                  // +0x6C
    long  CurrentCount;                                     // +0x70
    float CurrentTimeLeft;                                  // +0x74
    float WaitingTimeLeft;                                  // +0x78
    virtual void DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos,
                                   const C2DVector& size, const CRGBColour& colour, unsigned char alpha, bool flash);
};

void CAutoPickupBar::DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos,
                                       const C2DVector& size, const CRGBColour& colour, unsigned char alpha, bool flash)
{
    if (!Pickups.empty()) {
        if (m_state == 0) {
            CAutoPickup& pickup = Pickups.front();
            if (!g_pMainGameComponent->GameState->Paused && pickup.DefIndex != CurrentDefIndex) {
                CurrentDefIndex = pickup.DefIndex;
                pickup.TimeRemaining = m_pInfo->PickupDisplayTime;
            }
            if (!g_pMainGameComponent->GameState->Paused) {
                if ((pickup.TimeRemaining -= info.FrameTime / m_pInfo->PickupDisplayTime) <= 0.0f) {
                    m_state = 3;
                    WaitingTimeLeft = m_pInfo->PickupWaitTime;
                    if (Pickups.empty())
                        CurrentDefIndex = 0;
                }
            }
        } else if (m_state == 1) {
            if (!g_pMainGameComponent->GameState->Paused) {
                if ((WaitingTimeLeft -= info.FrameTime / m_pInfo->PickupWaitTime) <= 0.0f) {
                    if (!Pickups.empty())
                        Pickups.pop_front();
                    if (!Pickups.empty())
                        m_state = 2;
                }
            }
        }
    }
    if (!Pickups.empty()) {
        CAutoPickup& pickup = Pickups.front();
        CDefPointer<CObjectDef> def;
        g_pMainGameComponent->PeekDefinitionManager().GetDef(pickup.DefIndex, &def);
        CDefPointer<CInventoryItemDef> itemDef;
        def->GetSubDef(&itemDef);
        GraphicFrame = itemDef->Graphic;
        Value = (float)pickup.Count;
    }
    CStatBar::DrawStatBarOnLeft(info, bank, pos, size, colour, alpha, flash);
}

}

#pragma optimize("", on)
// ---- stubs ----
static char g_log[256];
static int g_step = 0;
static void Log(const char* s) { char b[64]; _snprintf(b, sizeof b, "%d:%s;", ++g_step, s); strcat(g_log, b); }
void CIVCountedPointeeBase::Destroy(int) {}
static int g_deleted = 0;
void CIVCountedPointeeBase::Delete() { ++g_deleted; }
static CInventoryItemDef* g_item = 0;
void* FindDef_CObjectDef_GetSubDef() { Log("GetSubDef"); if (g_item) ++g_item->IVRefCount; return g_item; }
static CObjectDef* g_object = 0; static long g_index = -1;
void* FindDef_CDefinitionManager_GetDef() { Log("GetDef"); if (g_object) ++g_object->IVRefCount; return g_object; }
static CDefinitionManager g_defs;
const CDefinitionManager& CMainGameComponent::PeekDefinitionManager() const { Log("PeekDefs"); return g_defs; }
static CGameState g_state;
static CMainGameComponent g_game;
CMainGameComponent* g_pMainGameComponent = &g_game;
namespace NPlayerGui {
static int g_pops = 0;
void CRetailList_AutoPickup::pop_front()
{
    Log("pop"); ++g_pops;
    SNode* n = Node->next; Node->next = n->next; n->next->prev = Node;
}
static int g_drawn = 0; static const C2DVector* g_drawPos = 0; static unsigned char g_alpha = 0; static bool g_flash = false;
void CStatBar::DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo&, const CGraphicDataBank&, const C2DVector& pos, const C2DVector&,
                                 const CRGBColour&, unsigned char alpha, bool flash)
{ Log("Base"); ++g_drawn; g_drawPos = &pos; g_alpha = alpha; g_flash = flash; }
}
using namespace NPlayerGui;

static void Reset() { g_log[0] = 0; g_step = 0; g_pops = 0; g_drawn = 0; g_deleted = 0; }

int main()
{
    g_game.GameState = &g_state; g_state.Paused = false;
    static CStatBarInfo info; info.PickupDisplayTime = 2.0f; info.PickupWaitTime = 4.0f;
    CInventoryItemDef item; memset(&item, 0, sizeof item); *(void**)&item = *(void**)&CInventoryItemDef(); item.IVRefCount = 1; item.Graphic = 77; g_item = &item;
    CObjectDef object; object.IVRefCount = 1; g_object = &object;
    CAutoPickupBar bar; memset((char*)&bar + 4, 0, sizeof bar - 4); bar.m_pInfo = &info;
    CRetailList_AutoPickup::SNode head, a, b;
    head.next = &a; a.prev = &head; a.next = &b; b.prev = &a; b.next = &head; head.prev = &b;
    a.value.DefIndex = 11; a.value.Count = 3; a.value.TimeRemaining = 0.0f;
    b.value.DefIndex = 12; b.value.Count = 1; b.value.TimeRemaining = 0.0f;
    bar.Pickups.Node = &head;
    CEnginePrimitivePrepareInfo prep = { 0.5f };
    C2DVector pos = { 1, 2 }, size = { 3, 4 }; CRGBColour colour = { 1, 2, 3, 4 };
    const CGraphicDataBank& bank = *(const CGraphicDataBank*)&info;
    // 1: showing a new front pickup: it becomes current with the full display time, then counts down by 0.5 / 2
    {
        Reset(); bar.m_state = 0; bar.CurrentDefIndex = 0;
        bar.DrawStatBarOnLeft(prep, bank, pos, size, colour, 200, true);
        CHECK(bar.CurrentDefIndex == 11 && a.value.TimeRemaining == 1.75f && bar.m_state == 0);
        CHECK(strcmp(g_log, "1:PeekDefs;2:GetDef;3:GetSubDef;4:Base;") == 0);
        CHECK(bar.GraphicFrame == 77 && bar.Value == 3.0f && g_drawn == 1 && g_drawPos == &pos && g_alpha == 200 && g_flash);
        CHECK(item.IVRefCount == 1 && object.IVRefCount == 1 && g_deleted == 0);
    }
    // 2: the same pickup keeps counting; once at or below zero the bar switches to done and starts the wait
    {
        Reset(); a.value.TimeRemaining = 0.25f;
        bar.DrawStatBarOnLeft(prep, bank, pos, size, colour, 0, false);
        CHECK(a.value.TimeRemaining == 0.0f && bar.m_state == 3 && bar.WaitingTimeLeft == 4.0f && bar.CurrentDefIndex == 11);
    }
    // 3: paused: nothing counts down
    {
        Reset(); bar.m_state = 0; a.value.TimeRemaining = 1.0f; g_state.Paused = true; bar.CurrentDefIndex = 5;
        bar.DrawStatBarOnLeft(prep, bank, pos, size, colour, 0, false);
        CHECK(a.value.TimeRemaining == 1.0f && bar.CurrentDefIndex == 5 && bar.m_state == 0 && g_drawn == 1);
        g_state.Paused = false;
    }
    // 4: waiting: count the wait down by 0.5 / 4; at zero drop the front and go to updating while pickups remain
    {
        Reset(); bar.m_state = 1; bar.WaitingTimeLeft = 0.25f;
        bar.DrawStatBarOnLeft(prep, bank, pos, size, colour, 0, false);
        CHECK(bar.WaitingTimeLeft == 0.125f && g_pops == 0);
        Reset(); bar.WaitingTimeLeft = 0.1f;
        bar.DrawStatBarOnLeft(prep, bank, pos, size, colour, 0, false);
        CHECK(g_pops == 1 && head.next == &b && bar.m_state == 2 && bar.Value == 1.0f);
    }
    // 5: the last pickup dropped: no update (state stays waiting), no def lookup, still drawn
    {
        Reset(); bar.m_state = 1; bar.WaitingTimeLeft = 0.1f;
        bar.DrawStatBarOnLeft(prep, bank, pos, size, colour, 0, false);
        CHECK(g_pops == 1 && head.next == &head && bar.m_state == 1 && strcmp(g_log, "1:pop;2:Base;") == 0);
    }
    // 6: last references: both defs are released and deleted
    {
        head.next = &a; a.prev = &head; a.next = &head; head.prev = &a;
        Reset(); bar.m_state = 2; item.IVRefCount = 0; object.IVRefCount = 0;
        bar.DrawStatBarOnLeft(prep, bank, pos, size, colour, 0, false);
        CHECK(g_deleted == 2 && item.IVRefCount == 0 && object.IVRefCount == 0);
    }
    if (fails == 0) printf("00646dbd_TEST PASS\n");
    return fails;
}