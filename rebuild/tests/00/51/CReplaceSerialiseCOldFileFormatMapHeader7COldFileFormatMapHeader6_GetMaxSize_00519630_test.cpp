// Behaviour test for 0x00519630 CReplaceSerialise<COldFileFormatMapHeader7,COldFileFormatMapHeader6>::GetMaxSize: the result is the largest of the
// caller's size and the sizeof every record level from the top version down to the requested one (unknown versions walk
// the whole chain).
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#pragma pack(push, 1)
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned int ByteLength;                  // +0x00
    unsigned char SerialiseVersionNumber;      // +0x04
    static unsigned short __fastcall GetClassVersionNumber() { return T::VERSION; }
    static long __fastcall GetMaxSize(long version, long size)
    {
        if (version == GetClassVersionNumber())
            return MAX(size, sizeof(T));
        return MAX(TOld::GetMaxSize(version, size), sizeof(T));
    }
};
template <class T, long V> class CBaseSerialise {
public:
    unsigned int ByteLength;                  // +0x00
    unsigned char SerialiseVersionNumber;      // +0x04
    static long __fastcall GetMaxSize(long version, long size) { return MAX(size, sizeof(T)); }
};
class COldFileFormatMapHeader1 : public CBaseSerialise<COldFileFormatMapHeader1, 1> { public: enum { VERSION = 1 }; };
class COldFileFormatMapHeader2 : public CReplaceSerialise<COldFileFormatMapHeader2, COldFileFormatMapHeader1> { public: enum { VERSION = 2 }; };
class COldFileFormatMapHeader3 : public CReplaceSerialise<COldFileFormatMapHeader3, COldFileFormatMapHeader2> { public: enum { VERSION = 3 }; };
class COldFileFormatMapHeader4 : public CReplaceSerialise<COldFileFormatMapHeader4, COldFileFormatMapHeader3> {
public:
    enum { VERSION = 4 };
    unsigned char _payload[0x840E - 5];
};
class COldFileFormatMapHeader5 : public CReplaceSerialise<COldFileFormatMapHeader5, COldFileFormatMapHeader4> {
public:
    enum { VERSION = 5 };
    unsigned char _payload[0x8416 - 5];
};
class COldFileFormatMapHeader6 : public CReplaceSerialise<COldFileFormatMapHeader6, COldFileFormatMapHeader5> {
public:
    enum { VERSION = 6 };
    unsigned char _payload[0x841A - 5];        // Ego_r.pdb: 0x841A bytes
};
class COldFileFormatMapHeader7 : public CReplaceSerialise<COldFileFormatMapHeader7, COldFileFormatMapHeader6> {
public:
    enum { VERSION = 7 };
    unsigned char _payload[0x841E - 5];        // Ego_r.pdb: 0x841E bytes
};
#pragma pack(pop)


int main()
{
    int ok = 1;
    ok &= CReplaceSerialise<COldFileFormatMapHeader7, COldFileFormatMapHeader6>::GetMaxSize(7, 0x0) == 0x841E;
    ok &= CReplaceSerialise<COldFileFormatMapHeader7, COldFileFormatMapHeader6>::GetMaxSize(7, 0x9000) == 0x9000;
    ok &= CReplaceSerialise<COldFileFormatMapHeader7, COldFileFormatMapHeader6>::GetMaxSize(7, 0x841E) == 0x841E;
    ok &= CReplaceSerialise<COldFileFormatMapHeader7, COldFileFormatMapHeader6>::GetMaxSize(6, 0x0) == 0x841E;
    ok &= CReplaceSerialise<COldFileFormatMapHeader7, COldFileFormatMapHeader6>::GetMaxSize(6, 0x841B) == 0x841E;
    ok &= CReplaceSerialise<COldFileFormatMapHeader7, COldFileFormatMapHeader6>::GetMaxSize(6, 0x9001) == 0x9001;
    ok &= CReplaceSerialise<COldFileFormatMapHeader7, COldFileFormatMapHeader6>::GetMaxSize(5, 0x8417) == 0x841E;
    ok &= CReplaceSerialise<COldFileFormatMapHeader7, COldFileFormatMapHeader6>::GetMaxSize(4, 0x10) == 0x841E;
    ok &= CReplaceSerialise<COldFileFormatMapHeader7, COldFileFormatMapHeader6>::GetMaxSize(3, 0x20000) == 0x20000;
    ok &= CReplaceSerialise<COldFileFormatMapHeader7, COldFileFormatMapHeader6>::GetMaxSize(1, 0x0) == 0x841E;
    ok &= CReplaceSerialise<COldFileFormatMapHeader7, COldFileFormatMapHeader6>::GetMaxSize(42, 0x841F) == 0x841F;
    if (!ok) { std::printf("00519630_TEST FAIL\n"); return 1; }
    std::printf("00519630_TEST PASS\n");
    return 0;
}