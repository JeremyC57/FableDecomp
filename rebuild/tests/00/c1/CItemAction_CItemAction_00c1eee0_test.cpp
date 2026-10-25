// Behaviour test for 00c1eee0: CItemAction::CItemAction(dispatcher, name, named) -- bind to the dispatcher's item for the name, else create a named (keeps the name) or hashed item from the name's hash and attach it
#include <cstdio>
#include <cstring>

static int g_live = 0;
struct CString {
    const char* m_pchData;
    CString(const char* s) : m_pchData(s) { ++g_live; }
    CString(const CString& o) : m_pchData(o.m_pchData) { ++g_live; }
    ~CString() { --g_live; }
    unsigned int GetHash() const { unsigned int h = 0; for (const char* p = m_pchData; *p; ++p) h = h * 31 + (unsigned char)*p; return h; }
};

struct CItemDispatcher;
static int g_attached = 0;
struct CItem {
    int m_kind;
    virtual ~CItem() { }
    virtual void Attach(CItemDispatcher*) { ++g_attached; }
};
struct CNamedItem : CItem {
    unsigned int m_hash; CString m_name;
    CNamedItem(const CString& name, unsigned int hash) : m_hash(hash), m_name(name) { m_kind = 3; }
};
struct CHashedItem : CItem {
    unsigned int m_hash;
    CHashedItem(unsigned int hash) : m_hash(hash) { m_kind = 2; }
};
struct CItemDispatcher {
    const char* m_known; CItem* m_item; long m_lastCollisionArg;
    CItem* Find(const CString& name, long* pCollisions) { m_lastCollisionArg = pCollisions ? 1 : 0; return std::strcmp(name.m_pchData, m_known) == 0 ? m_item : 0; }
};

struct CItemAction {
    CItemDispatcher* m_pDispatcher;
    CItem* m_pItem;
    CItemAction(CItemDispatcher* dispatcher, CString name, bool named)
        : m_pDispatcher(dispatcher), m_pItem(dispatcher->Find(name, 0))
    {
        if (!m_pItem) {
            if (named)
                m_pItem = new CNamedItem(name, name.GetHash());
            else
                m_pItem = new CHashedItem(name.GetHash());
            m_pItem->Attach(m_pDispatcher);
        }
    }
};

int main()
{
    CItem known; known.m_kind = 1;
    CItemDispatcher d; d.m_known = "door"; d.m_item = &known; d.m_lastCollisionArg = -1;
    {
        CItemAction a(&d, CString("door"), true);
        if (a.m_pItem != &known || g_attached != 0 || d.m_lastCollisionArg != 0) { std::printf("00c1eee0_TEST FAIL\n"); return 1; }
    }
    if (g_live != 0) { std::printf("00c1eee0_TEST FAIL\n"); return 1; }   // the by-value name is destroyed
    CItemAction b(&d, CString("gate"), true);
    CNamedItem* n = static_cast<CNamedItem*>(b.m_pItem);
    if (n->m_kind != 3 || n->m_hash != CString("gate").GetHash() || std::strcmp(n->m_name.m_pchData, "gate") != 0 || g_attached != 1) {
        std::printf("00c1eee0_TEST FAIL\n"); return 1;
    }
    CItemAction c(&d, CString("gate"), false);
    CHashedItem* h = static_cast<CHashedItem*>(c.m_pItem);
    if (h->m_kind != 2 || h->m_hash != n->m_hash || g_attached != 2) { std::printf("00c1eee0_TEST FAIL\n"); return 1; }
    delete n; delete h;
    if (g_live != 0) { std::printf("00c1eee0_TEST FAIL\n"); return 1; }
    std::printf("00c1eee0_TEST PASS\n");
    return 0;
}