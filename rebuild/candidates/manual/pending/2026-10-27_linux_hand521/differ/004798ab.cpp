// CPersistContext::TransferVectorOfSubComponents(std::vector<CEngineLocalDetailLayerDef>& value) -- binary persist of a vector of CEngineLocalDetailLayerDef sub-components: any mode but binary load writes the count then each element's own virtual Transfer; binary load reads the count, resizes with a default element and transfers each element
// Grade: DIFFER(244v245) -- the inline CDataInputStream::Read of the binary-load count keeps the new stream position in eax and the zero high dword in edx (`add eax,4 ; xor edx,edx`, one byte short); retail computes it into edx (`lea edx,[eax+4] ; xor eax,eax ; ... cmp edx,7FFFFFFFh`): a register-allocation tie, shared with parks 00476e3e / 00477cd2; everything else matches
// Leaf: ??$TransferVectorOfSubComponents@VCEngineLocalDetailLayerDef@@@CPersistContext@@QAEXAAV?$vector@VCEngineLocalDetailLayerDef@@V?$allocator@VCEngineLocalDetailLayerDef@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004798AB, 245 bytes, thiscall `ret 4`; size-optimised def TU (`dec eax ; dec eax`, `push N ; pop ecx` divisor).
// Identity (proven): CPersistContext::TransferVectorOfSubComponents<CEngineLocalDetailLayerDef>; evidence: (1) the Ego_r/FableWin.pdb name sets hold
//   ??$TransferVectorOfSubComponents@VCEngineLocalDetailLayerDef@@@CPersistContext@@QAEXPBDAAV?$vector@VCEngineLocalDetailLayerDef@@...@Z; TLC retail dropped the
//   persist-name argument (the body checks the pooled empty key 0x0122D70E, `ret 4`), so the retail symbol keeps the
//   template name and the vector parameter only; (2) the element is CEngineLocalDetailLayerDef: the size() divide and stride are 0x54 bytes
//   (Ego_r.pdb 96 minus 4 for the retail CVectorMap minus 4 per STLport vector member), the default element's vtable
//   0x012362E4 is .?AVCEngineLocalDetailLayerDef@@ (RTTI, base CParentDefClassBase), and the out-of-line resize 0x004799C6 is the landed / bound
//   vector<CEngineLocalDetailLayerDef>::resize; (3) its only caller is its def Transfer (site 0x004798A3).
// Label: the catalog's "TransferVectorOfSubComponents<CAbilityButtonGuiDef>" is propagated across the family; earlier bindings spell some
//   members of the family as a non-template CPersistContext::Transfer(vector<T>&) overload (no PDB support).
// Callees: CPersistContext::CheckCRC 0x00404500 (protected, PDB), CDataOutputStream::WriteSLONG 0x00993EB0,
//   CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (private, PDB), std::vector<CEngineLocalDetailLayerDef>::resize(n, const T&) 0x004799C6,
//   CParentDefClassBase::CParentDefClassBase() 0x00430370 / ~CParentDefClassBase() 0x00430300 (vtable 0x01230CBC, RTTI),
//   and each element's virtual Transfer(CPersistContext&) (def vtable slot 18, +0x48).
// Shape: a switch on Mode with only `case 2` (binary load) and a default (the save arm is the fall-through); the count is a
//   named local (down-counter homed in the dead `value` slot); ReadSLONG -> ReadType<long> -> Read inline (the __int64
//   position check, the 4-byte fast path, else ReadWithSrcChunkOverflow); `value.resize(count)` is STLport's inline
//   resize(n) = resize(n, _Tp()): T() 0x004799A0 and ~T() 0x0047747C are called out of line (T owns an STLport vector).
// Layout: CPersistContext from Ego_r.pdb (44 bytes, no engine header); the def base chain from RTTI with the PDB members, retail
//   CVectorMap 0x10 (CParentDefClassBase 0x39, derived members from +0x3C); virtual slots from the retail vtables (0x01230CBC) and
//   the PDB virtual names (slots 14/15 GetPSubDefInfo order probable).
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {          // STLport 4.6, 12 bytes
public:
    typedef _Tp& reference;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    pointer _M_start;
    pointer _M_finish;
    _STLP_alloc_proxy<pointer, _Alloc> _M_end_of_storage;
    iterator begin() { return this->_M_start; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    reference operator[](size_type __n) { return *(begin() + __n); }
    void resize(size_type __new_size, const _Tp& __x);                          // out of line (0x004799C6 for CEngineLocalDetailLayerDef)
    __forceinline void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}   // namespace std

template <class T> class CArray : public std::vector<T> { };   // 12 bytes in retail (16 in Ego_r.pdb)
class CCharString;
class CDefString {                                       // Ego_r.pdb, 4 bytes
public:
    long TablePos;
};
class C2DVector {                                        // Ego_r.pdb, 8 bytes
public:
    float X;
    float Y;
};
class CRGBColour {                                       // Ego_r.pdb, 4 bytes (an anonymous union of the channel bytes and the packed dword)
public:
    unsigned long Colour;
};
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes (natural packing)
public:
    CArray<std::pair<K, V> > Data;                       // +0x00
    TCompare                 Compare;                    // +0x0C
    bool                     Dirty;                      // +0x0D
};
class CPersistContext;
class CDataOutputStream;
class CValidateDefinitionInitBase;

