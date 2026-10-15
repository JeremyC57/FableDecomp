// Behaviour test for 0x009038D0 (CScriptGameResourceObjectScriptedThing::GetScriptThing): the live
// thing's handle, or a null handle when there is no thing or it is dead.
#include <cstdio>

struct CScriptThing {
    void* PImp_Data;
    CScriptThing() : PImp_Data(0) { }
    explicit CScriptThing(void* imp) : PImp_Data(imp) { }
    CScriptThing(const CScriptThing& other) : PImp_Data(other.PImp_Data) { }
};

struct CThing_Retail {
    unsigned char m_dead;
    CScriptThing GetScriptThing() { return CScriptThing(this); }
};
struct CIntelligentPointer_Retail {
    CThing_Retail* m_pItem;
    CThing_Retail* GetPItem() { return m_pItem; }
};
struct CScriptGameResourceObjectScriptedThing {
    CIntelligentPointer_Retail PThing;
    CScriptThing GetScriptThing();
};

CScriptThing CScriptGameResourceObjectScriptedThing::GetScriptThing()
{
    CIntelligentPointer_Retail* thing = &PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead)
            return thing->GetPItem()->GetScriptThing();
    }
    return CScriptThing();
}

int main()
{
    CThing_Retail t;
    t.m_dead = 0;
    CScriptGameResourceObjectScriptedThing s;
    s.PThing.m_pItem = &t;
    if (s.GetScriptThing().PImp_Data != &t) { std::printf("009038d0_TEST FAIL\n"); return 1; }
    t.m_dead = 1;
    if (s.GetScriptThing().PImp_Data != 0) { std::printf("009038d0_TEST FAIL\n"); return 1; }
    s.PThing.m_pItem = 0;
    if (s.GetScriptThing().PImp_Data != 0) { std::printf("009038d0_TEST FAIL\n"); return 1; }
    std::printf("009038d0_TEST PASS\n");
    return 0;
}