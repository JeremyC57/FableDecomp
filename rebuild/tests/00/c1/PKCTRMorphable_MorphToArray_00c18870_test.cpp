// Behaviour test for 0x00c18870 PKCTRMorphable::MorphToArray: a copy of the logic over fake containers.
#include <cstdio>
#include <cstring>

static int g_reports = 0, g_breaks = 0, g_deleted = 0;
static char g_last[256];
struct CDebugStream {
    char m_buf[256];
    char* m_pBuffer;
    CDebugStream(char*) : m_pBuffer(m_buf) { m_buf[0] = 0; }
    CDebugStream& operator<<(const char* text) { std::strcat(m_buf, text); return *this; }
    CDebugStream& operator<<(long value) { char n[16]; std::sprintf(n, "%ld", value); std::strcat(m_buf, n); return *this; }
};
struct CDebugBreak { CDebugBreak() { g_breaks++; } };
static char* g_pDebugBuffer = 0;
static void Report(const char* text) { g_reports++; std::strcpy(g_last, text); }
static void (*g_pfnErrorReport)(const char*) = Report;

class PKCContainer {
public:
    void* items[8]; int count;
    PKCContainer() : count(0) {}
    virtual int IsArray() = 0;
    virtual int IsTree() = 0;
    virtual void Reserve(long) {}
    virtual void Add(void*, void* item) { items[count++] = item; }
    virtual void Remove(void*, void* item)
    {
        for (int i = 0; i < count; i++)
            if (items[i] == item) { for (int j = i; j + 1 < count; j++) items[j] = items[j + 1]; count--; return; }
    }
    virtual long GetCount() { return count; }
    virtual void* GetFirst() { return count ? items[0] : 0; }
    virtual ~PKCContainer() { g_deleted++; }
};
class PKCTRArray : public PKCContainer { public: PKCTRArray(unsigned int) {} int IsArray() { return 1; } int IsTree() { return 0; } };
class PKCTRRedblackTree : public PKCContainer { public: PKCTRRedblackTree(void*) {} int IsArray() { return 0; } int IsTree() { return 1; } };

static void MoveAllItems(void* owner, PKCContainer* source, PKCContainer* dest)
{
    dest->Reserve(source->GetCount() + dest->GetCount());
    for (void* item = source->GetFirst(); item; item = source->GetFirst()) {
        source->Remove(owner, item);
        dest->Add(owner, item);
    }
}

struct PKCTRMorphable {
    PKCContainer* m_pImplementation;
    void MorphToArray(void* comparator)
    {
        if (m_pImplementation == 0)
        {
            {
                CDebugStream s(g_pDebugBuffer);
                s << ".\\PKCTRMorphable.cpp" << "(" << 50 << ") : " << "Error: NULL implimentation" << "\n";
                if (g_pfnErrorReport)
                    g_pfnErrorReport(s.m_pBuffer);
            }
            CDebugBreak();
            return;   /* the retail body goes on to call through the null pointer; the test stops here */
        }
        if (!m_pImplementation->IsArray())
        {
            PKCContainer* implementation = new PKCTRArray(0);
            if (implementation == 0)
            {
                CDebugStream s(g_pDebugBuffer);
                s << ".\\PKCTRMorphable.cpp" << "(" << 54 << ") : " << "Error: EMEM" << "\n";
            }
            MoveAllItems(comparator, m_pImplementation, implementation);
            delete m_pImplementation;
            m_pImplementation = implementation;
        }
    }
};

int main()
{
    int a = 1, b = 2, c = 3;
    PKCTRMorphable m;
    m.m_pImplementation = new PKCTRRedblackTree(0);
    m.m_pImplementation->Add(0, &a); m.m_pImplementation->Add(0, &b); m.m_pImplementation->Add(0, &c);
    PKCContainer* old = m.m_pImplementation;
    m.MorphToArray(0);
    int fail = 0;
    if (m.m_pImplementation == old || !m.m_pImplementation->IsArray() || g_deleted != 1) fail |= 1;
    if (m.m_pImplementation->GetCount() != 3 || m.m_pImplementation->items[0] != &a || m.m_pImplementation->items[2] != &c) fail |= 2;
    PKCContainer* now = m.m_pImplementation;
    m.MorphToArray(0);                       /* already the right kind: unchanged */
    if (m.m_pImplementation != now || g_deleted != 1) fail |= 4;
    PKCTRMorphable e; e.m_pImplementation = 0;
    e.MorphToArray(0);
    if (g_reports != 1 || g_breaks != 1 || std::strcmp(g_last, ".\\PKCTRMorphable.cpp(50) : Error: NULL implimentation\n") != 0) fail |= 8;
    delete m.m_pImplementation;
    if (fail) { std::printf("00c18870_TEST FAIL %d\n", fail); return 1; }
    std::printf("00c18870_TEST PASS\n");
    return 0;
}