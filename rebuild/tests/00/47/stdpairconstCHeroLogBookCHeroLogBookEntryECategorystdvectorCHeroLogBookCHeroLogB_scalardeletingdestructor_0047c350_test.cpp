// Behaviour test for 0x0047C350 pair<const ECategory, vector<CHeroLogBookEntry> > scalar deleting destructor (model).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_stringDtors = 0, g_frees = 0;
struct CWideStringModel { int id; ~CWideStringModel() { ++g_stringDtors; } };
struct CHeroLogBookEntry { CWideStringModel Name, AbbreviatedName, Content; };

struct VectorModel {
    CHeroLogBookEntry* _M_start;
    CHeroLogBookEntry* _M_finish;
    CHeroLogBookEntry* _M_end_of_storage;
    ~VectorModel()
    {
        for (CHeroLogBookEntry* p = _M_start; p != _M_finish; ++p)
            p->~CHeroLogBookEntry();
        if (_M_start != 0) { std::free(_M_start); ++g_frees; }
    }
};
struct LogEntryValue { long first; VectorModel second; };

int main()
{
    LogEntryValue* v = new LogEntryValue;
    v->first = 3;
    v->second._M_start = (CHeroLogBookEntry*)std::malloc(4 * sizeof(CHeroLogBookEntry));
    for (int i = 0; i < 2; ++i) new (v->second._M_start + i) CHeroLogBookEntry();
    v->second._M_finish = v->second._M_start + 2;
    v->second._M_end_of_storage = v->second._M_start + 4;
    delete v;
    LogEntryValue* e = new LogEntryValue;
    e->second._M_start = e->second._M_finish = e->second._M_end_of_storage = 0;
    delete e;
    if (g_stringDtors != 6 || g_frees != 1) { std::printf("0047c350_TEST FAIL\n"); return 1; }
    std::printf("0047c350_TEST PASS\n");
    return 0;
}