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
// Layout: original PDB44bytes; native Transfer helpers independently read
// Mode+18, PLoadStream+24 and PSaveStream+28. All original fields retained.
class CPersistContext_TransferableVectorGenericType;
class CPersistContext_TransferableComponentGenericType;
class CPersistContextWriteSection;
template<class T> class CPersistTraits;
class CPersistContext {
public:
#ifdef FABLE_RETAIL_PERSIST_OPINION
    void PersistError(const CStringParser&,const CCharString&) const;
#endif
#if defined(FABLE_RETAIL_SHAPE_PERSIST) || defined(FABLE_RETAIL_QUEST_SCRIPT_PERSIST)
    template<class T> void Transfer(const char*,T&,const T& =T());
#elif defined(FABLE_RETAIL_SHOP)
    template<class T> void Transfer(const char*,T&,const T&);
#endif
#ifdef FABLE_RETAIL_SHOP
    template<class T> void Transfer(const char*,std::vector<T,std::allocator<T> >&);
#endif
#ifdef FABLE_RETAIL_PARTICLE_SERIALISE
    CPersistContext(CStringParser&,void (__fastcall*)(const CCharString&),bool,const CDefinitionManager*,const CSymbolMap*,CCharString*);
    bool IsSaving()const{return Mode==MODE_SAVE_TEXT||Mode==MODE_SAVE_BINARY;}
    CStringParser* GetPParser(){return PParser;}
    void WriteString(const CCharString&);
    template<class T>void Transfer(const char*,T&);
    template<class T>void Transfer(const char*,T&,const T&);
#endif
    enum EMode { MODE_LOAD_TEXT=0,MODE_SAVE_TEXT=1,MODE_LOAD_BINARY=2,MODE_SAVE_BINARY=3,MODE_CLEAR=4 };
#if defined(FABLE_RETAIL_QUEST_COMPLETION) || defined(FABLE_RETAIL_INVENTORY_MAP_SERIALISE)
    bool IsSaving() const { return Mode==MODE_SAVE_TEXT || Mode==MODE_SAVE_BINARY; }
    template<class T> void TransferVector(const char*,std::vector<T,std::allocator<T> >&);
#ifdef FABLE_RETAIL_QUEST_TRANSFER_BODY
protected:
#ifndef FABLE_RETAIL_CTHINGMANAGER_BINARY
    void CheckCRC(const char*);
#endif
    void TransferVectorSaveText(const char*,CPersistContext_TransferableVectorGenericType&);
    void TransferVectorLoadText(const char*,CPersistContext_TransferableVectorGenericType&);
public:
#endif
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
    CPersistContext(CMemoryDataInputStream&,bool);
    CMemoryDataInputStream* GetPLoadStream() { return PLoadStream; }
#endif
#ifdef FABLE_RETAIL_ENTITY_PERSIST
    CPersistContext(CDataOutputStream&,bool);
    CDataOutputStream* GetPSaveStream() { return PSaveStream; }
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    bool IsBinary() const { return Mode==MODE_LOAD_BINARY || Mode==MODE_SAVE_BINARY; }
    void StartSection(CPersistContextWriteSection&,const CCharString&);
    void EndSection(CPersistContextWriteSection&);
    void WriteString(const CCharString&);
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_SAVE
    template<class T> void Transfer(T& val) {
        CheckCRC("");
        switch(Mode) {
        case MODE_LOAD_BINARY: { CPersistTraits<T> traits;traits.TransferBinaryIn(*PLoadStream,val);break; }
        case MODE_SAVE_BINARY: { CPersistTraits<T> traits;traits.TransferBinaryOut(*PSaveStream,val);break; }
        }
    }
#else
    template<class T> void Transfer(T&);
#endif
#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
    template<class T> void Transfer(const char*,T&);
#endif
    template<class T> void TransferVector(std::vector<T,std::allocator<T> >&);
    template<class V,class K> void TransferMap(std::map<K,V,std::less<K>,std::allocator<std::pair<const K,V> > >&);
    template<class T> void TransferVectorOfSubComponents(std::vector<T,std::allocator<T> >&);
protected:
#if defined(FABLE_RETAIL_CTHINGMANAGER_BINARY) || defined(FABLE_RETAIL_SHAPE_PERSIST)
    void CheckCRC(const char*);
#endif
#ifdef FABLE_RETAIL_SHAPE_PERSIST
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
