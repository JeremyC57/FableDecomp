// Behaviour test for the 0x00A0A610 record Sizeof: 8 + object Sizeof (when set) + CCharString Sizeof (4 empty, 0x15 + length otherwise).
#include <cstdio>

struct SStringData { long _a; long _b; unsigned long m_length; };
struct CCharString {
    SStringData* m_pData;
    unsigned long Sizeof() const { return m_pData ? (m_pData->m_length & 0x7fffffff) + 0x15 : 4; }
};
struct CSizedObject {
    unsigned long m_size;
    virtual unsigned long Sizeof() const { return m_size; }
};
struct CCountedPointer_CSizedObject {
    CSizedObject* Data;
    void* Info;
    unsigned long Sizeof() const { return Data ? Data->Sizeof() + 8 : 8; }
};
struct SNamedCountedPointer {
    CCharString Name;
    CCountedPointer_CSizedObject Pointer;
    unsigned long NamedCountedPointerSizeof() const { return Pointer.Sizeof() + Name.Sizeof(); }
};

int main()
{
    SNamedCountedPointer record;
    record.Name.m_pData = 0;
    record.Pointer.Data = 0;
    record.Pointer.Info = 0;
    if (record.NamedCountedPointerSizeof() != 12) { std::printf("00a0a610_TEST FAIL\n"); return 1; }
    SStringData text = { 0, 0, 5 };
    CSizedObject object;
    object.m_size = 100;
    record.Name.m_pData = &text;
    record.Pointer.Data = &object;
    if (record.NamedCountedPointerSizeof() != 100 + 8 + 5 + 0x15) { std::printf("00a0a610_TEST FAIL\n"); return 1; }
    std::printf("00a0a610_TEST PASS\n");
    return 0;
}
