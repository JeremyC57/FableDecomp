// behaviour test for 0x007D9D70 vector<KnickKnack>::_M_fill_insert (STLport 4.6 insert(pos, n, x)): both in-capacity
// branches (elems_after > n and elems_after <= n) and the regrow branch through a stand-in _M_insert_overflow
#include <cstdio>
#include <new>

class C3DVector { public: float X, Y, Z; };
struct KnickKnack { C3DVector Pos; int Thing; };

static KnickKnack* uninit_copy(KnickKnack* first, KnickKnack* last, KnickKnack* result)
{
    for (; first != last; ++first, ++result) new (result) KnickKnack(*first);
    return result;
}
static KnickKnack* copy_back(KnickKnack* first, KnickKnack* last, KnickKnack* result)
{
    for (int n = (int)(last - first); n > 0; --n) *--result = *--last;
    return result;
}
static void fill_range(KnickKnack* first, KnickKnack* last, const KnickKnack& v)
{
    for (; first != last; ++first) *first = v;
}

struct KVector {
    KnickKnack* _M_start;
    KnickKnack* _M_finish;
    KnickKnack* _M_end_of_storage;
    int overflowCalls;
    void _M_insert_overflow(KnickKnack* pos, const KnickKnack& x, unsigned int n)
    {
        ++overflowCalls;
        unsigned int size = (unsigned int)(_M_finish - _M_start);
        KnickKnack* mem = (KnickKnack*)operator new((size + n) * sizeof(KnickKnack));
        KnickKnack* f = uninit_copy(_M_start, pos, mem);
        for (unsigned int i = 0; i < n; ++i, ++f) new (f) KnickKnack(x);
        f = uninit_copy(pos, _M_finish, f);
        operator delete(_M_start);
        _M_start = mem; _M_finish = f; _M_end_of_storage = mem + size + n;
    }
    void _M_fill_insert(KnickKnack* position, unsigned int n, const KnickKnack& x)
    {
        if (n != 0) {
            if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
                KnickKnack x_copy = x;
                const unsigned int elems_after = (unsigned int)(_M_finish - position);
                KnickKnack* old_finish = _M_finish;
                if (elems_after > n) {
                    uninit_copy(_M_finish - n, _M_finish, _M_finish);
                    _M_finish += n;
                    copy_back(position, old_finish - n, old_finish);
                    fill_range(position, position + n, x_copy);
                } else {
                    KnickKnack* cur = _M_finish;
                    for (unsigned int k = n - elems_after; k > 0; --k, ++cur) new (cur) KnickKnack(x_copy);
                    _M_finish += n - elems_after;
                    uninit_copy(position, old_finish, _M_finish);
                    _M_finish += elems_after;
                    fill_range(position, old_finish, x_copy);
                }
            } else
                _M_insert_overflow(position, x, n);
        }
    }
};

static KnickKnack K(int id) { KnickKnack k; k.Pos.X = (float)id; k.Pos.Y = 0.0f; k.Pos.Z = 0.0f; k.Thing = id; return k; }

static bool Check(const KVector& v, const int* ids, int count)
{
    if (v._M_finish - v._M_start != count) return false;
    for (int i = 0; i < count; ++i)
        if (v._M_start[i].Thing != ids[i] || v._M_start[i].Pos.X != (float)ids[i]) return false;
    return true;
}

int main()
{
    KVector v;
    v._M_start = (KnickKnack*)operator new(10 * sizeof(KnickKnack));
    v._M_end_of_storage = v._M_start + 10;
    v._M_finish = v._M_start;
    v.overflowCalls = 0;
    for (int i = 1; i <= 5; ++i) new (v._M_finish++) KnickKnack(K(i));
    v._M_fill_insert(v._M_start + 1, 2, K(9));            // elems_after 4 > 2
    int e1[] = { 1, 9, 9, 2, 3, 4, 5 };
    bool ok = Check(v, e1, 7);
    v._M_fill_insert(v._M_start + 6, 3, K(7));            // elems_after 1 <= 3
    int e2[] = { 1, 9, 9, 2, 3, 4, 7, 7, 7, 5 };
    ok = ok && Check(v, e2, 10) && v.overflowCalls == 0;
    v._M_fill_insert(v._M_start, 1, K(8));                // no room: regrow
    int e3[] = { 8, 1, 9, 9, 2, 3, 4, 7, 7, 7, 5 };
    ok = ok && Check(v, e3, 11) && v.overflowCalls == 1;
    v._M_fill_insert(v._M_start, 0, K(6));                // n == 0: no change
    ok = ok && Check(v, e3, 11) && v.overflowCalls == 1;
    operator delete(v._M_start);
    if (!ok) { std::printf("007d9d70_TEST FAIL\n"); return 1; }
    std::printf("007d9d70_TEST PASS\n");
    return 0;
}