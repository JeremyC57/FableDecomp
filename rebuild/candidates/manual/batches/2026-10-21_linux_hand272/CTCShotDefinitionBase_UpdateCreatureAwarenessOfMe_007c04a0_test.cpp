// Behaviour test for CTCShotDefinitionBase::UpdateCreatureAwarenessOfMe (0x007c04a0): search box, shooter exclusion, first hearing AI creature plays a random ARROW_FLY_BY sound once.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#endif
struct C3DVector { float X, Y, Z; };
struct C2DBoxI {
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(long minX, long minY, long maxX, long maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) {}
};
struct CCharString {
    void* m_data;
    CCharString(const char* text, long length = -1) { m_data = (void*)text; (void)length; }
    ~CCharString() {}
};
static unsigned long g_lastRange; unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed) { g_lastRange = range; *seed += 1; return range - 1; }
struct CSoundInit {
    float Volume, Pitch; long a, b, c, d, e, f;
    CSoundInit() : Volume(1), Pitch(1), a(0), b(1), c(1), d(0), e(0), f(-1) {}
};
struct CSoundGroup { unsigned char* first; unsigned char* last; };
struct CSoundDef {
    virtual void v00() {}
    virtual void Destroy() {}
    long RefCount;
    unsigned char _pad08[0x20];
    struct CMap { CSoundGroup* Group; CSoundGroup* Find(const CCharString& name) { return strcmp((const char*)name.m_data, "ARROW_FLY_BY") == 0 ? Group : 0; } } Sounds;
};
struct CSoundDefPtr {
    CSoundDef* Object;
    CSoundDefPtr() : Object(0) {}
    ~CSoundDefPtr() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
};
struct CDefClassBase { bool GetSubDef(CSoundDefPtr* out); };   // 0x0065F604
static CSoundDef* g_def; CSoundDef* FindRef_CDefClassBase_GetSubDef(CDefClassBase*) { if (g_def) ++g_def->RefCount; return g_def; }
// Stand-in body (the real 0x0065F604 sits in this TU out of line): it only writes the def through the out parameter, so
// VC7.1 knows the caller's def pointer does not escape the call (retail keeps the def in edi across the later calls).
bool CDefClassBase::GetSubDef(CSoundDefPtr* out)
{
    CSoundDef* def = FindRef_CDefClassBase_GetSubDef(this);
    out->Object = def;
    return def != 0;
}

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last; SEntry* end;
    SEntry* LowerBound(const long& key) { SEntry* it = first; while (it != last && it->type < key) ++it; return it; }
};
static int g_plays; static unsigned char g_lastSound; struct CTCAI { void PlaySound(const unsigned char& sound, const CSoundInit& init, long a, long b) { ++g_plays; g_lastSound = sound; (void)init; (void)a; (void)b; } };
struct CThing {
    virtual void v00() {}
    virtual void v01() {}
    virtual void v02() {}
    virtual void v03() {}
    virtual void v04() {}
    virtual void v05() {}
    virtual void v06() {}
    virtual void v07() {}
    virtual void v08() {}
    virtual void v09() {}
    virtual void v10() {}
    virtual void v11() {}
    virtual void v12() {}
    virtual void v13() {}
    virtual void v14() {}
    virtual void v15() {}
    virtual void v16() {}
    virtual void v17() {}
    virtual void v18() {}
    virtual void v19() {}
    virtual void v20() {}
    virtual void v21() {}
    virtual void v22() {}
    virtual void v23() {}
    virtual void v24() {}
    virtual void v25() {}
    virtual void v26() {}
    virtual void v27() {}
    virtual void v28() {}
    virtual void v29() {}
    virtual void v30() {}
    virtual void v31() {}
    virtual void v32() {}
    virtual void v33() {}
    virtual void v34() {}
    virtual void v35() {}
    virtual void v36() {}
    virtual void v37() {}
    virtual void v38() {}
    virtual void v39() {}
    virtual void v40() {}
    virtual void v41() {}
    virtual void v42() {}
    virtual void v43() {}
    virtual void v44() {}
    virtual void v45() {}
    virtual void v46() {}
    virtual void v47() {}
    virtual void v48() {}
    virtual void v49() {}
    virtual void v50() {}
    virtual void v51() {}
    virtual void v52() {}
    virtual void v53() {}
    virtual void v54() {}
    virtual void v55() {}
    virtual void v56() {}
    virtual void v57() {}
    virtual void v58() {}
    virtual void v59() {}
    virtual void v60() {}
    virtual void v61() {}
    virtual void v62() {}
    virtual void v63() {}
    virtual void v64() {}
    virtual void v65() {}
    virtual void v66() {}
    virtual void v67() {}
    virtual void v68() {}
    virtual void v69() {}
    virtual void v70() {}
    virtual void v71() {}
    virtual void v72() {}
    virtual void v73() {}
    virtual void v74() {}
    virtual void v75() {}
    virtual void v76() {}
    virtual bool CanHear() { return Hears; }
    unsigned char _pad04[0x1c];
    unsigned long TCBits[4];                        // +0x20
    unsigned char _pad30[0x14];
    CInterfaceMap Interfaces;                       // +0x44
    unsigned char _pad50[0x20];
    CDefClassBase* DefClass;                        // +0x70
    unsigned char _pad74[0x1d];
    unsigned char Dead : 1;                         // +0x91 bit 0
    C3DVector Pos; bool Hears;
    const C3DVector& GetPosition() const { return Pos; }
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (TCBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &Interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (T*)(it == map->last || type < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingVector {
    CThing** _M_start; CThing** _M_finish; CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingVector() { }
};
struct CBaseIntelligentPointer {
    void* __vftable; void* m_pItem; long m_link;
    CThing* GetPItem() const { return (CThing*)m_pItem; }
};
struct CThingFilter_IsNot { const CThing* Thing; };
struct CThingFilter_All { CThingFilter_All() {} };
struct CThingSearchTools {
    CThing** Things; int Count; C2DBoxI LastArea; int Calls;
    CThingSearchTools() : LastArea(0, 0, 0, 0) {}
    void Fill(const C2DBoxI& area, const CThing* skip, CThingVector& out)
    {
        LastArea = area; ++Calls;
        static CThing* buf[16]; int n = 0;
        for (int i = 0; i < Count; ++i) if (Things[i] != skip) buf[n++] = Things[i];
        out._M_start = buf; out._M_finish = buf + n; out._M_end_of_storage = buf + n;
    }
    bool GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_IsNot& filter, CThingVector& out) { Fill(area, filter.Thing, out); return true; }
    bool GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_All& filter, CThingVector& out) { (void)filter; Fill(area, 0, out); return true; }
};
struct CWorld { unsigned long Seed; unsigned long* GetRandomSeed() { return &Seed; } };
struct CTCShotDefinitionBase {
    void*   __vftable;
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x20];
    CBaseIntelligentPointer PShooter;               // +0x28
    unsigned char _pad34[0x31];
    bool    MadeSound;                              // +0x65
    CThingSearchTools* Tools; CWorld* World;
    CThingSearchTools* GetThingSearchTools() const { return Tools; }
    CWorld* GetWorld() { return World; }
    CDefClassBase* GetOwnerDefClass() const { return ParentThing->DefClass; }
    void UpdateCreatureAwarenessOfMe();
};

