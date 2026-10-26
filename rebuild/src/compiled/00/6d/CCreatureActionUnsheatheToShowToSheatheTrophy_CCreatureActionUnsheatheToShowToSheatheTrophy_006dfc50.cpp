// CCreatureAction_UnsheatheToShowToSheatheTrophy::CCreatureAction_UnsheatheToShowToSheatheTrophy(const CCreatureAction_UnsheatheToShowToSheatheTrophy&) -- copy constructor: base CCreatureAction_UnsheatheItemFromInventory(other), own vtable (0x0125F8D4), the animation name at +0xb8 copied, the CIntelligentPointer at +0xbc copied through GetPItem/SetPItem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DFC50, length 85 bytes.
// Callees: CCreatureAction_UnsheatheItemFromInventory::CCreatureAction_UnsheatheItemFromInventory(const CCreatureAction_UnsheatheItemFromInventory&) @ 0x006A0E80,
// CCharString::CCharString(const CCharString&) @ 0x0099EC30, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (then the
// CIntelligentPointer vtable 0x01238C6C), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: the landed 30-body copy-constructor family (0x008C9790): the source rides in ebx, the pointer member's user copy
// constructor (`CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }`) is inlined after the string copy. `ret 4`.
class CBaseObject;

class CCharString { public:                        // one pointer; the copy constructor runs out of line
    void* m_pStringData;
    CCharString(const CCharString& other);   // 0x0099EC30
};
struct CBaseIntelligentPointer {            // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                   // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    CBaseObject* GetPItem() const;          // 0x00A01B50
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

class CCreatureAction_UnsheatheItemFromInventory { public:   // retail sizeof 0xb8
    virtual void v00();
    unsigned char _body[0xb8 - 4];
    CCreatureAction_UnsheatheItemFromInventory(const CCreatureAction_UnsheatheItemFromInventory& other);   // 0x006A0E80
};

class CCreatureAction_UnsheatheToShowToSheatheTrophy : public CCreatureAction_UnsheatheItemFromInventory { public:   // vtable 0x0125F8D4
    CCharString m_animationName;            // +0xb8
    CIntelligentPointer PTrophy;            // +0xbc
    virtual void v00();
    CCreatureAction_UnsheatheToShowToSheatheTrophy(const CCreatureAction_UnsheatheToShowToSheatheTrophy& other);
};

CCreatureAction_UnsheatheToShowToSheatheTrophy::CCreatureAction_UnsheatheToShowToSheatheTrophy(const CCreatureAction_UnsheatheToShowToSheatheTrophy& other)
    : CCreatureAction_UnsheatheItemFromInventory(other)
    , m_animationName(other.m_animationName)
    , PTrophy(other.PTrophy)
{
}