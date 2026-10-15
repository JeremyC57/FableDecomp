// Leaf: ??1CMouseCursor@
// NUISystem::CMouseCursor::~CMouseCursor() -- the three vtables of this class (0x0124C22C at +0, 0x0124C208 at +4, 0x0124C200 at +0x18: the CSprite primary and its two secondary interface bases), the cursor unregistered from the front-end manager (GetInstance @ 0x0041E5F2, virtual slot 5 with the listener base at +4), the manager's cursor word (+0xb8) cleared, then the CSprite destructor (0x0041AB40, a sibling jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055C510, length 63 bytes.
// Callees: CFrontEndManager::GetInstance @ 0x0041E5F2 (`__fastcall`, no arguments; twice), the manager's virtual slot 5 (+0x14), CSprite::~CSprite @ 0x0041AB40. The secondary base at +4 is 0x14 bytes (vptr + 0x10), the one at +0x18 just a vptr: the landed three-vtable shape (0x0053F120). Vtables from rtti-vtables.tsv. Shape: `this` in esi, the +4 base in edi, `ret` through the jump.
struct ISpritePrimary_0055c510 {                   // the primary base (vptr at +0)
    virtual void v00();
};
struct ISpriteListener_0055c510 {                  // the secondary base at +4 (0x14 bytes)
    virtual void a00();
    unsigned char _pad04[0x10];
};
struct ISpriteSecondaryB_0055c510 {                // the secondary base at +0x18
    virtual void b00();
};
struct CSprite : ISpritePrimary_0055c510, ISpriteListener_0055c510, ISpriteSecondaryB_0055c510 {
    unsigned char _pad1c[0x18];                    // +0x1c .. +0x34
    ~CSprite();                                    // 0x0041AB40
};

namespace NUISystem { struct CMouseCursor; }
struct CFrontEndManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void RemoveListener(ISpriteListener_0055c510* listener);   // slot 5 (+0x14)
    unsigned char           _pad04[0xb4];          // +0x04 .. +0xb8
    NUISystem::CMouseCursor* m_pMouseCursor;       // +0xb8
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};

namespace NUISystem {
struct CMouseCursor : CSprite {                    // vtables 0x0124C22C / 0x0124C208 / 0x0124C200
    virtual void v00();
    virtual void a00();
    virtual void b00();
    virtual ~CMouseCursor();
};

CMouseCursor::~CMouseCursor()
{
    CFrontEndManager::GetInstance()->RemoveListener(this);
    CFrontEndManager::GetInstance()->m_pMouseCursor = 0;
}
}
