// behaviour test for 0x00C101D0 CNamedSet::Find: look an entry up by the name's hash; count a collision when a named entry's name differs
#include <cstdio>
#include <cstring>

struct CString {
    char* m_pchData;
    CString(const char* lpsz) : m_pchData((char*)lpsz) { }   // stand-in for 0x00C0BFE0 (the test keeps literals)
    ~CString() { }                                           // stand-in for 0x00C0BA40
    int Compare(const char* lpsz) const { return std::strcmp(m_pchData, lpsz); }   // 0x00C0BCE0
    unsigned long GetHash() const                            // stand-in for 0x00C0DBB0: first letter only, to force collisions
    {
        return (unsigned char)m_pchData[0];
    }
    operator const char*() const { return m_pchData; }
};

inline bool operator!=(const CString& s1, const CString& s2) { return s1.Compare(s2) != 0; }
inline bool operator!=(const CString& s1, const char* s2) { return s1.Compare(s2) != 0; }
inline bool operator!=(const char* s1, const CString& s2) { return s2.Compare(s1) != 0; }

struct CNamedEntry {
    CString m_name;
    unsigned long m_hash;
    CNamedEntry(const char* name) : m_name(name), m_hash(m_name.GetHash()) { }
    virtual const CString& GetName() { return m_name; }
};

struct CNamedIndex {
    CNamedEntry* m_entries[4];
    int m_count;
    CNamedIndex() : m_count(0) { }
    virtual CNamedEntry* Find(const unsigned long& key)
    {
        for (int i = 0; i < m_count; ++i) if (m_entries[i]->m_hash == key) return m_entries[i];
        return 0;
    }
};

struct CNamedSet {
    CNamedIndex m_index;
    CNamedEntry* Find(const CString& name, long* pCollisions);
};

CNamedEntry* CNamedSet::Find(const CString& name, long* pCollisions)
{
    unsigned long hash = name.GetHash();
    CNamedEntry* entry = m_index.Find(hash);
    if (entry && entry->GetName() != CString("")) {
        if (entry->GetName() != name && pCollisions)
            ++*pCollisions;
    }
    return entry;
}

int main()
{
    CNamedEntry apple("apple"), blank("");
    CNamedSet set;
    set.m_index.m_entries[0] = &apple; set.m_index.m_entries[1] = &blank; set.m_index.m_count = 2;
    long collisions = 0;
    if (set.Find(CString("apple"), &collisions) != &apple || collisions != 0) { std::printf("00c101d0_TEST FAIL\n"); return 1; }
    if (set.Find(CString("avocado"), &collisions) != &apple || collisions != 1) { std::printf("00c101d0_TEST FAIL\n"); return 1; }
    if (set.Find(CString("avocado"), 0) != &apple) { std::printf("00c101d0_TEST FAIL\n"); return 1; }
    if (set.Find(CString("zebra"), &collisions) != 0 || collisions != 1) { std::printf("00c101d0_TEST FAIL\n"); return 1; }
    char empty[] = { 0, 'x', 0 };                                // hash 0 finds the blank entry: no collision counted
    if (set.Find(CString(empty), &collisions) != &blank || collisions != 1) { std::printf("00c101d0_TEST FAIL\n"); return 1; }
    std::printf("00c101d0_TEST PASS\n");
    return 0;
}