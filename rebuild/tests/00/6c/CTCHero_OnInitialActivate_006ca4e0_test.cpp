#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __int64 long long
#endif
#include <cstdio>
#include <cstring>
class CThing;
class CBaseObject;

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem((CBaseObject*)other.GetPItem());
        return *this;
    }
};
#pragma pack(pop)

template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long RefCount;                          // +0x04
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x00580034 for CHeroDef
};
class CHeroDef : public CDefClassBase {
public:
    unsigned char _pad08[0x28 - 0x08];
    long DefaultTitle;                              // +0x28
};

class CThingManager {
public:
    CThing* GetThingWithUID(__int64 uid);           // 0x0051ED80
};

class CThing {
public:
    unsigned char _pad00[0x70];
    CDefClassBase* PBaseDef;                        // +0x70 CDefPointer<const CThingBaseDef>::Object
};

class CTCHero {                                     // the vptr as a plain word (an __int64 member pads a real one to 8)
public:
    void* __vfptr;                                  // +0x00
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[4];
    bool WeaponsUsable;                             // +0x0C
    bool WillUsable;                                // +0x0D
    bool WeaponUseOverriddenByScripts;              // +0x0E
    bool WillUseOverriddenByScripts;                // +0x0F
    long Title;                                     // +0x10 EHeroTitle
    long HeroTitleObjectDef;                        // +0x14
    void* PHeroTitleSubDef;                         // +0x18 CDefPointer<const CHeroTitleDef>
    bool OnBoastingPodium;                          // +0x1C
    CIntelligentPointer<CThing> LastWeaponEquipped; // +0x1D
    unsigned char _pad25[3];
    __int64 LastWeaponEquippedID;                   // +0x28
    CThingManager* GetThingManager();               // 0x00686AE0 (CTCBase)
    void SetTitle(long titleObjectDef);             // 0x006CA400
    void OnInitialActivate();
};

void CTCHero::OnInitialActivate()
{
    if (LastWeaponEquippedID != 0)
        LastWeaponEquipped = CIntelligentPointer<CThing>(GetThingManager()->GetThingWithUID(LastWeaponEquippedID));
    if (HeroTitleObjectDef) {
        SetTitle(HeroTitleObjectDef);
        return;
    }
    {
        CDefClassBase* baseDef = ParentThing->PBaseDef;
        CDefPointer<const CHeroDef> out;
        if (baseDef) {
            bool found = baseDef->GetSubDef(&out);
            const CHeroDef* heroDef = out.Object;
            if (found && heroDef->DefaultTitle)
                SetTitle(heroDef->DefaultTitle);
            if (heroDef && --heroDef->RefCount == 0)
                heroDef->Destroy();
        }
    }
}

// ---- test stubs for the retail callees ----
static __int64 g_uid; static CThing* g_found; static long g_title = -1; static int g_titles, g_destroyed;
static CDefClassBase* g_heroDef;
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; }
void CBaseIntelligentPointer::v00() {}
template<class T> void CIntelligentPointer<T>::v00() {}
void CDefClassBase::v00() {}
void CDefClassBase::Destroy() const { ++g_destroyed; }
template<class T> bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{ if (!g_heroDef) return false; ++g_heroDef->RefCount; out->Object = (const T*)g_heroDef; return true; }
CThing* CThingManager::GetThingWithUID(__int64 uid) { g_uid = uid; return g_found; }
static CThingManager g_tm;
CThingManager* CTCHero::GetThingManager() { return &g_tm; }
void CTCHero::SetTitle(long t) { g_title = t; ++g_titles; }

int main()
{
    int ok = 1;
    CThing weapon, owner; CHeroDef heroDef; heroDef.RefCount = 1; heroDef.DefaultTitle = 7;
    owner.PBaseDef = &heroDef; g_heroDef = &heroDef; g_found = &weapon;
    unsigned char raw[sizeof(CTCHero)]; std::memset(raw, 0, sizeof(raw));
    CTCHero& hero = *(CTCHero*)raw; hero.ParentThing = &owner;
    hero.LastWeaponEquippedID = ((__int64)5 << 32) | 9; hero.HeroTitleObjectDef = 3;
    hero.OnInitialActivate();       // weapon relinked; saved title wins
    if (g_uid != (((__int64)5 << 32) | 9) || hero.LastWeaponEquipped.m_pItem != &weapon || g_title != 3 || g_titles != 1) ok = 0;
    hero.LastWeaponEquippedID = 0; hero.LastWeaponEquipped.m_pItem = 0; hero.HeroTitleObjectDef = 0; g_uid = 0;
    hero.OnInitialActivate();       // default title from the hero def, def released (count back to 1)
    if (g_uid != 0 || hero.LastWeaponEquipped.m_pItem != 0 || g_title != 7 || g_titles != 2 || heroDef.RefCount != 1 || g_destroyed) ok = 0;
    heroDef.DefaultTitle = 0;
    hero.OnInitialActivate();       // no default: SetTitle not called
    if (g_titles != 2) ok = 0;
    owner.PBaseDef = 0;
    hero.OnInitialActivate();
    if (g_titles != 2) ok = 0;
    if (!ok) { std::printf("006ca4e0_TEST FAIL\n"); return 1; }
    std::printf("006ca4e0_TEST PASS\n");
    return 0;
}