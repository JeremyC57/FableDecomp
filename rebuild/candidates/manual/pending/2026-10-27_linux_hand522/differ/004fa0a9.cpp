// CPersistContext::TransferVectorOfSubComponents<CCardDef>(std::vector<CCardDef>& value) -- binary persist of a vector of CCardDef sub-components: on a binary load read the element count, resize with a default-constructed CCardDef and let each element Transfer itself; otherwise write the count and Transfer each element
// Grade: DIFFER(254v255) -- only the stream-position check of the inlined Read: retail forms the new position as `lea edx,[eax+4] ; xor eax,eax` (low word in edx, kept for the StreamPos store), ours `add eax,4 ; xor edx,edx` with every later use renamed; tried an explicit < 0 || > 0x7fffffff test, the landed IsValidPosition helper, an unsigned long position local
// Leaf: ??$TransferVectorOfSubComponents@VCCardDef@@@CPersistContext@@QAEXAAV?$vector@VCCardDef@@V?$allocator@VCCardDef@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004FA0A9, 255 bytes (size-optimised def TU: packed address, ebp frame,
//   `push 44h ; pop ecx`, `dec eax ; dec eax`), thiscall, `ret 4`.
// Identity (proven): the PDB name sets hold ??$TransferVectorOfSubComponents@VCCardDef@@@CPersistContext@@ (Ego_r.pdb
//   spells it with a leading const char* name, which TLC's release persist macros drop: the sole caller passes only the
//   vector, as for the landed CUIStateDef / CBlurredAttitudesDef instantiations 0x00632E00 / 0x0067D6B0); its sole caller
//   is CTavernGameCardBaseDef::Transfer 0x004F9A90 (site 0x004F9B59) on CardPack (this+0x78, vector<CCardDef>), the
//   element is 0x44 bytes (`idiv 44h`, stride 0x44) and the resize temporary stores vtable 0x01243A44 (.?AVCCardDef@@).
// Label correction: catalogued as TransferVectorOfSubComponents<CAbilityButtonGuiDef> (propagated); the landed caller
//   binds it as ?Transfer@CPersistContext@@QAEXAAV?$vector@VCCardDef... (NAME_FIXES.tsv).
// Callees: CPersistContext::CheckCRC(const char*) 0x00404500 (protected; the pooled empty key 0x0122D70E),
//   CDataOutputStream::WriteSLONG(long) 0x00993EB0 (retail order EBOOL, UBYTE, SBYTE, CBYTE, UWORD, SWORD, ULONG, SLONG
//   at 0x00993E30.. matches the Ego_r.pdb donor order), CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (private),
//   CParentDefClassBase::CParentDefClassBase() 0x00430370 / ~CParentDefClassBase() 0x00430300 (the temporary's inline
//   CCardDef constructor and implicit destructor), vector<CCardDef>::resize(size_type, const CCardDef&) 0x004FA1A8
//   (landed), each element's virtual Transfer (slot 18, +0x48).
// Layout: Ego_r.pdb CPersistContext (Mode +0x18, PLoadStream +0x24, PSaveStream +0x28; no engine/ header), CDataInputStream
//   (StreamPos +4, StreamSize +8, CurrentSourcePtr +0xC, SourceChunkStreamPos +0x10, SourceChunkBytesRemaining +0x14), the
//   def base chain and its 25 virtual slots as the landed 0x004573FD audit.
// Shape: the stream read chain (ReadSLONG -> ReadType<long> -> Read) is __forceinline: the /Os TU does not inline it on its own.
#pragma optimize("s", on)
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {   // STLport, 12 bytes
public:
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
    void resize(size_type __new_size, const _Tp& __x);   // 0x004FA1A8 for CCardDef
};
}   // namespace std

