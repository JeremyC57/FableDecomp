// Behaviour test for 007b8680: body copied from src/007b8680.cpp (CTCTavernTable::GetTankardName), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_strCtors = 0, g_strCopies = 0, g_strDtors = 0;
struct CCharString {
    void* m_data;
    CCharString(const char* text, long length = -1) : m_data((void*)text) { ++g_strCtors; CHECK(length == -1); }
    CCharString(const CCharString& other) : m_data(other.m_data) { ++g_strCopies; }
    ~CCharString() { ++g_strDtors; }
};
static int g_destroyed = 0;
struct CDefPointee {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_count;
};
struct CTavernTableDef : CDefPointee {
    unsigned char _pad_0x08[0x1d];
    bool AddTankards;
    bool HighQualityTankards;
};
struct CDefPointer {
    CTavernTableDef* Object;
    CDefPointer() : Object(0) {}
};
struct CDefReference {
    CTavernTableDef* Object;
    CDefReference(CTavernTableDef* object) : Object(object) {}
    ~CDefReference()
    {
        if (--Object->m_count == 0)
            Object->Destroy();
    }
    CTavernTableDef* operator->() const { return Object; }
};
static CTavernTableDef* g_tableDef = 0; static int g_getSubDef = 0;
struct CDefClassBase {
    bool GetSubDef_007b9700(CDefPointer* out) { ++g_getSubDef; ++g_tableDef->m_count; out->Object = g_tableDef; return true; }
};
struct CThing {
    unsigned char _pad_0x00[0x70];
    CDefClassBase* DefHolder;
};
struct CTCTavernTable {
    virtual void v00() {}
    CThing* ParentThing;
    CTavernTableDef* TakeTableDef()
    {
        CDefPointer out;
        ParentThing->DefHolder->GetSubDef_007b9700(&out);
        return out.Object;
    }
    CCharString GetTankardName();
};
CCharString CTCTavernTable::GetTankardName()
{
    CDefReference def(TakeTableDef());
    return def->HighQualityTankards ? CCharString("OBJECT_TG_TANKARD_01") : CCharString("OBJECT_VILLAGE_TAVERN_TANKARD");
}
int main()
{
    static CThing thing; static CDefClassBase classDef; static CTavernTableDef def; def.m_count = 1; g_tableDef = &def; thing.DefHolder = &classDef;
    CTCTavernTable t; t.ParentThing = &thing;
    def.HighQualityTankards = true;
    { CCharString n = t.GetTankardName(); CHECK(strcmp((const char*)n.m_data, "OBJECT_TG_TANKARD_01") == 0); CHECK(g_getSubDef == 1 && def.m_count == 1 && g_destroyed == 0); }
    def.HighQualityTankards = false;
    { CCharString n = t.GetTankardName(); CHECK(strcmp((const char*)n.m_data, "OBJECT_VILLAGE_TAVERN_TANKARD") == 0); }
    CHECK(g_strCtors == 2 && g_strDtors == g_strCtors + g_strCopies);
    // a def held only by the lookup is destroyed once the name is built
    def.m_count = 0; { CCharString n = t.GetTankardName(); CHECK(g_destroyed == 1 && strcmp((const char*)n.m_data, "OBJECT_VILLAGE_TAVERN_TANKARD") == 0); }
    if (fails == 0) printf("007b8680_TEST PASS\n"); else printf("007b8680_TEST FAIL\n");
    return fails;
}