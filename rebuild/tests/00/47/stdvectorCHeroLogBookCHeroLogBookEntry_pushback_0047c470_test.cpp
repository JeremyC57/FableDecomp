// Behaviour test for 0x0047C470 vector<CHeroLogBook::CHeroLogBookEntry>::push_back (model with a grow stand-in).
#include <cstdio>
#include <new>

static int g_copies = 0, g_overflows = 0;
struct CHeroLogBookEntry {
    unsigned long Name;
    long          Content;
    CHeroLogBookEntry() : Name(0), Content(0) { }
    CHeroLogBookEntry(unsigned long id, long s) : Name(id), Content(s) { }
    CHeroLogBookEntry(const CHeroLogBookEntry& o) : Name(o.Name), Content(o.Content) { ++g_copies; }
};

struct VectorModel {
    CHeroLogBookEntry* _M_start;
    CHeroLogBookEntry* _M_finish;
    CHeroLogBookEntry* _M_end_of_storage;
    void _M_insert_overflow(CHeroLogBookEntry* pos, const CHeroLogBookEntry& x, unsigned int n, bool atend)
    {
        ++g_overflows;
        (void)pos; (void)n; (void)atend;
        new (_M_finish) CHeroLogBookEntry(x);   // the test buffer has slack past end_of_storage
        ++_M_finish;
        ++_M_end_of_storage;
    }
    void push_back(const CHeroLogBookEntry& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) CHeroLogBookEntry(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, 1, true);
    }
};

int main()
{
    static unsigned char buf[8 * sizeof(CHeroLogBookEntry)];
    CHeroLogBookEntry* raw = (CHeroLogBookEntry*)buf;
    VectorModel v;
    v._M_start = raw; v._M_finish = raw; v._M_end_of_storage = raw + 2;
    v.push_back(CHeroLogBookEntry(1, 10));
    v.push_back(CHeroLogBookEntry(2, 20));
    v.push_back(CHeroLogBookEntry(3, 30));
    int ok = v._M_finish == raw + 3 && raw[0].Name == 1 && raw[2].Content == 30
          && g_overflows == 1 && g_copies == 3;
    if (!ok) { std::printf("0047c470_TEST FAIL\n"); return 1; }
    std::printf("0047c470_TEST PASS\n");
    return 0;
}