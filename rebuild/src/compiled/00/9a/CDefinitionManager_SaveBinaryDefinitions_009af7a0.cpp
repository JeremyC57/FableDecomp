// 009af7a0 CDefinitionManager::SaveBinaryDefinitions
// Leaf: ?SaveBinaryDefinitions@CDefinitionManager@@QAEXAAVCDataOutputStream@@@Z
// Flags: none
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
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_WRITESECTIONS
#define FABLE_RETAIL_CARRAY_DEALLOCATEUNUSEDMEMORY
#define FABLE_RETAIL_CARRAY_GETBYTESIZE_PASS23
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#define FABLE_RETAIL_CARRAY_PUSHBACK
#define FABLE_RETAIL_CARRAY_ORIGINAL_USER_SPECIAL_MEMBERS_PASS22
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE
#define FABLE_RETAIL_CMEMORYALLOCATOR_SAVEBINARY_PASS23
#include "engine/CDataOutputStream.h"
#include "engine/CMemoryDataOutputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CDefinitionManager.h"
#include "engine/CDefClassBase.h"
#include "engine/CMemoryAllocator.h"

void CDefinitionManager::SaveBinaryDefinitions(CDataOutputStream& os)
{
 GDefStringTable.SaveTable(false);
 os.WriteEBOOL(UseSafeBinaryFormat);
 os.WriteULONG(DependencyCheckCRC);
 os.WriteULONG(GDefStringTable.GetTableRandomID());
 unsigned long no_defs=InstantiatedDefs.size();
 os.WriteULONG(no_defs);
 CMemoryDataOutputStream uncompressed_buffer;
 CArray<unsigned short> def_offsets;
 CompressedDefData.clear();CompressedOffsets.clear();
 unsigned long first_compressed_def=0;
 for(long c_def=0;c_def<no_defs;++c_def){
  CInstantiatedDefInfo& def_info=*InstantiatedDefs[c_def];
  def_info.ClassName.TransferBinaryOut(os);
  def_info.InstantiationName.TransferBinaryOut(os);
  os.WriteULONG(def_info.ClassIndex);
  CMemoryDataOutputStream def_os;
  def_info.PDef->InternalSaveBinary(def_os);
  CMemoryDataOutputStream transfer_os;
  CPersistContext transfer_os_context;
  transfer_os_context.Init(transfer_os,UseSafeBinaryFormat);
  def_info.PDef->Transfer(transfer_os_context);
  unsigned long def_transfer_size=transfer_os.GetLength();
  def_os.Write(transfer_os.PeekData(),def_transfer_size);
  unsigned long current_size=uncompressed_buffer.GetLength()+def_offsets.GetByteSize();
  unsigned long projected_size=def_os.GetLength()+current_size+2;
  if(current_size>0&&projected_size>0x8000){
   CompressDefinitionBlock(uncompressed_buffer,def_offsets,first_compressed_def);
   first_compressed_def=c_def;
  }
  unsigned long current_offset=uncompressed_buffer.GetLength();
  def_offsets.push_back((unsigned short)current_offset);
  uncompressed_buffer.Write(def_os.PeekData(),def_os.GetLength());
 }
 CompressDefinitionBlock(uncompressed_buffer,def_offsets,first_compressed_def);
 CompressedOffsets.Insert(no_defs,CompressedDefData.size());
 CompressedOffsets.Update();
 CompressedDefData.DeallocateUnusedMemory();CompressedOffsets.DeallocateUnusedMemory();
 os.WriteULONG(CompressedOffsets.size());
 os.Write(CompressedOffsets.GetAsCArray(),CompressedOffsets.GetByteSize());
 os.WriteSLONG(CompressedDefData.size());
 os.Write(CompressedDefData.GetAsCArray(),CompressedDefData.GetByteSize());
 long mem_start=0,areas_start=0;
 if(CMemoryAllocator::Get()){
  mem_start=CMemoryAllocator::Get()->GetTotalMemoryAllocated();
  areas_start=CMemoryAllocator::Get()->GetNoAllocatedAreas();
 }
 for(long c_def=0;c_def<no_defs;++c_def){
  CInstantiatedDefInfo& def_info=*InstantiatedDefs[c_def];
  def_info.PDef.Reset(0);
 }
 if(CMemoryAllocator::Get()){
  long mem_freed=mem_start-CMemoryAllocator::Get()->GetTotalMemoryAllocated();
  long areas_freed=areas_start-CMemoryAllocator::Get()->GetNoAllocatedAreas();
 }
}