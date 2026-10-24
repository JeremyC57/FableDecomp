// CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8>::GetMaxSize(long version, long size) -- the largest buffer any map header version up to 9 needs: walk down the version chain to the record's own version, then take the max with each level's sizeof
// Leaf: ?GetMaxSize@?$CReplaceSerialise@VCFileFormatMapHeader@@VCOldFileFormatMapHeader8@@
//
// Retail 0x0051ADF0, 110 bytes (+ int3 padding to 112), static __fastcall (version in ecx, size in edx), plain `ret`.  FableWin.pdb
//   ?GetMaxSize@?$CReplaceSerialise@VCFileFormatMapHeader@@VCOldFileFormatMapHeader8@@@@SIJJJ@Z.
// Callee: 0x00519630 CReplaceSerialise<COldFileFormatMapHeader7,COldFileFormatMapHeader6>::GetMaxSize (this chunk; the
//   version-8 level is inline here, version 7 and below out of line).
// Layout: Ego_r.pdb sizes CFileFormatMapHeader 0x10822, COldFileFormatMapHeader8 0x1081E (pack 1).
// Shape: the 0x00519630 template one level up -- `if (version == 9) return MAX(size, 0x10822); return
//   MAX(Header8::GetMaxSize(version, size), 0x10822);` with the version-8 level inlined twice (the macro's compare and
//   result) and, inside each copy, the dominated second call to the same-TU version-7 level reused: two calls in all.
// Label: catalog "GetMaxSize" (module _global): it is the CReplaceSerialise<CFileFormatMapHeader,Header8> instance.
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#pragma pack(push, 1)
template <class T, class TOld> class CReplaceSerialise {
public:
    unsigned long ByteLength;                  // +0x00
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
    unsigned long ByteLength;                  // +0x00
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
class COldFileFormatMapHeader8 : public CReplaceSerialise<COldFileFormatMapHeader8, COldFileFormatMapHeader7> {
public:
    enum { VERSION = 8 };
    unsigned char _payload[0x1081E - 5];       // Ego_r.pdb: 0x1081E bytes
};
class CFileFormatMapHeader : public CReplaceSerialise<CFileFormatMapHeader, COldFileFormatMapHeader8> {
public:
    enum { VERSION = 9 };
    unsigned char _payload[0x10822 - 5];       // Ego_r.pdb: 0x10822 bytes
};
#pragma pack(pop)

template long __fastcall CReplaceSerialise<CFileFormatMapHeader, COldFileFormatMapHeader8>::GetMaxSize(long, long);