void CTCShotDefinitionBase::UpdateCreatureAwarenessOfMe()
{
    CThingVector things;
    const C3DVector& pos = ParentThing->GetPosition();
    long x = (long)pos.X;
    long y = (long)pos.Y;
    C2DBoxI area(x - 5, y - 5, x + 5, y + 5);
    if (PShooter.GetPItem() && !PShooter.GetPItem()->Dead)
    {
        CThingFilter_IsNot filter;
        filter.Thing = PShooter.GetPItem();
        GetThingSearchTools()->GetAllThingsInRoughArea(area, filter, things);
    }
    else
        GetThingSearchTools()->GetAllThingsInRoughArea(area, CThingFilter_All(), things);
    for (CThing** it = things._M_start; it != things._M_finish; ++it)
    {
        CThing* thing = *it;
        if (thing && !thing->Dead && !MadeSound && thing->CanHear())
        {
            CSoundDefPtr def;
            CTCAI* ai;
            if (thing->GetInterface(0x10, &ai) && GetOwnerDefClass()->GetSubDef(&def))
            {
                CSoundGroup* group;
                {
                    CCharString name("ARROW_FLY_BY");
                    group = def.Object->Sounds.Find(name);
                }
                if (group)
                {
                    ai->PlaySound(group->first[GFRandom(group->last - group->first, GetWorld()->GetRandomSeed())], CSoundInit(), 0, 0);
                    MadeSound = true;
                }
            }
        }
    }
}

