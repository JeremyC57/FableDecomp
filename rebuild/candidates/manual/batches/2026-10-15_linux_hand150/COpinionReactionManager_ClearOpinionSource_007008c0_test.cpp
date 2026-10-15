// behaviour test for 0x007008C0 COpinionReactionManager::ClearOpinionSource
#include <cstdio>
struct CTCOpinionSource {
    int depth;
    bool PopSourceDef() { if (!depth) return false; --depth; return true; }
    bool HasNoSourceDefs() const { return depth == 0; }
};
struct CThing {
    CTCOpinionSource* tc; int removed;
    unsigned int HasTC(long) const { return tc != 0; }
    CTCOpinionSource* GetTC(long) { return tc; }
    void RemoveTC(long) { tc = 0; ++removed; }
};
struct CIntelligentPointer { void* item; void* GetPItem() const { return item; } void SetPItem(void* p) { item = p; } };
struct COpinionReactionManager {
    CIntelligentPointer ExclusiveOpinionSource;
    void ClearOpinionSource(CThing* thing, bool popOneDef)
    {
        if (thing->HasTC(0x50)) {
            CTCOpinionSource* source = thing->GetTC(0x50);
            if (!popOneDef || (source->PopSourceDef(), source->HasNoSourceDefs()))
                thing->RemoveTC(0x50);
        }
        if (thing == ExclusiveOpinionSource.GetPItem())
            ExclusiveOpinionSource.SetPItem(0);
    }
};
int main()
{
    CTCOpinionSource src = { 2 };
    CThing t = { &src, 0 };
    COpinionReactionManager m; m.ExclusiveOpinionSource.item = &t;
    m.ClearOpinionSource(&t, true);
    int bad = src.depth != 1 || t.removed != 0 || m.ExclusiveOpinionSource.item != 0;
    m.ExclusiveOpinionSource.item = &src;
    m.ClearOpinionSource(&t, true);
    bad |= src.depth != 0 || t.removed != 1 || t.tc != 0 || m.ExclusiveOpinionSource.item != &src;
    CTCOpinionSource s2 = { 5 }; CThing u = { &s2, 0 };
    m.ClearOpinionSource(&u, false);
    bad |= s2.depth != 5 || u.removed != 1;
    m.ClearOpinionSource(&u, true);
    bad |= u.removed != 1;
    if (bad) { std::printf("007008c0_TEST FAIL\n"); return 1; }
    std::printf("007008c0_TEST PASS\n");
    return 0;
}
