// CPersistContext::TransferVectorOfSubComponents<CFeatDef>(const char* name, std::vector<CFeatDef>& value) -- persist a vector of CFeatDef sub-component defs: CRC-check the key, then by mode text load (through the transferable-vector wrapper) / binary load (count, resize, each element's Transfer) / binary save (count, each element) / reset pass (each element)
// Leaf: ??$TransferVectorOfSubComponents@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F34330, 400 bytes incl. the 5-entry switch table at 0x00F344AC (entry 1 is 0: no
//   text-save arm), `ret 8`. PDB spelling by the Ego_r pattern of its siblings (??$TransferVectorOfSubComponents@VCArenaRoundDef@@@...):
//   ??$TransferVectorOfSubComponents@VCFeatDef@@@CPersistContext@@QAEXPBDAAV?$vector@VCFeatDef@@V?$allocator@VCFeatDef@@@std@@@std@@@Z
// Identity (proven): (1) the sole caller 0x00F332CE (the owning def's Transfer, un-catalogued, right before the landed
//   TransferVectorOfSubComponents<CArenaRoundDef> call at 0x00F332E1 with "ArenaRounds") passes the key 0x012FBA04 and the owner's
//   vector at +0x1038; (2) the element stride 0x64 (the 0x51EB851F / sar 5 count division) is sizeof(CFeatDef) (Ego_r.pdb 100:
//   CDefClassBase 0x28 + FeatName .. CO_ItemName), the wrapper vtable 0x01301C54 holds the catalogued
//   CPersistContext_TransferableVectorOfSubComponents<CFeatDef> slots (0x00F344F0 Size, 0x00F349B0 Resize, 0x00F34510
//   AreDefaultValsApplied, 0x00F34530 SetDefaultValsApplied, 0x00F34560 Transfer; Ego_r names), and the element ctor / resize are
//   0x00F344C0 (??0CFeatDef@@QAE@XZ; the catalog's CAIStateGroup_HoverHornetQueen / CVtable012fb778 labels are propagated) and
//   0x00F34580 (?resize@?$vector@VCFeatDef@@V?$allocator@VCFeatDef@@@std@@@std@@QAEXIABVCFeatDef@@@Z). CFeatDef declares no
//   destructor: the default element's implicit ~CFeatDef is expanded inline (the CDefPointeeBase vtable 0x01230BA0 store, then
//   CResource::~CResource 0x009FC550). The catalog label "Transfer<signed_char>" is propagated (NAME_FIXES.tsv).
// Callees: CPersistContext::CheckCRC 0x00404500, CPersistContext::TransferVectorOfSubComponentsLoadText 0x009BC120, CDataOutputStream::
//   WriteLong 0x00993EB0, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, CFeatDef() 0x00F344C0, vector<CFeatDef>::resize(n, x)
//   0x00F34580, CResource::~CResource 0x009FC550, the element's virtual Transfer(CPersistContext&) (CDefClassBase slot 18, +0x48).
// Shape: as the landed TransferVectorOfSubComponents<CArenaRoundDef> 0x00F349F0 template with STLport's two-step resize(n) ->
//   resize(n, _Tp()) over a def with an implicit destructor (engine/CDefClassBase.h retail chain).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CPersistContext;
class CDataInputStream { friend class CMemoryDataInputStream;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
class CMemoryDataInputStream : public CDataInputStream { public:
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14 bytes left in the current chunk
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline long ReadLong()
    {
        long value;
        Read(&value, sizeof(value));
        return value;
    }
};
class CDataOutputStream { public:
    void WriteLong(long value);            // 0x00993EB0
};

namespace std {
template <class _Tp> class allocator { public:
    typedef _Tp value_type;
    typedef unsigned int size_type;
    allocator() { }
    allocator(const allocator<_Tp>&) { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base { public:
    typedef _Alloc allocator_type;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {   // STLport 4.6, 12 bytes
public:
    typedef _Tp value_type;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    void resize(size_type __new_size, const _Tp& __x);     // out of line in retail (stl/_vector.h: erase / insert)
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}

class CPersistContext_TransferableVectorOfSubComponentsGenericType { public:   // Ego_r ??0...GenericType@@QAE@XZ
    CPersistContext_TransferableVectorOfSubComponentsGenericType() { }
private:
    virtual long Size() = 0;                                              // slot 0
    virtual void Resize(long) = 0;                                        // slot 1
    virtual bool AreDefaultValsApplied(long) = 0;                         // slot 2
    virtual void SetDefaultValsApplied(long, bool) = 0;                   // slot 3
    virtual void Transfer(CPersistContext&, long) = 0;                    // slot 4
};
template <class T> class CPersistContext_TransferableVectorOfSubComponents : public CPersistContext_TransferableVectorOfSubComponentsGenericType { public:
    CPersistContext_TransferableVectorOfSubComponents(std::vector<T>& v) : TransVector(&v) { }
private:
    virtual long Size();                                                  // EAEJXZ
    virtual void Resize(long);                                            // EAEXJ@Z
    virtual bool AreDefaultValsApplied(long);                             // EAE_NJ@Z
    virtual void SetDefaultValsApplied(long, bool);                       // EAEXJ_N@Z
    virtual void Transfer(CPersistContext&, long);                        // EAEXAAVCPersistContext@@J@Z
    std::vector<T>* TransVector;                                          // +4 (Ego_r layout)
};

class CDefinitionManager; class CSymbolMap; class CStringParser; class CCharString;
class CPersistContext { public:                                           // Ego_r layout, 0x2c
    enum EMode { MODE_LOAD_TEXT = 0, MODE_SAVE_TEXT = 1, MODE_LOAD_BINARY = 2, MODE_SAVE_BINARY = 3, MODE_RESET = 4 };
    CDefinitionManager*     PDefinitionManager;   // +0x00
    CSymbolMap*             PSymbolMap;           // +0x04
    CStringParser*          PParser;              // +0x08
    void (__cdecl* ErrorCallback)(CCharString*);  // +0x0c
    CCharString*            PStringToClean;       // +0x10
    bool                    UseDefaults;          // +0x14
    bool                    UseCRCCheck;          // +0x15
    bool                    SingleTransfer;       // +0x16
    bool                    Finished;             // +0x17
    EMode                   Mode;                 // +0x18
    bool                    Binary;               // +0x1c
    CCharString*            PStringToSaveTo;      // +0x20
    CMemoryDataInputStream* PLoadStream;          // +0x24
    CDataOutputStream*      PSaveStream;          // +0x28
    template <class T> void TransferVectorOfSubComponents(const char* name, std::vector<T>& value);
protected:
    void CheckCRC(const char* name);                                                                                    // 0x00404500
    void TransferVectorOfSubComponentsLoadText(const char* name, CPersistContext_TransferableVectorOfSubComponentsGenericType& item);   // 0x009BC120
};

template <class T>
void CPersistContext::TransferVectorOfSubComponents(const char* name, std::vector<T>& value)
{
    CheckCRC(name);
    switch (Mode) {
    case MODE_RESET: {
        long count = value.size();
        for (long i = 0; i < count; ++i)
            value[i].Transfer(*this);
        break;
    }
    case MODE_SAVE_BINARY: {
        long count = value.size();
        PSaveStream->WriteLong(count);
        for (long i = 0; i < count; ++i)
            value[i].Transfer(*this);
        break;
    }
    case MODE_LOAD_BINARY: {
        long count = PLoadStream->ReadLong();
        value.resize(count);
        for (long i = 0; i < count; ++i)
            value[i].Transfer(*this);
        break;
    }
    case MODE_LOAD_TEXT: {
        CPersistContext_TransferableVectorOfSubComponents<T> item(value);
        TransferVectorOfSubComponentsLoadText(name, item);
        break;
    }
    default:
        __assume(0);
    }
}

class CDefString { public:                                    // Ego_r.pdb: one long (a string-table position)
    long TablePos;
};
enum EFeatAttackType { FEAT_ATTACK_TYPE_NONE };
class CFeatDef : public CDefClassBase { public:               // Ego_r.pdb, 0x64 bytes, no user destructor
    CFeatDef();                                               // 0x00F344C0
    virtual void Transfer(CPersistContext& context);          // slot 18 (Ego_r ?Transfer@CFeatDef@@UAEXAAVCPersistContext@@@Z)
    virtual void Copy(const CDefClassBase* other);            // slot 19 (Ego_r ?Copy@CFeatDef@@UAEXPBVCDefClassBase@@@Z)
    virtual unsigned long GetSizeofClass();                   // slot 20 (Ego_r ?GetSizeofClass@CFeatDef@@UAEKXZ)
    CDefString      FeatName;                                 // +0x28
    CDefString      Verb;                                     // +0x2c
    float           TimeLimit;                                // +0x30
    long            TargetNumber;                             // +0x34
    long            GoldReward;                               // +0x38
    long            XPReward;                                 // +0x3c
    CDefString      ItemReward;                               // +0x40
    bool            NoBlocking;                               // +0x44
    EFeatAttackType KN_AttackType;                            // +0x48
    bool            KN_Perfect;                               // +0x4c
    long            KN_CreatureType;                          // +0x50
    CDefString      GF_FromRegion;                            // +0x54
    CDefString      GF_ToRegion;                              // +0x58
    bool            GF_NoTeleporting;                         // +0x5c
    CDefString      CO_ItemName;                              // +0x60
};
FABLE_STATIC_ASSERT(sizeof(CFeatDef) == 0x64);

template void CPersistContext::TransferVectorOfSubComponents<CFeatDef>(const char* name, std::vector<CFeatDef>& value);