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
    // retry11a (row 0x00521620): the save-side constructor 0x009BAE50 (Ego_r.pdb / FableWin.pdb
    // ??0CPersistContext@@QAE@AAVCDataOutputStream@@_N@Z) and the PSaveStream accessor (inline in retail).
    CPersistContext(CDataOutputStream&,bool);
    CDataOutputStream* GetPSaveStream() { return PSaveStream; }
    // pass 12 header wave (0x009BC120): Ego_r / FableWin ?PersistError@CPersistContext@@QBEXABVCStringParser@@ABVCCharString@@@Z,
    // ?CheckUnknownSymbols@CPersistContext@@QAEXXZ (out of line: 0x009BAA90 / 0x009BBBF0); SetAsSingleTransfer has no copy in
    // either PDB name set (inline: 0x009BC120 stores SingleTransfer=true on its nested context).
    void PersistError(const CStringParser&,const CCharString&) const;
    void CheckUnknownSymbols();
    void SetAsSingleTransfer() { SingleTransfer=true; }
#endif
    template<class T> void Transfer(T&);
#ifdef FABLE_RETAIL_CPERSISTCONTEXT
    // Retail branch (chunk 728, row 0x006D2BD0 CTCWife::OnSerialise): the two-argument Transfer forwards to the three-argument one with
    // the protected static GetDefaultVal<T>() (FableWin.pdb ??$Transfer@_N@CPersistContext@@QAEXPBDAA_N@Z 0x018951A0 /
    // ??$GetDefaultVal@_N@CPersistContext@@KI_NXZ 0x0189C330 and 102 more GetDefaultVal instances; Ego_r keeps ??$Transfer@J@..QAEXPBDAAJ@Z
    // 0x0040D2E0 / @M@ 0x0040D370 out of line). Inline in retail: the caller builds the name (its CCharString temporaries) before it
    // pushes the default temporary, the value and the name (0x006D2E10..0x006D2E4F).
    template<class T> void Transfer(const char* name,T& value) { Transfer(name,value,GetDefaultVal<T>()); }
protected:
    template<class T> static T __fastcall GetDefaultVal() { return T(); }
public:
#else
    template<class T> void Transfer(const char*,T&);
#endif
    template<class T> void TransferVector(std::vector<T,std::allocator<T> >&);
    template<class V,class K> void TransferMap(std::map<K,V,std::less<K>,std::allocator<std::pair<const K,V> > >&);
    template<class T> void TransferVectorOfSubComponents(std::vector<T,std::allocator<T> >&);
protected:
    void CheckCRC(const char*);
#ifdef FABLE_RETAIL_CPERSISTCONTEXT
    // Protected in both PDBs: ?TransferVectorOfSubComponentsLoadText@CPersistContext@@IAEXPBDAAVCPersistContext_TransferableVectorOfSubComponentsGenericType@@@Z
    // (0x009BC120) and ?CleanOutLine@CPersistContext@@IAEXJJ@Z (0x009BB0D0, out of line).
    void TransferVectorOfSubComponentsLoadText(const char*,CPersistContext_TransferableVectorOfSubComponentsGenericType&);
    void CleanOutLine(long,long);
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
