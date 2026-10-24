// Behaviour test for 00c196a0: object-map Find -- picks the key's scope (its own record's +0xc scope, else the owner's default), then finds by name when the key has one, else by id (type 0) or alternate id (type 1); other types give 0
#include <cstdio>
#include <cstring>

static int g_stringsLive = 0;
struct CString {
    const char* m_text;
    CString(const char* text) : m_text(text) { ++g_stringsLive; }
    ~CString() { --g_stringsLive; }
};

struct CObject { int m_id; };

struct CScope {
    CObject m_objects[3];
    const char* m_names[3];
    int m_lastCall;
    CObject* FindByName(const CString& name, long flags)
    {
        m_lastCall = 1;
        if (flags != 0 || g_stringsLive != 1) return 0;
        for (int i = 0; i < 3; ++i) if (std::strcmp(m_names[i], name.m_text) == 0) return &m_objects[i];
        return 0;
    }
    CObject* FindById(long id) { m_lastCall = 2; return (id >= 0 && id < 3) ? &m_objects[id] : 0; }
    CObject* FindByAltId(long id) { m_lastCall = 3; return (id >= 10 && id < 13) ? &m_objects[id - 10] : 0; }
};

struct SScopeRecord { long m_0, m_4, m_8; CScope m_scope; };   // the scope sits at +0xc of its record

struct SObjectKey {
    long m_0;
    long m_type;
    long m_value;
    const char* m_pName;
    SScopeRecord* m_pScope;
};

struct CObjectMapOwner { CScope m_defaultScope; };

struct CObjectMap {
    CObjectMapOwner* m_pOwner;
    CScope* GetScope(SScopeRecord* record)
    {
        CScope* scope = &m_pOwner->m_defaultScope;
        if (record)
            scope = &record->m_scope;
        return scope;
    }
    CObject* Find(const SObjectKey* key)
    {
        CScope* scope = GetScope(key->m_pScope);
        CObject* result = 0;
        if (key->m_pName) {
            result = scope->FindByName(key->m_pName, 0);
        } else {
            switch (key->m_type) {
            case 0:
                result = scope->FindById(key->m_value);
                break;
            case 1:
                result = scope->FindByAltId(key->m_value);
                break;
            }
        }
        return result;
    }
};

static void Init(CScope& s, const char* a, const char* b, const char* c)
{
    s.m_names[0] = a; s.m_names[1] = b; s.m_names[2] = c; s.m_lastCall = 0;
    for (int i = 0; i < 3; ++i) s.m_objects[i].m_id = i;
}

int main()
{
    CObjectMapOwner owner;
    SScopeRecord record;
    Init(owner.m_defaultScope, "alpha", "beta", "gamma");
    Init(record.m_scope, "one", "two", "three");
    CObjectMap map = { &owner };
    SObjectKey byName = { 0, 7, 0, "beta", 0 };
    if (map.Find(&byName) != &owner.m_defaultScope.m_objects[1] || g_stringsLive != 0) { std::printf("00c196a0_TEST FAIL\n"); return 1; }
    SObjectKey scoped = { 0, 0, 0, "three", &record };
    if (map.Find(&scoped) != &record.m_scope.m_objects[2] || record.m_scope.m_lastCall != 1) { std::printf("00c196a0_TEST FAIL\n"); return 1; }
    SObjectKey byId = { 0, 0, 2, 0, 0 };
    if (map.Find(&byId) != &owner.m_defaultScope.m_objects[2] || owner.m_defaultScope.m_lastCall != 2) { std::printf("00c196a0_TEST FAIL\n"); return 1; }
    SObjectKey byAlt = { 0, 1, 11, 0, &record };
    if (map.Find(&byAlt) != &record.m_scope.m_objects[1] || record.m_scope.m_lastCall != 3) { std::printf("00c196a0_TEST FAIL\n"); return 1; }
    owner.m_defaultScope.m_lastCall = 0;
    SObjectKey other = { 0, 5, 1, 0, 0 };
    if (map.Find(&other) != 0 || owner.m_defaultScope.m_lastCall != 0) { std::printf("00c196a0_TEST FAIL\n"); return 1; }   // unknown type: no finder runs
    std::printf("00c196a0_TEST PASS\n");
    return 0;
}