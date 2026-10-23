#include <cstdio>
struct CDefPointer { const void* Object; CDefPointer() : Object(0) { } };
static const int g_rodDef = 5;
struct CDefClassBase { bool m_has; bool GetSubDef_007e2390(CDefPointer* out) { if (m_has) out->Object = &g_rodDef; return m_has; } };
struct CThing { CDefClassBase* m_pDefClass; };
struct CTCBase { CThing* ParentThing; bool ToBeRemoved; CTCBase(CThing* parent) : ParentThing(parent), ToBeRemoved(false) { } };
struct CTCFishingRod : CTCBase {
    CDefPointer PDef;
    CTCFishingRod(CThing* parent) : CTCBase(parent), PDef() { ParentThing->m_pDefClass->GetSubDef_007e2390(&PDef); }
};
int main()
{
    CDefClassBase dc; dc.m_has = true; CThing t; t.m_pDefClass = &dc;
    CTCFishingRod r(&t);
    dc.m_has = false; CTCFishingRod none(&t);
    if (r.ParentThing != &t || r.PDef.Object != &g_rodDef || none.PDef.Object != 0) { std::printf("007e2450_TEST FAIL\n"); return 1; }
    std::printf("007e2450_TEST PASS\n");
    return 0;
}