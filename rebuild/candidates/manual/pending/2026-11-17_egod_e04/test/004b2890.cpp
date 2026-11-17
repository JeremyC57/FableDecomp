// Behaviour test for CQuestManager::PostBoastTransfer (0x004B2890) with its same-TU helper GetAcceptedBoastsForScript (0x004B1960): each
// transferred boast that matches a known boast (id + script) marks that boast failed / completed as the transferred copy says, moves it to
// the accepted list (removed from the known ones) and, if it is neither completed nor failed, posts a "boast made" message pointing at the
// accepted copy; then, if the main player's creature exists and is alive, the first active quest that is between its start and end
// screens and has accepted boasts gets its name into the hero's quest-completion UI (when the hero has one) plus a "leaving quest start
// screen" message, and the scan stops there.
#include <cstdio>
#include <cstring>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
struct Boast { long id; std::string script; bool failed, completed; };
struct Msg { int type; const Boast* extra; };
struct Player { bool dead; bool has_ui; std::string ui_name; };
struct QM {
    std::list<Boast> temp, known, accepted; std::vector<std::string> active, between; std::vector<Msg> msgs; Player* player;
    Boast* GetBoast(long id, const std::string& s) { for (std::list<Boast>::iterator i = known.begin(); i != known.end(); ++i) if (i->id == id && i->script == s) return &*i; return 0; }
    void RemoveBoast(long id, const std::string& s) { for (std::list<Boast>::iterator i = known.begin(); i != known.end(); ++i) if (i->id == id && i->script == s) { known.erase(i); return; } }
    void GetAcceptedBoastsForScript(const std::string& name, std::list<Boast>& boasts) const
    {
        boasts.clear();
        for (std::list<Boast>::const_iterator it = accepted.begin(); it != accepted.end(); ++it)
            if (it->script == name) boasts.push_back(*it);
    }
    void PostBoastTransfer()
    {
        for (std::list<Boast>::iterator it = temp.begin(); it != temp.end(); ++it) {
            Boast* pboast = GetBoast(it->id, it->script);
            if (pboast) {
                if (it->failed) pboast->failed = true;
                if (it->completed) pboast->completed = true;
                accepted.push_back(*pboast);
                long id = pboast->id; std::string sc = pboast->script; bool c = pboast->completed, f = pboast->failed;
                RemoveBoast(id, sc);
                if (!c && !f) { Msg m = { 41, &accepted.back() }; msgs.push_back(m); }
            }
        }
        if (player && !player->dead) {
            for (size_t q = 0; q < active.size(); ++q) {
                if (std::find(between.begin(), between.end(), active[q]) == between.end()) continue;
                std::list<Boast> boasts;
                GetAcceptedBoastsForScript(active[q], boasts);
                if (!boasts.empty()) {
                    if (player->has_ui) { player->ui_name = active[q]; Msg m = { 73, 0 }; msgs.push_back(m); }
                    break;
                }
            }
        }
    }
};
static int fail(int n) { std::printf("004b2890_TEST FAIL %d\n", n); return 1; }
static Boast B(long id, const char* s, bool f, bool c) { Boast b = { id, s, f, c }; return b; }
int main()
{
    Player p = { false, true, "" };
    QM qm; qm.player = &p;
    qm.known.push_back(B(1, "QA", false, false)); qm.known.push_back(B(2, "QB", false, false)); qm.known.push_back(B(3, "QB", false, false));
    qm.temp.push_back(B(1, "QA", false, false));      // open: message
    qm.temp.push_back(B(2, "QB", true, false));       // failed: no message
    qm.temp.push_back(B(3, "QB", false, true));       // completed: no message
    qm.temp.push_back(B(9, "QZ", false, false));      // unknown: ignored
    qm.active.push_back("QC"); qm.active.push_back("QB"); qm.active.push_back("QA");
    qm.between.push_back("QA"); qm.between.push_back("QB"); qm.between.push_back("QC");
    qm.PostBoastTransfer();
    if (qm.accepted.size() != 3 || !qm.known.empty()) return fail(1);
    std::list<Boast>::iterator a = qm.accepted.begin();
    if (a->id != 1 || a->failed || a->completed) return fail(2);
    ++a; if (!a->failed || a->completed) return fail(3);
    ++a; if (!a->completed || a->failed) return fail(4);
    if (qm.msgs.size() != 2 || qm.msgs[0].type != 41 || qm.msgs[0].extra != &qm.accepted.front()) return fail(5);
    if (p.ui_name != "QB" || qm.msgs[1].type != 73) return fail(6);   // QC has no boasts; QB is first with boasts; stop there
    Player d = { true, true, "" }; QM q2 = qm; q2.player = &d; q2.temp.clear(); q2.msgs.clear(); q2.PostBoastTransfer();
    if (!d.ui_name.empty() || !q2.msgs.empty()) return fail(7);                // dead player
    QM q3 = qm; q3.player = 0; q3.temp.clear(); q3.msgs.clear(); q3.PostBoastTransfer();
    if (!q3.msgs.empty()) return fail(8);                                       // no player
    Player n = { false, false, "" }; QM q4 = qm; q4.player = &n; q4.temp.clear(); q4.msgs.clear(); q4.PostBoastTransfer();
    if (!q4.msgs.empty() || !n.ui_name.empty()) return fail(9);                // no UI component: nothing, still stops
    Player r = { false, true, "" }; QM q5 = qm; q5.player = &r; q5.temp.clear(); q5.msgs.clear(); q5.between.clear(); q5.between.push_back("QA");
    q5.PostBoastTransfer();
    if (r.ui_name != "QA") return fail(10);                                     // QB not between screens: skipped
    std::list<Boast> pre; pre.push_back(B(77, "QX", false, false));
    qm.GetAcceptedBoastsForScript("QA", pre);                                   // helper clears the out list first
    if (pre.size() != 1 || pre.front().id != 1) return fail(11);
    std::printf("004b2890_TEST PASS\n");
    return 0;
}
