#include <cstdio>
// CModeDragonFlying::OnDeactivate model.
struct CTCFlagged { unsigned char flag; };
struct CThing {
    unsigned char type; bool notWillable; bool liftable; int resumes; bool hasComponent; CTCFlagged component;
    void SetSuspendAnimation(bool s, void*) { if (!s) ++resumes; }
    bool GetInterface(CTCFlagged** out, long) { if (!hasComponent) return false; *out = &component; return true; }
};
static void SetThingAsNotBloodyWillableAtAll(CThing* t, bool v) { t->notWillable = v; }
static void SetThingIsAbleToBeLifted(CThing* t, bool v) { t->liftable = v; }
struct CModeDragonFlying {
    CThing* ParentThing; bool Active;
    void OnDeactivate()
    {
        SetThingAsNotBloodyWillableAtAll(ParentThing, false);
        SetThingIsAbleToBeLifted(ParentThing, true);
        if (ParentThing->type == 1)
            ParentThing->SetSuspendAnimation(false, this);
        Active = false;
        CTCFlagged* tc;
        if (ParentThing->GetInterface(&tc, 0x3a))
            tc->flag = 1;
    }
};
int main()
{
    CThing t; t.type = 1; t.notWillable = true; t.liftable = false; t.resumes = 0; t.hasComponent = true; t.component.flag = 0;
    CModeDragonFlying m; m.ParentThing = &t; m.Active = true;
    m.OnDeactivate();
    CThing u; u.type = 0; u.notWillable = true; u.liftable = false; u.resumes = 0; u.hasComponent = false; u.component.flag = 0;
    CModeDragonFlying n; n.ParentThing = &u; n.Active = true;
    n.OnDeactivate();
    if (!(!t.notWillable && t.liftable && t.resumes == 1 && !m.Active && t.component.flag == 1 && u.resumes == 0 && u.component.flag == 0)) { std::printf("008dc590_TEST FAIL\n"); return 1; }
    std::printf("008dc590_TEST PASS\n");
    return 0;
}
