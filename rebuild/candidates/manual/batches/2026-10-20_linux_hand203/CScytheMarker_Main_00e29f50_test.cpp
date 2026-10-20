// Behaviour test for 0x00e29f50 CScytheMarker::Main
#include <cstdio>
#include <cstring>

static int g_destroys = 0, g_blockDeletes = 0;
struct CCharString { const char* text; CCharString(const char* t, long = -1) : text(t) { } };
struct C3DVector { float X, Y, Z; };
struct CCountedItem { void Destroy() { ++g_destroys; } };
struct SCountBlock { long m_references; void (CCountedItem::*m_pfnDestroy)(); CCountedItem* m_pItem; };
struct CCountedPointer {
    void* Data; SCountBlock* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->m_references; }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) { (Info->m_pItem->*Info->m_pfnDestroy)(); ++g_blockDeletes; }
        }
        Data = 0; Info = 0;
    }
};
struct CScriptThing {
    CCountedPointer PImp; C3DVector pos;
    const C3DVector& GetPos() const { return pos; }
};
static CCountedItem g_item; static SCountBlock g_block;
struct CGameScriptInterface {
    int creates, removes; const char* def; const char* script; const C3DVector* where; bool flag; const CScriptThing* removed; bool ra, rb;
    CScriptThing CreateCreature(const CCharString& d, const C3DVector& p, const CCharString& s, bool b)
    {
        ++creates; def = d.text; script = s.text; where = &p; flag = b;
        g_block.m_references = 0; g_block.m_pfnDestroy = &CCountedItem::Destroy; g_block.m_pItem = &g_item;
        CScriptThing t; t.PImp.Info = &g_block; t.PImp.Data = &g_item; g_block.m_references = 1;
        return t;   // the copy bumps the count, the local's release drops it (with or without NRVO)
    }
    void RemoveThing(const CScriptThing& t, bool a, bool b) { ++removes; removed = &t; ra = a; rb = b; if (g_destroys) removes = 100; }
};
struct CScytheMarker {
    CGameScriptInterface* Game; CScriptThing Me;
    void Main()
    {
        CScriptThing scythe = Game->CreateCreature("CREATURE_RIVAL_HERO_SCYTHE", Me.GetPos(), "ScytheNearOracle", false);
        Game->RemoveThing(Me, false, true);
    }
};

int main()
{
    CGameScriptInterface gsi; gsi.creates = 0; gsi.removes = 0;
    CScytheMarker m; m.Game = &gsi;
    m.Main();
    // the creature handle is released after RemoveThing (removes stays 1), the last reference destroys the item
    if (gsi.creates != 1 || gsi.removes != 1 || std::strcmp(gsi.def, "CREATURE_RIVAL_HERO_SCYTHE") != 0
        || std::strcmp(gsi.script, "ScytheNearOracle") != 0 || gsi.where != &m.Me.pos || gsi.flag || gsi.removed != &m.Me
        || gsi.ra || !gsi.rb || g_destroys != 1 || g_blockDeletes != 1) { std::printf("00e29f50_TEST FAIL\n"); return 1; }
    std::printf("00e29f50_TEST PASS\n");
    return 0;
}
