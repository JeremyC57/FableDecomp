#include <cstdio>
#include <new>
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);               // 0x0099EC30
};
class CHeroLogBook {
public:
    class CQuestObjectiveDesc {
    public:
        unsigned long SubObjectiveID;                    // +0x00
        CCharString   TextDescription;                   // +0x04
        bool          QuestObjectiveCompleted;           // +0x08
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
template CHeroLogBook::CQuestObjectiveDesc* __fastcall __uninitialized_copy<const CHeroLogBook::CQuestObjectiveDesc*, CHeroLogBook::CQuestObjectiveDesc*>(const CHeroLogBook::CQuestObjectiveDesc*, const CHeroLogBook::CQuestObjectiveDesc*, CHeroLogBook::CQuestObjectiveDesc*, const __false_type&);
}

static int g_copies = 0;
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
typedef CHeroLogBook::CQuestObjectiveDesc Desc;
int main()
{
    static unsigned long raw[9] = { 1, 0x100, 1, 2, 0x200, 0, 3, 0x300, 1 };
    static unsigned long out[12];
    const CHeroLogBook::CQuestObjectiveDesc* s = (const CHeroLogBook::CQuestObjectiveDesc*)raw;
    Desc* d = (Desc*)out;
    Desc* e = std::__uninitialized_copy(s, s + 3, d, std::__false_type());
    bool ok = e == d + 3 && g_copies == 3 && d[0].SubObjectiveID == 1 && d[1].SubObjectiveID == 2 && d[2].SubObjectiveID == 3
        && d[0].TextDescription.PStringData == (CCharStringData*)0x100 && d[2].TextDescription.PStringData == (CCharStringData*)0x300
        && d[0].QuestObjectiveCompleted && !d[1].QuestObjectiveCompleted && d[2].QuestObjectiveCompleted && out[9] == 0;
    ok = ok && std::__uninitialized_copy(s, s, d, std::__false_type()) == d && g_copies == 3;
    if (ok) std::printf("0047b830_TEST PASS\n");
    return 0;
}