#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
#pragma pack(push, 1)
struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
#pragma pack(pop)
struct SRecord18 {
    long m_word0; long m_word4; long m_word8; C3DVector m_vector;
    SRecord18() : m_word0(0), m_word4(0), m_word8(0) { }
    SRecord18(const SRecord18& other) : m_word0(other.m_word0), m_word4(other.m_word4), m_word8(other.m_word8), m_vector(other.m_vector) { }
};
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair() { }
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<long, SRecord18> SKeyedRecord1C;
typedef SKeyedRecord1C TElem;
static void Fill(TElem& e, int i)
{
    e.first = 5 * i; e.second.m_word0 = i; e.second.m_word4 = 2 * i; e.second.m_word8 = 3 * i; e.second.m_vector.X = 0.5f * i; e.second.m_vector.Y = 4.0f; e.second.m_vector.Z = -1.0f * i;
}
static bool Check(const TElem& e, int i)
{
    return e.first == 5 * i && e.second.m_word0 == i && e.second.m_word4 == 2 * i && e.second.m_word8 == 3 * i && e.second.m_vector.X == 0.5f * i && e.second.m_vector.Y == 4.0f && e.second.m_vector.Z == -1.0f * i;
}
static TElem* UninitCopy(const TElem* first, const TElem* last, TElem* result, const __false_type&)
{
    TElem* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) TElem(*first);
    return cur;
}
int main()
{
    static double srcStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    static double dstStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    TElem* src = (TElem*)srcStorage;
    TElem* dst = (TElem*)dstStorage;
    int i;
    for (i = 0; i < 3; ++i) { new ((void*)(src + i)) TElem(); Fill(src[i], i + 1); }
    __false_type tag;
    TElem* end = UninitCopy(src, src + 3, dst, tag);
    bool ok = end == dst + 3;
    for (i = 0; i < 3; ++i) if (!Check(dst[i], i + 1)) ok = false;
    if (UninitCopy(src, src, dst + 3, tag) != dst + 3) ok = false;
    
    if (!ok) { std::printf("00543810_TEST FAIL\n"); return 1; }
    std::printf("00543810_TEST PASS\n");
    return 0;
}