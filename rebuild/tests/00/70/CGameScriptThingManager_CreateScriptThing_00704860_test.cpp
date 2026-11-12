#include <cstdio>
#include <list>
// Behaviour model of CGameScriptThingManager::CreateScriptThing (0x00704860): find-or-create a script thing by the thing's
// unique id; counted pointers shared between the list entry and the returned wrapper.
struct Block { unsigned long refs; int* deleted; };
struct Obj { int attach; Obj() : attach(0) { } };
struct Ptr {
    Obj* data; Block* info;
    Ptr() : data(0), info(0) { }
    Ptr(const Ptr& o) : data(o.data), info(o.info) { if (info) ++info->refs; }
    ~Ptr() { release(); }
    void release() { if (info) { if (--info->refs == 0) { ++*info->deleted; delete data; delete info; } } data = 0; info = 0; }
    Ptr& operator=(const Ptr& o) { if (info != o.info) { release(); data = o.data; info = o.info; if (info) ++info->refs; } return *this; }
    void Reset(Obj* d, int* counter) { release(); data = d; if (d) { info = new Block; info->refs = 1; info->deleted = counter; } }
};
struct Thing { unsigned __int64 uid; };
struct ScriptThing { Ptr imp; void SetImplementation(Ptr p) { imp = p; } };
struct Info { unsigned __int64 uid; Ptr p; };
static int g_deleted = 0;
struct Manager {
    std::list<Info> things;
    bool Find(const Thing& t, Info** out) {
        for (std::list<Info>::iterator it = things.begin(); it != things.end(); ++it)
            if ((*it).uid == t.uid) { *out = &*it; return true; }
        return false;
    }
    ScriptThing Create(const Thing& t) {
        Info* found;
        if (Find(t, &found)) {
            found->p.data->attach++;
            ScriptThing st; st.SetImplementation(found->p); return st;
        } else {
            ScriptThing st; Info info;
            info.p.Reset(new Obj, &g_deleted);
            info.uid = t.uid;
            things.push_back(info);
            st.SetImplementation(info.p);
            return st;
        }
    }
};
int main()
{
    int ok = 1;
    {
        Manager m;
        Thing a = { 0x100000002ULL }, b = { 0x200000002ULL };
        ScriptThing s1 = m.Create(a);
        if (m.things.size() != 1 || s1.imp.info->refs != 2 || s1.imp.data->attach != 0) ok = 0;
        ScriptThing s2 = m.Create(a);
        if (m.things.size() != 1 || s2.imp.data != s1.imp.data || s1.imp.data->attach != 1 || s1.imp.info->refs != 3) ok = 0;
        ScriptThing s3 = m.Create(b);
        if (m.things.size() != 2 || s3.imp.data == s1.imp.data || m.things.back().uid != b.uid) ok = 0;
    }
    if (g_deleted != 2) ok = 0;
    if (!ok) { std::printf("00704860_TEST FAIL\n"); return 1; }
    std::printf("00704860_TEST PASS\n");
    return 0;
}