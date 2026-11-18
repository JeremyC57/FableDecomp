#include <cassert>
#include <cmath>
#include <cstdio>

struct ThingFixture
{
    bool alive;
    float x;
};

struct PerceptionFixture
{
    const ThingFixture* perceived;
    bool aware;
    float smell_radius, sound_radius, extended_sight_radius, facing_x, fov;
};

class CGameScriptInterface
{
public:
    const PerceptionFixture* perception;
    bool CanThingBe_Sensed_ByOtherThing(const ThingFixture& thing,
                                      const ThingFixture& other_thing, long sense) const
    {
        assert(sense == 5);
        if (!thing.alive || !other_thing.alive)
            return false;
        if (!perception)
            return false;
        if (perception->perceived == &thing)
            return perception->aware;
        const float sense_radius = perception->smell_radius > perception->sound_radius
                                 ? perception->smell_radius : perception->sound_radius;
        const float from_see_er_to_see_ee = thing.x - other_thing.x;
        const float squared = from_see_er_to_see_ee * from_see_er_to_see_ee;
        if (squared <= sense_radius * sense_radius)
            return true;
        const float sight_radius = perception->extended_sight_radius;
        if (squared <= sight_radius * sight_radius) {
            const float direction = from_see_er_to_see_ee < 0.0f ? -1.0f : 1.0f;
            return std::cos(perception->fov * double(6.2831855f)) <
                   perception->facing_x * direction;
        }
        return false;
    }

    bool IsThingAwareOfOtherThingInAnyWay(const ThingFixture& thing, const ThingFixture& other_thing) const
    {
        return CanThingBe_Sensed_ByOtherThing(thing, other_thing, 5);
    }
};

int main()
{
    CGameScriptInterface game;
    ThingFixture target = {true, 0.0f};
    ThingFixture observer = {true, 0.0f};
    const ThingFixture unrelated = {true, 0.0f};
    for (unsigned mode = 0; mode < 32; ++mode) {
        for (int x = -8; x <= 8; ++x) {
            PerceptionFixture perception = {
                mode & 8 ? &target : &unrelated, (mode & 16) != 0, 2, 3, 5, 1, 0.125f
            };
            game.perception = mode & 4 ? &perception : 0;
            target.alive = (mode & 1) != 0;
            observer.alive = (mode & 2) != 0;
            target.x = float(x);
            const bool expected = (mode & 7) == 7 &&
                ((mode & 8) ? ((mode & 16) != 0) : x >= -3 && x <= 5);
            assert(game.IsThingAwareOfOtherThingInAnyWay(target, observer) == expected);
        }
    }
    std::puts("0089ce70_TEST PASS");
    return 0;
}
