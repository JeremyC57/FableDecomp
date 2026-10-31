// 008365f0 std::for_each -- preload STANDARD_DEAD animations
// Codex parked-50 best attempt: DIFFER(119v119); residue: 119/119 bytes; zero/argument register is EDX rather than EAX around string comparison; hidden functor-return copy uses a different scratch register.
// Leaf: ??$for_each@PBU?$pair@KVCAnimationEntry@@@std@@VCDeadCreatureAnimPreloader@@@std@@YI?AVCDeadCreatureAnimPreloader@@PBU?$pair@KVCAnimationEntry@@@0@0V1@@Z
// Flags: /Gr
#define CBankFile CBankFile_Layout
#include "engine/CBankFile.h"
#undef CBankFile
#define CMeshDataBank CMeshDataBank_Layout
#include "engine/CMeshDataBank.h"
#undef CMeshDataBank
#define CMeshDataBankEntry CMeshDataBankEntry_Layout
#include "engine/CMeshDataBankEntry.h"
#undef CMeshDataBankEntry
#define C3DAnimationInfo C3DAnimationInfo_Layout
#include "engine/C3DAnimationInfo.h"
#undef C3DAnimationInfo
extern "C" int __cdecl strcmp(const char*,const char*);
#pragma intrinsic(strcmp)
#include "engine/CBaseClassNonCopyable.h"
class CBankFile:public CBaseClassNonCopyable {
public:
    // Layout: bank state is unread; preserve the imported retail extent.
    unsigned char unobservedBankState[sizeof(CBankFile_Layout)-sizeof(CBaseClassNonCopyable)];
    virtual ~CBankFile();
};
class CBankFileAsync:public CBankFile {
public:
    // Layout: CBankFile.h documents the retail164h async-base boundary.
    unsigned char unobservedAsyncBankState[0x164-sizeof(CBankFile)];
    virtual ~CBankFileAsync();
};
class CMeshDataBankEntry {
public:
    virtual ~CMeshDataBankEntry();
    // Layout: imported Bank/Index state is unread by this row.
    unsigned char unobservedBankEntryState[sizeof(CMeshDataBankEntry_Layout)-4];
};
class C3DAnimationInfo:public CMeshDataBankEntry {
public:
    unsigned char unobservedAnimationInfo[sizeof(C3DAnimationInfo_Layout)-sizeof(CMeshDataBankEntry)];
    void BeginLoading(bool)const;
};
class CMeshDataBank:public CBankFileAsync {
public:
    unsigned char unobservedMeshBankState[sizeof(CMeshDataBank_Layout)-sizeof(CBankFileAsync)];
    const C3DAnimationInfo* PeekAnimation(unsigned long)const;
};
class CDefStringTable {
public:const char* GetStringAsCharArray(long)const;
};
extern CDefStringTable GDefStringTable;
class CDefString { public:long TablePos; const char* GetAsCharArray()const{return GDefStringTable.GetStringAsCharArray(TablePos);} };
class CAnimationEntry {
public:
    long BankIndex;CDefString AnimName;CDefString GroupName;
    // Layout: original Components counted pointer occupies four unread bytes.
private:
    unsigned char Components[4];
};
namespace std {
template<class T1,class T2>struct pair { T1 first;T2 second; };
template<class I,class F>F __fastcall for_each(I first,I last,F f) {
    for(;first!=last;++first) f(*first);
    return f;
}
}
class CDeadCreatureAnimPreloader {
public:
    CDeadCreatureAnimPreloader(const CMeshDataBank& bank):MeshBank(bank) { }
    void operator()(const std::pair<unsigned long,CAnimationEntry>& entry) const {
        if(strcmp(entry.second.AnimName.GetAsCharArray(),"STANDARD_DEAD")==0)
            MeshBank.PeekAnimation(entry.second.BankIndex)->BeginLoading(false);
    }
private:
    const CMeshDataBank& MeshBank;
};
template CDeadCreatureAnimPreloader __fastcall std::for_each(const std::pair<unsigned long,CAnimationEntry>*,const std::pair<unsigned long,CAnimationEntry>*,CDeadCreatureAnimPreloader);
