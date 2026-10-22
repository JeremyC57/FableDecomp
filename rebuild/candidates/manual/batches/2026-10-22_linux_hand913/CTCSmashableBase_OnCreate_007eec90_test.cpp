#include <cstdio>

struct CSubDef { long id; };
struct CDefClassBase {
    long RefCount; int destroyed; CSubDef sub;
    void GetSubDef(CSubDef** out) { *out = &sub; }
    void DestroySelf() { ++destroyed; }
};
struct CDefinitionManager {
    CDefClassBase* table[4];
    bool GetDef(long index, CDefClassBase** out) { *out = table[index]; if (*out) ++(*out)->RefCount; return index != 3 && *out != 0; }
};
static CDefinitionManager g_manager;
struct CThing { long DefIndex; };
struct CTCSmashableBase {
    CThing* ParentThing; long LastWorldFrameHit; bool Smashed; CSubDef* Definition;
    void OnCreate()
    {
        LastWorldFrameHit = 0;
        Smashed = false;
        CDefClassBase* object = 0;
        bool found = g_manager.GetDef(ParentThing->DefIndex, &object);
        if (found)
            object->GetSubDef(&Definition);
        if (object != 0 && --object->RefCount == 0)
            object->DestroySelf();
    }
};
static bool Run()
{
    CDefClassBase d; d.RefCount = 0; d.destroyed = 0; d.sub.id = 5;
    g_manager.table[0] = 0; g_manager.table[1] = &d; g_manager.table[3] = &d;
    CThing t; t.DefIndex = 1;
    CTCSmashableBase s; s.ParentThing = &t; s.LastWorldFrameHit = 9; s.Smashed = true; s.Definition = 0;
    s.OnCreate();
    bool ok = s.LastWorldFrameHit == 0 && !s.Smashed && s.Definition == &d.sub && d.RefCount == 0 && d.destroyed == 1;
    d.RefCount = 1; t.DefIndex = 3; s.Definition = 0; s.OnCreate();
    ok = ok && s.Definition == 0 && d.RefCount == 1 && d.destroyed == 1;
    t.DefIndex = 0; s.OnCreate();
    return ok && s.Definition == 0;
}

int main()
{
    if (!Run()) { std::printf("007eec90_TEST FAIL\n"); return 1; }
    std::printf("007eec90_TEST PASS\n");
    return 0;
}
