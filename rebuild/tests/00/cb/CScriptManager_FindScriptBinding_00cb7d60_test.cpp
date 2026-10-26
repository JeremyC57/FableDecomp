// Behaviour test for 0x00CB7D60 (CScriptManager::FindScriptBinding): a sorted vector map is searched
// with LowerBound; found only when the lower bound exists and the key does not sort before it.
#include <cstdio>
#include <cstring>

class CCharString { public:
    const char* m_pData;
    bool FastCompare(const CCharString& other) const { return std::strcmp(m_pData, other.m_pData) < 0; }
};

struct SScriptBindingPair {
    CCharString first;
    void*       second_Data;
    void*       second_Info;
};

struct CScriptBindingMap {
    SScriptBindingPair* m_first;
    SScriptBindingPair* m_last;
    SScriptBindingPair* m_end;
    const SScriptBindingPair* LowerBound(const CCharString& key) const
    {
        const SScriptBindingPair* p = m_first;
        while (p != m_last && p->first.FastCompare(key))
            ++p;
        return p;
    }
    const SScriptBindingPair* end() const { return m_last; }
    const SScriptBindingPair* Find(const CCharString& key) const
    {
        const SScriptBindingPair* it = LowerBound(key);
        const SScriptBindingPair* last = end();
        return (it == last || key.FastCompare(it->first)) ? last : it;
    }
};

struct CScriptManager {
    CScriptBindingMap m_bindings;
    long FindScriptBinding(const CCharString& name) const;
};

long CScriptManager::FindScriptBinding(const CCharString& name) const
{
    return m_bindings.Find(name) != m_bindings.end();
}

int main()
{
    SScriptBindingPair entries[3];
    entries[0].first.m_pData = "Alpha";
    entries[1].first.m_pData = "Gamma";
    entries[2].first.m_pData = "Omega";
    CScriptManager mgr;
    mgr.m_bindings.m_first = entries;
    mgr.m_bindings.m_last = entries + 3;
    mgr.m_bindings.m_end = entries + 3;
    CCharString a = { "Gamma" }, b = { "Beta" }, c = { "Zeta" }, d = { "Omega" };
    if (mgr.FindScriptBinding(a) != 1 || mgr.FindScriptBinding(b) != 0 || mgr.FindScriptBinding(c) != 0 || mgr.FindScriptBinding(d) != 1) {
        std::printf("00cb7d60_TEST FAIL\n");
        return 1;
    }
    mgr.m_bindings.m_last = entries;
    if (mgr.FindScriptBinding(a) != 0) {
        std::printf("00cb7d60_TEST FAIL\n");
        return 1;
    }
    std::printf("00cb7d60_TEST PASS\n");
    return 0;
}