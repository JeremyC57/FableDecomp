// CCreatureAction_UnsheatheToShowToSheatheTrophy::CCreatureAction_UnsheatheToShowToSheatheTrophy(CThingCreatureBase*, CThing*, const CCharString&) -- base CCreatureAction_UnsheatheItemFromInventory(owner, trophy, no follow-up, 1000), own vtable (0x0125F8D4), the animation name copied at +0xb8, a CIntelligentPointer at +0xbc default-built then set to the trophy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DF800, length 84 bytes.
// Callees: CCreatureAction_UnsheatheItemFromInventory::CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase*, CThing*, CCreatureActionBase*, long) @ 0x006A0150
// (manifest prototype), CCharString::CCharString(const CCharString&) @ 0x0099EC30, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10
// (then the CIntelligentPointer vtable 0x01238C6C), CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: the trophy rides in ebx (used for the base call and the SetPItem), the string argument is read from its slot after the base
// call, the pointer member is addressed once in edi; SetPItem is a body statement after the init list. CCharString is modelled
// locally because fable_string.h inlines the copy constructor retail calls out of line. `ret 0Ch`: three arguments.
struct CThingCreatureBase;
struct CThing;
struct CCreatureActionBase;
struct CBaseObject;

struct CCharString {                        // one pointer; the copy constructor runs out of line
    void* m_pStringData;
    CCharString(const CCharString& other);   // 0x0099EC30
};
struct CBaseIntelligentPointer {            // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                   // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CCreatureAction_UnsheatheItemFromInventory {   // retail sizeof 0xb8
    virtual void v00();
    unsigned char _body[0xb8 - 4];
    CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase* owner, CThing* item, CCreatureActionBase* followUp, long priority);   // 0x006A0150
};

struct CCreatureAction_UnsheatheToShowToSheatheTrophy : CCreatureAction_UnsheatheItemFromInventory {   // vtable 0x0125F8D4
    CCharString m_animationName;            // +0xb8
    CIntelligentPointer PTrophy;            // +0xbc
    virtual void v00();
    CCreatureAction_UnsheatheToShowToSheatheTrophy(CThingCreatureBase* owner, CThing* trophy, const CCharString& animationName);
};

CCreatureAction_UnsheatheToShowToSheatheTrophy::CCreatureAction_UnsheatheToShowToSheatheTrophy(CThingCreatureBase* owner, CThing* trophy, const CCharString& animationName)
    : CCreatureAction_UnsheatheItemFromInventory(owner, trophy, 0, 1000)
    , m_animationName(animationName)
    , PTrophy()
{
    PTrophy.SetPItem((CBaseObject*)trophy);
}