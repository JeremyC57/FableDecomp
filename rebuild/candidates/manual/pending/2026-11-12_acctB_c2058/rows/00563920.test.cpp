// Behaviour test for CTCHeroOnlineScoreboard::DisplayScoreboardGUI (0x00563920): a plain model of the row's control flow and of
// the GUI it builds -- the picture cache cleared and the state machine idle first; nothing more without an inventory, off the
// scoreboard screen (menu 8) or without the scoreboard screen; otherwise the mouse pointer is reset (white, 1.0, observes mouse
// movement) and the connecting message goes inactive; an empty scoreboard shows TEXT_GUI_MENU_CLAN_EMPTY and stops; else the
// clan list gets one entry per activity (0..38) whose downloaded record vector exists and is not empty: best score + holder,
// the activity description when the holder is a clan member, and a sub list of every record (score, holder, the member's
// description when found, the member's picture), then the clan menu entry gets a select event and the tasks reset (state 0,
// task 100). Mutation checks: four deliberately wrong variants of the model must fail the same checks.
#include <cstdio>
#include <cstring>

enum { NUM_ACTIVITIES = 39, MAX_RECORDS = 4, MAX_MEMBERS = 4, MAX_ENTRIES = 40 };

struct Record { float score; const char* holder; };
struct Member { const char* name; long id; const char* description; };
struct ActivityInfo { const char* name; const char* description; };

struct SubEntry { float score; const char* holder; const char* description; long picture_id; };
struct Entry {
    int activity; float best_score; const char* best_holder; const char* description; int record_count;
    SubEntry records[MAX_RECORDS];
};

struct Model {
    // inputs
    bool has_inventory; int open_menu; bool has_screen;
    bool present[NUM_ACTIVITIES]; int record_count[NUM_ACTIVITIES]; Record records[NUM_ACTIVITIES][MAX_RECORDS];
    Member members[MAX_MEMBERS]; int member_count; ActivityInfo info[NUM_ACTIVITIES];
    // state / outputs
    int cache_size; int scoreboard_state; int task_state; bool clan_menu_entry;
    int mouse_colour; float mouse_time; bool mouse_observed; int connecting_state;
    const char* error; int list_offsets; Entry entries[MAX_ENTRIES]; int entry_count; int select_events;
};

static bool IsScoreboardEmpty(const Model& m)
{
    for (int i = 0; i < NUM_ACTIVITIES; ++i)
        if (m.present[i] && m.record_count[i] != 0)
            return false;
    return true;
}

static int FindMember(const Model& m, const char* name)
{
    for (int i = 0; i < m.member_count; ++i)
        if (std::strcmp(m.members[i].name, name) == 0)
            return i;
    return -1;
}

// mutation: 0 = the row; 1 = keeps empty activities; 2 = description without the member test; 3 = no early return when
// empty; 4 = records listed from the second one on.
static void DisplayScoreboardGUI(Model& m, int mutation)
{
    m.cache_size = 0;
    m.scoreboard_state = 0;
    if (!m.has_inventory)
        return;
    if (m.open_menu != 8 || !m.has_screen)
        return;
    m.mouse_colour = 0xffffffff;
    m.mouse_time = 1.0f;
    m.mouse_observed = true;
    m.connecting_state = 0;
    if (IsScoreboardEmpty(m)) {
        m.error = "TEXT_GUI_MENU_CLAN_EMPTY";
        if (mutation != 3)
            return;
    }
    m.list_offsets = NUM_ACTIVITIES;
    m.entry_count = 0;
    for (int i = 0; i < NUM_ACTIVITIES; ++i) {
        if (!m.present[i] || (m.record_count[i] == 0 && mutation != 1))
            continue;
        Entry& e = m.entries[m.entry_count++];
        e.activity = i;
        e.best_score = m.record_count[i] ? m.records[i][0].score : 0.0f;
        e.best_holder = m.record_count[i] ? m.records[i][0].holder : "";
        e.description = 0;
        if (m.record_count[i] && (FindMember(m, e.best_holder) >= 0 || mutation == 2))
            e.description = m.info[i].description;
        e.record_count = 0;
        for (int j = (mutation == 4 ? 1 : 0); j < m.record_count[i]; ++j) {
            SubEntry& s = e.records[e.record_count++];
            int member = FindMember(m, m.records[i][j].holder);
            s.score = m.records[i][j].score;
            s.holder = m.records[i][j].holder;
            s.description = member >= 0 ? m.members[member].description : 0;
            s.picture_id = member >= 0 ? m.members[member].id : -1;
        }
    }
    m.select_events++;
    m.clan_menu_entry = false;
    m.scoreboard_state = 0;
    m.task_state = 100;
}

