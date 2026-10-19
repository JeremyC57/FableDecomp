// CBasicString<char>::Truncate(unsigned long) -- allocate the aligned prefix, release the old buffer, update the packed capacity/length, and clear tail padding; retail 0x009a0080 length 0xa6. Allocator calls are operator new[] @ 0x00bfeb22 and operator delete[] @ 0x00bfeb1c.
#include <string.h>

// Ego_r.pdb CBasicString<char> is a packed 13-byte subobject: PData +0,
// StringLength +4, DataLength:31 +8, UseFastExtend:1 +0xc. The actual packed
// CCharStringData layout corroborates this template field placement.
#pragma pack(push, 1)
struct CBasicStringChar {
    char* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;
    void Truncate(unsigned long length);
};
#pragma pack(pop)

void CBasicStringChar::Truncate(unsigned long length) {
    if (length < StringLength) {
        unsigned long allocation = (length + 4) & ~3UL;
        char* data = new char[allocation];
        data = (char*)memcpy(data, PData, length);
        data[length] = 0;
        if (PData) {
            delete[] PData;
            PData = 0;
        }
        DataLength = allocation;
        PData = data;
        StringLength = length;
        PData[StringLength] = 0;
        unsigned long used = StringLength + 1;
        unsigned long end = (used + 3) & ~3UL;
        memset(PData + used, 0, end - used);
    }
}

#include <stddef.h>
#include <stdio.h>
#include <string.h>

static int check(bool ok, const char* name)
{
    if (!ok) { printf("FAIL %s\n", name); return 1; }
    return 0;
}

static unsigned long read_capacity_word(const CBasicStringChar& value)
{
    unsigned long word = 0;
    const char* bytes = (const char*)&value;
    const unsigned long offset = (unsigned long)offsetof(CBasicStringChar, StringLength) + (unsigned long)sizeof(unsigned long);
    memcpy(&word, bytes + offset, sizeof(word));
    return word;
}

static void write_capacity_word(CBasicStringChar& value, unsigned long word)
{
    char* bytes = (char*)&value;
    const unsigned long offset = (unsigned long)offsetof(CBasicStringChar, StringLength) + (unsigned long)sizeof(unsigned long);
    memcpy(bytes + offset, &word, sizeof(word));
}

int main()
{
#ifdef _MSC_VER
    typedef char native_packed_size[(sizeof(CBasicStringChar) == 13) ? 1 : -1];
    typedef char native_data_offset[(offsetof(CBasicStringChar, PData) == 0) ? 1 : -1];
    typedef char native_length_offset[(offsetof(CBasicStringChar, StringLength) == 4) ? 1 : -1];
    (void)sizeof(native_packed_size); (void)sizeof(native_data_offset); (void)sizeof(native_length_offset);
#endif
    CBasicStringChar value;
    value.PData = new char[16];
    memcpy(value.PData, "abcdefghijk", 11);
    value.StringLength = 11;
    value.DataLength = 16;
    value.UseFastExtend = true;
#ifdef _MSC_VER
    const unsigned long highBit = 1UL << ((unsigned long)sizeof(unsigned long) * 8UL - 1UL);
    unsigned long capacity = read_capacity_word(value) | highBit;
    write_capacity_word(value, capacity);
#endif
    char* original = value.PData;

    int failures = 0;
    value.Truncate(11);
    failures += check(value.PData == original && value.StringLength == 11 && value.DataLength == 16,
                      "length at current end leaves storage intact");

    value.Truncate(5);
    failures += check(value.PData != original, "truncation replaces old allocation");
    failures += check(value.StringLength == 5 && value.DataLength == 8,
                      "truncation stores length and aligned capacity");
    failures += check(value.UseFastExtend, "capacity update preserves fast-extend bit");
#ifdef _MSC_VER
    failures += check((read_capacity_word(value) & highBit) != 0,
                      "capacity update preserves reserved top bit");
#endif
    failures += check(memcmp(value.PData, "abcde", 5) == 0 && value.PData[5] == 0,
                      "truncation copies prefix and terminates string");
    failures += check(value.PData[6] == 0 && value.PData[7] == 0,
                      "truncation clears aligned tail padding");

    value.Truncate(0);
    failures += check(value.StringLength==0 && value.DataLength==4 && value.PData[0]==0 && value.PData[1]==0 && value.PData[2]==0 && value.PData[3]==0,"zero prefix clears aligned allocation");
    char* empty=value.PData;value.Truncate(10);
    failures += check(value.PData==empty && value.StringLength==0,"longer request is no-op");
    delete[] value.PData;
    if (failures) { printf("009a0080_TEST FAIL\n"); return 1; }
    printf("009a0080_TEST PASS\n");
    return 0;
}