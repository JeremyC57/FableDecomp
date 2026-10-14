// NUISystem::CMouseCursor::`scalar deleting destructor'(unsigned int flags) -- the three vtables of this class, the cursor removed from the front-end manager's observers (GetInstance @ 0x0041E5F2, slot 5 with the CObserver sub-object at +4), the manager's cursor pointer (+0xb8) cleared, CSprite::~CSprite (0x0041AB40), then free for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055C8D0, length 96 bytes.
// Leaf: ??_GCMouseCursor@NUISystem@@UAEPAXI@Z
// Shape: `this` in esi, the observer sub-object in edi, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct IPrimary_0055c8d0 { virtual void v00(); };
struct CObserver {                                 // the secondary base at +4 (0x14 bytes)
    virtual void a00();
    unsigned char _pad04[0x10];
    void ObserveEvent(long event);                 // 0x0052DA20
};
struct ISecondaryB_0055c8d0 { virtual void b00(); };
struct CSprite : IPrimary_0055c8d0, CObserver, ISecondaryB_0055c8d0 {   // 0x18 + ...
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
    CMouseCursor(void* def) : CSprite(def) { CFrontEndManager::GetInstance()->AddObserver(this); ObserveEvent(0x19); CFrontEndManager::GetInstance()->m_pMouseCursor = this; }   // 0x0055C650, inline here so the factory pulls the vtables in
    __forceinline virtual ~CMouseCursor()
    {
        CFrontEndManager::GetInstance()->RemoveObserver(this);
        CFrontEndManager::GetInstance()->m_pMouseCursor = 0;
    }
};
}

void* MakeOne_0055c8d0(void* def)                  // forces the vtables (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new NUISystem::CMouseCursor(def);
}
