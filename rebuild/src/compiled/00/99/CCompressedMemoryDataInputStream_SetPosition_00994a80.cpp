// CCompressedMemoryDataInputStream::SetPosition(unsigned long pos) -- seek within a chunk or decompress forward.
// Leaf: ?SetPosition@CCompressedMemoryDataInputStream@@UAEXK@Z
// Retail: 00994a80,141 bytes,thiscall.
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_CHUNKSEEK
#define FABLE_RETAIL_CCOMPRESSEDMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CACOMPRESSOR
#define FABLE_RETAIL_CCOMPRESSORZLIB
#define FABLE_RETAIL_CCOMPRESSORZLIB_STREAM
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CAFILE_ERRORCALLBACK
#include "engine/CCompressorZlib.h"
#include "engine/CCompressedMemoryDataInputStream.h"
#include "engine/CAFile.h"
void CCompressedMemoryDataInputStream::SetPosition(unsigned long pos) {
 unsigned long current_pos=GetPosition();
 if(pos==current_pos)return;
 if(CanSeekToPosWithinSourceChunk(pos)) {SetPositionAssumeWithinSourceChunk(pos);return;}
 if(pos>current_pos) {
  while(pos>current_pos) {
   long bytes_to_read=(unsigned long)BufferSize<pos-current_pos?BufferSize:pos-current_pos;
   bool finished=false;
   unsigned long size=Compressor->DecompressStream(Buffer,bytes_to_read,finished);
   if(size!=(unsigned long)bytes_to_read)CAFile::DoErrorCallback();
   current_pos+=bytes_to_read;
  }
  CDataInputStream::SetPosition(pos);
 }
}