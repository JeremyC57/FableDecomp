// CReplaceSerialise<COldFileFormatMapHeader7,COldFileFormatMapHeader6>::GetMaxSize(long version, long size) -- the largest buffer any map header version up to 7 needs: walk down the version chain to the record's own version, then take the max with each level's sizeof
// Leaf: ?GetMaxSize@?$CReplaceSerialise@VCOldFileFormatMapHeader7@@VCOldFileFormatMapHeader6@@
//
// Retail 0x00519630, 110 bytes (+ int3 padding to 112), static __fastcall (version in ecx, size in edx), plain `ret`.  FableWin.pdb
//   ?GetMaxSize@?$CReplaceSerialise@VCOldFileFormatMapHeader7@@VCOldFileFormatMapHeader6@@@@SIJJJ@Z.
// Callee: 0x00515FA0 CReplaceSerialise<COldFileFormatMapHeader6,COldFileFormatMapHeader5>::GetMaxSize (the version-6
//   level is inline here, version 5 and below out of line).
// Callers: CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8>::GetMaxSize 0x0051ADF0 and the inlined
//   chains in ReadFromFile 0x0051CBB0 (landed).
// Layout: Ego_r.pdb sizes COldFileFormatMapHeader7 0x841E, COldFileFormatMapHeader6 0x841A (pack 1).
// Shape: GetMaxSize is the static __fastcall template `if (version == GetClassVersionNumber()) return MAX(size, sizeof(T));
//   return MAX(TOld::GetMaxSize(version, size), sizeof(T));` with MAX the evaluate-twice macro, and the whole version
//   chain (down to CBaseSerialise<COldFileFormatMapHeader1, 1>) defined in the TU.  VC inlines the version-6 level twice
//   (the macro's two evaluations: compare, then result) and keeps version 5 out of line (0x00515FA0, inline depth).
//   Inside each inlined version-6 copy the macro's second call to the version-5 level is dominated by the first, and
//   VC knows that same-TU callee is side-effect free, so it reuses the result: one call per copy, two in all (retail).
//   A named-local form (`maxSize = ...; return MAX(maxSize, ...)`, the landed 0x0051CBB0 model) folds to one call
//   (43 v 110 bytes); the early-return form with only a declaration of the version-5 level keeps all four calls
//   (131 v 110).  Compares are unsigned because sizeof is.
// Label: catalog "GetMaxSize" (module _global): it is the CReplaceSerialise<Header7,Header6> instance.
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
#pragma pack(pop)

template long __fastcall CReplaceSerialise<COldFileFormatMapHeader7, COldFileFormatMapHeader6>::GetMaxSize(long, long);