class CCharString;
class CPersistContext;
class CDataInputStream;
class CDataOutputStream;
class CValidateDefinitionInitBase;
class CDefinitionManager;
class CResourceList;
class CDefString {
public:
    long TablePos;
};
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    void*    Start;                                         // CArray<pair<K, V> > (STLport vector)
    void*    Finish;
    void*    End;
    TCompare Compare;
    bool     Dirty;
};
#pragma pack(push, 1)
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;
    unsigned long IVRefCount;
};
class CResource : public CIVCountedPointeeBase {
public:
    virtual ~CResource();
    virtual bool UnloadResource();
    virtual unsigned long GetUnloadableMemory() const;
    virtual bool IsResourceLoaded() const;
    virtual void OnResourceLoaded();
    virtual bool CanAbortResourceLoad(long) const;
    virtual void OnResourceLoadAborted();
    virtual unsigned long GetDebugMemoryInUse() const;
    virtual void ResetDebugMemoryInUse();
    CResourceList* ResourceList;
    CResource*     PrevResource;
    CResource*     NextResource;
    unsigned long  ResourceSize;
    unsigned long  LastUsedFrame;
};
class CDefPointeeBase : public CResource {
};
class CDefClassBase : public CDefPointeeBase {
public:
    class CSubDefInfo { public: long DefIndex; long OriginalParentDefIndex; };
    typedef CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> > SubDefMap;
    virtual ~CDefClassBase();
    virtual bool UnloadResource();
    virtual CDefString GetInstantiationName() const;
    virtual void SetInstantiationName(CDefString);
    virtual void AddSubDef(const CCharString&, long, long);
    virtual void RemoveSubDef(const CCharString&);
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString&) const;
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString&);
    virtual bool IsParentDef() const;
    virtual void PostTransfer();
    virtual void Transfer(CPersistContext& context) = 0;    // slot 18 (+0x48)
    virtual void Copy(const CDefClassBase*) = 0;            // slot 19
    virtual unsigned long GetSizeofClass() = 0;             // slot 20
    virtual void InternalSaveBinary(CDataOutputStream&);
    virtual void InternalLoadBinary(CDataInputStream&);
    virtual void Validate(const CValidateDefinitionInitBase&, const CCharString&) const;
    virtual SubDefMap* GetPSubDefInfoMap();
    CDefinitionManager* PDefManager;                        // +0x1C
    unsigned long       GlobalIndex;                        // +0x20
    unsigned char       Setup : 1;                          // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
class CParentDefClassBase : public CDefClassBase {           // vtable 0x01230CBC, retail 0x39 (packed)
public:
    CParentDefClassBase();                                  // 0x00430370
    virtual ~CParentDefClassBase();                         // 0x00430300
    CDefString InstantiationName;                           // +0x25
    SubDefMap  SubDefInfoMap;                               // +0x29
};
#pragma pack(pop)
class CCardDef : public CParentDefClassBase {               // vtable 0x01243A44, retail 0x44 bytes
public:
    long CardName;                                          // +0x3C (Ego_r +0x40)
    long CardVal;                                           // +0x40 (Ego_r +0x44)
    virtual void Transfer(CPersistContext& context);        // 0x004EC0B3
    virtual void Copy(const CDefClassBase*);                // 0x004EC0F0
    virtual unsigned long GetSizeofClass();                 // 0x004EC0AF
};

class CDataInputStream {
public:
    virtual ~CDataInputStream();
    unsigned long        StreamPos;                         // +0x04
    long                 StreamSize;                        // +0x08
    const unsigned char* CurrentSourcePtr;                  // +0x0C
    unsigned long        SourceChunkStreamPos;              // +0x10
    long                 SourceChunkBytesRemaining;         // +0x14
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    template <class T> __forceinline T ReadType()
    {
        T value;
        Read(&value, sizeof(T));
        return value;
    }
    __forceinline long ReadSLONG() { return ReadType<long>(); }
private:
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
};
class CMemoryDataInputStream : public CDataInputStream {
public:
    unsigned char* StreamSource;                            // +0x18
};
class CDataOutputStream {
public:
    void WriteSLONG(long value);                            // 0x00993EB0
};

class CStringParser;
class CSymbolMap;
class CPersistContext {
public:
    enum EMode { MODE_BINARY_LOAD = 2 };                    // enumerator spelling descriptive
    CDefinitionManager*      PDefinitionManager;            // +0x00
    CSymbolMap*              PSymbolMap;                    // +0x04
    CStringParser*           PParser;                       // +0x08
    void (__cdecl*           ErrorCallback)(CCharString*);  // +0x0C
    CCharString*             PStringToClean;                // +0x10
    bool                     UseDefaults;                   // +0x14
    bool                     UseCRCCheck;                   // +0x15
    bool                     SingleTransfer;                // +0x16
    bool                     Finished;                      // +0x17
    EMode                    Mode;                          // +0x18
    bool                     Binary;                        // +0x1C
    CCharString*             PStringToSaveTo;               // +0x20
    CMemoryDataInputStream*  PLoadStream;                   // +0x24
    CDataOutputStream*       PSaveStream;                   // +0x28
    template <class T> void TransferVectorOfSubComponents(std::vector<T>& value);
protected:
    void CheckCRC(const char* name);                        // 0x00404500
};

template <class T>
void CPersistContext::TransferVectorOfSubComponents(std::vector<T>& value)
{
    CheckCRC("");
    switch (Mode) {
    case MODE_BINARY_LOAD: {
        long count = PLoadStream->ReadSLONG();
        value.resize(count, T());
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

template void CPersistContext::TransferVectorOfSubComponents<CCardDef>(std::vector<CCardDef>& value);
