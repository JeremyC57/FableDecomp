#include <cassert>
#include <cstdio>

namespace {

struct Player {
    bool main;
    bool has_creature;
    bool creature_alive;
    float health_fraction;
};

enum Sprite { LeftGlow, Left, MiddleGlow, Middle, RightGlow, Right, Fill };

struct Submission {
    unsigned int handle;
    Sprite sprite;
    float left, top, right, bottom;
};

struct Fixture {
    unsigned int handles, count, lookups;
    bool drawing;
    Submission submissions[28];
    float left_width, left_height, inner_x, inner_y;

    Fixture() : handles(7), count(0), lookups(0), drawing(false), left_width(13.0f),
                left_height(6.0f), inner_x(3.0f), inner_y(1.0f) {}

    void Submit(unsigned int handle, Sprite sprite, float x, float y, float width, float height) {
        assert(handle < handles && count < 28);
        Submission& output = submissions[count++];
        output.handle = handle;
        output.sprite = sprite;
        output.left = x;
        output.top = y;
        output.right = x + width;
        output.bottom = y + height;
    }

    void Draw(const Player (&players)[4]) {
        drawing = true;
        handles = 0;
        handles = 40;
        float offset = 0.0f;
        unsigned int primitive_offset = 0;
        for (unsigned int player = 0; player != 4; ++player) {
            lookups += 2;
            const Player& candidate = players[player];
            if (candidate.main || !candidate.has_creature || !candidate.creature_alive)
                continue;
            const float left = 250.0f + offset;
            const float middle = left + left_width;
            Submit(primitive_offset + 3, LeftGlow, left, 410.0f, 0.0f, 0.0f);
            Submit(primitive_offset + 4, Left, left, 410.0f, 0.0f, 0.0f);
            Submit(primitive_offset + 5, MiddleGlow, middle, 410.0f, 0.0f, left_height);
            Submit(primitive_offset + 6, Middle, middle, 410.0f, 0.0f, left_height);
            Submit(primitive_offset + 7, RightGlow, middle, 410.0f, 0.0f, 0.0f);
            Submit(primitive_offset + 8, Right, middle, 410.0f, 0.0f, 0.0f);
            const float width = (left_width - inner_x) * 2.0f * candidate.health_fraction;
            Submit(primitive_offset, Fill, left + inner_x, 410.0f + inner_y, width, left_height);
            offset += 77.0f;
            primitive_offset += 10;
        }
    }
};

void CheckGroup(const Fixture& fixture, unsigned int group, float x, float fill_width)
{
    const Submission* group_items = fixture.submissions + 7 * group;
    const unsigned int offsets[] = {3, 4, 5, 6, 7, 8, 0};
    for (unsigned int i = 0; i != 7; ++i) {
        assert(group_items[i].sprite == static_cast<Sprite>(i));
        assert(group_items[i].handle == 10 * group + offsets[i]);
    }
    assert(group_items[LeftGlow].left == x && group_items[LeftGlow].top == 410.0f);
    assert(group_items[Left].left == x && group_items[Left].top == 410.0f);
    assert(group_items[MiddleGlow].left == x + 13.0f && group_items[MiddleGlow].right == x + 13.0f);
    assert(group_items[MiddleGlow].bottom == 416.0f && group_items[Middle].bottom == 416.0f);
    assert(group_items[RightGlow].left == group_items[MiddleGlow].right);
    assert(group_items[Right].left == group_items[Middle].right);
    assert(group_items[Fill].left == x + 3.0f && group_items[Fill].right == x + 3.0f + fill_width);
    assert(group_items[Fill].top == 411.0f && group_items[Fill].bottom == 417.0f);
}

} // namespace

int main()
{
    const Player absent = {false, false, false, 1.0f};
    const Player dead = {false, true, false, 1.0f};
    const Player main = {true, true, true, 1.0f};
    const Player half = {false, true, true, 0.5f};
    const Player full = {false, true, true, 1.0f};
    const Player zero = {false, true, true, 0.0f};

    const Player empty[] = {main, absent, dead, absent};
    Fixture none;
    none.Draw(empty);
    assert(none.drawing && none.handles == 40 && none.count == 0 && none.lookups == 8);

    const Player one[] = {absent, dead, main, half};
    Fixture last;
    last.Draw(one);
    assert(last.count == 7 && last.lookups == 8);
    CheckGroup(last, 0, 250.0f, 10.0f);

    const Player separated[] = {full, main, half, zero};
    Fixture three;
    three.Draw(separated);
    assert(three.count == 21 && three.lookups == 8);
    CheckGroup(three, 0, 250.0f, 20.0f);
    CheckGroup(three, 1, 327.0f, 10.0f);
    CheckGroup(three, 2, 404.0f, 0.0f);

    const Player all[] = {zero, half, full, half};
    Fixture four;
    four.Draw(all);
    assert(four.count == 28 && four.lookups == 8);
    CheckGroup(four, 0, 250.0f, 0.0f);
    CheckGroup(four, 1, 327.0f, 10.0f);
    CheckGroup(four, 2, 404.0f, 20.0f);
    CheckGroup(four, 3, 481.0f, 10.0f);

    std::printf("0064e2cc_TEST PASS\n");
    return 0;
}