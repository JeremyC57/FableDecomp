// 007fb270 behaviour test: CTCKraken::GetClosestTentacleToTarget (control flow + data of the final.cpp body)
#include <stdio.h>
#include <math.h>

float AngleDifference(float a, float b)
{
    float d = b - a;
    while (d > 0.5f) d -= 1.0f;
    while (d < -0.5f) d += 1.0f;
    return d;
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* lo = first;
        long count = last - first;
        while (count > 0) {
            long half = count >> 1;
            SEntry* mid = lo + half;
            if (mid->type < *type) { lo = mid + 1; count -= half + 1; } else count = half;
        }
        return lo;
    }
};
struct CPhysicsTC {
    float Facing;
    float GetFacingAngleXY() { return Facing; }
};
class CThing {
public:
    unsigned long PresentBits[9];
    CInterfaceMap m_interfaces;
    CPhysicsTC*   PhysicsTC;
    unsigned char Flags91;
    C3DVector     Position;
    const C3DVector& GetPosition() const { return Position; }
    unsigned long HasTC(long type) const { return PresentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> bool GetInterface(T** out, long kind)
    {
        if (HasTC(kind)) {
            long key = kind;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)((it == map->last || kind < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> T* FindTC(long kind)
    {
        long key = kind;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || kind < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
struct CTCKrakenTentacle {
    float AngleOffset;
};
struct CTCScriptedHook {
    bool Mode54;
    bool HasMode(long mode) { return mode == 0x54 && Mode54; }
};
struct STentacleNode {
    STentacleNode* _M_next;
    STentacleNode* _M_prev;
    CBaseIntelligentPointer _M_data;
};
struct CTentacleIterator { STentacleNode* _M_node; };
struct CTentacleList {
    STentacleNode* _M_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        for (STentacleNode* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
};
struct CTCKraken {
    CThing*                 ParentThing;
    CBaseIntelligentPointer Body;
    CBaseIntelligentPointer Target;
    CTentacleList           Tentacles;
    bool GetClosestTentacleToTarget(CTentacleIterator& closest);
};
bool CTCKraken::GetClosestTentacleToTarget(CTentacleIterator& closest)
{
    if (Target.GetPItem() && !(Target.GetPItem()->Flags91 & 1) && Tentacles.size() > 0) {
        C3DVector targetPos(Target.GetPItem()->GetPosition());
        C3DVector bodyPos(Body.GetPItem()->GetPosition());
        float heading = ParentThing->PhysicsTC->GetFacingAngleXY();
        float bearing = (float)atan2(targetPos.X - bodyPos.X, targetPos.Y - bodyPos.Y) * 0.15915493667125702f;
        if (bearing < 0.0f || bearing >= 1.0f) {
            bearing = (float)fmod((double)bearing, 1.0);
            if (bearing < 0.0f)
                bearing += 1.0f;
        }
        bool found = false;
        float best = 1.0f / Tentacles.size();
        for (STentacleNode* node = Tentacles._M_node->_M_next; node != Tentacles._M_node; node = node->_M_next) {
            CThing* thing = node->_M_data.GetPItem();
            CTCKrakenTentacle* tentacle;
            thing->GetInterface(&tentacle, 0xe3);
            {
                float facing = heading + tentacle->AngleOffset;
                float difference = (float)fabs(AngleDifference(facing, bearing));
                if (difference < best) {
                    best = difference;
                    closest._M_node = node;
                    found = true;
                }
            }
        }
        if (found) {
            CThing* thing = closest._M_node->_M_data.GetPItem();
            if (thing->HasTC(0x31) && !thing->FindTC<CTCScriptedHook>(0x31)->HasMode(0x54))
                return true;
        }
    }
    closest._M_node = Tentacles._M_node;
    return false;
}

static int g_fail;
static void Check(const char* name, bool cond) { if (!cond) { printf("FAIL %s\n", name); g_fail = 1; } }

struct Tentacle {
    CThing thing;
    CTCKrakenTentacle tc;
    CTCScriptedHook hook;
    CInterfaceMap::SEntry entries[2];
    STentacleNode node;
    void Init(float offset, bool hasHook)
    {
        for (int i = 0; i < 9; ++i) thing.PresentBits[i] = 0;
        thing.PresentBits[0x31 >> 5] |= 1UL << (0x31 & 31);
        if (!hasHook) thing.PresentBits[0x31 >> 5] = 0;
        thing.PresentBits[0xe3 >> 5] |= 1UL << (0xe3 & 31);
        entries[0].type = 0x31; entries[0].component = (CTCBase*)&hook;
        entries[1].type = 0xe3; entries[1].component = (CTCBase*)&tc;
        thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 2;
        thing.Flags91 = 0;
        tc.AngleOffset = offset;
        hook.Mode54 = false;
        node._M_data.m_pItem = &thing;
    }
};

int main()
{
    CPhysicsTC physics; physics.Facing = 0.0f;
    CThing parent; parent.PhysicsTC = &physics;
    CThing body; body.Position.X = 0; body.Position.Y = 0;
    CThing target; target.Flags91 = 0;
    CTCKraken kraken; kraken.ParentThing = &parent; kraken.Body.m_pItem = &body; kraken.Target.m_pItem = &target;
    STentacleNode head; head._M_next = head._M_prev = &head;
    kraken.Tentacles._M_node = &head;
    Tentacle t[4];
    for (int i = 0; i < 4; ++i) t[i].Init(0.25f * i, true);
    // ring: head -> t0 -> t1 -> t2 -> t3 -> head
    STentacleNode* prev = &head;
    for (int i = 0; i < 4; ++i) { prev->_M_next = &t[i].node; t[i].node._M_prev = prev; prev = &t[i].node; }
    prev->_M_next = &head; head._M_prev = prev;
    CTentacleIterator it;

    // target along +Y: atan2(0, 10) = 0 -> bearing 0 -> tentacle 0 (offset 0)
    target.Position.X = 0; target.Position.Y = 10;
    Check("bearing 0", kraken.GetClosestTentacleToTarget(it) && it._M_node == &t[0].node);
    // target along +X: atan2(10, 0) = pi/2 -> bearing 0.25 -> tentacle 1
    target.Position.X = 10; target.Position.Y = 0;
    Check("bearing 0.25", kraken.GetClosestTentacleToTarget(it) && it._M_node == &t[1].node);
    // heading shifts every facing by 0.25: tentacle 0 now faces 0.25
    physics.Facing = 0.25f;
    Check("heading", kraken.GetClosestTentacleToTarget(it) && it._M_node == &t[0].node);
    physics.Facing = 0.0f;
    // negative bearing wraps: atan2(-10, 0) -> -0.25 -> 0.75 -> tentacle 3
    target.Position.X = -10; target.Position.Y = 0;
    Check("wrap", kraken.GetClosestTentacleToTarget(it) && it._M_node == &t[3].node);
    // the chosen tentacle's scripted hook in mode 0x54 -> false, iterator at end
    t[3].hook.Mode54 = true;
    Check("mode54", !kraken.GetClosestTentacleToTarget(it) && it._M_node == &head);
    t[3].hook.Mode54 = false;
    // no hook interface -> false
    t[3].Init(0.75f, false);
    Check("no hook", !kraken.GetClosestTentacleToTarget(it) && it._M_node == &head);
    t[3].Init(0.75f, true);
    // target not saved / missing -> false, end
    target.Flags91 = 1;
    Check("unsaved target", !kraken.GetClosestTentacleToTarget(it) && it._M_node == &head);
    target.Flags91 = 0;
    kraken.Target.m_pItem = 0;
    Check("no target", !kraken.GetClosestTentacleToTarget(it) && it._M_node == &head);
    kraken.Target.m_pItem = &target;
    // empty list -> false
    STentacleNode empty; empty._M_next = empty._M_prev = &empty;
    kraken.Tentacles._M_node = &empty;
    Check("empty", !kraken.GetClosestTentacleToTarget(it) && it._M_node == &empty);
    kraken.Tentacles._M_node = &head;
    // nothing within one share (1/4) of a turn: four tentacles all offset by 0.5 from bearing? best starts at 0.25 strict
    for (int i = 0; i < 4; ++i) t[i].tc.AngleOffset = 0.5f;
    target.Position.X = 0; target.Position.Y = 10;
    Check("none close", !kraken.GetClosestTentacleToTarget(it) && it._M_node == &head);
    if (g_fail) return 1;
    printf("007fb270_TEST PASS\n");
    return 0;
}