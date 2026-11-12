// Behaviour test for 0x007B8B10 CTCTavernTable::GetEatingPoint: a copy of the function's logic over a small model of the
// named-action-point component (names "ACTION_POINT_SIT<n>", free flags) and the table's Food vector.
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct Points {
    const char* names[8];
    bool        free_[8];
    int         count;
    int         freed;
    bool GetFreeNamedActionPoint(const char* prefix, long& index, long start)
    {
        for (long i = start; i < count; ++i) {
            if (free_[i] && std::strncmp(names[i], prefix, std::strlen(prefix)) == 0) {
                free_[i] = false;
                index = i;
                return true;
            }
        }
        return false;
    }
    const char* GetName(long index) const { return names[index]; }
    void SetActionPointAsFree(long index) { free_[index] = true; ++freed; }
};

struct Table {
    Points* points;
    void*   food[4];
    unsigned long foodCount;
    long    ActionPointSitStrlen;
    bool    AvailableForUse;
    bool GetEatingPoint(long& action_point_index, long& seat_index)
    {
        if (AvailableForUse) {
            long start = 0;
            for (;;) {
                if (!points->GetFreeNamedActionPoint("ACTION_POINT_SIT", action_point_index, start)) {
                    action_point_index = -1;
                    return false;
                }
                const char* name = points->GetName(action_point_index);
                long chair = std::atol(name + ActionPointSitStrlen);
                if (foodCount <= (unsigned long)(chair - 1) || chair - 1 < 0) {
                    points->SetActionPointAsFree(action_point_index);
                    action_point_index = -1;
                    break;
                }
                if (food[chair - 1]) {
                    seat_index = chair - 1;
                    return true;
                }
                points->SetActionPointAsFree(action_point_index);
                start = action_point_index + 1;
            }
        }
        return false;
    }
};

static void Fill(Points& p)
{
    static const char* n[5] = { "ACTION_POINT_SIT1", "ACTION_POINT_SERVE1", "ACTION_POINT_SIT2", "ACTION_POINT_SIT3", "ACTION_POINT_SIT9" };
    p.count = 5; p.freed = 0;
    for (int i = 0; i < 5; ++i) { p.names[i] = n[i]; p.free_[i] = true; }
}

int main()
{
    int dummy = 0;
    bool ok = true;
    Points p; Table t;
    t.points = &p; t.foodCount = 3; t.ActionPointSitStrlen = 16;
    long point, seat;

    // 1: table not available -> false, nothing touched.
    Fill(p); t.AvailableForUse = false; t.food[0] = &dummy; t.food[1] = &dummy; t.food[2] = &dummy;
    point = 77; seat = 55;
    if (t.GetEatingPoint(point, seat) || point != 77 || seat != 55 || p.freed != 0) ok = false;

    // 2: food only on seat 3 (index 2): points 0 and 2 are claimed then freed, point 3 is kept.
    Fill(p); t.AvailableForUse = true; t.food[0] = 0; t.food[1] = 0; t.food[2] = &dummy;
    point = 77; seat = 55;
    if (!t.GetEatingPoint(point, seat) || point != 3 || seat != 2 || p.freed != 2) ok = false;
    if (!p.free_[0] || !p.free_[2] || p.free_[3] || !p.free_[1]) ok = false;

    // 3: no food anywhere: seats 1..3 are skipped, then "SIT9" is out of range -> freed, index -1, false.
    Fill(p); t.food[2] = 0;
    point = 77; seat = 55;
    if (t.GetEatingPoint(point, seat) || point != -1 || seat != 55 || p.freed != 4 || !p.free_[4]) ok = false;

    // 4: no sit point is free at all -> index -1, false, nothing freed.
    Fill(p); p.free_[0] = p.free_[2] = p.free_[3] = p.free_[4] = false; t.food[0] = &dummy;
    point = 77; seat = 55;
    if (t.GetEatingPoint(point, seat) || point != -1 || seat != 55 || p.freed != 0) ok = false;

    // 5: a name with no digits gives chair 0 -> seat -1 is rejected by the range test.
    Fill(p); p.names[0] = "ACTION_POINT_SIT"; t.food[0] = &dummy;
    point = 77; seat = 55;
    if (t.GetEatingPoint(point, seat) || point != -1 || p.freed != 1 || !p.free_[0]) ok = false;

    // 6: food on seat 1 -> first point taken at once.
    Fill(p); t.food[0] = &dummy;
    point = 77; seat = 55;
    if (!t.GetEatingPoint(point, seat) || point != 0 || seat != 0 || p.freed != 0 || p.free_[0]) ok = false;

    if (!ok) { std::printf("007b8b10_TEST FAIL\n"); return 1; }
    std::printf("007b8b10_TEST PASS\n");
    return 0;
}