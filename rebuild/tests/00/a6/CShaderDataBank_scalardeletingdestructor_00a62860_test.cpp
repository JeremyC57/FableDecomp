// Behaviour test for the CShaderDataBank deleting destructor (0x00A62860): the shader entries are
// torn down first, the state block and the Shaders storage go next, the CBankFile base last.
#include <cstdio>
#include <cstdlib>

static int g_events[8];
static int g_count = 0;

static void FreeBlock(void* p) { g_events[g_count++] = 3; std::free(p); }

class CBankFile { public:
    virtual ~CBankFile() { g_events[g_count++] = 4; }
    unsigned char _body[0x10c];
};

class CShaderBankEntry;

struct CShaderEntryVector {
    CShaderBankEntry** m_first; CShaderBankEntry** m_last; CShaderBankEntry** m_end;
    CShaderEntryVector() : m_first(0), m_last(0), m_end(0) {}
    ~CShaderEntryVector() { if (m_first) FreeBlock(m_first); }
};

class CBankStateBlock { public:
    virtual ~CBankStateBlock() { g_events[g_count++] = 2; }
};

class CShaderDataBank : public CBankFile { public:
    CShaderEntryVector Shaders;
    CBankStateBlock    StateBlock;
    CShaderDataBank() {}
    void DeleteShaderEntries() { g_events[g_count++] = 1; }
    virtual ~CShaderDataBank()
    {
        DeleteShaderEntries();
    }
};

int main()
{
    CShaderDataBank* bank = new CShaderDataBank();
    bank->Shaders.m_first = (CShaderBankEntry**)std::malloc(8);
    CBankFile* base = bank;
    delete base;
    bool ok = g_count == 4 && g_events[0] == 1 && g_events[1] == 2 && g_events[2] == 3 && g_events[3] == 4;
    CShaderDataBank* empty = new CShaderDataBank();
    delete empty;
    ok = ok && g_count == 7 && g_events[4] == 1 && g_events[5] == 2 && g_events[6] == 4;
    if (!ok) {
        std::printf("00a62860_TEST FAIL\n");
        return 1;
    }
    std::printf("00a62860_TEST PASS\n");
    return 0;
}