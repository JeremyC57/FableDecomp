// CTCInventoryTrade::IsSpecialCase(long itemType) -- an item is a special trade case when it is a hair card or a tattoo card, or when its definition carries an ability sub-def
// Grade: DIFFER(240v274) -- destructor layout: retail expands the card-def releases twice (one chain for the true exits, one for the false exit) with esi as the zero register and this parked in edi; VC7.1 here shares one release chain and uses ebx. First draft, one grade.
// Leaf: IsSpecialCase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059C983, length 274 bytes, `ret 4`. Frame-pointer size TU: optimize("y",off) +
// ("s",on). FableWin.pdb ?IsSpecialCase@CTCInventoryTrade@@QAE_NJ@Z.
// Callees: GetHairCardDef(long) @ 0x005D9A10 and GetTattooCardDef(long) @ 0x005D9AA0 (`__fastcall`, a counted def pointer by
// value through the hidden pointer; the catalog labels both GetHairCardDef), CTCBase::GetDefinitions() @ 0x006869D0,
// CDefinitionManager::GetDef(long, CDefPointer*) @ 0x0042AF3C, CDefClassBase::GetSubDef<CAbilityDef>(CDefPointer*) @
// 0x005B1E5A, the defs' virtual slot 1 on release.
// Shape: the landed ProcessTradeAction (0x005AEBFE) card-def temporaries (long-form release through the def's
// ReleaseTemporary; the tattoo pointer's dtor also clears its word); the item def and its ability sub-def are plain out
// slots zeroed from the esi zero register, copied into pointers and released by hand (short form) on every exit.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CRefCounted {                                 // the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                          // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {              // a counted def reference returned by value
    T* m_p;
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
};
template <class T> struct CClearingDefPointer {      // the same, clearing its word after the release
    T* m_p;
    ~CClearingDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};
template <class T> struct CDefSlot {                 // a plain out slot for GetDef / GetSubDef
    T* m_p;
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && --def->m_count == 0)
        def->Destroy();
}
struct CTattooCardDef : CDefBase { };
struct CHairCardDef : CDefBase { };
struct CAbilityDef : CDefBase { };
struct CDefClassBase : CDefBase {
    bool GetSubDef(CDefSlot<CAbilityDef>* out);      // 0x005B1E5A
};
struct CDefinitionManager {
    bool GetDef(long index, CDefSlot<CDefClassBase>* out);   // 0x0042AF3C
};
CDefPointer<CHairCardDef> __fastcall GetHairCardDef(long itemType);              // 0x005D9A10
CClearingDefPointer<CTattooCardDef> __fastcall GetTattooCardDef(long itemType);  // 0x005D9AA0

struct CTCInventoryTrade {
    CDefinitionManager* GetDefinitions() const;      // 0x006869D0 (CTCBase)
    bool IsSpecialCase(long itemType);
};

bool CTCInventoryTrade::IsSpecialCase(long itemType)
{
    CDefPointer<CHairCardDef> hair = GetHairCardDef(itemType);
    if (hair.m_p)
        return true;
    CClearingDefPointer<CTattooCardDef> tattoo = GetTattooCardDef(itemType);
    if (tattoo.m_p)
        return true;
    CDefSlot<CDefClassBase> itemSlot;
    CDefSlot<CAbilityDef> abilitySlot;
    itemSlot.m_p = 0;
    abilitySlot.m_p = 0;
    bool found = GetDefinitions()->GetDef(itemType, &itemSlot);
    CDefClassBase* itemDef = itemSlot.m_p;
    if (found) {
        bool hasAbility = itemDef->GetSubDef(&abilitySlot);
        CAbilityDef* ability = abilitySlot.m_p;
        if (hasAbility) {
            ReleaseDef(ability);
            ReleaseDef(itemDef);
            return true;
        }
        ReleaseDef(ability);
    }
    ReleaseDef(itemDef);
    return false;
}