static void InitThing(CThing& t, float x, float y)
{
    memset(&t.TCBits, 0, sizeof(t.TCBits)); t.Dead = 0; t.Pos.X = x; t.Pos.Y = y; t.Pos.Z = 0; t.Hears = true;
    t.Interfaces.first = t.Interfaces.last = t.Interfaces.end = 0; t.DefClass = 0;
}
int main()
{
    int fails = 0;
    static CThing owner, shooter, deaf, listener;
    InitThing(owner, 12.7f, -3.2f); InitThing(shooter, 12, -3); InitThing(deaf, 13, -2); InitThing(listener, 11, -4);
    deaf.Hears = false;
    static CTCAI ai; static CTCAI shooterAi;
    CInterfaceMap::SEntry e1[1] = { { 0x10, (CTCBase*)&ai } };
    CInterfaceMap::SEntry e2[1] = { { 0x10, (CTCBase*)&shooterAi } };
    listener.TCBits[0] |= 1UL << 16; listener.Interfaces.first = e1; listener.Interfaces.last = e1 + 1;
    shooter.TCBits[0] |= 1UL << 16; shooter.Interfaces.first = e2; shooter.Interfaces.last = e2 + 1;
    deaf.TCBits[0] |= 1UL << 16; deaf.Interfaces.first = e1; deaf.Interfaces.last = e1 + 1;
    unsigned char sounds[3] = { 7, 8, 9 };
    CSoundGroup group; group.first = sounds; group.last = sounds + 3;
    static CSoundDef def; def.RefCount = 1; def.Sounds.Group = &group; g_def = &def;
    CDefClassBase defClass; owner.DefClass = &defClass;
    CThing* list[4] = { &shooter, &deaf, &listener, 0 };
    CThingSearchTools tools; tools.Things = list; tools.Count = 4; tools.Calls = 0;
    CWorld world; world.Seed = 0;
    CTCShotDefinitionBase shot; shot.ParentThing = &owner; shot.Tools = &tools; shot.World = &world; shot.MadeSound = false;
    shot.PShooter.m_pItem = &shooter;
    shot.UpdateCreatureAwarenessOfMe();
    if (tools.LastArea.MinX != 7 || tools.LastArea.MinY != -8 || tools.LastArea.MaxX != 17 || tools.LastArea.MaxY != 2) ++fails;
    if (g_plays != 1 || g_lastSound != 9 || g_lastRange != 3 || !shot.MadeSound) ++fails;
    if (def.RefCount != 1) ++fails;
    shot.UpdateCreatureAwarenessOfMe();                       // already made its sound
    if (g_plays != 1) ++fails;
    shot.MadeSound = false; shooter.Dead = 1;                 // dead shooter: unfiltered search, shooter skipped as dead
    shot.UpdateCreatureAwarenessOfMe();
    if (g_plays != 2 || tools.Calls != 3) ++fails;
    shot.MadeSound = false; g_def = 0;                        // no sound def: nothing plays
    shot.UpdateCreatureAwarenessOfMe();
    if (g_plays != 2 || shot.MadeSound) ++fails;
    if (fails) { printf("007c04a0_TEST FAIL %d\n", fails); return 1; }
    printf("007c04a0_TEST PASS\n");
    return 0;
}
