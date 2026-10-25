// NInventory::CTCInventoryBase::AddCloseInventoryEvent() -- queues a "close inventory" game event (type 0x1B) for the local player, carrying one zero byte
// Leaf: ?AddCloseInventoryEvent@CTCInventoryBase@NInventory@@UAEXXZ
// Flags: /GS
// Name: the class is NInventory::CTCInventoryBase (RTTI vtable 0x01253DBC .?AVCTCInventoryBase@NInventory@@, slot 32);
// Ego_r.exe ?AddCloseInventoryEvent@CTCInventoryBase@NInventory@@UAEXXZ 0x0061F3AF.
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC527, length 95 bytes.
// Callees: the player-manager getter @ 0x00686D50 (`mov eax,[0x013B8A1C] ; mov eax,[eax+30h]`; manifest label
//          "CViewBase::GetDisplayEngine" is propagated) and its player-number accessor @ 0x00449700 (`mov eax,[ecx+1Ch]`),
//          CMemoryBuffer::Copy(unsigned long, const void*) @ 0x009F1760 (landed), CMainGameComponent::AddGameEvent
//          (const CGameEvent&) @ 0x0041649C on the global component @ 0x013B86A0, __security_check_cookie @ 0x00BFE9F9.
// GS note: /GS -- the event's 32-byte data array gets the cookie frame (cookie @ 0x0139C8A8 stored at [ebp-4]).
// Pragmas: frame-pointer size TU (`optimize("y", off)` + `("s", on)`); without them the body is esp-based (105v95).
// Layout: CGameEvent from Ego_r.pdb (40 bytes): Type +0, Player +4 (signed char), Data[32] +5, EndPos +0x25,
//         Valid +0x26, Replacement +0x27; it derives from CMemoryBuffer at offset 0 (landed 0x004159E0).
// Shape: the inline constructor stores Type, Player = -1, EndPos 0, Valid 1, Replacement 0; the player number
//        overwrites Player; AddUBYTE(0) is inlined (its by-value byte homed in a frame slot, `mov byte ptr
//        [ebp-30h],0` right before the Copy call).

struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};

struct CGameEvent : CMemoryBuffer {
    long          Type;                           // +0x00
    signed char   Player;                         // +0x04
    unsigned char Data[32];                       // +0x05
    unsigned char EndPos;                         // +0x25
    bool          Valid;                          // +0x26
    bool          Replacement;                    // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddUBYTE(unsigned char value) { Copy(1, &value); }
};

struct CMainGameComponent {
    void AddGameEvent(const CGameEvent& event);   // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;  // 0x013B86A0

struct CPlayerManager {
    long GetMainPlayerNumber();                   // 0x00449700
};
CPlayerManager* GetPlayerManager_00686d50();               // 0x00686D50

enum { GAME_EVENT_CLOSE_INVENTORY = 0x1B };

namespace NInventory {

class CTCInventoryBase {
public:
    virtual ~CTCInventoryBase();                                  // slot 0
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual void _v04();
    virtual void _v05();
    virtual void _v06();
    virtual void _v07();
    virtual void _v08();
    virtual void _v09();
    virtual void _v10();
    virtual void _v11();
    virtual void _v12();
    virtual void _v13();
    virtual void _v14();
    virtual void _v15();
    virtual void _v16();
    virtual void _v17();
    virtual void _v18();
    virtual void _v19();
    virtual void _v20();
    virtual void _v21();
    virtual void _v22();
    virtual void _v23();
    virtual void _v24();
    virtual void _v25();
    virtual void _v26();
    virtual void _v27();
    virtual void _v28();
    virtual void _v29();
    virtual void _v30();
    virtual void _v31();
    virtual void AddCloseInventoryEvent();                            // slot 32 (+0x80)
};

} // namespace NInventory

void NInventory::CTCInventoryBase::AddCloseInventoryEvent()
{
    CGameEvent event(GAME_EVENT_CLOSE_INVENTORY);
    event.Player = (signed char)GetPlayerManager_00686d50()->GetMainPlayerNumber();
    event.AddUBYTE(0);
    g_pMainGameComponent->AddGameEvent(event);
}