class CIVCountedPointeeBase {                             // Ego_r.pdb 8 bytes
public:
    virtual ~CIVCountedPointeeBase();                    // slot 0
    virtual void Delete() const;                         // slot 1
    unsigned long IVRefCount;                            // +0x04
};
class CResourceList;
class CResource : public CIVCountedPointeeBase {          // Ego_r.pdb 0x1C
public:
    virtual bool UnloadResource();                       // slot 2
    virtual unsigned long GetUnloadableMemory() const;   // slot 3
    virtual bool IsResourceLoaded() const;               // slot 4
    virtual void OnResourceLoaded();                     // slot 5
    virtual bool CanAbortResourceLoad(long) const;       // slot 6
    virtual void OnResourceLoadAborted();                // slot 7
    virtual unsigned long GetDebugMemoryInUse() const;   // slot 8
    virtual void ResetDebugMemoryInUse();                // slot 9
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0C
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
};
class CDefPointeeBase : public CResource {               // vtable 0x01230BA0
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {           // vtable 0x01230BCC, 0x25 bytes (packed)
public:
    class CSubDefInfo { public:
        long DefIndex;
        long OriginalParentDefIndex;
    };
    virtual CDefString GetInstantiationName() const;                                          // slot 10
    virtual void SetInstantiationName(CDefString name);                                       // slot 11
    virtual void AddSubDef(const CCharString& name, long def_index, long parent_index);       // slot 12
    virtual void RemoveSubDef(const CCharString& name);                                       // slot 13
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString& name) const;                 // slots 14/15
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString& name);
    virtual bool IsParentDef() const;                                                         // slot 16
    virtual void PostTransfer();                                                              // slot 17
    virtual void Transfer(CPersistContext& context) = 0;                                      // slot 18
    virtual void Copy(const CDefClassBase* other) = 0;                                        // slot 19
    virtual unsigned long GetSizeofClass() = 0;                                               // slot 20
    virtual void InternalSaveBinary(CDataOutputStream& out);                                  // slot 21
    virtual void InternalLoadBinary(class CDataInputStream& in);                              // slot 22
    virtual void Validate(const CValidateDefinitionInitBase& init, const CCharString& name) const;   // slot 23
    virtual CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> >* GetPSubDefInfoMap();   // slot 24
    CDefinitionManager* PDefManager;                     // +0x1C
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
class CParentDefClassBase : public CDefClassBase {       // vtable 0x01230CBC, retail 0x39 (packed)
public:
    CParentDefClassBase();                               // 0x00430370
    virtual ~CParentDefClassBase();                      // 0x00430300
    CDefString InstantiationName;                        // +0x25
    CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> > SubDefInfoMap;   // +0x29
};
#pragma pack(pop)

