// CTCInventoryTrade::DeleteComponents() -- deletes the trade screen's currently open trade menu component (and drops it from the Components list), then has the front-end manager tidy up and clears the global front-end node list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059C8AE, 68 bytes.
// Callees: NUISystem::CComponent scalar deleting destructor (virtual slot 0, `push 1`), the front-end manager accessor
//          CFrontEndManager::GetInstance @ 0x0041E5F2 (`__fastcall`, as in the landed CTCInventoryClothing::OpenInventory
//          0x005B4E7C) and its virtual slots 37 (+0x94) and 29 (+0x74) (names unknown), the global VC6 node list
//          @ 0x013B8A98 (98 references) cleared by its out-of-line clear @ 0x004855A3 (tail jump; manifest label
//          "CGuiControlTreePane::SortTreeRecursively" is propagated).
//
// Shape: a size-optimised TU (`#pragma optimize("s", on)`; `and dword ptr [esi+224h],0`): `delete PCurrentOpenTradeMenu` on a
// class with a virtual destructor is the null test + `push 1 ; call [eax]`; the base's Components vector loses its
// last element with `add dword ptr [esi+10h],-4` (VC6 pop_back of a pointer vector: `--_Last`, Components at retail
// +0x0C -- PDB +0x0C, the first base vector); the reset follows inside the same `if`. The front-end manager is fetched
// twice (two calls). Member: CTCInventoryTrade::PCurrentOpenTradeMenu is PDB +0x274, retail +0x224: the retail
// constructor (0x0059EC22) clears the byte InventoryTradeOpen at +0x218 then PMouse_Pointer / PTop_Level_List /
// PCurrentOpenTradeMenu at +0x21C/+0x220/+0x224, i.e. engine/CTCInventoryTrade.h sits 0x50 above retail here (smaller
// base, vector, list and nine 12-byte maps), so the header is not included and a local model carries the PDB names.
// The local view is named CTCInventoryTrade_Retail so it does not shadow the PDB header engine/CTCInventoryTrade.h.
#pragma optimize("s", on)
namespace NUISystem {
class CComponent { public:
    virtual ~CComponent();
};
}
struct CFrontEndManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28();
    virtual void Slot29();                    // +0x74
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36();
    virtual void Slot37();                    // +0x94
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
struct CNodeList {
    void* m_pHead;
    void clear();                             // 0x004855A3
};
extern CNodeList g_frontEndNodeList;          // 0x013B8A98

template <class T> struct CRetailVector {
    T* _First;
    T* _Last;
    T* _End;
    void pop_back() { --_Last; }
};
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x0c - 4];
    CRetailVector<NUISystem::CComponent*> Components;   // +0x0C
    unsigned char _pad_0x18[0x130 - 0x18];
};
struct CTCInventoryTrade_Retail : CTCInventoryBase_Retail {
    unsigned char _pad_0x130[0x224 - 0x130];
    NUISystem::CComponent* PCurrentOpenTradeMenu;    // +0x224
    void DeleteComponents();
};
void CTCInventoryTrade_Retail::DeleteComponents()
{
    if (PCurrentOpenTradeMenu) {
        delete PCurrentOpenTradeMenu;
        Components.pop_back();
        PCurrentOpenTradeMenu = 0;
    }
    CFrontEndManager::GetInstance()->Slot37();
    CFrontEndManager::GetInstance()->Slot29();
    g_frontEndNodeList.clear();
}