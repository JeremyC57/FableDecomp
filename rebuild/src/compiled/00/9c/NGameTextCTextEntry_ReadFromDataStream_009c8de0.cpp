// NGameText::CTextEntry::ReadFromDataStream -- deserialize text and tags under their string allocation category.
// Leaf: ?ReadFromDataStream@CTextEntry@NGameText@@QAEXAAVCDataInputStream@@@Z
// Flags: (none)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STRING_MEMTRACKING_PASS26
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCE_SETRESOURCESIZE
#define FABLE_RETAIL_NGAMETEXT_GETPTEXTENTRY
#define FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
#define FABLE_RETAIL_NGAMETEXT_TEXTENTRY_READ_PASS26
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CDATAINPUTSTREAM_LOADGAME_PASS24
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_VECTOR_CHAIN_PASS22
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "fable_string.h"
#include "engine/CDataInputStream.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CScopedMemTrackingID.h"
void NGameText::CTextEntry::ReadFromDataStream(CDataInputStream& info_stream)
{
 long old_ws_id=CWideString::MemTrackingID;
 long old_cs_id=CCharString::MemTrackingID;
 CWideString::MemTrackingID=11;
 CCharString::MemTrackingID=11;
 CScopedMemTrackingID mem_id(11);
 Text=info_stream.ReadNullTerminatedWString();
 SpeechBank=info_stream.ReadPresizedString();
 Narrator=info_stream.ReadPresizedString();
 SoundName=info_stream.ReadPresizedString();
 long size=info_stream.ReadSLONG();
 Tags.resize(size);
 for(long c1=0;c1<size;++c1) {
  Tags[c1].Pos=info_stream.ReadSLONG();
  Tags[c1].Name=info_stream.ReadNullTerminatedString();
 }
 SetResourceSize(Sizeof());
 CWideString::MemTrackingID=old_ws_id;
 CCharString::MemTrackingID=old_cs_id;
}