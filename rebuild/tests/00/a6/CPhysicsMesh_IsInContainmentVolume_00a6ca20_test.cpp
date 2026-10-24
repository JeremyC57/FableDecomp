// Behaviour test for 0x00A6CA20: CPhysicsMesh::IsInContainmentVolume
#include <cstdio>
#include <cstdlib>
#include <new>

template <class T> class CBasicString {
public:
    const T* m_pData;
    long Compare(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    bool operator<(const CBasicString& other) const { return Compare(other) < 0; }
};

static CBasicString<char> gStrData[6] = { { "alpha" }, { "bravo" }, { "charlie" }, { "delta" }, { "echo" }, { "foxtrot" } };
static int gLiveStrings = 0;

class CCharString {
public:
    CBasicString<char>* PData;
    CCharString() : PData(0) { ++gLiveStrings; }
    explicit CCharString(int k) : PData(&gStrData[k]) { ++gLiveStrings; }
    CCharString(const CCharString& other) : PData(other.PData) { ++gLiveStrings; }
    ~CCharString() { --gLiveStrings; }
    CCharString& operator=(const CCharString& other) { PData = other.PData; return *this; }
    bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return *PData < *other.PData;
    }
    bool FastCompare(const CCharString& other) const { return *this < other; }
    int Key() const { return PData ? (int)(PData - gStrData) : -1; }
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class CContainmentVolume {
public:
    C3DVector Centre;
    float     Radius;
    float     Fraction;          // the rough proportion this volume reports
};

template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Info;
};

class C3DMeshVolume {
public:
    CCharString                         Name;
    CCountedPointer<CContainmentVolume> ConvexVolume;
    bool IsContainingPartOfSphere(const C3DVector& position, float radius) const
    {
        const CContainmentVolume* v = ConvexVolume.Data;
        float dx = position.X - v->Centre.X, dy = position.Y - v->Centre.Y, dz = position.Z - v->Centre.Z;
        float r = radius + v->Radius;
        return dx * dx + dy * dy + dz * dz < r * r;
    }
    float PeekRoughProportionOfSphereContained(const C3DVector&, float) const { return ConvexVolume.Data->Fraction; }
};

class C3DHelperPoint {
public:
    C3DVector Position;
    long      BoneIndex;
};

namespace _STL {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
};
}

template <class K, class V, class C> class CVectorMap {
public:
    typedef _STL::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    value_type* _M_start;
    value_type* _M_finish;
    value_type* _M_end_of_storage;
    C           Compare;
    bool        Dirty;
    const_iterator end() const { return _M_finish; }
    const_iterator LowerBound(const K& key) const
    {
        const_iterator it = _M_start;
        while (it != _M_finish && it->first < key)
            ++it;
        return it;
    }
    const_iterator UpperBound(const K& key) const
    {
        const_iterator it = _M_start;
        while (it != _M_finish && !(key < it->first))
            ++it;
        return it;
    }
    const_iterator Find(const K& key) const
    {
        const_iterator it = LowerBound(key);
        const_iterator last = end();
        if (it == last || Compare(key, *it))
            return last;
        return it;
    }
};

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const K& key, const _STL::pair<K, V>& entry) const { return key < entry.first; }
};

template <class V> class CCompareString {
public:
    bool operator()(const CCharString& key, const _STL::pair<CCharString, V>& entry) const { return key.FastCompare(entry.first); }
};

typedef CVectorMap<CCharString, C3DMeshVolume, CKeyPairCompareLess<CCharString, C3DMeshVolume> > TVolumeMap;
typedef CVectorMap<CCharString, C3DHelperPoint, CCompareString<C3DHelperPoint> > THelperPointMap;

