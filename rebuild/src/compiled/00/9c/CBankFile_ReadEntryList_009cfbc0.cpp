// 009cfbc0 CBankFile::ReadEntryList
// Leaf: ?ReadEntryList@CBankFile@@AAE_NAAVCFileDataInputStream@@KK@Z
// Flags: none
// FableWin 02F9F977 compares the source address to this, in that order.
#define FABLE_RETAIL_BANKFILE_FILENAME_COPY_SITE_PASS26
#define FABLE_RETAIL_STLP_FILL_CHAR
#define FABLE_RETAIL_BANKFILE_INDEX_UPDATE_PASS26
#define FABLE_RETAIL_CBANKFILE_PACKRUNTIMEDATA
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CARRAY_DEALLOCATEUNUSEDMEMORY
#define FABLE_RETAIL_CBANKFILE_READENTRIES_PASS26
#define FABLE_RETAIL_CBANKFILE_FILENAMEINDICES
#define FABLE_RETAIL_CBANKFILE_ADDENTRYMULTIPLEFILES
#define FABLE_RETAIL_CBANKFILE_CRUNTIMEDATA_CTOR
#define FABLE_RETAIL_CBANKFILE_ALLOCATEBUFFERS
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEENTRYUPDATEDATA_RETAIL
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
#define FABLE_RETAIL_CFILEDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_LOADGAME_PASS24
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_ORIGINAL_USER_SPECIAL_MEMBERS_PASS22
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#include <stddef.h>
#include <string.h>
void* operator new[](size_t);
void operator delete[](void*) throw();
#include "engine/CBankFileDataTypeCount.h"
#include "engine/CVectorMap.h"
#include "engine/CBankFile.h"
#include "engine/CBankFileEntryUpdateData.h"
#include "engine/CFileDataInputStream.h"

// Original donor 02FA2EA0 clears the previous array, allocates count strings and narrows the stored size.
template<class T,int N> void CSmallVector<T,N>::Allocate(unsigned long count)
{
    Clear();
    Start = new T[count];
    Size = (signed char)count;
}

// Original donor 02FA2FA0 first resizes, then assigns elements in ascending index order.
template<class T,int N> void CSmallVector<T,N>::Copy(const CSmallVector& other)
{
    Allocate(other.Size);
    for(unsigned long i=0; i<(unsigned long)other.Size; ++i)
        Start[i]=other.Start[i];
}

// FableWin 02F9C690 calls Update, the storage trim and optional runtime packing in order.
inline void CBankFile::PostInitEntries()
{
    SymbolCRCIndices.Update();
    SymbolCRCIndices.DeallocateUnusedMemory();
    if(ReadOnly) PackRuntimeData();
}

bool CBankFile::ReadEntryList(CFileDataInputStream& is,unsigned long max_index,unsigned long entry_count)
{
    AllocateBuffers(max_index+1);
    unsigned long no_types=is.ReadULONG();
    CArray<CBankFileDataTypeCount> datatype_counts(no_types);
    for(unsigned long c_type=0;c_type<no_types;++c_type) {
        datatype_counts[c_type].DataType=is.ReadULONG();
        datatype_counts[c_type].Count=is.ReadULONG();
    }
    NotifyBankSize(GetSize(),datatype_counts);
    for(unsigned long c_entry=0;c_entry<entry_count;++c_entry) {
        unsigned long magic=is.ReadULONG();
        unsigned long index=is.ReadULONG();
        unsigned long data_type=is.ReadULONG();
        unsigned long data_size=is.ReadULONG();
        unsigned long data_offset=is.ReadULONG();
        unsigned long state_block_crc=is.ReadULONG();
        CCharString symbol=is.ReadPresizedString();
        unsigned long checksum=is.ReadULONG();
        unsigned long file_count=is.ReadULONG();
        CSmallVector<CCharString,8> filenames(file_count);
        for(unsigned long c_file=0;c_file<file_count;++c_file) filenames[c_file]=is.ReadPresizedString();
        unsigned long info_size=is.ReadULONG();
        CArray<char> info_buffer(info_size);
        is.Read(info_buffer.GetAsCArray(),info_size);
        CRuntimeData& runtime_data=RuntimeData[index];
        runtime_data.DataType=(unsigned char)data_type;
        runtime_data.DataSize=data_size;
        runtime_data.DataOffset=data_offset;
        runtime_data.Valid=true;
        if(OpenFlags&8) Symbols[index]=symbol;
        if(OpenFlags&16) Checksums[index]=checksum;
        if(OpenFlags&32) {
            CBankFileEntryUpdateData* update_data=new CBankFileEntryUpdateData;
            update_data->SetStateBlockCRC(state_block_crc);
            update_data->SetInfo(info_buffer);
            update_data->Filenames=filenames;
            UpdateData[index]=update_data;
        }
        CCharString filename=filenames.empty() ? CCharString("") : filenames[0];
        InitEntry(index,symbol,filename);
        unsigned long current_offset=is.GetPosition();
        NotifyEntryAdded(data_type,index,info_buffer);
        is.SetPosition(current_offset);
    }
    PostInitEntries();
    NotifyPostEntriesLoaded();
    FileValid=true;
    return true;
}