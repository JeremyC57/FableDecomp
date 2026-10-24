// Behaviour test for QuadricOptimiserInternals::CIndexList::GetUnion(const CIndexList&) const @ 0x00ACA2B0
// Returns a new index list holding the indices present in either list (STLport set_union into an inserter).
#include <cstdio>
#include <set>

typedef std::set<unsigned int> TIndexSet;

struct CSetInserter {
    TIndexSet* container;
    TIndexSet::iterator iter;
    CSetInserter(TIndexSet& c, TIndexSet::iterator i) : container(&c), iter(i) { }
    CSetInserter& operator=(const unsigned int& value) { iter = container->insert(iter, value); ++iter; return *this; }
    CSetInserter& operator*() { return *this; }
    CSetInserter& operator++() { return *this; }
};
typedef TIndexSet::const_iterator TIter;

static TIndexSet Make(const unsigned int* values, unsigned int n)
{
    TIndexSet s;
    for (unsigned int i = 0; i < n; ++i) s.insert(values[i]);
    return s;
}

static int Equals(const TIndexSet& s, const unsigned int* values, unsigned int n)
{
    if (s.size() != n) return 0;
    unsigned int i = 0;
    for (TIter it = s.begin(); it != s.end(); ++it, ++i) if (*it != values[i]) return 0;
    return 1;
}

static CSetInserter SetUnion(TIter first1, TIter last1, TIter first2, TIter last2, CSetInserter result)
{
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            *result = *first1;
            ++first1;
        }
        else if (*first2 < *first1) {
            *result = *first2;
            ++first2;
        }
        else {
            *result = *first1;
            ++first1;
            ++first2;
        }
        ++result;
    }
    for ( ; first1 != last1; ++first1, ++result) *result = *first1;
    for ( ; first2 != last2; ++first2, ++result) *result = *first2;
    return result;
}

struct CIndexListModel {
    TIndexSet Indices;
    CIndexListModel GetUnion(const CIndexListModel& other) const
    {
        CIndexListModel result;
        SetUnion(Indices.begin(), Indices.end(), other.Indices.begin(), other.Indices.end(),
                 CSetInserter(result.Indices, result.Indices.begin()));
        return result;
    }
};

int main()
{
    static const unsigned int a[] = { 1, 3, 4, 7, 9, 12 };
    static const unsigned int b[] = { 0, 3, 7, 8, 12, 15 };
    static const unsigned int either[] = { 0, 1, 3, 4, 7, 8, 9, 12, 15 };
    CIndexListModel la, lb, empty;
    la.Indices = Make(a, 6);
    lb.Indices = Make(b, 6);
    int ok = Equals(la.GetUnion(lb).Indices, either, 9) && Equals(lb.GetUnion(la).Indices, either, 9);
    if (!Equals(la.GetUnion(empty).Indices, a, 6) || !Equals(empty.GetUnion(lb).Indices, b, 6)) ok = 0;
    if (!empty.GetUnion(empty).Indices.empty()) ok = 0;
    if (!ok) { std::printf("00aca2b0_TEST FAIL\n"); return 1; }
    std::printf("00aca2b0_TEST PASS\n");
    return 0;
}