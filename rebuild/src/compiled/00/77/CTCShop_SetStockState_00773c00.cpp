// CTCShop::SetStockState(long state) -- on a change: store it, tell the keeper's CTCShopKeeper component (interface 0x90) to repress opinions unless the state is 1, then refresh the player GUI
// Leaf: SetStockState
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe 0x00773C00, 140 bytes (`ret 4`; the GUI refresh is a `mov [esp+4],0 ; jmp` tail call).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (inline GetInterface), CTCShopKeeper::RepressOpinions(bool) @ 0x007996B0, CPlayerGui::UpdateMiniMapMarkers(bool)
// @ 0x004378C0 (`mov ecx,[ecx+160h] ; jmp`; manifest "_Cons_val<...>" is propagated) on the global @ 0x013B8790.
// Layout: retail CTCShop: StockState +0x0C, PShopKeeper +0x10; CThing presence bitset +0x20 (0x90 = bit 16 of +0x30),
// interface map +0x44, dead flag +0x91 bit 0.
// Shape: the new state is stored before the keeper tests; `state != 1` is pushed as `setne dl`. The shop-keeper
// component comes from the out-parameter GetInterface whose entry pick is one end/hit ternary: the component loads
// straight into ecx before the bool is formed (a HasInterface test + GetTC, or the if-form pick, forms the bool first).
struct CTCBaseAny;
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}
class CTCShopKeeper { public:
    void RepressOpinions(bool repress);             // 0x007996B0
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    void UpdateMiniMapMarkers(bool flag);           // 0x004378C0
};
class CTCShop { public:
    virtual void v00();
    void*                   ParentThing;            // +0x04 (CTCBase)
    bool                    ToBeRemoved;            // +0x08
    bool                    IsInGlobalUpdateTCs;    // +0x09
    long                    StockState;             // +0x0c
    CIntelligentPointer<CThing> PShopKeeper;            // +0x10
    void SetStockState(long state);
};

void CTCShop::SetStockState(long state)
{
    if (state != StockState) {
        StockState = state;
        if (IS_THING_ALIVE(PShopKeeper)) {
            CThing* keeper = PShopKeeper.Get();
            CTCShopKeeper* shopKeeper;
            if (keeper->GetTC(TCI_SHOP_KEEPER, &shopKeeper))
                shopKeeper->RepressOpinions(state != 1);
        }
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().UpdateMiniMapMarkers(false);
    }
}