class CEngineLocalDetailObjectDef;                       // element of CEngineLocalDetailLayerDef::Objects
class CEngineLocalDetailLayerDef : public CParentDefClassBase {    // vtable 0x012362E4, retail 0x54 bytes
public:
    CEngineLocalDetailLayerDef();                      // 0x004799A0 (implicit in retail; its own COMDAT because of the vector member)
    virtual ~CEngineLocalDetailLayerDef();              // 0x0047747C (implicit in retail, own COMDAT: frees the vector buffer(s), then ~CParentDefClassBase)
    virtual void Transfer(CPersistContext& context);     // slot 18
    virtual void Copy(const CDefClassBase* other);       // slot 19
    virtual unsigned long GetSizeofClass();              // slot 20
    std::vector<float>                       SpacingFromLayer;          // +0x3C (Ego_r.pdb +0x40)
    std::vector<CEngineLocalDetailObjectDef> Objects;                   // +0x48 (Ego_r.pdb +0x50)
};

class CBaseClass { public: virtual ~CBaseClass(); };                  // 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { };
class CDataStream : public CBaseClassNonCopyable { };                 // vptr +0x00
class CDataInputStream : public CDataStream {            // Ego_r.pdb 0x18
public:
    unsigned long  StreamPos;                            // +0x04
    long           StreamSize;                           // +0x08
    unsigned char* CurrentSourcePtr;                     // +0x0C
    unsigned long  SourceChunkStreamPos;                 // +0x10
    long           SourceChunkBytesRemaining;            // +0x14
    static __forceinline bool IsValidPosition(__int64 pos) { return pos >= 0 && pos <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 new_pos = (__int64)(StreamPos + size);
        if (!IsValidPosition(new_pos))
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
    template <class T> __forceinline T ReadType() { T value; Read(&value, sizeof(T)); return value; }
    __forceinline long ReadSLONG() { return ReadType<long>(); }
private:
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
};
class CMemoryDataInputStream : public CDataInputStream { };
class CDataOutputStream : public CDataStream {
public:
    void WriteSLONG(long value);                         // 0x00993EB0
};
class CStringParser;
class CSymbolMap;
class CPersistContext {                                  // Ego_r.pdb 44 bytes
public:
    enum EMode { };
    CDefinitionManager*     PDefinitionManager;          // +0x00
    CSymbolMap*             PSymbolMap;                  // +0x04
    CStringParser*          PParser;                     // +0x08
    void (__cdecl* ErrorCallback)(CCharString*);         // +0x0C
    CCharString*            PStringToClean;              // +0x10
    bool                    UseDefaults;                 // +0x14
    bool                    UseCRCCheck;                 // +0x15
    bool                    SingleTransfer;              // +0x16
    bool                    Finished;                    // +0x17
    EMode                   Mode;                        // +0x18 (2 = binary load)
    bool                    Binary;                      // +0x1C
    CCharString*            PStringToSaveTo;             // +0x20
    CMemoryDataInputStream* PLoadStream;                 // +0x24
    CDataOutputStream*      PSaveStream;                 // +0x28
    template <class T> void TransferVectorOfSubComponents(std::vector<T>& value);
protected:
    void CheckCRC(const char* name);                     // 0x00404500
};

template <class T>
void CPersistContext::TransferVectorOfSubComponents(std::vector<T>& value)
{
    CheckCRC("");
    switch (Mode) {
    case 2: {
        long count = PLoadStream->ReadSLONG();
        value.resize(count);
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

template void CPersistContext::TransferVectorOfSubComponents<CEngineLocalDetailLayerDef>(std::vector<CEngineLocalDetailLayerDef>& value);
