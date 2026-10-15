// Behaviour test for the record reset at 0x00B2FA40 (catalog: CV_BanditTollScript::Init).
#include <cstdio>
#include <string.h>

inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

template <class T>
struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last)
    {
        T* i = (T*)__copy_trivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};

struct SFrameSource { long m_frame; };
SFrameSource* g_pFrameSource = 0;

struct SOcclusionRecord {
    long m_count;
    CRetailVector<unsigned char> m_bytes;
    long m_frame;
};

struct CV_BanditTollScript { void Init(SOcclusionRecord* record); };

void CV_BanditTollScript::Init(SOcclusionRecord* record)
{
    record->m_count = 0;
    record->m_bytes.clear();
    record->m_frame = g_pFrameSource->m_frame;
}

int main()
{
    unsigned char buf[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    SFrameSource src; src.m_frame = 1234; g_pFrameSource = &src;
    SOcclusionRecord r; r.m_count = 9; r.m_bytes._M_start = buf; r.m_bytes._M_finish = buf + 6; r.m_bytes._M_end_of_storage = buf + 8; r.m_frame = 0;
    CV_BanditTollScript m;
    m.Init(&r);
    if (r.m_count != 0 || r.m_bytes._M_finish != buf || r.m_bytes._M_start != buf || r.m_bytes._M_end_of_storage != buf + 8
        || r.m_frame != 1234 || buf[0] != 1) {
        std::printf("00b2fa40_TEST FAIL\n");
        return 1;
    }
    std::printf("00b2fa40_TEST PASS\n");
    return 0;
}