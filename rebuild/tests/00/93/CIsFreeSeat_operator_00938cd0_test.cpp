// Behaviour test for 0x00938CD0 CIsFreeSeat::operator(): mock interfaces log every call; the body is the source's, verbatim.
#include <cstdio>
#include <cstring>

static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int Check(const char* expect) { int ok = std::strcmp(g_log, expect) == 0; if (!ok) std::printf("got  %s\nwant %s\n", g_log, expect); g_log[0] = 0; return ok; }

class CTCUserActionPointBase { public: bool Free; bool IsActionPointFree() { return Free; } };
class CThing {
public:
    int m_thingType; unsigned long Flags; CTCUserActionPointBase* Point;
    unsigned long GetStateFlags() const { return Flags; }
    bool GetInterfaceConst(long type, CTCUserActionPointBase** out) const { *out = Point; return type == 0x2d && Point != 0; }
};
template <class T> class CIntelligentPointer { public: T* Item; T* GetPItem() const { return Item; } };
class CIsFreeSeat { public: bool operator()(const CIntelligentPointer<CThing>& thing) const; };
bool CIsFreeSeat::operator()(const CIntelligentPointer<CThing>& thing) const
{
    if (thing.GetPItem() && thing.GetPItem()->m_thingType == 5) {
        CThing* seat = thing.GetPItem();
        if (((unsigned char)(seat->GetStateFlags() >> 16) & 1) || ((unsigned char)(seat->GetStateFlags() >> 17) & 1)) {
            CTCUserActionPointBase* actionPoint;
            if (thing.GetPItem()->GetInterfaceConst(0x2d, &actionPoint))
                return actionPoint->IsActionPointFree();
        }
    }
    return false;
}
int main()
{
    CTCUserActionPointBase ap; ap.Free = true;
    CThing chair; chair.m_thingType = 5; chair.Flags = 1ul << 16; chair.Point = &ap;
    CIntelligentPointer<CThing> p; p.Item = &chair; CIsFreeSeat pred;
    int ok = 1;
    ok &= pred(p);
    chair.Flags = 1ul << 17; ok &= pred(p);
    chair.Flags = 1ul << 18; ok &= !pred(p);
    chair.Flags = 1ul << 16; ap.Free = false; ok &= !pred(p);
    ap.Free = true; chair.Point = 0; ok &= !pred(p);
    chair.Point = &ap; chair.m_thingType = 4; ok &= !pred(p);
    p.Item = 0; ok &= !pred(p);
    if (ok) std::printf("00938cd0_TEST PASS\n");
    return ok ? 0 : 1;
}