// SRecord::SRecord(const SRecord& other) -- copy two counted pointers (bumping their shared counts) and four strings
// Leaf: ??0SRecord
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009922F0, length 97 bytes (`ret 4`).
// Identity: the catalog label GFPredicateAnd<CIsThingAlive, CIsThingAlive> is propagated; the body is the copy constructor of a
//   0x20-byte record {CCountedPointer A, B; CCharString S0..S3} (callers: RegisterSoundBanks, ~CEditFractal, a vector insert).
// Callees: CCharString copy constructor 0x0099EC30 (four times).
// Shape: CCountedPointer's copy is `Data(o.Data), Info(o.Info) { if (Info) ++Info->Count; }`; the constructor is defined out of class.
struct CCharStringData;
struct CCharString { CCharStringData* PStringData; CCharString(const CCharString& other); };   // 0x0099EC30
struct SCountBlock { long Count; };
template <class T> struct CCountedPointer {
    T* Data; SCountBlock* Info;
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->Count; }
};
struct CThing;
struct SRecord {
    CCountedPointer<CThing> A; CCountedPointer<CThing> B;
    CCharString S0, S1, S2, S3;
    SRecord(const SRecord& o);
};
SRecord::SRecord(const SRecord& o) : A(o.A), B(o.B), S0(o.S0), S1(o.S1), S2(o.S2), S3(o.S3) { }