static void Reset(Model& m)
{
    std::memset(&m, 0, sizeof(m));
    m.has_inventory = true; m.open_menu = 8; m.has_screen = true;
    m.cache_size = 5; m.scoreboard_state = 7; m.task_state = 3; m.clan_menu_entry = true;
    m.connecting_state = 1; m.list_offsets = -1;
    static const char* names[NUM_ACTIVITIES];
    for (int i = 0; i < NUM_ACTIVITIES; ++i) { names[i] = "ACTIVITY"; m.info[i].name = names[i]; m.info[i].description = "DESC"; }
    Member a = { "Alice", 11, "Leader" }; Member b = { "Bob", 22, "Member" };
    m.members[0] = a; m.members[1] = b; m.member_count = 2;
}

static bool Fail(const char* what) { std::printf("check failed: %s\n", what); return false; }

// all checks of one run of the model (mutation 0 must pass every one)
static bool Check(int mutation)
{
    Model m;
    // no inventory / wrong menu / no scoreboard screen: only the cache and the state are reset
    Reset(m); m.has_inventory = false; DisplayScoreboardGUI(m, mutation);
    if (m.cache_size != 0 || m.scoreboard_state != 0 || m.connecting_state != 1 || m.task_state != 3 || !m.clan_menu_entry) return Fail("no inventory");
    Reset(m); m.open_menu = 2; DisplayScoreboardGUI(m, mutation);
    if (m.connecting_state != 1 || m.mouse_observed || m.list_offsets != -1) return Fail("menu");
    Reset(m); m.has_screen = false; DisplayScoreboardGUI(m, mutation);
    if (m.connecting_state != 1 || m.list_offsets != -1) return Fail("screen");
    // empty scoreboard (a present but empty vector counts as empty)
    Reset(m); m.present[4] = true; DisplayScoreboardGUI(m, mutation);
    if (m.error == 0 || std::strcmp(m.error, "TEXT_GUI_MENU_CLAN_EMPTY") != 0 || m.connecting_state != 0 || !m.mouse_observed
        || m.mouse_colour != (int)0xffffffff || m.mouse_time != 1.0f || m.list_offsets != -1 || !m.clan_menu_entry || m.task_state != 3)
        return Fail("empty");
    // two activities with records, one present but empty, one absent
    Reset(m);
    m.present[0] = true; m.record_count[0] = 2;
    m.records[0][0].score = 9.5f; m.records[0][0].holder = "Alice"; m.records[0][1].score = 3.0f; m.records[0][1].holder = "Zed";
    m.present[5] = true; m.record_count[5] = 0;
    m.present[38] = true; m.record_count[38] = 1; m.records[38][0].score = 1.0f; m.records[38][0].holder = "Carl";
    m.info[38].description = "LAST";
    DisplayScoreboardGUI(m, mutation);
    if (m.error != 0 || m.list_offsets != NUM_ACTIVITIES || m.entry_count != 2) return Fail("entries");
    if (m.entries[0].activity != 0 || m.entries[0].best_score != 9.5f || std::strcmp(m.entries[0].best_holder, "Alice") != 0
        || m.entries[0].description == 0 || std::strcmp(m.entries[0].description, "DESC") != 0 || m.entries[0].record_count != 2)
        return Fail("entry 0");
    if (m.entries[0].records[0].picture_id != 11 || std::strcmp(m.entries[0].records[0].description, "Leader") != 0
        || m.entries[0].records[1].picture_id != -1 || m.entries[0].records[1].description != 0 || m.entries[0].records[1].score != 3.0f)
        return Fail("entry 0 records");
    if (m.entries[1].activity != 38 || m.entries[1].description != 0 || m.entries[1].record_count != 1) return Fail("entry 1");
    if (m.select_events != 1 || m.clan_menu_entry || m.scoreboard_state != 0 || m.task_state != 100 || m.cache_size != 0) return Fail("tail");
    return true;
}

int main()
{
    if (!Check(0)) { std::printf("00563920_TEST FAIL\n"); return 1; }
    for (int mutation = 1; mutation <= 4; ++mutation)
        if (Check(mutation)) { std::printf("mutation %d not caught\n00563920_TEST FAIL\n", mutation); return 1; }
    std::printf("00563920_TEST PASS\n");
    return 0;
}
