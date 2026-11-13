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
#ifdef FABLE_RETAIL_CPERSISTCONTEXT_ISSAVING_OUTOFLINE
    // Per-TU opt-in (chunk 849, row 0x0057E2F1 CTCHeroStats::OnSerialise): ?IsSaving@CPersistContext@@QBE_NXZ declared only. That TU
    // calls the out-of-line copy at 0x004AEF50 (`mov ecx,esi ; call 0x004AEF50 ; test al,al`, 0x0057E8E0 and 0x0057EB91) instead of
    // expanding the Mode test.
    bool IsSaving()const;
#else
    bool IsSaving()const{return Mode==MODE_SAVE_TEXT||Mode==MODE_SAVE_BINARY;}
#endif
    CStringParser* GetPParser(){return PParser;}
#ifdef FABLE_RETAIL_CPERSISTCONTEXT_PEEKPSYMBOLMAP
    // Retail opt-in (chunk 1020, row 0x009E81E0 via CPersistTraits<float>::TransferIn 0x0040F7E0): Ego_r.pdb / FableWin.pdb
    // ?PeekPSymbolMap@CPersistContext@@QBEPBVCSymbolMap@@XZ, inline in retail (`mov ecx,[ebx+4]` before FindSymbol 0x009E5170).
    const CSymbolMap* PeekPSymbolMap() const { return PSymbolMap; }
#endif
    void WriteString(const CCharString&);
    enum EMode { MODE_LOAD_TEXT=0,MODE_SAVE_TEXT=1,MODE_LOAD_BINARY=2,MODE_SAVE_BINARY=3,MODE_CLEAR=4 };
#ifdef FABLE_RETAIL_CPERSISTCONTEXT_ISPARSINGTEXT
    // Retail opt-in (chunk 1018, row 0x0067E290 CDegradableDef::Transfer): Ego_r.pdb / FableWin.pdb ?IsParsingText@CPersistContext@@QBE_NXZ,
    // inline in retail (`mov eax,[esi+18h] ; test eax,eax ; jne` at 0x0067E369).
    bool IsParsingText() const { return Mode==MODE_LOAD_TEXT; }
#endif
    template<class T> void TransferVector(const char*,std::vector<T,std::allocator<T> >&);
    CPersistContext(CMemoryDataInputStream&,bool);
    CMemoryDataInputStream* GetPLoadStream() { return PLoadStream; }
#ifdef FABLE_RETAIL_CPERSISTCONTEXT_ISBINARY
    // Retail opt-in (chunk 846, row 0x0051FD80 CThingManager::LoadSingleThing): Ego_r.pdb / FableWin.pdb ?IsBinary@CPersistContext@@QBE_NXZ,
    // inline in retail (`mov eax,[ecx+18h] ; cmp eax,2 ; je ; cmp eax,3 ; jne` at 0x0051FDD0 and 0x00520186). Body only: no layout change.
    bool IsBinary() const { return Mode==MODE_LOAD_BINARY||Mode==MODE_SAVE_BINARY; }
#endif
#ifdef FABLE_RETAIL_CPERSISTCONTEXT
    CPersistContext() {}
    void Init(CStringParser&,void (__fastcall*)(const CCharString&),bool,const CDefinitionManager*,const CSymbolMap*,CCharString*);
    // Codex pass 13 (row 0x00CDC070): the default ctor and dtor (??0CPersistContext@@QAE@XZ / ??1CPersistContext@@QAE@XZ, one PDB
    // each; inline in retail) and the parser Init (?Init@CPersistContext@@QAEXAAVCStringParser@@...@Z, both PDBs).
    ~CPersistContext() {}
    // retry11a (row 0x00521620): the save-side constructor 0x009BAE50 (Ego_r.pdb / FableWin.pdb
    // ??0CPersistContext@@QAE@AAVCDataOutputStream@@_N@Z) and the PSaveStream accessor (inline in retail).
