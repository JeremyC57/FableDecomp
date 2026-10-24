// Behaviour test for 0076c750 CParticleUpdateParameters<unsigned long,unsigned long>::Update (copy of the model;
// the STLport sort is a working insertion-sort stand-in that counts its calls).
#include <cstdio>
struct C3DVector { float X, Y, Z; };
template <class T1, class T2> struct pair { T1 first; T2 second; };
static int g_sorts = 0;
template <class RandomIt, class Compare> void sort(RandomIt first, RandomIt last, Compare comp)
{
    ++g_sorts;
    for (RandomIt i = first; i != last; ++i)
        for (RandomIt j = i; j != first && comp(*j, *(j - 1)); --j) { typename Compare::value_type t = *j; *j = *(j - 1); *(j - 1) = t; }
}
template <class T> struct CAllocProxy { T* _M_data; };
template <class T> struct CVector {
    T* _M_start; T* _M_finish; CAllocProxy<T> _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
};
template <class K, class V, class C> struct CVectorMap {
    CVector<pair<K, V> > Data;
    C                    Compare;
    bool                 Dirty;
    void Sort()
    {
        if (Dirty) {
            sort(Data.begin(), Data.end(), Compare);
            Dirty = false;
        }
    }
};
template <class V> struct CCompareKey {
    typedef pair<unsigned long, V> value_type;
    bool operator()(const value_type& a, const value_type& b) const { return a.first < b.first; }
};
struct CParticleUpdateParamDesc { void* vt; void* Callback; unsigned long Index; };
struct CParticleUpdateParameters {
    void* vt;
    CVectorMap<unsigned long, C3DVector, CCompareKey<C3DVector> >                               VectorParams;
    CVectorMap<unsigned long, CParticleUpdateParamDesc, CCompareKey<CParticleUpdateParamDesc> > UpdateParams;
    void Update();
};
void CParticleUpdateParameters::Update()
{
    VectorParams.Sort();
    UpdateParams.Sort();
}
int main()
{
    pair<unsigned long, C3DVector> v[3];
    v[0].first = 30; v[1].first = 10; v[2].first = 20;
    pair<unsigned long, CParticleUpdateParamDesc> d[2];
    d[0].first = 5; d[1].first = 1;
    CParticleUpdateParameters p;
    p.VectorParams.Data._M_start = v; p.VectorParams.Data._M_finish = v + 3; p.VectorParams.Dirty = true;
    p.UpdateParams.Data._M_start = d; p.UpdateParams.Data._M_finish = d + 2; p.UpdateParams.Dirty = false;
    int ok = 1;
    p.Update();
    if (g_sorts != 1 || p.VectorParams.Dirty || v[0].first != 10 || v[2].first != 30 || d[0].first != 5) ok = 0;
    p.UpdateParams.Dirty = true;
    p.Update();
    if (g_sorts != 2 || p.UpdateParams.Dirty || d[0].first != 1 || d[1].first != 5) ok = 0;
    p.Update();
    if (g_sorts != 2) ok = 0;                                  // both clean: nothing sorted
    if (!ok) { std::printf("0076c750_TEST FAIL\n"); return 1; }
    std::printf("0076c750_TEST PASS\n");
    return 0;
}