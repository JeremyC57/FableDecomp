// Behaviour test for MicroThread::CreateMicroThread (0x009D8710): the new thread's saved stack image is the
// 0x20-byte initial frame (chain head, four zero registers, entry, return thunk, argument).
#include <cstdio>
#include <cstring>
#include <cstdlib>

struct ByteVector
{
    unsigned char* m_start; unsigned char* m_finish; unsigned char* m_end;
    ByteVector() : m_start(0), m_finish(0), m_end(0) {}
    unsigned char* begin() { return m_start; }
    unsigned int size() const { return (unsigned int)(m_finish - m_start); }
    void insert(unsigned char* pos, const unsigned char* first, const unsigned char* last)
    {
        unsigned int n = (unsigned int)(last - first), at = (unsigned int)(pos - m_start), old = size();
        unsigned char* p = (unsigned char*)std::malloc(old + n);
        if (old) std::memcpy(p, m_start, at);
        std::memcpy(p + at, first, n);
        if (old) std::memcpy(p + at + n, m_start + at, old - at);
        std::free(m_start);
        m_start = p; m_finish = p + old + n; m_end = m_finish;
    }
};

struct CDummyExceptionBlock { void* Next; void* Handler; };
CDummyExceptionBlock DummyExceptionBlock = { (void*)-1, 0 };
static int g_returned = 0;
void MicroThreadReturn() { ++g_returned; }

class MicroThreadInfo
{
public:
    MicroThreadInfo() : Dead(false) {}
    virtual ~MicroThreadInfo() {}
    bool Dead;
};
class MicroThreadDetails : public MicroThreadInfo
{
public:
    ByteVector Stack;
};

struct SInitialStackFrame
{
    CDummyExceptionBlock* ExceptionChain;
    unsigned int Edi, Esi, Ebp, Ebx;
    void (*Entry)(void*);
    void (*Return)();
    void* Argument;
};

MicroThreadInfo* CreateMicroThread(void (*entry)(void*), void* argument)
{
    MicroThreadDetails* details = new MicroThreadDetails;
    SInitialStackFrame frame;
    frame.ExceptionChain = &DummyExceptionBlock;
    frame.Edi = 0; frame.Esi = 0; frame.Ebp = 0; frame.Ebx = 0;
    frame.Entry = entry;
    frame.Return = MicroThreadReturn;
    frame.Argument = argument;
    details->Stack.insert(details->Stack.begin(), (unsigned char*)&frame, (unsigned char*)(&frame + 1));
    return details;
}

static void Entry(void*) {}

int main()
{
    int cookie = 7;
    MicroThreadDetails* d = static_cast<MicroThreadDetails*>(CreateMicroThread(Entry, &cookie));
    if (d->Dead || d->Stack.size() != sizeof(SInitialStackFrame)) { std::printf("009d8710_TEST FAIL\n"); return 1; }
    SInitialStackFrame copy;
    std::memcpy(&copy, d->Stack.begin(), sizeof(copy));
    if (copy.ExceptionChain != &DummyExceptionBlock || copy.Edi || copy.Esi || copy.Ebp || copy.Ebx) { std::printf("009d8710_TEST FAIL\n"); return 1; }
    if (copy.Entry != Entry || copy.Return != MicroThreadReturn || copy.Argument != &cookie) { std::printf("009d8710_TEST FAIL\n"); return 1; }
    std::free(d->Stack.m_start);
    delete d;
    std::printf("009d8710_TEST PASS\n");
    return 0;
}