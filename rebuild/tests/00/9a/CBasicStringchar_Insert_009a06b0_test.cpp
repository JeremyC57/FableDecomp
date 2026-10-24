#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#endif
#pragma intrinsic(memcpy, memset)

#pragma pack(push, 1)
template<class T> struct CBasicString {
    T* PData;                                   // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool UseFastExtend : 1;
    void ExtendData(unsigned long newLength);                       // 0x009A01C0
    void AppendData(const T* data, unsigned long length);           // 0x009A04E0
    static unsigned long Length(const T* text)
    {
        unsigned long n = 0;
        while (text[n])
            ++n;
        return n;
    }
    void ClearTail()
    {
        unsigned long used = StringLength + 1;
        unsigned long end = (used + 3) & ~3UL;
        memset(PData + used, 0, end - used);
    }
    void Insert(unsigned long position, const T* text);
};
#pragma pack(pop)

template<class T> void CBasicString<T>::Insert(unsigned long position, const T* text)
{
    unsigned long oldLength = StringLength;
    if (position == oldLength) {
        AppendData(text, Length(text));
        return;
    }
    unsigned long length = Length(text);
    ExtendData(length + oldLength);
    unsigned long newLength = StringLength + length;
    StringLength = newLength;
    PData[newLength] = 0;
    ClearTail();
    memmove(PData + length + position, PData + position, oldLength - position);
    memcpy(PData + position, text, length);
}

template void CBasicString<char>::Insert(unsigned long position, const char* text);

static int g_appends = 0;
template<class T> void CBasicString<T>::ExtendData(unsigned long newLength)
{
    unsigned long allocation = (newLength + 4) & ~3UL;
    if (allocation <= DataLength)
        return;
    T* data = (T*)std::malloc(allocation);
    std::memset(data, 0x5a, allocation);
    std::memcpy(data, PData, StringLength + 1);
    std::free(PData);
    PData = data;
    DataLength = allocation;
}
template<class T> void CBasicString<T>::AppendData(const T* data, unsigned long length)
{
    ++g_appends;
    ExtendData(StringLength + length);
    std::memcpy(PData + StringLength, data, length);
    StringLength += length;
    PData[StringLength] = 0;
    ClearTail();
}
int main()
{
    CBasicString<char> s;
    s.PData = (char*)std::malloc(8);
    std::strcpy(s.PData, "Fable");
    s.StringLength = 5;
    s.DataLength = 8;
    s.UseFastExtend = false;
    s.Insert(2, "XYZ");                              // middle: grow, move "ble" up, copy the text in
    if (std::strcmp(s.PData, "FaXYZble") != 0 || s.StringLength != 8 || g_appends != 0) { std::printf("009a06b0_TEST FAIL\n"); return 1; }
    for (unsigned long i = s.StringLength + 1; i < ((s.StringLength + 4) & ~3UL); ++i)
        if (s.PData[i] != 0) { std::printf("009a06b0_TEST FAIL\n"); return 1; }
    s.Insert(8, "!!");                               // at the end: appended
    if (std::strcmp(s.PData, "FaXYZble!!") != 0 || s.StringLength != 10 || g_appends != 1) { std::printf("009a06b0_TEST FAIL\n"); return 1; }
    s.Insert(0, "");                                 // empty text at the front: unchanged
    if (std::strcmp(s.PData, "FaXYZble!!") != 0 || s.StringLength != 10) { std::printf("009a06b0_TEST FAIL\n"); return 1; }
    s.Insert(0, ">");
    if (std::strcmp(s.PData, ">FaXYZble!!") != 0 || s.StringLength != 11) { std::printf("009a06b0_TEST FAIL\n"); return 1; }
    std::printf("009a06b0_TEST PASS\n");
    return 0;
}