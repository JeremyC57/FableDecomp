// NUISystem::CScrollableList::OnActivate() -- refresh the visible list, reset the scroll timer, register the list's three input events and hand the list and both scroll arrows to the front-end manager
// Leaf: OnActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00556200, length 160 bytes (147 + int3 padding).
// Vtable: 0x0124A72C (NUISystem::CScrollableList) slot 140.
// Callees: CScrollableList::UpdateVisibleList(float, float) @ 0x00555A90,
// NUISystem::CFrontEndManager::GetInstance @ 0x0041E5F2 (`__fastcall`, three times), the input-client
// base's virtual slot 3 (+0xc) three times (events 0x25, 0x24, 4), the manager's slots 2 (+0x8) and 5 (+0x14).
// Shape: the input client is a secondary base at +4, so its virtuals run with `this` re-based (`lea esi,
// [edi+4]`) and the arrow components convert to it with VC's null test (`lea esi,[eax+4]` / `xor esi,esi`);
// the down-arrow call is tail-duplicated by the compiler.
// Layout: PDB NUISystem::CScrollableList has ScrollTimer +0x19c, PUpArrow +0x1a0, PDownArrow +0x1a4;
// retail sits 0x30 lower (+0x16c / +0x170 / +0x174), so the class is modelled locally with those offsets.
namespace NUISystem {

struct CInputClient {                                // secondary base at +4
    virtual void ic00(); virtual void ic01(); virtual void ic02();
    virtual void AddInputEvent(long eventType);      // slot 3 (+0xc)
};

struct CComponentBase {
    virtual void v00();
};

class CComponent : public CComponentBase, public CInputClient { public:
    unsigned char _pad08[0x164];                     // +0x08
};

class CFrontEndManager { public:
    virtual void m00(); virtual void m01();
    virtual void AddInputClient(CInputClient* client);       // slot 2 (+0x8)
    virtual void m03(); virtual void m04();
    virtual void SetNavigationClient(CInputClient* client);  // slot 5 (+0x14)
    static CFrontEndManager* __fastcall GetInstance();       // 0x0041E5F2
};

class CScrollableList : public CComponent { public:
    float       ScrollTimer;                         // +0x16c
    CComponent* PUpArrow;                            // +0x170
    CComponent* PDownArrow;                          // +0x174
    void UpdateVisibleList(float a, float b);        // 0x00555A90
    void OnActivate();
};

void CScrollableList::OnActivate()
{
    UpdateVisibleList(0.0f, 0.2f);
    ScrollTimer = 0.2f;
    AddInputEvent(0x25);
    AddInputEvent(0x24);
    AddInputEvent(4);
    CFrontEndManager::GetInstance()->AddInputClient(this);
    CFrontEndManager::GetInstance()->SetNavigationClient(PUpArrow);
    CFrontEndManager::GetInstance()->SetNavigationClient(PDownArrow);
}

}