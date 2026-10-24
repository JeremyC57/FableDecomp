// Behaviour test for 0069f980 (model copy: PeekTC over a sorted interface map, carrying/weapon components).
#include <cstdio>
#include <cstring>
class CTCBase { public: int tag; };
enum ETCInterfaceType { TC_DISCOVERY_K = 0x40 };
struct CInterfaceMap {
    struct SEntry { ETCInterfaceType type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end_of_storage;
    SEntry* LowerBound(const ETCInterfaceType& type) const
    {
        SEntry* it = first;
        while (it != last && it->type < type) ++it;
        return it;
    }
    SEntry* Find(long type) const
    {
        SEntry* it = LowerBound((ETCInterfaceType)type);
        return (it == last || type < it->type) ? last : it;
    }
};
class CThing {
public:
    unsigned int UsingComponents[9];
    CInterfaceMap Components;
    unsigned char Dead : 1;
    CInterfaceMap::SEntry entries[4];
    int n;
    CThing() : n(0) { std::memset(UsingComponents, 0, sizeof(UsingComponents)); Dead = 0; Components.first = Components.last = Components.end_of_storage = entries; }
    void Add(long type, CTCBase* tc)    // keep sorted by type
    {
        int i = n;
        while (i > 0 && entries[i - 1].type > type) { entries[i] = entries[i - 1]; --i; }
        entries[i].type = (ETCInterfaceType)type; entries[i].component = tc; ++n;
        Components.last = entries + n;
        UsingComponents[type >> 5] |= 1u << (type & 31);
    }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1u << (type & 31)))
            return true;
        return false;
    }
    template<class T> bool PeekTC(T** pp) const
    {
        if (HasTC(T::StatGetInterfaceType())) {
            *pp = (T*)Components.Find(T::StatGetInterfaceType())->component;
            return true;
        }
        return false;
    }
};
inline bool IsThingAlive(const CThing* thing)
{
    if (thing && !thing->Dead)
        return true;
    return false;
}

class CTCWeapon : public CTCBase {
public:
    static long StatGetInterfaceType() { return 0x48; }
    bool melee, projectile;
    const char* sheathe;
    const char* unsheathe;
    bool IsMeleeWeapon() const { return melee; }
    bool IsProjectileWeapon() const { return projectile; }
};
class CTCCarrying : public CTCBase {
public:
    static long StatGetInterfaceType() { return 0x46; }
    CThing* primary;
    CThing* GetThingInPrimarySlot() const { return primary; }
};
static const char* GetAnimationToUse(const CThing& creature, const CThing& item)
{
    const CTCWeapon* weapon;
    if (item.PeekTC(&weapon))
        return weapon->unsheathe;
    return "STANDARD_UNSHEATHE";
}

int main()
{
    CThing plain, hero, item, rock;
    CTCWeapon weapon; weapon.melee = true; weapon.projectile = false; weapon.sheathe = "SWORD_SHEATHE"; weapon.unsheathe = "SWORD_UNSHEATHE";
    CTCCarrying carrying; carrying.primary = &item;
    CTCBase other;
    hero.Add(0x48 + 3, &other); hero.Add(0x46, &carrying); hero.Add(0x10, &other);
    item.Add(0x48, &weapon); item.Add(0x40, &other);
    rock.Add(0x47, &other);
    plain.Add(0x45, &other);
    int fail = 0;
    if (std::strcmp(GetAnimationToUse(hero, rock), "STANDARD_UNSHEATHE") || std::strcmp(GetAnimationToUse(hero, item), "SWORD_UNSHEATHE")) fail = 1;
    if (fail) { std::printf("0069f980_TEST FAIL\n"); return 1; }
    std::printf("0069f980_TEST PASS\n");
    return 0;
}