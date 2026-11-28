#include <algorithm>
#include <set>
#include <cstdio>

struct Item { int id; };

struct Fixture {
    std::set<Item*> PositionChildren;

    void PositionChildDying(Item* child)
    {
        std::set<Item*>::iterator it = std::find(PositionChildren.begin(), PositionChildren.end(), child);
        if (it != PositionChildren.end())
            PositionChildren.erase(it);
    }
};

int main()
{
    Item entries[6];
    Item* keys[6];
    for (int i = 0; i < 6; ++i) keys[i] = &entries[i];
    const int sequence[] = {5, 2, 0, 4, 4, 5, 3, 1, 0};
    for (int count = 0; count <= 5; ++count) {
        Fixture fixture;
        bool present[6] = {false, false, false, false, false, false};
        for (int i = 0; i < count; ++i) {
            fixture.PositionChildren.insert(keys[i]);
            fixture.PositionChildren.insert(keys[i]);
            present[i] = true;
        }
        for (int step = 0; step < 9; ++step) {
            const int key = sequence[step];
            fixture.PositionChildDying(keys[key]);
            present[key] = false;
            unsigned expected = 0;
            for (int i = 0; i < 6; ++i) {
                expected += present[i] ? 1 : 0;
                if (fixture.PositionChildren.count(keys[i]) != (present[i] ? 1u : 0u))
                    return 1;
            }
            if (fixture.PositionChildren.size() != expected)
                return 1;
        }
    }
    std::puts("00531b40_TEST PASS");
    return 0;
}
