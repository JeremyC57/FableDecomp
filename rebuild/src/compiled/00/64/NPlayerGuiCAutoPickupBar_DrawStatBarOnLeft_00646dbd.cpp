// NPlayerGui::CAutoPickupBar::DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos, const C2DVector& size, const CRGBColour& colour, unsigned char alpha, bool flash) -- advance the auto-pickup queue by the frame time (show the front pickup for the display time, then wait and drop it), point the bar at the front pickup's inventory graphic and count, then draw it as a stat bar
// Leaf: DrawStatBarOnLeft
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00646DBD, 381 bytes (size TU, frame pointer), `ret 1Ch`; Ego_r.pdb
//   ?DrawStatBarOnLeft@CAutoPickupBar@NPlayerGui@@UAEXABVCEnginePrimitivePrepareInfo@@ABVCGraphicDataBank@@ABVC2DVector@@2ABVCRGBColour@@E_N@Z.
// Callees: list<CAutoPickup>::pop_front 0x00655AA4 (catalog owner label propagated), CMainGameComponent::PeekDefinitionManager
//   0x00415FBC (catalog "GFGetPreMainMemoryUsed"), CDefinitionManager::GetDef<CObjectDef> 0x0042B040 and
//   CDefClassBase::GetSubDef<CInventoryItemDef> 0x005BADE1 (catalog labels propagated; same-TU out-only stand-ins),
//   CStatBar::DrawStatBarOnLeft 0x00646762; the defs' slot 1.
// Layout: as the landed sibling CAutoPickupBar::AddObject 0x00647511 (state +0x28, pickups list +0x68, CurrentDefIndex +0x6c,
//   WaitingTimeLeft +0x78; the info's display/wait times +0x520/+0x524); CStatBar shown value +0x10, GraphicFrame +0x50;
//   CInventoryItemDef Graphic +0x25 (packed); CMainGameComponent::Get() 0x013B86A0 (+0x24 state, paused byte +0xa4).
// Shape: the countdowns as the landed CMultiStatBar::DrawStatBarOnLeft 0x00646C44 (`t -= frameTime / period`, tested <= 0),
//   the paused test repeated per statement; the defs held by CDefPointer locals.
#include "fable_compiler.h"
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
// agentNF15: the pointee is held non-const whatever T is (engine/CDefPointer.h stores CDefPointeeBase*), so the
//   CDefPointer<const T> out slots of the const def lookups keep the landed member code.
template <class T> struct CDefNonConst { typedef T Type; };
template <class T> struct CDefNonConst<const T> { typedef T Type; };
template <class T> class CDefPointer { public:
    typename CDefNonConst<T>::Type* Object;
    FABLE_FORCEINLINE CDefPointer() : Object(0) {}
    FABLE_FORCEINLINE ~CDefPointer()
    {
        if (Object && --Object->IVRefCount == 0)
            Object->Delete();
    }
    typename CDefNonConst<T>::Type* operator->() const { return Object; }
};
class CDefClassBase : public CIVCountedPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x005BADE1 GetSubDef<CInventoryItemDef> (Ego_r.pdb member template)
};
class CThingObjectDef : public CDefClassBase { public:
};
void* FindDef_CObjectDef_GetSubDef();   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CInventoryItemDef>(CDefPointer<const CInventoryItemDef>* out) const
{
    void* def = FindDef_CObjectDef_GetSubDef();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042B040 GetDef<CThingObjectDef> (Ego_r.pdb member template)
};
class CGameDefinitionManager : public CDefinitionManager
{
};
void* FindDef_CDefinitionManager_GetDef();   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefinitionManager::GetDef<CThingObjectDef>(long index, CDefPointer<const CThingObjectDef>* out) const
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
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad00[0x24];
    CGameState*   GameState;                                // +0x24
public:
    const CGameDefinitionManager& PeekDefinitionManager() const;   // 0x00415FBC
};

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
            if (!CMainGameComponent::Get()->GameState->Paused && pickup.DefIndex != CurrentDefIndex) {
                CurrentDefIndex = pickup.DefIndex;
                pickup.TimeRemaining = m_pInfo->PickupDisplayTime;
            }
            if (!CMainGameComponent::Get()->GameState->Paused) {
                if ((pickup.TimeRemaining -= info.FrameTime / m_pInfo->PickupDisplayTime) <= 0.0f) {
                    m_state = 3;
                    WaitingTimeLeft = m_pInfo->PickupWaitTime;
                    if (Pickups.empty())
                        CurrentDefIndex = 0;
                }
            }
        } else if (m_state == 1) {
            if (!CMainGameComponent::Get()->GameState->Paused) {
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
        CDefPointer<const CThingObjectDef> def;
        CMainGameComponent::Get()->PeekDefinitionManager().GetDef(pickup.DefIndex, &def);
        CDefPointer<const CInventoryItemDef> itemDef;
        def->GetSubDef(&itemDef);
        GraphicFrame = itemDef->Graphic;
        Value = (float)pickup.Count;
    }
    CStatBar::DrawStatBarOnLeft(info, bank, pos, size, colour, alpha, flash);
}

}