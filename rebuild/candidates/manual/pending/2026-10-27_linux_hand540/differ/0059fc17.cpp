// CTCInventoryTrade::AlreadyHaveSpecial(long defIndex) -- true when the hero already has what the special shop item gives: the hair style, a tattoo it would duplicate or conflict with (or a remove-all tattoo when the hero has none), or the hero title
// Grade: DIFFER(715v716) -- residue: the tattoo loop's register allocation only: retail keeps the tattoo def in edi, re-reads def->BankIndex each pass (mov edx,[edi+34h]) and keeps the redundant second bank compare (cmp edx,eax), with the count stored then shifted in memory (sar [ebp-0Ch],2); ours hoists &def->BankIndex into edi, folds the second compare and spills the def to [ebp-10h]; lane gap: register allocation / loop-invariant hoisting | tried: full PDB-named rewrite (GetDef<CThingBaseDef> / GetSubDef<CHeroTitleDef> same-TU bodies, CDefPointer<const T> results, CTCHero title check), compare order swaps, size() per pass (36), signed count, cached bank locals (11, +6 bytes), def by reference, operator-> instead of a def local (23), element by const ref | next: none known
// Leaf: ?AlreadyHaveSpecial@CTCInventoryTrade@@AAE_NJ@Z (Ego_r.pdb / FableWin.pdb: private)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059FC17, 716 bytes, thiscall `ret 4`, ebp frame + size TU (pragmas below).
// Callees (PDB spellings): CTCInventoryItem::GetHairCardDef(long) 0x005D9A10 / GetTattooCardDef(long) 0x005D9AA0 (static
//   __fastcall, CDefPointer<const T> by value); CTCHeroAttachableAppearanceModifiers::IsWearingAppearanceModifier 0x00706000
//   (interface 0x5E); CTCHeroMorph::GetTattoos 0x0071DD50 / HasAPermanentTattooCoveringArea 0x0071C2D0 (interface 3);
//   vector<CDefPointer<const CTattooDef> >::~vector 0x00581353; CBasicString<char>::operator== 0x004A93C0;
//   CTattooDef::IsConflicting(const CTattooDef&) const 0x0059B679; CTCBase::GetDefinitionManager() const 0x006869D0;
//   CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C; CDefClassBase::GetSubDef<CHeroTitleDef> 0x005B1E5A; CTCHero
//   (interface 0x29) HeroTitleObjectDef +0x14. Layout: Ego_r CTattooDef BankIndex +0x34, RemovesAllTattoos +0x3C,
//   Permanent +0x3D, CustomFileName +0x4C; CHairCardDef HairObject +0x28.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define CTCBase CTCBaseLayout
#include "engine/CTCBase.h"
#undef CTCBase
template <class T> class CBasicString {
public:
    bool operator==(const CBasicString& other) const;  // 0x004A93C0
};
class CCharStringData {
public:
    CBasicString<char> Data;
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    __forceinline bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData)
            return PStringData->Data == other.PStringData->Data;
        return false;
    }
};
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();                  // slot 0
    virtual void Delete() const;                       // slot 1
    mutable unsigned long IVRefCount;                  // +0x04
};
class CResource : public CIVCountedPointeeBase {       // retail slot order (vtable 0x01230BCC, FableWin.pdb names)
public:
    virtual bool UnloadResource();                     // slot 2
    virtual unsigned long GetUnloadableMemory() const; // slot 3
    virtual bool IsResourceLoaded() const;             // slot 4
    virtual void OnResourceLoaded();                   // slot 5
    virtual bool CanAbortResourceLoad(long frames) const;   // slot 6
    virtual void OnResourceLoadAborted();              // slot 7
    virtual unsigned long GetDebugMemoryInUse() const; // slot 8
    virtual void ResetDebugMemoryInUse();              // slot 9
    unsigned char _pad08[0x1c - 0x08];
};
class CDefPointeeBase : public CResource { };
class CTattooDef;
class CHairCardDef;
class CDefClassBase;
template <class T> class CDefPointer;
template <> class CDefPointer<const CDefClassBase> {   // the manager's result: released through the two-step test, then nulled
public:
    CDefPointeeBase* Object;
    CDefPointer() { }
    __forceinline ~CDefPointer()
    {
        if (Object) {
            CDefPointeeBase* p = Object;
            --p->IVRefCount;
            if (p->IVRefCount == 0)
                p->Delete();
            Object = 0;
        }
    }
};
template <> class CDefPointer<CDefClassBase> {         // the protected overload's result (as the landed GetSubDef models)
public:
    CDefPointeeBase* Object;
    CDefPointer() { }
    __forceinline ~CDefPointer()
    {
        if (Object) {
            CDefPointeeBase* p = Object;
            --p->IVRefCount;
            if (p->IVRefCount == 0)
                p->Delete();
            Object = 0;
        }
    }
};
template <class T> class CDefPointer {                 // out-parameter locals: one-step release
public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer<CDefClassBase>& other) : Object((T*)other.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->IVRefCount == 0)
                    Object->Delete();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
    __forceinline ~CDefPointer() { if (Object) { if (--Object->IVRefCount == 0) Object->Delete(); } }
    T* operator->() const { return Object; }
};
template <> class CDefPointer<const CHairCardDef> {    // by-value results: two-step release, then nulled
public:
    const CHairCardDef* Object;
    __forceinline ~CDefPointer();
    const CHairCardDef* operator->() const { return Object; }
};
template <> class CDefPointer<const CTattooDef> {
public:
    const CTattooDef* Object;
    __forceinline ~CDefPointer();
    const CTattooDef* operator->() const { return Object; }
};
class CDefinitionManager {
public:
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index) const;               // 0x009AD9E0
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);                           // 0x009ADA10
    friend class CDefClassBase;
};
class CGameDefinitionManager : public CDefinitionManager { };
class CDefString {
public:
    long Index;
};
class CDefClassBase : public CDefPointeeBase {
public:
    class CSubDefInfo {
    public:
        long DefIndex;                                 // +0x00
        long OriginalParentDefIndex;                   // +0x04
    };
    virtual CDefString GetInstantiationName() const;   // slot 10
    virtual void SetInstantiationName(CDefString name);    // slot 11
    virtual void AddSubDef(const CCharString& name, long defIndex, long parentIndex);   // slot 12
    virtual void RemoveSubDef(const CCharString& name);    // slot 13
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString& name);               // slot 15
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString& name) const;   // slot 14 (+0x38)
    CDefinitionManager* PDefManager;                   // +0x1C
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;
};
class CThingBaseDef : public CDefClassBase { };
class CHeroTitleDef : public CDefClassBase { };
// Same-TU bodies of the retail callees (PDB template names; the landed 0x0042AF3C / 0x005B1E5A shapes).
#pragma auto_inline(off)
template <>
__declspec(noinline) bool CDefinitionManager::GetDef<CThingBaseDef>(long index, CDefPointer<const CThingBaseDef>* out) const   // 0x0042AF3C
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
    if (def.Object) {
        CDefPointer<const CThingBaseDef>& o = *out;
        if (o.Object != def.Object) {
            if (o.Object) {
                if (--o.Object->IVRefCount == 0)
                    o.Object->Delete();
                o.Object = 0;
            }
            o.Object = (const CThingBaseDef*)def.Object;
            if (o.Object)
                ++o.Object->IVRefCount;
        }
        return true;
    }
    return false;
}
template <>
__declspec(noinline) bool CDefClassBase::GetSubDef<CHeroTitleDef>(CDefPointer<const CHeroTitleDef>* out) const   // 0x005B1E5A
{
    const CSubDefInfo* info;
    {
        CCharString name("CHeroTitleDef", -1);
        info = GetPSubDefInfo(name);
    }
    if (info) {
        CDefPointer<const CHeroTitleDef> result(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
class CHairCardDef : public CIVCountedPointeeBase {
public:
    unsigned char _pad08[0x28 - 0x08];
    long HairObject;                                   // +0x28
};
class CTattooDef : public CIVCountedPointeeBase {
public:
    unsigned char _pad08[0x34 - 0x08];
    long BankIndex;                                    // +0x34
    unsigned char _pad38[0x3c - 0x38];
    bool RemovesAllTattoos;                            // +0x3C
    bool Permanent;                                    // +0x3D
    unsigned char _pad3e[0x4c - 0x3e];
    CCharString CustomFileName;                        // +0x4C
    bool IsConflicting(const CTattooDef& other) const; // 0x0059B679
};
__forceinline CDefPointer<const CHairCardDef>::~CDefPointer()
{
    if (Object) {
        const CHairCardDef* p = Object;
        --p->IVRefCount;
        if (p->IVRefCount == 0)
            p->Delete();
        Object = 0;
    }
}
__forceinline CDefPointer<const CTattooDef>::~CDefPointer()
{
    if (Object) {
        const CTattooDef* p = Object;
        --p->IVRefCount;
        if (p->IVRefCount == 0)
            p->Delete();
        Object = 0;
    }
}
class CTCInventoryItem {
public:
    static CDefPointer<const CHairCardDef> __fastcall GetHairCardDef(long defIndex);   // 0x005D9A10
    static CDefPointer<const CTattooDef> __fastcall GetTattooCardDef(long defIndex);   // 0x005D9AA0
};
namespace std {
template <class T> class allocator { };
template <class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector();                                         // 0x00581353 (CDefPointer<const CTattooDef>)
    unsigned int size() const { return _M_finish - _M_start; }
    bool empty() const { return _M_start == _M_finish; }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
class CTCHeroMorph {
public:
    void GetTattoos(std::vector<CDefPointer<const CTattooDef> >& tattoos) const;   // 0x0071DD50
    long HasAPermanentTattooCoveringArea() const;      // 0x0071C2D0
};
enum EHeroAttachableAppearanceModifierType { HERO_ATTACHABLE_APPEARANCE_MODIFIER_HAIR };   // value 0 (descriptive enumerator)
class CTCHeroAttachableAppearanceModifiers {
public:
    bool IsWearingAppearanceModifier(long defIndex, EHeroAttachableAppearanceModifierType type) const;   // 0x00706000
};
class CTCHero {
public:
    unsigned char _pad00[0x14];
    long HeroTitleObjectDef;                           // +0x14
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CPresenceBits { bool test(long kind) const; };   // 0x00410DE0
class CThing {
public:
    unsigned char  _pad00[0x20];
    CPresenceBits  m_present;                           // +0x20
    unsigned char  _pad21[0x44 - 0x21];
    CInterfaceMap  m_interfaces;                        // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long kind)
    {
        if (m_present.test(kind)) {
            long key = kind;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)((it == map->last || kind < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CTCBase : public CTCBaseLayout {
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};
class CTCInventoryTrade : public CTCBase {
private:
    bool AlreadyHaveSpecial(long defIndex);
};
bool CTCInventoryTrade::AlreadyHaveSpecial(long defIndex)
{
    CDefPointer<const CHairCardDef> hair = CTCInventoryItem::GetHairCardDef(defIndex);
    if (hair.Object) {
        CTCHeroAttachableAppearanceModifiers* modifiers;
        if (ParentThing->GetInterface(&modifiers, 0x5e)) {
            if (modifiers->IsWearingAppearanceModifier(hair->HairObject, HERO_ATTACHABLE_APPEARANCE_MODIFIER_HAIR))
                return true;
        }
    }
    CDefPointer<const CTattooDef> tattoo = CTCInventoryItem::GetTattooCardDef(defIndex);
    if (tattoo.Object) {
        CTCHeroMorph* morph;
        if (ParentThing->GetInterface(&morph, 3))
            ;
        if (morph) {
            std::vector<CDefPointer<const CTattooDef> > tattoos;
            morph->GetTattoos(tattoos);
            if (tattoos.empty()) {
                if (tattoo->RemovesAllTattoos)
                    return true;
                return false;
            }
            const CTattooDef* def = tattoo.Object;
            if (def->RemovesAllTattoos) {
                if (tattoos.size() == 1 && morph->HasAPermanentTattooCoveringArea())
                    return true;
                return false;
            }
            unsigned int count = tattoos.size();
            for (unsigned int i = 0; i < count; ++i) {
                const CTattooDef* t = tattoos[i].Object;
                if (t->BankIndex == def->BankIndex) {
                    if (t->BankIndex != -1 || t->BankIndex != def->BankIndex)
                        return true;
                    if (t->CustomFileName == def->CustomFileName)
                        return true;
                } else if (t->Permanent && t->IsConflicting(*def))
                    return true;
            }
        }
    }
    CDefPointer<const CThingBaseDef> def;
    CDefPointer<const CHeroTitleDef> title;
    if (GetDefinitionManager().GetDef(defIndex, &def)) {
        if (def->GetSubDef(&title)) {
            CTCHero* hero;
            ParentThing->GetInterface(&hero, 0x29);
            if (hero && hero->HeroTitleObjectDef == defIndex)
                return true;
        }
    }
    return false;
}
