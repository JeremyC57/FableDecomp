// CTCInventoryTrade::IsSpecialCase(long itemType) -- an item is a special trade case when it is a hair card or a tattoo card, or when its definition carries a hero-title sub-def
// Leaf: ?IsSpecialCase@CTCInventoryTrade@@QAE_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059C983, length 274 bytes, `ret 4`. Frame-pointer size TU: optimize("y",off) +
// ("s",on). FableWin.pdb ?IsSpecialCase@CTCInventoryTrade@@QAE_NJ@Z.
// Callees: GetHairCardDef(long) @ 0x005D9A10 and GetTattooCardDef(long) @ 0x005D9AA0 (`__fastcall`, a counted def pointer by
// value through the hidden pointer; the catalog labels both GetHairCardDef), CTCBase::GetDefinitionManager() const @ 0x006869D0,
// CDefinitionManager::GetDef<CThingBaseDef>(long, CDefPointer<const CThingBaseDef>*) const @ 0x0042AF3C, CDefClassBase::GetSubDef<CHeroTitleDef>(CDefPointer<const CHeroTitleDef>*) const @
// 0x005B1E5A (its StatGetTypeName literal at 0x005B1E64 is "CHeroTitleDef"; was spelt GetSubDef<CAbilityDef>), the defs'
// virtual slot 1 on release.
// Shape: the landed ProcessTradeAction (0x005AEBFE) card-def temporaries (long-form release through the def's
// ReleaseTemporary; the tattoo pointer's dtor also clears its word); the item def and its hero-title sub-def are owning out
// slots (zeroed from the esi zero register, short-form release) filled by same-TU out-only stand-ins of GetDef and
// GetSubDef, so VC7.1 keeps the fetched defs in edi/ecx; one `&&` test, and every dtor forced inline so the true and false
// exits each get their own release chain.
#include "fable_compiler.h"
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
    FABLE_FORCEINLINE void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:              // a counted def reference returned by value
    T* m_p;
    FABLE_FORCEINLINE ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
};
template <class T> struct CClearingDefPointer {      // the same, clearing its word after the release
    T* m_p;
    FABLE_FORCEINLINE ~CClearingDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};
template <class T> class CDefPointer;
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x005B1E5A GetSubDef<CHeroTitleDef> (Ego_r / FableWin template)
};
template <class T> class CDefPointer<const T> { public:   // an owning out slot for GetDef / GetSubDef (CDefPointer<const T>)
    CDefClassBase* m_p;
    CDefPointer() : m_p(0) { }
    FABLE_FORCEINLINE ~CDefPointer()
    {
        if (m_p && --m_p->m_count == 0)
            m_p->Destroy();
    }
};
struct CTattooCardDef : CDefBase { };
class CHairCardDef : public CDefBase { public: };
class CHeroTitleDef : public CDefClassBase { public: };
class CThingBaseDef : public CDefClassBase { public: };
class CDefinitionManager { public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042AF3C GetDef<CThingBaseDef> (FableWin.pdb template)
};
class CGameDefinitionManager : public CDefinitionManager { public: };   // RTTI CGameDefinitionManager : CDefinitionManager
CDefClassBase* FindRef_CDefinitionManager_GetDef(const CDefinitionManager* manager, long index);   // stand-in callee
CDefClassBase* FindRef_CDefClassBase_GetSubDef(const CDefClassBase* defClass);                     // stand-in callee
#pragma auto_inline(off)
// Same-TU stand-ins for GetDef<CThingBaseDef> 0x0042AF3C and GetSubDef 0x005B1E5A: they only write the out slot, so VC7.1
// knows the slots do not escape (retail's link-time knowledge) and keeps the fetched defs in registers.
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    CDefClassBase* def = FindRef_CDefinitionManager_GetDef(this, index);
    out->m_p = def;
    return def != 0;
}
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    CDefClassBase* def = FindRef_CDefClassBase_GetSubDef(this);
    out->m_p = def;
    return def != 0;
}
#pragma auto_inline(on)
CDefPointer<CHairCardDef> __fastcall GetHairCardDef(long itemType);              // 0x005D9A10
CClearingDefPointer<CTattooCardDef> __fastcall GetTattooCardDef(long itemType);  // 0x005D9AA0

class CTCBase { public:                              // the component base (members this row reads)
    virtual ~CTCBase();
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (Ego_r.pdb / FableWin.pdb IBE)
};
class CTCInventoryTrade : public CTCBase { public:
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
    CDefPointer<const CThingBaseDef> itemDef;
    CDefPointer<const CHeroTitleDef> heroTitle;
    if (GetDefinitionManager().GetDef(itemType, &itemDef) && itemDef.m_p->GetSubDef(&heroTitle))
        return true;
    return false;
}