#ifdef FABLE_RETAIL_CPERSISTCONTEXT_STRINGSAVE
    // Retail opt-in (chunk Q2, row 0x004C9120 CThing::Save): Ego_r.pdb ??0CPersistContext@@QAE@AAVCCharString@@@Z (retail 0x009BAE10, the
    // text-save context) and ?Init@CPersistContext@@QAEXAAVCCharString@@PBVCDefinitionManager@@@Z (0x009BAA10, `push 0 ; push &data`).
    CPersistContext(CCharString&);                           // 0x009BAE10
    void Init(CCharString&,const CDefinitionManager*);       // 0x009BAA10
#endif
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
#ifdef FABLE_RETAIL_CPERSISTCONTEXT_SETASFOUNDITEM
    // Nested opt-in (chunk 1020, row 0x005D8978 TransferMap<long,unsigned long>): Ego_r.pdb / FableWin.pdb ?SetAsFoundItem@CPersistContext@@QAEXXZ,
    // out of line at 0x009BAB40 (Ego_r donor order InitClearMode -> PersistError -> SetAsFoundItem -> WriteString = retail 0x009BAA70 /
    // 0x009BAA90 / 0x009BAB40 / 0x009BAB50). Declaration only.
    void SetAsFoundItem();
#endif
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
#ifdef FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS_NAMED
    // Retail opt-in (chunk 1018, row 0x00F334E0): the named overload, the PDB spelling ??$TransferVectorOfSubComponents@V<T>@@@CPersistContext@@
    // QAEXPBDAAV?$vector@...@std@@@Z kept by the TLC-added (no-RTTI) modules (`ret 8`; body below the class).
    template<class T> void TransferVectorOfSubComponents(const char*,std::vector<T,std::allocator<T> >&);
