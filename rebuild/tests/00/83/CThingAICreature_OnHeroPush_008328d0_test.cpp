// Behaviour test for 008328d0: src/008328d0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CThingAICreature::OnHeroPush(CThing* pusher, float distance, const C3DVector& direction)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingAICreature::OnHeroPush(CThing* pusher, float distance, const C3DVector& direction) -- when the pusher is the living main hero carrying the hero component, the creature's push component (added on demand) accumulates the distance, takes the direction and restarts its frame counter
// Leaf: OnHeroPush
// Retail: Fable.exe (TLC, VC7.1) @ 0x008328D0, 218 bytes (+6 int3). `ret 0Ch`.
// Callees: CThing::GetPlayerManager() 0x004C7A00 (manifest "CViewBase::GetDisplayEngine" propagated: g_pThingManager->PlayerManager),
//          CPlayerManager::GetMainPlayer() 0x00449970, CPlayer::GetHeroThing() 0x00487DC0, CVectorMap::LowerBound(const long*)
//          0x0040F020, CCharString::CCharString(const char*, long) 0x0099EBF0 ("CTCOnHeroPush" @ 0x0123CBB0), CThing::AddTC
//          (const CCharString&, bool, CTCParamsBase*) 0x004C9D60, ~CCharString 0x0099EAE0.
// Shape: the hero's two id words are compared against the pusher's; the pusher's 0x51 bit is word +0x28 bit 17 and the
//        creature's 0xC4 bit word +0x38 bit 4; the lookup key and the name temporary share the pusher's dead home slot;
//        the component pointer (esi, the `this` register reused) joins from both arms before the three stores.
//        The key and the name share that slot only with a same-TU noinline LowerBound stand-in (its address no longer
//        escapes) and the name as a named local in the AddTC block (008967f0 idiom; with the temporary argument 35 lines).
// Layout: CThing UniqueID +0x18, presence bits +0x20, Components +0x44, Dead +0x91 bit 0; CTCOnHeroPush DistPushed +0x0C,
//         PushDir +0x10, FrameSincePushed +0x1C (Ego_r.pdb).
class C3DVector { public: float X, Y, Z; };
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
};
class CTCBase; class CTCParamsBase;
class CTCOnHeroPush { public:                               // interface type 0xC4
    unsigned char _pad00[0x0c];
    float         DistPushed;                        // +0x0c
    C3DVector     PushDir;                           // +0x10
    long          FrameSincePushed;                  // +0x1c
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (not the retail body).
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - it;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) { it += half + 1; count -= half + 1; }
        else count = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char    _pad00[0x18];
    unsigned __int64 UniqueID;                       // +0x18
    unsigned long    m_presentBits[9];               // +0x20 UsingComponents
    CInterfaceMap    m_interfaces;                   // +0x44
    unsigned char    _pad4c[0x91 - 0x4c];
    unsigned char    Dead : 1;                       // +0x91 bit 0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31))) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    CTCBase* AddTC(const CCharString& name, bool a2, CTCParamsBase* params);   // 0x004C9D60
};
class CPlayer { public:
    CThing* GetHeroThing() const;                    // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer();                        // 0x00449970
};
class CThingAICreature : public CThing { public:
    CPlayerManager* GetPlayerManager();              // 0x004C7A00
    void OnHeroPush(CThing* pusher, float distance, const C3DVector& direction);
};
void CThingAICreature::OnHeroPush(CThing* pusher, float distance, const C3DVector& direction)
{
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
    if (hero && !hero->Dead && hero->UniqueID == pusher->UniqueID && pusher->HasTC(0x51)) {
        CTCOnHeroPush* push;
        if (!GetInterface(&push, 0xc4)) {
            CCharString name("CTCOnHeroPush", -1);
            push = (CTCOnHeroPush*)AddTC(name, true, 0);
        }
        push->DistPushed += distance;
        push->PushDir = direction;
        push->FrameSincePushed = 0;
    }
}

