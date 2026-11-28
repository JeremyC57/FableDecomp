#include <cassert>
#include <cstdio>

namespace {

enum Event { NotifyLeft, DeleteLeft, NotifyRight, DeleteRight, QueryMode,
             GetCreature, ReadTargeting, ReadPosition, Project, ReadTarget, QueryEnemies, QueryLock,
             SubmitLeft, SubmitRight, SubmitUp, SubmitDown };

struct Fixture {
    Fixture() : first_person(true), controlled_alive(true), has_targeting(true), has_position(true),
                projects(true), target_alive(true), enemy(false), locked(false), drawing(false),
                count(0), left_count(1), right_count(1), shared(false), graphics_base(0) {}

    bool first_person, controlled_alive, has_targeting, has_position, projects, target_alive, enemy, locked, drawing;
    int count, left_count, right_count;
    bool shared;
    int graphics_base;
    Event events[20];
    float x[4], y[4];

    void Record(Event event) { events[count++] = event; }
    void Clear(int& references, Event notify, Event destroy) {
        if (references == 0)
            return;
        if (references == 1)
            Record(notify);
        if (--references == 0)
            Record(destroy);
    }

    void Draw() {
        Clear(left_count, NotifyLeft, DeleteLeft);
        Clear(shared ? left_count : right_count, NotifyRight, DeleteRight);
        Record(QueryMode);
        if (!first_person)
            return;
        Record(GetCreature);
        if (!controlled_alive)
            return;
        Record(ReadTargeting);
        if (!has_targeting)
            return;
        Record(ReadPosition);
        if (!has_position)
            return;
        Record(Project);
        Record(ReadTarget);
        bool targeting = false;
        if (target_alive) {
            Record(ReadTarget);
            Record(QueryEnemies);
            if (enemy) {
                targeting = true;
            } else {
                Record(QueryLock);
                targeting = locked;
            }
        }
        if (!projects)
            return;
        drawing = true;
        graphics_base = targeting ? 100 : 200;
        const float projected_x = -4.75f;
        const float projected_y = 21.875f;
        const float offset_x[] = {-3.0f, 3.0f, 0.0f, 0.0f};
        const float offset_y[] = {0.0f, 0.0f, -5.0f, 5.0f};
        for (int i = 0; i != 4; ++i) {
            x[i] = (float)(long)projected_x + offset_x[i];
            y[i] = (float)(long)projected_y + offset_y[i];
            Record(static_cast<Event>(SubmitLeft + i));
        }
    }
};

template <unsigned int Count> void Check(Fixture& fixture, const Event (&expected)[Count])
{
    fixture.Draw();
    assert(fixture.count == static_cast<int>(Count));
    for (unsigned int i = 0; i != Count; ++i)
        assert(fixture.events[i] == expected[i]);
}

} // namespace

int main()
{
    const Event inactive[] = {NotifyLeft, DeleteLeft, NotifyRight, DeleteRight, QueryMode};
    Fixture mode;
    mode.first_person = false;
    Check(mode, inactive);
    assert(!mode.drawing && mode.left_count == 0 && mode.right_count == 0);

    const Event shared_events[] = {NotifyRight, DeleteRight, QueryMode};
    Fixture shared;
    shared.first_person = false;
    shared.left_count = 2;
    shared.shared = true;
    Check(shared, shared_events);
    assert(shared.left_count == 0);

    const Event no_creature[] = {QueryMode, GetCreature};
    Fixture dead;
    dead.left_count = dead.right_count = 0;
    dead.controlled_alive = false;
    Check(dead, no_creature);

    const Event absent[] = {QueryMode, GetCreature, ReadTargeting};
    Fixture no_targeting;
    no_targeting.left_count = no_targeting.right_count = 0;
    no_targeting.has_targeting = false;
    Check(no_targeting, absent);

    const Event no_pos[] = {QueryMode, GetCreature, ReadTargeting, ReadPosition};
    Fixture invalid;
    invalid.left_count = invalid.right_count = 0;
    invalid.has_position = false;
    Check(invalid, no_pos);

    const Event projection_failed[] = {QueryMode, GetCreature, ReadTargeting, ReadPosition, Project,
                                       ReadTarget, ReadTarget, QueryEnemies, QueryLock};
    Fixture projection;
    projection.left_count = projection.right_count = 0;
    projection.projects = false;
    Check(projection, projection_failed);
    assert(!projection.drawing);

    const Event dead_target[] = {QueryMode, GetCreature, ReadTargeting, ReadPosition, Project, ReadTarget,
                                 SubmitLeft, SubmitRight, SubmitUp, SubmitDown};
    Fixture neutral;
    neutral.left_count = neutral.right_count = 0;
    neutral.target_alive = false;
    Check(neutral, dead_target);
    assert(neutral.drawing && neutral.graphics_base == 200);
    assert(neutral.x[0] == -7.0f && neutral.x[1] == -1.0f && neutral.x[2] == -4.0f && neutral.x[3] == -4.0f);
    assert(neutral.y[0] == 21.0f && neutral.y[1] == 21.0f && neutral.y[2] == 16.0f && neutral.y[3] == 26.0f);

    const Event enemy_target[] = {QueryMode, GetCreature, ReadTargeting, ReadPosition, Project, ReadTarget, ReadTarget,
                                  QueryEnemies, SubmitLeft, SubmitRight, SubmitUp, SubmitDown};
    Fixture enemy;
    enemy.left_count = enemy.right_count = 0;
    enemy.enemy = true;
    Check(enemy, enemy_target);
    assert(enemy.graphics_base == 100);

    const Event friendly_target[] = {QueryMode, GetCreature, ReadTargeting, ReadPosition, Project, ReadTarget, ReadTarget,
                                     QueryEnemies, QueryLock, SubmitLeft, SubmitRight, SubmitUp, SubmitDown};
    Fixture friendly;
    friendly.left_count = friendly.right_count = 0;
    Check(friendly, friendly_target);
    assert(friendly.graphics_base == 200);
    friendly.count = 0;
    friendly.locked = true;
    Check(friendly, friendly_target);
    assert(friendly.graphics_base == 100);

    std::printf("00647575_TEST PASS\n");
    return 0;
}