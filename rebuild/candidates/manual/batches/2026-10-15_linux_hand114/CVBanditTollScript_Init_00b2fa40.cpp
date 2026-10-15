// CV_BanditTollScript::Init(SOcclusionRecord*) -- reset a record: zero its count, clear its byte vector (STLport erase(begin(), end())), stamp it with the current frame word (+0x4010 of the global @ 0x01436E5C)
// Retail: Fable.exe @ 0x00B2FA40, 66 bytes (+ int3 padding), `ret 4`, `this` unused.
// Callees: memmove @ 0x00BFEAE6 (on the never-taken branch of the inlined trivial copy).
// Label note: "CV_BanditTollScript::Init" is a propagated manifest label; the body is an engine-module member called
// with ecx = the manager global @ 0x01436E58 (from 0x00B911CE in CEnginePrimitiveManagerOcclusionBase::RenderBoundingBox
// and 0x00B924E9); the record and the globals are modelled descriptively.
// Shape: STLport `vector<unsigned char>::erase(begin(), end())` inlines `copy(last, _M_finish, first)` as
// `__copy_trivial`: `last == first ? result : (char*)memmove(result, first, n) + n` -- here last and first are both
// _M_finish, so retail keeps the `cmp eax,eax` and the dead memmove arm; the destroy of a POD range is empty.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

inline void* __copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

template <class T>
struct CRetailVector {                                   // STLport vector of a POD element
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
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

struct SFrameSource {
    unsigned char _pad[0x4010];
    long m_frame;                                        // +0x4010
};
extern SFrameSource* g_pFrameSource;                     // 0x01436E5C

struct SOcclusionRecord {
    long _pad00[2];
    long m_count;                                        // +0x08
    CRetailVector<unsigned char> m_bytes;                // +0x0c
    long _pad18;                                         // +0x18
    long m_frame;                                        // +0x1c
};

struct CV_BanditTollScript {
    void Init(SOcclusionRecord* record);
};

void CV_BanditTollScript::Init(SOcclusionRecord* record)
{
    record->m_count = 0;
    record->m_bytes.clear();
    record->m_frame = g_pFrameSource->m_frame;
}
