#include <cstdio>
#include <new>
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);               // 0x0099EC30
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    vector(const vector& other);                         // 0x0047BA50 for CQuestObjectiveDesc
};
}
class CHeroLogBook {
public:
    class CQuestObjectiveDesc;
    enum EQuestStatus { QUEST_STATUS_NONE };
    class CQuestDesc {
    public:
        unsigned long                          ScriptID;             // +0x00
        CCharString                            TextDescription;      // +0x04
        EQuestStatus                           QuestStatus;          // +0x08
        std::vector<CQuestObjectiveDesc>       QuestSubObjectives;   // +0x0c
    };
};
namespace std {
struct __false_type {};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class InputIter, class ForwardIter>
ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template CHeroLogBook::CQuestDesc* __fastcall __uninitialized_copy<CHeroLogBook::CQuestDesc*, CHeroLogBook::CQuestDesc*>(CHeroLogBook::CQuestDesc*, CHeroLogBook::CQuestDesc*, CHeroLogBook::CQuestDesc*, const __false_type&);
}


static int g_str, g_vec;
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_str; }
template <> std::vector<CHeroLogBook::CQuestObjectiveDesc>::vector(const vector& other) : _M_start(other._M_start), _M_finish(other._M_finish), _M_end_of_storage(0) { ++g_vec; }
typedef CHeroLogBook::CQuestDesc Desc;
int main()
{
    static unsigned long raw[12] = { 1, 0x10, 2, 0xa, 0xb, 0xc, 3, 0x30, 4, 0xd, 0xe, 0xf };
    static unsigned long out[15];
    Desc* s = (Desc*)raw; Desc* d = (Desc*)out;
    Desc* e = std::__uninitialized_copy(s, s + 2, d, std::__false_type());
    bool ok = e == d + 2 && g_str == 2 && g_vec == 2 && out[0] == 1 && out[1] == 0x10 && out[2] == 2 && out[3] == 0xa && out[4] == 0xb
        && out[5] == 0 && out[6] == 3 && out[8] == 4 && out[9] == 0xd && out[12] == 0;
    ok = ok && std::__uninitialized_copy(s, s, d, std::__false_type()) == d && g_str == 2;
    if (ok) std::printf("0047c290_TEST PASS\n");
    return 0;
}