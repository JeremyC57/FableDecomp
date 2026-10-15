// CVectorPairOwner_00aa5bd0::ClearBoth() -- clears two STLport vectors held at +0x48 (pointer elements) and +0x54 (8-byte records) through the inlined erase(begin(), end())
// Leaf: ClearBoth
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA5BD0, length 80 bytes.
// Callees: memmove @ 0x00BFEAE6 (the trivial-copy path, `__cdecl`).
// Label: the manifest's "CTCCameraPointScripted::EditSetThingForEditorSetThing" is propagated and rejected; no
// caller or vtable reference was found, so the owner is named by address.
// Shape: STLport `clear()` = `erase(begin(), end())` = `_M_finish = copy(_M_finish, _M_finish, _M_start)`:
// for the pointer vector the copy is `__copy_trivial` -- `(last == first) ? result : memmove(...) +
// bytes` (the retail `cmp eax,eax ; jne` on the twice-loaded finish pointer); for the record vector,
// whose element has a user assignment, it is the counted random-access `__copy` loop (`sar edx,3`,
// two dword moves per element). Both results are stored back as the new finish.
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6

struct SKeyFrame {                                  // 8-byte element with a user assignment
    long m_time;
    long m_value;
    SKeyFrame& operator=(const SKeyFrame& other) { m_time = other.m_time; m_value = other.m_value; return *this; }
};

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

template <class T> inline T* CopyAssign(T* first, T* last, T* result)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

struct CPointerVector {                             // STLport vector<T*>
    void** _M_start;
    void** _M_finish;
    void** _M_end_of_storage;
    void clear()
    {
        void** i = (void**)CopyTrivial(_M_finish, _M_finish, _M_start);
        _M_finish = i;
    }
};

struct CKeyFrameVector {                            // STLport vector<SKeyFrame>
    SKeyFrame* _M_start;
    SKeyFrame* _M_finish;
    SKeyFrame* _M_end_of_storage;
    void clear()
    {
        SKeyFrame* i = CopyAssign(_M_finish, _M_finish, _M_start);
        _M_finish = i;
    }
};

struct CVectorPairOwner_00aa5bd0 {
    unsigned char   _pad[0x48];
    CPointerVector  m_pointers;                     // +0x48
    CKeyFrameVector m_records;                      // +0x54
    void ClearBoth();
};

void CVectorPairOwner_00aa5bd0::ClearBoth()
{
    m_pointers.clear();
    m_records.clear();
}