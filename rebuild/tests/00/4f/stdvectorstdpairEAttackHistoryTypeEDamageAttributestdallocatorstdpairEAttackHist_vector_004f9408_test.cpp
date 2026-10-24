// Behaviour test for 0x004F9408 std::vector<pair<EAttackHistoryType,EDamageAttribute> >(const vector& x): a fresh block
// of x.size() pairs holding member-wise copies; finish == start + size, end of storage == start + size; empty x gives
// an empty (null) vector.
#include <cstdio>
#include <cstdlib>
#include <new>

enum EAttackHistoryType { AH_A = 1, AH_B = 2 };
enum EDamageAttribute { DA_X = 7, DA_Y = 9 };

struct PairModel {
    EAttackHistoryType first;
    EDamageAttribute second;
    PairModel(EAttackHistoryType a, EDamageAttribute b) : first(a), second(b) { }
    PairModel(const PairModel& o) : first(o.first), second(o.second) { }
};

struct vector_model {
    PairModel* _M_start;
    PairModel* _M_finish;
    PairModel* _M_end_of_storage;
    vector_model() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    vector_model(const vector_model& x)
    {
        unsigned int n = (unsigned int)(x._M_finish - x._M_start);
        _M_start = _M_finish = _M_end_of_storage = 0;
        _M_start = n != 0 ? (PairModel*)std::malloc(n * sizeof(PairModel)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        PairModel* cur = _M_start;
        for (const PairModel* f = x._M_start; f != x._M_finish; ++f, ++cur)
            new (cur) PairModel(*f);
        _M_finish = cur;
    }
};

int main()
{
    vector_model a;
    a._M_start = (PairModel*)std::malloc(3 * sizeof(PairModel));
    new (a._M_start + 0) PairModel(AH_A, DA_X);
    new (a._M_start + 1) PairModel(AH_B, DA_Y);
    new (a._M_start + 2) PairModel(AH_A, DA_Y);
    a._M_finish = a._M_end_of_storage = a._M_start + 3;
    vector_model b(a);
    if (b._M_start == a._M_start || b._M_finish != b._M_start + 3 || b._M_end_of_storage != b._M_start + 3) { std::printf("004f9408_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (b._M_start[i].first != a._M_start[i].first || b._M_start[i].second != a._M_start[i].second) { std::printf("004f9408_TEST FAIL\n"); return 1; }
    vector_model e;
    vector_model c(e);
    if (c._M_start != 0 || c._M_finish != 0 || c._M_end_of_storage != 0) { std::printf("004f9408_TEST FAIL\n"); return 1; }
    std::printf("004f9408_TEST PASS\n");
    return 0;
}