// 009afb90 CDefinitionManager::LoadBinaryDefinitions
// Leaf: ?LoadBinaryDefinitions@CDefinitionManager@@QAE_NAAVCDataInputStream@@@Z
// Flags: /Gr
#define FABLE_RETAIL_STLP_DEFINITION_STREAM_BYTE_COPIES
#define FABLE_RETAIL_STLP_VECTOR_ULONG_ERASE_COPY_PASS27
#define FABLE_RETAIL_STLP_ORIGINAL_COPY_TRIVIAL_BACKWARD_PASS22
#define FABLE_RETAIL_STLP_VECTOR_ORIGINAL_RESIZE_INSERT_CHAIN_PASS22
#define FABLE_RETAIL_CDEFSTRING_TRANSFER
#define FABLE_RETAIL_CDATAINPUTSTREAM_TEXT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER_SAVEBINARY_PASS23
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRINGTABLE_SAVEBINARY_PASS23
#define FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CDEFPOINTER_RESET
#define FABLE_RETAIL_CCREATUREDEF_TRANSFER
#define FABLE_RETAIL_CARRAY_DEALLOCATEUNUSEDMEMORY
#define FABLE_RETAIL_CARRAY_GETBYTESIZE_PASS23
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_CARRAY_ORIGINAL_USER_SPECIAL_MEMBERS_PASS22
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CDEFINITIONMANAGER_LOADBINARY_PASS29
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#include "fable_assert.h"
#include "engine/CDataInputStream.h"
#include "engine/CDefinitionManager.h"
#include "engine/CDefClassBase.h"
#include "engine/NProgressDisplay.h"

#if defined(_MSC_VER)
#pragma optimize("s",on)
#pragma optimize("y",off)
#endif
#ifdef _MSC_VER
#define FABLE_STLP_DEFLOAD_CALLED __declspec(noinline)
#else
#define FABLE_STLP_DEFLOAD_CALLED
#endif
namespace std {
// Inline-budget: native 00464afe calls malloc from the overflow body.
template<> FABLE_FORCEINLINE unsigned long* allocator<unsigned long>::allocate(size_t n,const void*) {return n?(unsigned long*)malloc(n*sizeof(unsigned long)):0;}
#if defined(_MSC_VER)
#pragma optimize("",on)
#endif
// Inline-budget: native 00464ace expands the fill loop between the two copy sites.
template<> FABLE_FORCEINLINE unsigned long* fill_n(unsigned long* first,unsigned int n,const unsigned long& x){for(;n>0;--n,++first)*first=x;return first;}
#if defined(_MSC_VER)
#pragma optimize("s",on)
#pragma optimize("y",off)
#endif
// Inline-budget: the leaf calls this overflow at 009afe94.
template<> inline FABLE_STLP_DEFLOAD_CALLED void vector<unsigned long>::_M_insert_overflow(iterator pos, const unsigned long& x, const __true_type&, size_type fill_len, bool at_end)
{
    const size_type old_size=size();
    const size_type len=old_size+(max)(old_size,fill_len);
    iterator new_start=_M_end_of_storage.allocate(len);
    iterator new_finish=(iterator)__copy_trivial(_M_start,pos,new_start);
    new_finish=fill_n(new_finish,fill_len,x);
    if (!at_end)
        new_finish=(iterator)__copy_trivial(pos,_M_finish,new_finish);
    _M_clear();
    _M_set(new_start,new_finish,new_start+len);
}
}
#if defined(_MSC_VER)
#pragma optimize("",on)
#endif
#if defined(_MSC_VER)
#pragma optimize("s",on)
#endif
// Inline-budget: the byte-vector resize calls the 30-byte erase specialization.
template<> inline FABLE_STLP_DEFLOAD_CALLED unsigned char* std::vector<unsigned char>::erase(iterator first,iterator last) {
#if defined(FABLE_RETAIL_STLP_DEFINITION_STREAM_BYTE_COPIES) && defined(_MSC_VER)
// Inline-budget: the retail byte-vector owner calls the shared pointer-copy routine here.
#pragma inline_depth(0)
#endif
    this->_M_finish=(unsigned char*)__copy_trivial(last,this->_M_finish,first);
#if defined(FABLE_RETAIL_STLP_DEFINITION_STREAM_BYTE_COPIES) && defined(_MSC_VER)
#pragma inline_depth()
#endif

    return first;
}
#if defined(_MSC_VER)
#pragma optimize("",on)
#endif
#define FABLE_RETAIL_STLP_ANIMATION_BYTE_VECTOR_OWNERS
#include "stlport/fable_animation_byte_vector_owners.h"
CDefinitionManager::CDefClassInfo* CDefinitionManager::GetPDefClassInfo(unsigned long crc)
{
    CVectorMap<unsigned long,CDefClassInfo>::iterator it = DefClassInfo.Find(crc);
    if (it == DefClassInfo.end())
        return 0;
    return &it->second;
}
bool CDefinitionManager::LoadBinaryDefinitions(CDataInputStream& is)
{
    if (is.IsEOF())
        return false;
    bool safe_format = is.ReadEBOOL();
    unsigned long saved_crc = is.ReadULONG();
    if (DependencyCheckCRC && saved_crc != DependencyCheckCRC)
        return false;
    unsigned long string_table_id = is.ReadULONG();
    if (string_table_id != GDefStringTable.GetTableRandomID())
        return false;
    UseSafeBinaryFormat = safe_format;
    ClearInstantiatedDefs();
    unsigned long no_defs = is.ReadULONG();
    InstantiatedDefs.resize(no_defs);
    for (long c_def = 0; c_def < no_defs; ++c_def) {
        NProgressDisplay::DisplayProgress("Definition Manager: Load Binary Def", -1.0f, false, false);
        InstantiatedDefs[c_def] = new CInstantiatedDefInfo;
        CInstantiatedDefInfo& def_info = *InstantiatedDefs[c_def];
        def_info.ClassName.TransferBinaryIn(is);
        def_info.InstantiationName.TransferBinaryIn(is);
        def_info.ClassIndex = is.ReadULONG();
        CDefClassInfo* pdef_type_info = GetPDefClassInfo(def_info.ClassName.GetCRC());
        if (!pdef_type_info) {
            FABLE_RETAIL_ASSERT(false);
            continue;
        }
        long class_index = pdef_type_info->GlobalIndices.size();
        FABLE_RETAIL_ASSERT(class_index == def_info.ClassIndex);
        pdef_type_info->GlobalIndices.push_back(c_def);
        NameCRCToIndexMap.Insert(def_info.InstantiationName.GetCRC(), c_def);
    }
    unsigned long offsets_size = is.ReadULONG();
    CompressedOffsets.resize(offsets_size);
    is.Read(CompressedOffsets.GetAsCArray(), CompressedOffsets.GetByteSize());
    unsigned long data_size = is.ReadULONG();
    CompressedDefData.resize(data_size);
    is.Read(CompressedDefData.GetAsCArray(), CompressedDefData.GetByteSize());
    NameCRCToIndexMap.Update();
    return true;
}