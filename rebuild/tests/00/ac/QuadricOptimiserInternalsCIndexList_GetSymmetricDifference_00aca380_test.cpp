// Behaviour test for QuadricOptimiserInternals::CIndexList::GetSymmetricDifference(const CIndexList&) const @ 0x00ACA380
// Returns a new index list holding the indices present in exactly one of the lists (STLport set_symmetric_difference).
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

static CSetInserter SetSymmetricDifference(TIter first1, TIter last1, TIter first2, TIter last2, CSetInserter result)
{
    while (first1 != last1 && first2 != last2)
        if (*first1 < *first2) {
            *result = *first1;
            ++first1;
            ++result;
        }
        else if (*first2 < *first1) {
            *result = *first2;
            ++first2;
            ++result;
        }
        else {
            ++first1;
            ++first2;
        }
    for ( ; first1 != last1; ++first1, ++result) *result = *first1;
    for ( ; first2 != last2; ++first2, ++result) *result = *first2;
    return result;
}

struct CIndexListModel {
    TIndexSet Indices;
    CIndexListModel GetSymmetricDifference(const CIndexListModel& other) const
    {
        CIndexListModel result;
        SetSymmetricDifference(Indices.begin(), Indices.end(), other.Indices.begin(), other.Indices.end(),
                               CSetInserter(result.Indices, result.Indices.begin()));
        return result;
    }
};

int main()
{
    static const unsigned int a[] = { 1, 3, 4, 7, 9, 12 };
    static const unsigned int b[] = { 0, 3, 7, 8, 12, 15 };
    static const unsigned int one[] = { 0, 1, 4, 8, 9, 15 };
    CIndexListModel la, lb, empty;
    la.Indices = Make(a, 6);
    lb.Indices = Make(b, 6);
    int ok = Equals(la.GetSymmetricDifference(lb).Indices, one, 6) && Equals(lb.GetSymmetricDifference(la).Indices, one, 6);
    if (!la.GetSymmetricDifference(la).Indices.empty() || !Equals(la.GetSymmetricDifference(empty).Indices, a, 6)) ok = 0;
    if (!Equals(empty.GetSymmetricDifference(lb).Indices, b, 6)) ok = 0;
    if (!ok) { std::printf("00aca380_TEST FAIL\n"); return 1; }
    std::printf("00aca380_TEST PASS\n");
    return 0;
}