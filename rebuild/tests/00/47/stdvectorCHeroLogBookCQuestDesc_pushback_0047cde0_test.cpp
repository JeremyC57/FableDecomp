// Behaviour test for 0x0047CDE0 vector<CHeroLogBook::CQuestDesc>::push_back (model with a grow stand-in).
#include <cstdio>
#include <new>

static int g_copies = 0, g_overflows = 0;
struct CQuestDesc {
    unsigned long ScriptID;
    long          QuestStatus;
    CQuestDesc() : ScriptID(0), QuestStatus(0) { }
    CQuestDesc(unsigned long id, long s) : ScriptID(id), QuestStatus(s) { }
    CQuestDesc(const CQuestDesc& o) : ScriptID(o.ScriptID), QuestStatus(o.QuestStatus) { ++g_copies; }
};

struct VectorModel {
    CQuestDesc* _M_start;
    CQuestDesc* _M_finish;
    CQuestDesc* _M_end_of_storage;
    void _M_insert_overflow(CQuestDesc* pos, const CQuestDesc& x, unsigned int n, bool atend)
    {
        ++g_overflows;
        (void)pos; (void)n; (void)atend;
        new (_M_finish) CQuestDesc(x);   // the test buffer has slack past end_of_storage
        ++_M_finish;
        ++_M_end_of_storage;
    }
    void push_back(const CQuestDesc& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) CQuestDesc(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, 1, true);
    }
};

int main()
{
    static unsigned char buf[8 * sizeof(CQuestDesc)];
    CQuestDesc* raw = (CQuestDesc*)buf;
    VectorModel v;
    v._M_start = raw; v._M_finish = raw; v._M_end_of_storage = raw + 2;
    v.push_back(CQuestDesc(1, 10));
    v.push_back(CQuestDesc(2, 20));
    v.push_back(CQuestDesc(3, 30));
    int ok = v._M_finish == raw + 3 && raw[0].ScriptID == 1 && raw[2].QuestStatus == 30
          && g_overflows == 1 && g_copies == 3;
    if (!ok) { std::printf("0047cde0_TEST FAIL\n"); return 1; }
    std::printf("0047cde0_TEST PASS\n");
    return 0;
}