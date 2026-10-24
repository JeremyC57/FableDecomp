// Behaviour test for 0x0047C420 vector<CHeroLogBook::CQuestObjectiveDesc>::push_back (model with a grow stand-in).
#include <cstdio>
#include <new>

static int g_copies = 0, g_overflows = 0;
struct CQuestObjectiveDesc {
    unsigned long SubObjectiveID;
    long QuestObjectiveCompleted;
    CQuestObjectiveDesc() : SubObjectiveID(0), QuestObjectiveCompleted(0) { }
    CQuestObjectiveDesc(unsigned long id, long s) : SubObjectiveID(id), QuestObjectiveCompleted(s) { }
    CQuestObjectiveDesc(const CQuestObjectiveDesc& o) : SubObjectiveID(o.SubObjectiveID), QuestObjectiveCompleted(o.QuestObjectiveCompleted) { ++g_copies; }
};

struct VectorModel {
    CQuestObjectiveDesc* _M_start;
    CQuestObjectiveDesc* _M_finish;
    CQuestObjectiveDesc* _M_end_of_storage;
    void _M_insert_overflow(CQuestObjectiveDesc* pos, const CQuestObjectiveDesc& x, unsigned int n, bool atend)
    {
        ++g_overflows;
        (void)pos; (void)n; (void)atend;
        new (_M_finish) CQuestObjectiveDesc(x);   // the test buffer has slack past end_of_storage
        ++_M_finish;
        ++_M_end_of_storage;
    }
    void push_back(const CQuestObjectiveDesc& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) CQuestObjectiveDesc(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, 1, true);
    }
};

int main()
{
    static unsigned char buf[8 * sizeof(CQuestObjectiveDesc)];
    CQuestObjectiveDesc* raw = (CQuestObjectiveDesc*)buf;
    VectorModel v;
    v._M_start = raw; v._M_finish = raw; v._M_end_of_storage = raw + 2;
    v.push_back(CQuestObjectiveDesc(1, 10));
    v.push_back(CQuestObjectiveDesc(2, 20));
    v.push_back(CQuestObjectiveDesc(3, 30));
    int ok = v._M_finish == raw + 3 && raw[0].SubObjectiveID == 1 && raw[2].QuestObjectiveCompleted == 30
          && g_overflows == 1 && g_copies == 3;
    if (!ok) { std::printf("0047c420_TEST FAIL\n"); return 1; }
    std::printf("0047c420_TEST PASS\n");
    return 0;
}