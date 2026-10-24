// Behaviour test for 0x00945DE0 CIsCookingPot::operator(): live kind-5 object with type-flag bit 19 and a user-action-
// point component; the free-point requirement applies only when the predicate asks for it.
#include <cstdio>

struct CTCUserActionPointBase {
    bool free;
    int calls;
    bool IsActionPointFree() { ++calls; return free; }
};

class CThing {
public:
    unsigned char m_kind;
    unsigned int flags;
    CTCUserActionPointBase* points;             // stands for the interface-0x2d lookup (null = no component)
    unsigned int GetThingTypeFlags() const { return flags; }
    template<class T> bool GetInterface(T** ppOut, long) const
    {
        if (points) { *ppOut = points; return true; }
        return false;
    }
};

template <class T> class CIntelligentPointer {
public:
    T* item;
    T* GetPItem() const { return item; }
};

class CIsCookingPot {
public:
    bool m_onlyWithFreePoint;
    bool operator()(const CIntelligentPointer<CThing>& item) const;
};

bool CIsCookingPot::operator()(const CIntelligentPointer<CThing>& item) const
{
    if (item.GetPItem() && item.GetPItem()->m_kind == 5 && ((unsigned char)(item.GetPItem()->GetThingTypeFlags() >> 19) & 1)) {
        CTCUserActionPointBase* points;
        if (item.GetPItem()->GetInterface(&points, 0x2d)) {
            if (m_onlyWithFreePoint)
                return points->IsActionPointFree();
            return true;
        }
    }
    return false;
}

int main()
{
    int ok = 1;
    CTCUserActionPointBase pts = { false, 0 };
    CThing pot;
    pot.m_kind = 5;
    pot.flags = 1u << 19;
    pot.points = &pts;
    CIntelligentPointer<CThing> p = { &pot };
    CIntelligentPointer<CThing> none = { 0 };
    CIsCookingPot any = { false };
    CIsCookingPot freeOnly = { true };
    if (any(none) || freeOnly(none)) ok = 0;
    if (!any(p) || pts.calls != 0) ok = 0;                 // no free-point requirement: no call
    if (freeOnly(p) || pts.calls != 1) ok = 0;             // occupied
    pts.free = true;
    if (!freeOnly(p)) ok = 0;
    pot.flags = 1u << 18;
    if (any(p)) ok = 0;                                    // wrong type flag
    pot.flags = (1u << 19) | 0xff;
    pot.m_kind = 4;
    if (any(p)) ok = 0;                                    // wrong kind
    pot.m_kind = 5;
    pot.points = 0;
    if (any(p) || freeOnly(p)) ok = 0;                     // no action points
    if (!ok) { std::printf("00945de0_TEST FAIL\n"); return 1; }
    std::printf("00945de0_TEST PASS\n");
    return 0;
}