// ---- stubs and driver ----
static int g_strCtors = 0, g_strDtors = 0, g_adds = 0;
static const char* g_lastText = 0;
CCharString::CCharString(const char* text, long length) { ++g_strCtors; g_lastText = text; CHECK(length == -1); PStringData = (void*)text; }
CCharString::~CCharString() { ++g_strDtors; }
static CTCOnHeroPush g_added; static bool g_addActivate = false; static CTCParamsBase* g_addParams = (CTCParamsBase*)1; static CThing* g_addedTo = 0;
CTCBase* CThing::AddTC(const CCharString& name, bool a2, CTCParamsBase* params)
{ ++g_adds; g_addedTo = this; CHECK(strcmp((const char*)name.PStringData, "CTCOnHeroPush") == 0); g_addActivate = a2; g_addParams = params; return (CTCBase*)&g_added; }
static CThing* g_hero = 0;
static CPlayer g_player; static CPlayerManager g_pm;
CThing* CPlayer::GetHeroThing() const { CHECK(this == &g_player); return g_hero; }
CPlayer* CPlayerManager::GetMainPlayer() { CHECK(this == &g_pm); return &g_player; }
CPlayerManager* CThingAICreature::GetPlayerManager() { return &g_pm; }
int main()
{
    static CThingAICreature creature; static CThing hero, pusher; static CTCOnHeroPush push;
    static CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x51; entries[0].component = 0;
    entries[1].type = 0xc4; entries[1].component = (CTCBase*)&push;
    creature.m_interfaces.first = &entries[0]; creature.m_interfaces.last = &entries[2];
    C3DVector dir; dir.X = 1.0f; dir.Y = 2.0f; dir.Z = 3.0f;
    hero.UniqueID = 0x123456789ULL; pusher.UniqueID = 0x123456789ULL;
    pusher.m_presentBits[2] = 1UL << 17;               // 0x51
    creature.m_presentBits[6] = 1UL << 4;              // 0xC4
    push.FrameSincePushed = 9;
    // 1) no hero: nothing
    g_hero = 0; creature.OnHeroPush(&pusher, 1.5f, dir);
    CHECK(push.DistPushed == 0.0f && push.FrameSincePushed == 9);
    // 2) dead hero: nothing
    g_hero = &hero; hero.Dead = 1; creature.OnHeroPush(&pusher, 1.5f, dir);
    CHECK(push.DistPushed == 0.0f);
    // 3) different id (high word differs): nothing
    hero.Dead = 0; pusher.UniqueID = 0x223456789ULL; creature.OnHeroPush(&pusher, 1.5f, dir);
    CHECK(push.DistPushed == 0.0f);
    // 4) pusher lacks 0x51: nothing
    pusher.UniqueID = hero.UniqueID; pusher.m_presentBits[2] = 0; creature.OnHeroPush(&pusher, 1.5f, dir);
    CHECK(push.DistPushed == 0.0f);
    // 5) the hero pushes: distance accumulated, direction copied, frame counter reset
    pusher.m_presentBits[2] = 1UL << 17;
    creature.OnHeroPush(&pusher, 1.5f, dir);
    creature.OnHeroPush(&pusher, 2.0f, dir);
    CHECK(push.DistPushed == 3.5f && push.PushDir.X == 1.0f && push.PushDir.Y == 2.0f && push.PushDir.Z == 3.0f);
    CHECK(push.FrameSincePushed == 0 && g_adds == 0 && g_strCtors == 0);
    // 6) no push component: "CTCOnHeroPush" added (active, no params) and updated
    creature.m_presentBits[6] = 0;
    creature.OnHeroPush(&pusher, 4.0f, dir);
    CHECK(g_adds == 1 && g_addedTo == &creature && g_addActivate == true && g_addParams == 0);
    CHECK(g_strCtors == 1 && g_strDtors == 1 && g_added.DistPushed == 4.0f && g_added.PushDir.Z == 3.0f);
    CHECK(g_lastText && strcmp(g_lastText, "CTCOnHeroPush") == 0);
    if (fails == 0) printf("008328d0_TEST PASS\n");
    return fails;
}
