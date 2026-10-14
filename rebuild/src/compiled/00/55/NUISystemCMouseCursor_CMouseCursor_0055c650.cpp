// NUISystem::CMouseCursor::CMouseCursor(void* def) -- the CSprite base (0x0041B800 with the def), the three vtables of this class (0x0124C22C at +0, 0x0124C208 at +4, 0x0124C200 at +0x18), then the cursor registers itself as an observer with the front-end manager (GetInstance @ 0x0041E5F2, its slot 2 with the CObserver sub-object at +4), observes event 0x19 (CObserver::ObserveEvent @ 0x0052DA20) and stores itself as the manager's cursor (+0xb8)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055C650, length 76 bytes.
// Vtables from rtti-vtables.tsv. Shape: `this` in esi, the observer sub-object in edi, `ret 4`.
struct IPrimary_0055c650 { virtual void v00(); };
struct CObserver {                                 // the secondary base at +4 (0x14 bytes)
    virtual void a00();
    unsigned char _pad04[0x10];
    void ObserveEvent(long event);                 // 0x0052DA20
};
struct ISecondaryB_0055c650 { virtual void b00(); };
struct CSprite : IPrimary_0055c650, CObserver, ISecondaryB_0055c650 {   // 0x18 + ...
    CSprite(void* def);                            // 0x0041B800
    ~CSprite();                                    // 0x0041AB40
};
namespace NUISystem { struct CMouseCursor; }
struct CFrontEndManager {
    virtual void v000(); virtual void v001();
    virtual void AddObserver(CObserver* observer);     // slot 2 (+0x08)
    virtual void v003(); virtual void v004();
    virtual void RemoveObserver(CObserver* observer);  // slot 5 (+0x14)
    unsigned char _pad04[0xb4];
    NUISystem::CMouseCursor* m_pMouseCursor;       // +0xb8
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};

namespace NUISystem {
struct CMouseCursor : CSprite {                    // vtables 0x0124C22C / 0x0124C208 / 0x0124C200
    virtual void v00();
    virtual void a00();
    virtual void b00();
    CMouseCursor(void* def);
};
}

NUISystem::CMouseCursor::CMouseCursor(void* def)
    : CSprite(def)
{
    CFrontEndManager::GetInstance()->AddObserver(this);
    ObserveEvent(0x19);
    CFrontEndManager::GetInstance()->m_pMouseCursor = this;
}