#include <cstdio>
#include <cstring>
// Behaviour test for CTCMeshAutomaticEntityCreator::AddOwnedEntityTC @ 0x0072DD60
static char g_name[64]; static bool g_activate = true; static void* g_params = (void*)1; static int g_live = 0;
class CCharString {
public:
    CCharString(const char* text, long length = -1) : m_pText(text), m_length(length) { ++g_live; }
    ~CCharString() { --g_live; }
    const char* m_pText; long m_length;
};
class CThing;
static CThing* g_owner = 0;
class CTCBase { public: void SetPOwner(CThing* owner) { g_owner = owner; if (g_live != 0) g_owner = 0; } };
class CTCParamsBase;
static CTCBase g_component;
class CThing {
public:
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params)
    { std::strcpy(g_name, name.m_pText); g_activate = activate; g_params = params; return name.m_length == -1 ? &g_component : 0; }
};
class CTCMeshAutomaticEntityCreator {
public:
    void* m_vtable; CThing* m_pThing;
    void AddOwnedEntityTC(CThing* created)
    {
        CTCBase* owned = created->AddTC("CTCOwnedEntity", false, 0);
        owned->SetPOwner(m_pThing);
    }
};
int main()
{
    CThing owner, created; CTCMeshAutomaticEntityCreator creator; creator.m_vtable = 0; creator.m_pThing = &owner;
    creator.AddOwnedEntityTC(&created);
    if (std::strcmp(g_name, "CTCOwnedEntity") != 0 || g_activate || g_params != 0 || g_owner != &owner) { std::printf("0072dd60_TEST FAIL\n"); return 1; }
    std::printf("0072dd60_TEST PASS\n");
    return 0;
}