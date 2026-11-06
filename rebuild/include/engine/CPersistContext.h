#ifndef FABLE_ENGINE_CPERSISTCONTEXT_H
#define FABLE_ENGINE_CPERSISTCONTEXT_H
#include "rebuild_abi.h"
class CDefinitionManager;
class CSymbolMap;
class CStringParser;
class CCharString;
class CMemoryDataInputStream;
class CDataOutputStream;
namespace std { template<class T> class allocator; template<class A,class B> struct pair; template<class T> struct less; template<class K,class V,class C,class A> class map; template<class T,class A> class vector; }
// One model (Codex pass 11 consolidation; opted in through fable_persist_context.h FABLE_RETAIL_CPERSISTCONTEXT).
// Layout: original PDB44bytes; native Transfer helpers independently read
// Mode+18, PLoadStream+24 and PSaveStream+28. All original fields retained.
class CPersistContext_TransferableVectorGenericType;
class CPersistContext_TransferableComponentGenericType;
#ifdef FABLE_RETAIL_CPERSISTCONTEXT
class CPersistContext_TransferableVectorOfSubComponentsGenericType;
class CPersistContext_TransferableSubComponentGenericType;
class CPersistContextReadSection;
#endif
class CPersistContextWriteSection;
template<class T> class CPersistTraits;
class CPersistContext {
public:
    template<class T> void Transfer(const char*,T&,const T&);
    template<class T> void Transfer(const char*,std::vector<T,std::allocator<T> >&);
    CPersistContext(CStringParser&,void (__fastcall*)(const CCharString&),bool,const CDefinitionManager*,const CSymbolMap*,CCharString*);
    bool IsSaving()const{return Mode==MODE_SAVE_TEXT||Mode==MODE_SAVE_BINARY;}
    CStringParser* GetPParser(){return PParser;}
    void WriteString(const CCharString&);
    enum EMode { MODE_LOAD_TEXT=0,MODE_SAVE_TEXT=1,MODE_LOAD_BINARY=2,MODE_SAVE_BINARY=3,MODE_CLEAR=4 };
    template<class T> void TransferVector(const char*,std::vector<T,std::allocator<T> >&);
    CPersistContext(CMemoryDataInputStream&,bool);
    CMemoryDataInputStream* GetPLoadStream() { return PLoadStream; }
#ifdef FABLE_RETAIL_CPERSISTCONTEXT
    CPersistContext() {}
    void Init(CStringParser&,void (__fastcall*)(const CCharString&),bool,const CDefinitionManager*,const CSymbolMap*,CCharString*);
    // Codex pass 13 (row 0x00CDC070): the default ctor and dtor (??0CPersistContext@@QAE@XZ / ??1CPersistContext@@QAE@XZ, one PDB
    // each; inline in retail) and the parser Init (?Init@CPersistContext@@QAEXAAVCStringParser@@...@Z, both PDBs).
    ~CPersistContext() {}
    // retry11a (row 0x00521620): the save-side constructor 0x009BAE50 (Ego_r.pdb / FableWin.pdb
    // ??0CPersistContext@@QAE@AAVCDataOutputStream@@_N@Z) and the PSaveStream accessor (inline in retail).
    CPersistContext(CDataOutputStream&,bool);
    CDataOutputStream* GetPSaveStream() { return PSaveStream; }
    // Chunk 735 (rows 0x0047F58B CTCInGameMenu / 0x00597228 NUISystem::CFrontEndManager AddRegionAndTimeInfo): the binary-load Init
    // 0x009BAA30 (Ego_r.pdb / FableWin.pdb ?Init@CPersistContext@@QAEXAAVCMemoryDataInputStream@@_N@Z, `ret 8`: stream +0x24, Mode
    // MODE_LOAD_BINARY) and ReadSection 0x009BAE70 (?ReadSection@CPersistContext@@QAE_NAAVCPersistContextReadSection@@ABVCCharString@@@Z, landed).
    void Init(CMemoryDataInputStream&,bool);
    bool ReadSection(CPersistContextReadSection&,const CCharString&);
    // pass 12 header wave (0x009BC120): Ego_r / FableWin ?PersistError@CPersistContext@@QBEXABVCStringParser@@ABVCCharString@@@Z,
    // ?CheckUnknownSymbols@CPersistContext@@QAEXXZ (out of line: 0x009BAA90 / 0x009BBBF0); SetAsSingleTransfer has no copy in
    // either PDB name set (inline: 0x009BC120 stores SingleTransfer=true on its nested context).
    void PersistError(const CStringParser&,const CCharString&) const;
    void CheckUnknownSymbols();
    void SetAsSingleTransfer() { SingleTransfer=true; }
#endif
    template<class T> void Transfer(T&);
#ifdef FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
    // Retail opt-in (chunk 802, row 0x007ABBD0 CTCAIScratchpad::OnSerialise): Ego_r.pdb 0x0040D2E0 / FableWin.pdb 0x018951E0
    // ??$Transfer@J@CPersistContext@@QAEXPBDAAJ@Z (and the CCharString twin 0x0040DAD0 / 0x01895D30) is inline in retail: the
    // caller expands `Transfer(name, value, GetDefaultVal<T>())` (0x007ABC80: the long default 0 stored in the dead context slot;
    // 0x007ABCF1..0x007ABD08: GetDefaultVal<CCharString> builds a local, copies it out and destroys it, no NRVO).
    template<class T> void Transfer(const char* name,T& value) { Transfer(name,value,GetDefaultVal<T>()); }
    // Ego_r.pdb / FableWin.pdb ??$TransferMap@_NVCCharString@@@CPersistContext@@QAEXPBDAAV?$map@...@std@@AB_N@Z (retail 0x007AE3D0,
    // the named map transfer with a default value). Declaration only.
    template<class V,class K> void TransferMap(const char*,std::map<K,V,std::less<K>,std::allocator<std::pair<const K,V> > >&,const V&);
#else
    template<class T> void Transfer(const char*,T&);
#endif
    template<class T> void TransferVector(std::vector<T,std::allocator<T> >&);
    template<class V,class K> void TransferMap(std::map<K,V,std::less<K>,std::allocator<std::pair<const K,V> > >&);
    template<class T> void TransferVectorOfSubComponents(std::vector<T,std::allocator<T> >&);
protected:
    void CheckCRC(const char*);
#ifdef FABLE_RETAIL_CPERSISTCONTEXT
    // Codex pass 13 (row 0x00CDC070): original protected static fastcall template; both PDB PUBLIC32 records.
    template<class T> static T FABLE_FASTCALL GetDefaultVal();
    // Protected in both PDBs: ?TransferVectorOfSubComponentsLoadText@CPersistContext@@IAEXPBDAAVCPersistContext_TransferableVectorOfSubComponentsGenericType@@@Z
    // (0x009BC120) and ?CleanOutLine@CPersistContext@@IAEXJJ@Z (0x009BB0D0, out of line).
    void TransferVectorOfSubComponentsLoadText(const char*,CPersistContext_TransferableVectorOfSubComponentsGenericType&);
    void TransferVectorLoadText(const char*,CPersistContext_TransferableVectorGenericType&);
    void TransferSubComponentLoadText(const char*,CPersistContext_TransferableSubComponentGenericType&);
    void CleanOutLine(long,long);
    // Chunk 728 round 2 (row 0x006D2BD0 CTCWife::OnSerialise): the protected text-arm workers of the three-argument Transfer<T> body
    // (Ego_r.pdb 0x00B0F770 / 0x00B10710, FableWin.pdb 0x02FEBD50 / 0x02FEBDC0; retail 0x009BAD60 / 0x009BBA40, the only spellings
    // bound there). Declarations only: no layout change.
    void TransferObjectSaveText(const char*,CPersistContext_TransferableComponentGenericType&);
    void TransferObjectLoadText(const char*,CPersistContext_TransferableComponentGenericType&);
#endif
    const CDefinitionManager* PDefinitionManager;
    const CSymbolMap* PSymbolMap;
    CStringParser* PParser;
    void (__fastcall *ErrorCallback)(const CCharString&);
    CCharString* PStringToClean;
    bool UseDefaults,UseCRCCheck,SingleTransfer,Finished;
    EMode Mode;
    bool Binary;
    CCharString* PStringToSaveTo;
    CMemoryDataInputStream* PLoadStream;
    CDataOutputStream* PSaveStream;
};
FABLE_STATIC_ASSERT(sizeof(CPersistContext)==44);
#endif