class CPhysicsMesh {
public:
    unsigned char   _pad00[0x10];
    THelperPointMap HelperPoints;
    unsigned char   _pad20[0x44];
    TVolumeMap      ContainmentVolumes;
    C3DVector GetHelperPointPositionStatic(const CCharString& name, long index) const
    {
        return HelperPoints.LowerBound(name)[index].second.Position;
    }
    bool IsInContainmentVolume(const CCharString& name, const C3DVector& position, float radius) const
    {
        TVolumeMap::const_iterator last = ContainmentVolumes.UpperBound(name);
        for (TVolumeMap::const_iterator it = ContainmentVolumes.LowerBound(name); it != last; ++it)
            if (it->second.IsContainingPartOfSphere(position, radius))
                return true;
        return false;
    }
    float PeekRoughProportionOfSphereContained(const CCharString& name, const C3DVector& position, float radius) const
    {
        float total = 0.0f;
        TVolumeMap::const_iterator last = ContainmentVolumes.UpperBound(name);
        for (TVolumeMap::const_iterator it = ContainmentVolumes.LowerBound(name); it != last; ++it)
            total += it->second.PeekRoughProportionOfSphereContained(position, radius);
        if (total < 0.0f)
            return 0.0f;
        if (total > 1.0f)
            return 1.0f;
        return total;
    }
    bool HasContainmentVolume(const CCharString& name) const
    {
        TVolumeMap::const_iterator it = ContainmentVolumes.Find(name);
        return it != ContainmentVolumes.end();
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

// alpha: one volume; bravo: two; charlie: two; delta: one (negative fraction)
static CContainmentVolume gCV[6];
static _STL::pair<CCharString, C3DMeshVolume> gVolumes[6];
static _STL::pair<CCharString, C3DHelperPoint> gHelpers[4];

static void BuildMesh(CPhysicsMesh& m)
{
    static const int keys[6] = { 0, 1, 1, 2, 2, 3 };
    static const float frac[6] = { 0.3f, 0.4f, 0.5f, 0.8f, 0.7f, -0.5f };
    for (int i = 0; i < 6; ++i) {
        gCV[i].Centre = C3DVector(10.0f * i, 0.0f, 0.0f);
        gCV[i].Radius = 1.0f;
        gCV[i].Fraction = frac[i];
        gVolumes[i].first = CCharString(keys[i]);
        gVolumes[i].second.Name = CCharString(keys[i]);
        gVolumes[i].second.ConvexVolume.Data = &gCV[i];
        gVolumes[i].second.ConvexVolume.Info = 0;
    }
    m.ContainmentVolumes._M_start = gVolumes;
    m.ContainmentVolumes._M_finish = gVolumes + 6;
    m.ContainmentVolumes._M_end_of_storage = gVolumes + 6;
    m.ContainmentVolumes.Dirty = false;
    static const int hkeys[4] = { 0, 1, 1, 2 };
    for (int j = 0; j < 4; ++j) {
        gHelpers[j].first = CCharString(hkeys[j]);
        gHelpers[j].second.Position = C3DVector(1.0f + 3 * j, 2.0f + 3 * j, 3.0f + 3 * j);
        gHelpers[j].second.BoneIndex = j;
    }
    m.HelperPoints._M_start = gHelpers;
    m.HelperPoints._M_finish = gHelpers + 4;
    m.HelperPoints._M_end_of_storage = gHelpers + 4;
    m.HelperPoints.Dirty = false;
}

int main()
{
    int ok = 1;
    CPhysicsMesh m;
    BuildMesh(m);
    if (!m.IsInContainmentVolume(CCharString(1), C3DVector(20.5f, 0.0f, 0.0f), 0.1f))   // second bravo volume
        ok = 0;
    if (m.IsInContainmentVolume(CCharString(1), C3DVector(0.0f, 0.0f, 0.0f), 0.1f))     // alpha's volume only
        ok = 0;
    if (!m.IsInContainmentVolume(CCharString(0), C3DVector(1.5f, 0.0f, 0.0f), 1.0f))
        ok = 0;
    if (m.IsInContainmentVolume(CCharString(4), C3DVector(0.0f, 0.0f, 0.0f), 100.0f))   // no echo volume
        ok = 0;

    if (!ok) {
        std::printf("00a6ca20_TEST FAIL\n");
        return 1;
    }
    std::printf("00a6ca20_TEST PASS\n");
    return 0;
}