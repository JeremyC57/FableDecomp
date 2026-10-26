#include <cstdio>

// Model of CTCHero::Copy(const CTCBase&) @ 0x006CA040: copies WeaponsUsable, WillUsable, Title, HeroTitleObjectDef and
// the counted title sub-def (old def released and destroyed at zero, new one add-ref'd), leaves the override and podium
// flags alone, then chains to CTCBase::Copy.
static int g_destroyed = 0; static int g_baseCopies = 0;
class CDefPointeeBase { public: long m_count; virtual void Destroy() { ++g_destroyed; } virtual ~CDefPointeeBase() { }
    void Release() { if (--m_count == 0) Destroy(); } };
struct CDefPointer {
    CDefPointeeBase* Object;
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) { Object->Release(); Object = 0; }
            Object = other.Object;
            if (Object) ++Object->m_count;
        }
        return *this;
    }
};
class CTCBase { public: virtual void Copy(const CTCBase&) { ++g_baseCopies; } virtual ~CTCBase() { } };
class CTCHero : public CTCBase { public:
    bool WeaponsUsable, WillUsable, WeaponUseOverriddenByScripts, WillUseOverriddenByScripts;
    long Title; long HeroTitleObjectDef; CDefPointer PHeroTitleSubDef; bool OnBoastingPodium;
    virtual void Copy(const CTCBase& other)
    {
        const CTCHero& hero = static_cast<const CTCHero&>(other);
        WeaponsUsable = hero.WeaponsUsable; WillUsable = hero.WillUsable;
        Title = hero.Title; HeroTitleObjectDef = hero.HeroTitleObjectDef;
        PHeroTitleSubDef = hero.PHeroTitleSubDef;
        CTCBase::Copy(other);
    }
};

int main()
{
    CDefPointeeBase oldDef; oldDef.m_count = 1; CDefPointeeBase newDef; newDef.m_count = 1;
    CTCHero src; src.WeaponsUsable = true; src.WillUsable = true; src.WeaponUseOverriddenByScripts = true;
    src.WillUseOverriddenByScripts = true; src.Title = 5; src.HeroTitleObjectDef = 77; src.PHeroTitleSubDef.Object = &newDef; src.OnBoastingPodium = true;
    CTCHero dst; dst.WeaponsUsable = false; dst.WillUsable = false; dst.WeaponUseOverriddenByScripts = false;
    dst.WillUseOverriddenByScripts = false; dst.Title = 0; dst.HeroTitleObjectDef = 0; dst.PHeroTitleSubDef.Object = &oldDef; dst.OnBoastingPodium = false;
    CTCBase& base = dst; base.Copy(src);
    bool ok = dst.WeaponsUsable && dst.WillUsable && !dst.WeaponUseOverriddenByScripts && !dst.WillUseOverriddenByScripts &&
              dst.Title == 5 && dst.HeroTitleObjectDef == 77 && dst.PHeroTitleSubDef.Object == &newDef && !dst.OnBoastingPodium &&
              oldDef.m_count == 0 && g_destroyed == 1 && newDef.m_count == 2 && g_baseCopies == 1;
    dst.Copy(src);                                    // same def: no count change
    ok = ok && newDef.m_count == 2 && g_destroyed == 1 && g_baseCopies == 2;
    if (!ok) { std::printf("006ca040_TEST FAIL\n"); return 1; }
    std::printf("006ca040_TEST PASS\n");
    return 0;
}