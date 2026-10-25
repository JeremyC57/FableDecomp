// Behaviour test for 00c3ab50: CIndexMap::Add -- logs "Index <n> already exists!" for a duplicate index, otherwise inserts a new (value, index) node
#include <cstdio>
#include <cstring>

struct CDebugStream {
    char m_text[128];
    CDebugStream& operator<<(const char* text) { std::strcat(m_text, text); return *this; }
    CDebugStream& operator<<(unsigned long value) { char b[16]; std::sprintf(b, "%lu", value); std::strcat(m_text, b); return *this; }
};
struct CMessageLog {
    CDebugStream m_stream;
    int m_messages;
    CDebugStream* NewMessage() { ++m_messages; m_stream.m_text[0] = 0; return &m_stream; }
};

struct SIndexNode {
    void* m_pValue;
    unsigned long m_index;
    SIndexNode(void* value, unsigned long index) : m_pValue(value), m_index(index) { }
};

class CIndexMap {
public:
    SIndexNode* m_nodes[8];
    int m_count;
    void Insert(SIndexNode* node) { m_nodes[m_count++] = node; }
    SIndexNode* Lookup(const unsigned long* index)
    {
        for (int i = 0; i < m_count; ++i)
            if (m_nodes[i]->m_index == *index) return m_nodes[i];
        return 0;
    }
    void Add(CMessageLog* log, void* value, unsigned long index)
    {
        if (Lookup(&index)) {
            *log->NewMessage() << "Index " << index << " already exists!";
            return;
        }
        Insert(new SIndexNode(value, index));
    }
};

static int fail() { std::printf("00c3ab50_TEST FAIL\n"); return 1; }

int main()
{
    int a, b;
    CIndexMap map; map.m_count = 0;
    CMessageLog log; log.m_messages = 0;
    map.Add(&log, &a, 5);
    map.Add(&log, &b, 6);
    if (map.m_count != 2 || log.m_messages != 0 || map.m_nodes[1]->m_pValue != &b) return fail();
    map.Add(&log, &b, 5);
    if (map.m_count != 2 || log.m_messages != 1 || std::strcmp(log.m_stream.m_text, "Index 5 already exists!") != 0) return fail();
    delete map.m_nodes[0]; delete map.m_nodes[1];
    std::printf("00c3ab50_TEST PASS\n");
    return 0;
}