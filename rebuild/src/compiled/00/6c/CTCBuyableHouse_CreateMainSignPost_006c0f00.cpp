// CTCBuyableHouse::CreateMainSignPost() -- create the house's main sign post (the rent sign when owned and rented, the owned sign when owned, else the for-sale main sign) and keep it in MainSignPost
// Leaf: ?CreateMainSignPost@CTCBuyableHouse@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C0F00, length 109 bytes, thiscall, plain `ret`.
// Identity: FableWin.pdb ?CreateMainSignPost@CTCBuyableHouse@@AAEXXZ (private); the landed twin CreateDressingSignPost 0x006C0F70.
// Callees: CTCBuyableHouse::CreateSignPost(const CCharString&, bool) 0x006C0970 (PDB ?CreateSignPost@CTCBuyableHouse@@AAEPAVCThing@@ABVCCharString@@_N@Z),
//   CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, SetPItem 0x00A01B90 (temporary, then member), GetPItem 0x00A01B50,
//   ~CBaseIntelligentPointer 0x00A01C10; vtable 0x01238C6C = CIntelligentPointer<CThing>.
// Globals (descriptive; file-static CCharStrings built by the dynamic initialisers at 0x0121E0C0..0x0121E120, no PDB name):
//   0x013BAAF8 "OBJECT_BUYABLE_HOUSE_MAIN_SIGNPOST", 0x013BAB00 "OBJECT_BUYABLE_HOUSE_OWNED_SIGNPOST",
//   0x013BAB04 "OBJECT_BUYABLE_HOUSE_RENT_SIGNPOST" (0x013BAAFC "..._DRESSING_SIGNPOST" is the twin's g_DressingSignPostDefName).
// Shape: one whole `MainSignPost = CIntelligentPointer<CThing>(CreateSignPost(name, true));` statement per arm. VC7.1 hoists the
//   common `push 1`, tail-merges the three calls behind their `push offset` arms and keeps the merged block's this-before-argument
//   order (`lea ecx,[esp+8]` before `push edi`, `lea ecx,[esi+24h]` before `push eax`). One assignment of a nested conditional
//   (the parked 2026-10-15 form, and the twin's single-statement form) pushes before forming ecx.
// Layout: `this` from engine/CTCBuyableHouse.h (`#define` include pattern); MainSignPost is the header's opaque 8-byte
//   CIntelligentPointer<CThing> at +0x24, viewed through its real type with a reinterpret_cast in each arm (a reference local
//   to it makes VC7.1 keep &MainSignPost in esi instead of `this`: 10 lines off).
#define CTCBuyableHouse CTCBuyableHouseLayout
#include "engine/CTCBuyableHouse.h"
#undef CTCBuyableHouse

class CThing;
class CBaseObject;
class CBaseObjectPointer;
class CCharString { public: void* PStringData; };

class CBaseIntelligentPointer {                          // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // <CThing>: vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

extern const CCharString g_MainSignPostDefName;          // 0x013BAAF8 "OBJECT_BUYABLE_HOUSE_MAIN_SIGNPOST"
extern const CCharString g_OwnedSignPostDefName;         // 0x013BAB00 "OBJECT_BUYABLE_HOUSE_OWNED_SIGNPOST"
extern const CCharString g_RentSignPostDefName;          // 0x013BAB04 "OBJECT_BUYABLE_HOUSE_RENT_SIGNPOST"

class CTCBuyableHouse : public CTCBuyableHouseLayout {
private:
    CThing* CreateSignPost(const CCharString& defName, bool mainSign);   // 0x006C0970
    void CreateMainSignPost();
};

void CTCBuyableHouse::CreateMainSignPost()
{
    if (OwnedByPlayer) {
        if (Rented)
            reinterpret_cast<CIntelligentPointer<CThing>&>(MainSignPost) = CIntelligentPointer<CThing>(CreateSignPost(g_RentSignPostDefName, true));
        else
            reinterpret_cast<CIntelligentPointer<CThing>&>(MainSignPost) = CIntelligentPointer<CThing>(CreateSignPost(g_OwnedSignPostDefName, true));
    }
    else
        reinterpret_cast<CIntelligentPointer<CThing>&>(MainSignPost) = CIntelligentPointer<CThing>(CreateSignPost(g_MainSignPostDefName, true));
}