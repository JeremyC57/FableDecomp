// Behaviour test for 00af3080 CParticleUpdateParameters<unsigned long,unsigned long>::GetUpdateParam (copy of the
// model including the LowerBound body; the callback records its arguments).
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class T> struct CAllocProxy { T* _M_data; };
template <class T> struct CVector { T* _M_start; T* _M_finish; CAllocProxy<T> _M_end_of_storage; };
template <class K, class V> class CVectorMap { public:
    typedef pair<K, V> value_type;
    typedef const value_type* const_iterator;
    CVector<value_type> Data;
    char                Compare;
    bool                Dirty;
    const_iterator LowerBound(const K& key) const
    {
        const_iterator first = Data._M_start;
        int len = Data._M_finish - first;
        while (len > 0) {
            int half = len / 2;
            const_iterator middle = first + half;
            if (middle->first < key) { first = middle + 1; len = len - half - 1; }
            else len = half;
        }
        return first;
    }
    const_iterator end() const { return Data._M_finish; }
    const_iterator find(const K& key) const
    {
        const_iterator i = LowerBound(key);
        return (i == end() || key < i->first) ? end() : i;
    }
};
struct CParticleUpdateParameters;
static int g_calls = 0;
static unsigned long g_index = 0, g_depth = 0;
static float g_scale = 0;
static bool g_flag = false;
static const CParticleUpdateParameters* g_params = 0;
struct CParticleUpdateParamCallback {
    virtual ~CParticleUpdateParamCallback() {}
    virtual bool GetUpdateParam(C3DVector& result, unsigned long index, float timeScale, bool flag,
                                const CParticleUpdateParameters& params, unsigned long depth)
    {
        ++g_calls; g_index = index; g_scale = timeScale; g_flag = flag; g_params = &params; g_depth = depth;
        result.X = (float)index; return true;
    }
};
struct CParticleUpdateParamDesc {
    void* vt;
    CParticleUpdateParamCallback* Callback;
    unsigned long Index;
    bool GetUpdateParam(C3DVector& result, float timeScale, bool flag, const CParticleUpdateParameters& params, unsigned long depth) const
    {
        return Callback->GetUpdateParam(result, Index, timeScale, flag, params, depth);
    }
};
struct CParticleUpdateParameters {
    void* vt;
    CVectorMap<unsigned long, C3DVector>                VectorParams;
    CVectorMap<unsigned long, CParticleUpdateParamDesc> UpdateParams;
    bool GetUpdateParam(C3DVector& result, const unsigned long& type, float timeScale, bool flag, unsigned long depth) const;
};
bool CParticleUpdateParameters::GetUpdateParam(C3DVector& result, const unsigned long& type, float timeScale, bool flag, unsigned long depth) const
{
    if (type) {
        CVectorMap<unsigned long, CParticleUpdateParamDesc>::const_iterator i = UpdateParams.find(type);
        if (i != UpdateParams.end() && depth < 10)
            return i->second.GetUpdateParam(result, timeScale, flag, *this, depth + 1);
    }
    return false;
}
int main()
{
    CParticleUpdateParamCallback cb;
    pair<unsigned long, CParticleUpdateParamDesc> e[3];
    e[0].first = 2; e[1].first = 5; e[2].first = 9;
    for (int k = 0; k < 3; ++k) { e[k].second.Callback = &cb; e[k].second.Index = 100 + k; }
    CParticleUpdateParameters p;
    p.UpdateParams.Data._M_start = e; p.UpdateParams.Data._M_finish = e + 3;
    C3DVector r = { 0, 0, 0 };
    int ok = 1;
    unsigned long t = 5;
    if (!p.GetUpdateParam(r, t, 0.5f, true, 3) || g_calls != 1 || g_index != 101 || g_depth != 4 || !g_flag || g_scale != 0.5f || g_params != &p || r.X != 101.0f) ok = 0;
    t = 0;
    if (p.GetUpdateParam(r, t, 1.0f, false, 0) || g_calls != 1) ok = 0;            // zero type
    t = 6;
    if (p.GetUpdateParam(r, t, 1.0f, false, 0) || g_calls != 1) ok = 0;            // not present
    t = 10;
    if (p.GetUpdateParam(r, t, 1.0f, false, 0) || g_calls != 1) ok = 0;            // past the end
    t = 9;
    if (p.GetUpdateParam(r, t, 1.0f, false, 10) || g_calls != 1) ok = 0;           // depth limit
    if (!p.GetUpdateParam(r, t, 1.0f, false, 9) || g_calls != 2 || g_index != 102 || g_depth != 10) ok = 0;
    if (!ok) { std::printf("00af3080_TEST FAIL\n"); return 1; }
    std::printf("00af3080_TEST PASS\n");
    return 0;
}