#endif
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
#ifdef FABLE_RETAIL_GFSERIALISEMAPBINARY_DECL
// Retail opt-in (chunk 1020, row 0x005D8978 TransferMap<long,unsigned long>): the two global map serialisers the binary modes call
// out of line (__fastcall: stream in ecx, map in edx), Ego_r.pdb / FableWin.pdb
// ??$GFSerialiseMapBinaryIn@<V><K><C>@@YIXAAVCMemoryDataInputStream@@AAV?$map@<K><V><C>...@std@@@Z and
// ??$GFSerialiseMapBinaryOut@<V><K><C>@@YIXAAVCDataOutputStream@@ABV?$map@...@std@@@Z (0x0045C95A / 0x0045C91D for <long,unsigned long>).
// Declarations only (FABLE_RETAIL_GFSERIALISEMAPBINARYIN in engine/CPersistTraits.h holds the In body).
#ifndef FABLE_RETAIL_GFSERIALISEMAPBINARYIN
template<class V,class K,class C> void FABLE_FASTCALL GFSerialiseMapBinaryIn(CMemoryDataInputStream&,std::map<K,V,C,std::allocator<std::pair<const K,V> > >&);
#endif
template<class V,class K,class C> void FABLE_FASTCALL GFSerialiseMapBinaryOut(CDataOutputStream&,const std::map<K,V,C,std::allocator<std::pair<const K,V> > >&);
#endif
#ifdef FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS
// Retail opt-in (chunk 1015): the shared body of the member template
// ??$TransferVectorOfSubComponents@V<T>@@@CPersistContext@@QAEXAAV?$vector@V<T>@@V?$allocator@V<T>@@@std@@@std@@@Z (Ego_r.pdb /
// FableWin.pdb name sets; TLC's release persist macro drops the name argument, so the retail instances take the vector only,
// `ret 4`, and check the pooled empty key 0x0122D70E). One body for every element def: a binary load reads the count with the
// inline CDataInputStream::ReadSLONG, resizes (STLport's inline resize(n) = resize(n, _Tp())) and, for a non-empty vector,
// transfers each element; any other mode writes size() and transfers each element (CDefClassBase::Transfer, vtable slot 18,
// +0x48). A switch with only `case MODE_LOAD_BINARY` and a `default` gives retail's `mov eax,[ecx+18h] ; sub eax,2` (/Os `dec eax ;
// dec eax`) `; je` (an `if` compares the member in memory). The `count > 0` test ahead of the load loop is retail's: the loop
// guard repeats it, so it emits no code, but its extra block lowers the register priority of `this` (c2prio: 29 -> 27 in
// 0x004ECF6E), which is what gives every size-optimised instance retail's inline-read registers (`lea edx,[eax+4] ; xor eax,eax`,
// then the chunk pointer in eax: 0x004ECFCE, 0x00476089, 0x004794C7 ...); without it `this` is coloured first and the read
// comes out `add eax,4 ; xor edx,edx` (the 13 parked rows of retry 535). Instances reproduced with it: 0x00472A8D, 0x00476024,
// 0x0047652B, 0x004769D0, 0x00478AE5, 0x00479466, 0x004798AB, 0x00479EF4, 0x0047A2F5, 0x004ECF6E, 0x004ED953, 0x004EDE2B,
// 0x004F9C07, 0x004FA0A9, 0x0067D6B0. Needs the retail stream models (FABLE_RETAIL_CDATAINPUTSTREAM,
// FABLE_RETAIL_CMEMORYDATAINPUTSTREAM, FABLE_RETAIL_CDATAOUTPUTSTREAM) and the element's retail def model; the includer
// instantiates it explicitly.
#if !defined(FABLE_RETAIL_CMEMORYDATAINPUTSTREAM) || !defined(FABLE_RETAIL_CDATAINPUTSTREAM) || !defined(FABLE_RETAIL_CDATAOUTPUTSTREAM)
#error FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS needs the retail stream models
#endif
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_containers.h"
template<class T> void CPersistContext::TransferVectorOfSubComponents(std::vector<T,std::allocator<T> >& value)
{
    CheckCRC("");
    switch (Mode) {
    case MODE_LOAD_BINARY: {
        long count = PLoadStream->ReadSLONG();
        value.resize(count);
        if (count > 0)
            for (long i = 0; i < count; ++i)
                value[i].Transfer(*this);
        break;
    }
    default: {
        long count = value.size();
        PSaveStream->WriteSLONG(count);
        for (long i = 0; i < count; ++i)
            value[i].Transfer(*this);
        break;
    }
    }
}
#endif
#ifdef FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS_NAMED
// Retail opt-in (chunk 1018): the shared body of the named member template ??$TransferVectorOfSubComponents@V<T>@@@CPersistContext@@QAEXPBDAAV?$vector@
// V<T>@@V?$allocator@V<T>@@@std@@@std@@@Z (Ego_r.pdb / FableWin.pdb), `ret 8`. CheckCRC(name), then a closed switch on Mode with
// no text-save arm (the jump table's MODE_SAVE_TEXT entry is 0: `default` is unreachable): MODE_CLEAR transfers each element
// (CDefClassBase::Transfer, slot 18), MODE_SAVE_BINARY writes size() then each element, MODE_LOAD_BINARY reads the count with the
// inline CDataInputStream::ReadSLONG, resizes (STLport's resize(n) = resize(n, _Tp())) and transfers each element, MODE_LOAD_TEXT
// hands the CPersistContext_TransferableVectorOfSubComponents<T> wrapper to TransferVectorOfSubComponentsLoadText 0x009BC120.
// Arms in that source order (retail lays them out 4, 3, 2, 0: 0x00F334FD, 0x00F3353C, 0x00F3358D, 0x00F33621).
// Instances reproduced with it: 0x00F334E0 (CConversation). Needs the retail stream models; the includer instantiates it.
#if !defined(FABLE_RETAIL_CMEMORYDATAINPUTSTREAM) || !defined(FABLE_RETAIL_CDATAINPUTSTREAM) || !defined(FABLE_RETAIL_CDATAOUTPUTSTREAM)
#error FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOROFSUBCOMPONENTS_NAMED needs the retail stream models
#endif
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistContext_TransferableVectorOfSubComponents.h"
#include "stlport/fable_containers.h"
template<class T> void CPersistContext::TransferVectorOfSubComponents(const char* name,std::vector<T,std::allocator<T> >& value)
{
    CheckCRC(name);
    switch (Mode) {
    case MODE_CLEAR: {
        long count = value.size();
        for (long i = 0; i < count; ++i)
            value[i].Transfer(*this);
        break;
    }
    case MODE_SAVE_BINARY: {
        long count = value.size();
        PSaveStream->WriteSLONG(count);
        for (long i = 0; i < count; ++i)
            value[i].Transfer(*this);
        break;
    }
    case MODE_LOAD_BINARY: {
        long count = PLoadStream->ReadSLONG();
        value.resize(count);
        for (long i = 0; i < count; ++i)
            value[i].Transfer(*this);
        break;
    }
    case MODE_LOAD_TEXT: {
        CPersistContext_TransferableVectorOfSubComponents<T> item(value);
        TransferVectorOfSubComponentsLoadText(name,item);
        break;
    }
    default:
#ifdef _MSC_VER
        __assume(0);
#endif
        break;
    }
}
#endif
#ifdef FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BINARY_BODY
// Retail opt-in (chunk 1018, row 0x0067E290 CDegradableDef::Transfer): the shared bodies of the one-argument member templates
// ??$Transfer@<T>@CPersistContext@@QAEXAA<T>@Z and ??$TransferVector@<T>@CPersistContext@@QAEXAAV?$vector@...@std@@@Z as TLC's release
// persist macros call them (the name argument dropped: CheckCRC with the pooled empty key 0x0122D70E). Transfer<T>: a CPersistTraits<T>
// local, MODE_LOAD_BINARY -> traits.TransferBinaryIn(*PLoadStream, value), MODE_SAVE_BINARY -> traits.TransferBinaryOut(*PSaveStream,
// value) (retail `sub eax,2 ; je ; dec eax ; jne`, 0x0067E2A9; the out-of-line Transfer<float> 0x00431061 is the same body). TransferVector<T>:
// MODE_LOAD_BINARY -> GFSerialiseVectorBinaryIn, any other mode -> GFSerialiseVectorBinaryOut (`sub eax,2 ; mov edx,ebx ; je`, 0x0067E34D).
// Needs FABLE_RETAIL_CPERSISTCONTEXT; the traits come from engine/CPersistTraits.h.
#include "engine/CPersistTraits.h"
template<class T> void FABLE_FASTCALL GFSerialiseVectorBinaryIn(CMemoryDataInputStream&,std::vector<T,std::allocator<T> >&);
template<class T> void FABLE_FASTCALL GFSerialiseVectorBinaryOut(CDataOutputStream&,std::vector<T,std::allocator<T> >&);
template<class T> void CPersistContext::Transfer(T& value)
{
    CheckCRC("");
    CPersistTraits<T> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY:
        traits.TransferBinaryIn(*PLoadStream,value);
        break;
    case MODE_SAVE_BINARY:
        traits.TransferBinaryOut(*PSaveStream,value);
        break;
    }
}
template<class T> void CPersistContext::TransferVector(std::vector<T,std::allocator<T> >& value)
{
    CheckCRC("");
#ifdef FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERVECTOR_NODEFAULT
    // Nested opt-in (wf16f row 0x006D8F10 CHeroExperienceDef::Transfer): both binary modes are cases and the default is a braced
    // unreachable block (a NODEFAULT / ASSERT(false) style macro: `{ __assume(0); }`). Same bytes for the transfer itself (`sub eax,2 ;
    // je` with no `dec eax ; jne`), but the braced block keeps a dead-end default block alive up to register allocation, so a
    // callee-saved register used by these transfers cannot have its restore hoisted -- which is what lets VC7.1 shrink-wrap a
    // second register between them (retail `push ebp` 0x006D9064 / `pop ebp` 0x006D91A3). A bare `default: __assume(0);` is
    // pruned early and does not (693 v 692).
    switch (Mode) {
    case MODE_LOAD_BINARY:
        GFSerialiseVectorBinaryIn(*PLoadStream,value);
        break;
    case MODE_SAVE_BINARY:
        GFSerialiseVectorBinaryOut(*PSaveStream,value);
        break;
    default: {
#ifdef _MSC_VER
        __assume(0);
#endif
        break;
    }
    }
#else
    switch (Mode) {
    case MODE_LOAD_BINARY:
        GFSerialiseVectorBinaryIn(*PLoadStream,value);
        break;
    default:
        GFSerialiseVectorBinaryOut(*PSaveStream,value);
        break;
    }
#endif
}
#endif
#ifdef FABLE_RETAIL_CPERSISTCONTEXT_TRANSFERMAP_BINARY_BODY
// Retail opt-in (wf16 row 0x006D8F10 CHeroExperienceDef::Transfer): the shared body of the one-argument member template
// ??$TransferMap@<V>@<K>@CPersistContext@@QAEXAAV?$map@...@std@@@Z as the landed out-of-line copies spell it (0x0046FA22 etc.):
// CheckCRC(""), MODE_LOAD_BINARY -> GFSerialiseMapBinaryIn, MODE_SAVE_BINARY -> GFSerialiseMapBinaryOut (`sub eax,2 ; je ; dec eax ; jne`).
template<class V,class K,class C> void FABLE_FASTCALL GFSerialiseMapBinaryIn(CMemoryDataInputStream&,std::map<K,V,C,std::allocator<std::pair<const K,V> > >&);
template<class V,class K,class C> void FABLE_FASTCALL GFSerialiseMapBinaryOut(CDataOutputStream&,const std::map<K,V,C,std::allocator<std::pair<const K,V> > >&);
template<class V,class K> void CPersistContext::TransferMap(std::map<K,V,std::less<K>,std::allocator<std::pair<const K,V> > >& map)
{
    CheckCRC("");
    switch (Mode) {
    case MODE_LOAD_BINARY:
        GFSerialiseMapBinaryIn(*PLoadStream,map);
        break;
    case MODE_SAVE_BINARY:
        GFSerialiseMapBinaryOut(*PSaveStream,map);
        break;
    }
}
#endif
#endif
#if defined(FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BODY) && !defined(FABLE_ENGINE_CPERSISTCONTEXT_TRANSFER_BODY_H)
#define FABLE_ENGINE_CPERSISTCONTEXT_TRANSFER_BODY_H
// Retail opt-in (chunk 2010, row 0x005248B0 NThingManager::CEntityRuntimePersistence::LoadFromGameStateScript): the shared body of the
// member template ??$Transfer@<T>@CPersistContext@@QAEXPBDAA<T>AB<T>@Z (Ego_r.pdb / FableWin.pdb), the body of the landed instances
// 0x0044BB00 (unsigned __int64) and 0x004106F0 (unsigned long): CheckCRC(name), then a closed switch on Mode laid out 4, 3, 2, 1, 0 --
// MODE_CLEAR copies the default, the binary arms go through a block-scoped CPersistTraits<T>, the text arms hand the
// CPersistContext_TransferableComponent<T> wrapper to TransferObjectSaveText 0x009BAD60 / TransferObjectLoadText 0x009BBA40 (the load
// arm seeds the default first when UseDefaults is set). Retail calls the instances out of line; the body visible in the includer tells
// VC7.1 that the call only reads the default, so the caller's loop-invariant default stores are hoisted into the loop preheader
// (0x00524C6B..0x00524C7B: `xor edi,edi` and the three default words stored once before the dead-thing loop). Needs the retail stream
// models (FABLE_RETAIL_CMEMORYDATAINPUTSTREAM, FABLE_RETAIL_CDATAINPUTSTREAM, FABLE_RETAIL_CDATAOUTPUTSTREAM).
#if !defined(FABLE_RETAIL_CMEMORYDATAINPUTSTREAM) || !defined(FABLE_RETAIL_CDATAINPUTSTREAM) || !defined(FABLE_RETAIL_CDATAOUTPUTSTREAM)
#error FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BODY needs the retail stream models
#endif
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableComponent.h"
template<class T> void CPersistContext::Transfer(const char* name,T& value,const T& defaultValue)
{
    CheckCRC(name);
    switch (Mode) {
    case MODE_CLEAR:
        value = defaultValue;
        break;
    case MODE_SAVE_BINARY: {
        CPersistTraits<T> traits;
        traits.TransferBinaryOut(*PSaveStream, value);
        break;
    }
    case MODE_LOAD_BINARY: {
        CPersistTraits<T> traits;
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    }
    case MODE_SAVE_TEXT: {
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectSaveText(name, item);
        break;
    }
    case MODE_LOAD_TEXT: {
        if (UseDefaults)
            value = defaultValue;
        CPersistContext_TransferableComponent<T> item(value);
        TransferObjectLoadText(name, item);
        break;
    }
#ifdef _MSC_VER
    default:
        __assume(0);
#endif
    }
}
#endif
