// Behaviour test for CTCActionUseWallMount::OnActionUse (0x007EFE70).
#include <cstdio>

class CTCBase {};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
    const SEntry* LowerBound(const long* type) const { const SEntry* e = first; while (e != last && e->type < *type) ++e; return e; }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_stateFlags;
    CInterfaceMap::SEntry entries[8];
    bool HasTC(long t) const { if (m_presentBits[t >> 5] & (1UL << (t & 31))) return true; return false; }
    template <class T> T* GetTC(long t)
    {
        long type = t; CInterfaceMap* map = &m_interfaces; CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || t < it->type) ? map->last : it)->component;
    }
    template<class T> bool GetInterface(T** ppOut, long t) const
    {
        if (m_presentBits[t >> 5] & (1UL << (t & 31))) {
            long type = t; const CInterfaceMap* map = &m_interfaces; const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || t < it->type) it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    void Init() { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_interfaces.first = m_interfaces.last = entries; m_stateFlags = 0; }
    void Add(long t, CTCBase* c)   // types added in increasing order
    {
        m_presentBits[t >> 5] |= 1UL << (t & 31);
        m_interfaces.last->type = t; m_interfaces.last->component = c; ++m_interfaces.last;
    }
};
struct CCreatureActionBase { int kind; void* target; };
static int g_actions = 0; static void* g_target = 0; static int g_effectsUsed = 0; static int g_base = 0;
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(CCreatureActionBase* action) { ++g_actions; g_target = action->target; return true; }
};
inline bool IsThingAlive(const CThing* thing) { return thing && !(thing->m_stateFlags & 1); }
struct CTCWallMount : CTCBase { CThing* mounted; CThing* GetMountedThing() { return mounted; } };
struct CTCWallMountEffects : CTCBase {
    bool usable, handles;
    bool IsUsable() { return usable; }
    bool OnWallMountUseAction(CThingCreatureBase*) { ++g_effectsUsed; return handles; }
};
struct CCreatureAction_UseWallMount : CCreatureActionBase {
    CCreatureAction_UseWallMount(CThingCreatureBase*, void* t) { kind = 1; target = t; }
};
class CTCActionUseWallMount {
public:
    CThing* ParentThing;
    void BaseOnActionUse(CThingCreatureBase*) { ++g_base; }
    void OnActionUse(CThingCreatureBase* user)
    {
        BaseOnActionUse(user);
        if (IsThingAlive(user)) {
            if (ParentThing->HasTC(0xda)) {
                CTCWallMount* mount = ParentThing->GetTC<CTCWallMount>(0xda);
                if (mount) {
                    CThing* mounted = mount->GetMountedThing();
                    if (IsThingAlive(mounted) && mounted->HasTC(0xdb)) {
                        CTCWallMountEffects* effects = mounted->GetTC<CTCWallMountEffects>(0xdb);
                        if (effects->IsUsable() && effects->OnWallMountUseAction(user))
                            return;
                    }
                }
            }
            CCreatureAction_UseWallMount action(user, ParentThing);
            user->SetCurrentAction(&action);
        }
    }
};

int main()
{
    int ok = 1;
    CTCWallMountEffects fx; fx.usable = true; fx.handles = true;
    CThing trophy; trophy.Init(); trophy.Add(0xdb, &fx);
    CTCWallMount mount; mount.mounted = &trophy;
    CThing wall; wall.Init(); wall.Add(0xda, &mount);
    CThingCreatureBase hero; hero.Init();
    CTCActionUseWallMount use; use.ParentThing = &wall;
    use.OnActionUse(&hero);
    if (g_base != 1 || g_effectsUsed != 1 || g_actions != 0) ok = 0;        // the effects handled it
    fx.handles = false;
    use.OnActionUse(&hero);
    if (g_effectsUsed != 2 || g_actions != 1 || g_target != &wall) ok = 0; // not handled: the use action starts
    fx.usable = false;
    use.OnActionUse(&hero);
    if (g_effectsUsed != 2 || g_actions != 2) ok = 0;
    hero.m_stateFlags = 1;
    use.OnActionUse(&hero);
    if (g_actions != 2 || g_base != 4) ok = 0;                              // dead user: nothing
    if (!ok) { std::printf("007efe70_TEST FAIL\n"); return 1; }
    std::printf("007efe70_TEST PASS\n");
    return 0;
}