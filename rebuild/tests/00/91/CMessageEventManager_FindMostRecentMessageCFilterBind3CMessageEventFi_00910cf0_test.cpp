// behaviour test for 0x00910CF0 CMessageEventManager::FindMostRecentMessage<CFilter_Bind3<...> > (model; filters are stubs)
#include <cstdio>
#include <list>
class CMessageEvent { public: int type; int extra; };
struct FType { int want; bool operator()(const CMessageEvent& m) const { return m.type == want; } };
struct FExtra { int want; bool operator()(const CMessageEvent& m) const { return m.extra == want; } };
struct FAll { bool operator()(const CMessageEvent&) const { return true; } };
template <class TF1, class TF2, class TF3> class CFilter_Bind3 { public:
    TF1* F1; TF2* F2; TF3* F3;
    bool operator()(const CMessageEvent& m) const { return (*F1)(m) && (*F2)(m) && (*F3)(m); }
};
class CMessageEventManager { public:
    std::list<CMessageEvent> Messages;
    template <class TFilter> const CMessageEvent* FindMostRecentMessage(const TFilter& filter) const
    {
        for (std::list<CMessageEvent>::const_iterator it = Messages.begin(); it != Messages.end(); ++it)
            if (filter(*it))
                return &*it;
        return 0;
    }
};
int main()
{
    CMessageEventManager mgr;
    CMessageEvent a = { 5, 1 }, b = { 7, 2 }, c = { 7, 3 };
    mgr.Messages.push_back(a); mgr.Messages.push_back(b); mgr.Messages.push_back(c);
    FType t = { 7 }; FExtra e = { 3 }; FAll all;
    CFilter_Bind3<FType, FExtra, FAll> f = { &t, &e, &all };
    const CMessageEvent* r = mgr.FindMostRecentMessage(f);
    int ok = r != 0 && r->extra == 3;
    e.want = 9;
    ok = ok && mgr.FindMostRecentMessage(f) == 0;
    if (!ok) { std::printf("00910cf0_TEST FAIL\n"); return 1; }
    std::printf("00910cf0_TEST PASS\n");
    return 0;
}