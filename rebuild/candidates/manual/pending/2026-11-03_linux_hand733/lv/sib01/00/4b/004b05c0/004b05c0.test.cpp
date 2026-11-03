#include <cstdio>
#include <string>
#include <list>
#include <vector>
// Model of CQuestManager::TransferBoasts: the boast count is transferred first ("NumAcceptedBoasts"), then per index i the
// four fields "BoastScriptName<i>", "BoastId<i>", "BoastFailed<i>", "BoastCompleted<i>". Saving walks AcceptedBoasts; loading
// clears TempBoasts and rebuilds one boast per index (failed / completed set only when the loaded flag is true).
struct Boast { std::string name; long id; bool failed, completed; };
struct Store { std::vector<std::string> keys; std::vector<std::string> s; std::vector<long> l; };
static std::string Key(const char* p, int i) { char b[64]; std::sprintf(b, "%s%d", p, i); return b; }
static void Save(const std::list<Boast>& in, Store& st)
{
    st.keys.push_back("NumAcceptedBoasts"); st.l.push_back((long)in.size());
    int i = 0;
    for (std::list<Boast>::const_iterator it = in.begin(); it != in.end(); ++it, ++i) {
        st.keys.push_back(Key("BoastScriptName", i)); st.s.push_back(it->name);
        st.keys.push_back(Key("BoastId", i)); st.l.push_back(it->id);
        st.keys.push_back(Key("BoastFailed", i)); st.l.push_back(it->failed);
        st.keys.push_back(Key("BoastCompleted", i)); st.l.push_back(it->completed);
    }
}
static void Load(const Store& st, std::list<Boast>& temp)
{
    temp.clear();
    size_t li = 0, si = 0; long n = st.l[li++];
    for (int i = 0; i != n; ++i) {
        Boast b; b.failed = b.completed = false;
        b.name = st.s[si++]; b.id = st.l[li++];
        bool failed = st.l[li++] != 0, completed = st.l[li++] != 0;
        if (failed) b.failed = true;
        if (completed) b.completed = true;
        temp.push_back(b);
    }
}
int main()
{
    std::list<Boast> a; Boast b1 = { "BOAST_A", 3, false, true }, b2 = { "BOAST_B", 9, true, false }; a.push_back(b1); a.push_back(b2);
    Store st; Save(a, st);
    bool ok = st.keys.size() == 9 && st.keys[0] == "NumAcceptedBoasts" && st.keys[1] == "BoastScriptName0" && st.keys[8] == "BoastCompleted1";
    std::list<Boast> t; Boast junk = { "X", 1, true, true }; t.push_back(junk);
    Load(st, t);
    ok = ok && t.size() == 2 && t.front().name == "BOAST_A" && t.front().id == 3 && !t.front().failed && t.front().completed
            && t.back().name == "BOAST_B" && t.back().failed && !t.back().completed;
    std::printf(ok ? "004b05c0_TEST PASS\n" : "004b05c0_TEST FAIL\n");
    return ok ? 0 : 1;
}