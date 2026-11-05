// SRecord::SRecord(const SRecord& other) -- copy two counted pointers (bumping their shared counts) and four strings
// Leaf: ??0SRecord
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local SCountBlock (= CCPPointerInfo) members Count->RefCount are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009922F0, length 97 bytes (`ret 4`).
// Identity: the catalog label GFPredicateAnd<CIsThingAlive, CIsThingAlive> is propagated; the body is the copy constructor of a
//   0x20-byte record {CCountedPointer A, B; CCharString S0..S3} (callers: RegisterSoundBanks, ~CEditFractal, a vector insert).
// Callees: CCharString copy constructor 0x0099EC30 (four times).
// Shape: CCountedPointer's copy is `Data(o.Data), Info(o.Info) { if (Info) ++Info->Count; }`; the constructor is defined out of class.
class CCharStringData;
class CCharString { public: CCharStringData* PStringData; CCharString(const CCharString& other); };   // 0x0099EC30
#include "engine/CCountedPointer.h"
class CThing;
struct SRecord {
    CCountedPointer<CThing> A; CCountedPointer<CThing> B;
    CCharString S0, S1, S2, S3;
    SRecord(const SRecord& o);
};
SRecord::SRecord(const SRecord& o) : A(o.A), B(o.B), S0(o.S0), S1(o.S1), S2(o.S2), S3(o.S3) { }