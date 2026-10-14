#include <cstdio>
#include <cstring>
struct CRetailTree_erase004aba70 {
    void* head; unsigned long size;
    CRetailTree_erase004aba70& operator=(const CRetailTree_erase004aba70& other) { head = other.head; size = other.size; return *this; }
};
struct CRetailVector_char {
    char* first; char* finish;
    static __forceinline char* CopyTrivial(const char* copyFirst, const char* copyLast, char* result)
    {
        return (copyLast == copyFirst) ? result
             : static_cast<char*>(std::memmove(result, copyFirst, copyLast - copyFirst)) + (copyLast - copyFirst);
    }
    __forceinline char* erase(char* eraseFirst, char* eraseLast)
    {
        char* i = CopyTrivial(eraseLast, finish, eraseFirst);
        finish = i;
        return eraseFirst;
    }
    __forceinline void clear() { erase(first, finish); }
};
struct CKeyedBuffer_00bebe90 {
    unsigned char _pad[0x20]; CRetailTree_erase004aba70 m_keys; unsigned char _pad28[4]; CRetailVector_char m_buffer;
    void Assign(const CRetailTree_erase004aba70& keys);
};
void CKeyedBuffer_00bebe90::Assign(const CRetailTree_erase004aba70& keys)
{
    m_keys = keys;
    m_buffer.clear();
}
int main()
{
    char data[8] = "abcdefg";
    CKeyedBuffer_00bebe90 b; b.m_keys.head = 0; b.m_keys.size = 0; b.m_buffer.first = data; b.m_buffer.finish = data + 7;
    int node; CRetailTree_erase004aba70 keys = { &node, 5 };
    b.Assign(keys);
    if (b.m_keys.head != &node || b.m_keys.size != 5 || b.m_buffer.finish != data || b.m_buffer.first != data) { std::printf("00bebe90_TEST FAIL\n"); return 1; }
    // erase of a middle range keeps the tail (the inlined SGI copy)
    b.m_buffer.finish = data + 7;
    b.m_buffer.erase(data + 1, data + 3);
    if (b.m_buffer.finish != data + 5 || std::memcmp(data, "adefg", 5) != 0) { std::printf("00bebe90_TEST FAIL\n"); return 1; }
    std::printf("00bebe90_TEST PASS\n");
    return 0;
}
