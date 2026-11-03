#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <utility>
// Model of CTCWife::OnSerialise: in clear mode every scalar takes its default (FrameGotMarriedToThePlayer, FrameEnteredAttitudeHate
// and FrameEnteredLoveWithHusbandPresentAtHome 0x7FFFFFFF, GiftGivingPriceValue 500, the rest zero / false), the four
// BooleanHusbandAppearances[i] names are built per index; saving writes ThingsSeenThePlayerWear as def names and GiftsReceived as
// (def name, frame) pairs, skipping non-positive def indices; loading maps the names back to def indices and fills both containers.
enum Mode { CLEAR, SAVE, LOAD };
struct Context {
    Mode mode;
    std::vector<std::string> names;
    std::vector<std::string> seen; std::vector<std::pair<std::string,long> > gifts;
    template<class T> void Transfer(const char* n, T& v, const T& d) { names.push_back(n); if (mode == CLEAR) v = d; }
    template<class T> void Transfer(const std::string& n, T& v) { names.push_back(n); if (mode == CLEAR) v = T(); }
};
static std::string DefName(long i) { char b[16]; std::sprintf(b, "DEF_%ld", i); return b; }
static long DefIndex(const std::string& n) { long i = 0; std::sscanf(n.c_str(), "DEF_%ld", &i); return i; }
struct Wife {
    bool CourtingBlocked; long FrameGotMarried, FrameEnteredHate, FrameEnteredLove, GiftGivingPriceValue; float Love;
    bool Appear[4];
    std::set<long> Seen; std::map<long,long> Gifts;
    void OnSerialise(Context& c)
    {
        c.Transfer("CourtingBlocked", CourtingBlocked, false);
        c.Transfer("FrameGotMarriedToThePlayer", FrameGotMarried, 0x7fffffffL);
        c.Transfer("FrameEnteredAttitudeHate", FrameEnteredHate, 0x7fffffffL);
        c.Transfer("FrameEnteredLoveWithHusbandPresentAtHome", FrameEnteredLove, 0x7fffffffL);
        c.Transfer("GiftGivingPriceValue", GiftGivingPriceValue, 500L);
        for (long i = 0; i < 4; ++i) { char b[8]; std::sprintf(b, "%ld", i); c.Transfer("BooleanHusbandAppearances[" + std::string(b) + "]", Appear[i]); }
        c.Transfer("LoveAttitudeValue", Love, 0.0f);
        if (c.mode == SAVE) {
            c.seen.clear(); for (std::set<long>::iterator it = Seen.begin(); it != Seen.end(); ++it) c.seen.push_back(DefName(*it));
            c.gifts.clear();
            for (std::map<long,long>::iterator it = Gifts.begin(); it != Gifts.end(); ++it)
                if (it->first > 0) c.gifts.push_back(std::make_pair(DefName(it->first), it->second));
        } else if (c.mode == LOAD) {
            for (size_t i = 0; i < c.seen.size(); ++i) Seen.insert(DefIndex(c.seen[i]));
            for (size_t i = 0; i < c.gifts.size(); ++i) Gifts.insert(std::make_pair(DefIndex(c.gifts[i].first), c.gifts[i].second));
        }
    }
};
int main()
{
    int bad = 0;
    Wife w; w.CourtingBlocked = true; w.FrameGotMarried = 5; w.FrameEnteredHate = 6; w.FrameEnteredLove = 7; w.GiftGivingPriceValue = 1;
    w.Love = 0.5f; for (int i = 0; i < 4; ++i) w.Appear[i] = true;
    Context c; c.mode = CLEAR; w.OnSerialise(c);
    if (w.CourtingBlocked || w.FrameGotMarried != 0x7fffffff || w.FrameEnteredHate != 0x7fffffff || w.FrameEnteredLove != 0x7fffffff) bad = 1;
    if (w.GiftGivingPriceValue != 500 || w.Love != 0.0f || w.Appear[0] || w.Appear[3]) bad = 1;
    if (c.names.size() != 10 || c.names[5] != "BooleanHusbandAppearances[0]" || c.names[8] != "BooleanHusbandAppearances[3]") bad = 1;
    w.Seen.insert(3); w.Seen.insert(9); w.Gifts[4] = 100; w.Gifts[0] = 7; w.Gifts[-2] = 8;
    Context s; s.mode = SAVE; w.OnSerialise(s);
    if (s.seen.size() != 2 || s.seen[0] != "DEF_3" || s.gifts.size() != 1 || s.gifts[0].first != "DEF_4" || s.gifts[0].second != 100) bad = 1;
    Wife r; r.CourtingBlocked = false; r.FrameGotMarried = 1; r.FrameEnteredHate = 1; r.FrameEnteredLove = 1; r.GiftGivingPriceValue = 1; r.Love = 1.0f;
    s.mode = LOAD; r.OnSerialise(s);
    if (r.Seen.size() != 2 || !r.Seen.count(9) || r.Gifts.size() != 1 || r.Gifts[4] != 100) bad = 1;
    if (bad) { std::printf("006d2bd0_TEST FAIL\n"); return 1; }
    std::printf("006d2bd0_TEST PASS\n");
    return 0;
}
