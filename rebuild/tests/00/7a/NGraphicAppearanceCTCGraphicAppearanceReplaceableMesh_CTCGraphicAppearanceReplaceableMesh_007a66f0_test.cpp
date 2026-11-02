// Behaviour test for 0x007a66f0: unless the parent thing is dead the component unregisters itself (by its component subobject)
// from the parent's graphic appearance TC; then the def pointer is released (Delete on the last reference), then the bases.
#include <cstdio>
static char g_log[16];
static int g_n = 0;
static void Log(char c) { if (g_n < 15) g_log[g_n++] = c; g_log[g_n] = 0; }
static void Reset() { g_n = 0; g_log[0] = 0; }

struct CDef { unsigned long Ref; void Delete() { Log('D'); } };
struct CDefPointer {
    CDef* Object;
    ~CDefPointer() { if (Object) { CDef* o = Object; if (--o->Ref == 0) o->Delete(); Object = 0; } }
};
struct CAGraphicAppearanceComponent { virtual void OnAppearanceClear() {} ~CAGraphicAppearanceComponent() { Log('C'); } };
struct CTCGraphicAppearance {
    CAGraphicAppearanceComponent* Last;
    void UnregisterComponent(CAGraphicAppearanceComponent& c) { Last = &c; Log('U'); }
};
class CThing { public: CTCGraphicAppearance* PTCGraphicAppearance; unsigned char Dead : 1; bool IsDead() const { return Dead; } };
class CTCBase { public: CThing* ParentThing; virtual ~CTCBase() {} };
struct CMesh : public CTCBase, public CAGraphicAppearanceComponent {
    
    CDefPointer PDef;
    virtual ~CMesh()
    {
        if (!ParentThing->IsDead())
            ParentThing->PTCGraphicAppearance->UnregisterComponent(*this);
    }
};

int main()
{
    CTCGraphicAppearance app; app.Last = 0;
    CThing thing; thing.PTCGraphicAppearance = &app; thing.Dead = 0;
    CDef def; def.Ref = 1;
    CMesh* m = new CMesh; m->ParentThing = &thing; m->PDef.Object = &def;
    
    CAGraphicAppearanceComponent* comp = m;
    CTCBase* b = m;
    Reset(); delete b;
    const char* want = "UDC";
    int ok = app.Last == comp;
    for (int i = 0; want[i] || g_log[i]; ++i) if (want[i] != g_log[i]) ok = 0;
    thing.Dead = 1; def.Ref = 2; app.Last = 0;
    m = new CMesh; m->ParentThing = &thing; m->PDef.Object = &def;
    
    Reset(); delete (CTCBase*)m;
    ok = ok && app.Last == 0 && def.Ref == 1 && g_log[0] == 'C' && g_log[1] == 0;
    if (!ok) { std::printf("007a66f0_TEST FAIL\n"); return 1; }
    std::printf("007a66f0_TEST PASS\n");
    return 0;
}