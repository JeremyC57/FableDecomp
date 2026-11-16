#ifndef FABLE_LIBPK_CSTREAMBLOCKREADER_H
#define FABLE_LIBPK_CSTREAMBLOCKREADER_H
class CPooledResource;
// Native bank+4 protocol: Read slot1, GetBlockSize slot2; slot0 closes/releases the reader.
// Descriptive names, not the old propagated combat/boast labels.
class CStreamBlockReader {
public:
 virtual void Close();
 virtual bool Read(unsigned long,unsigned long,unsigned long,void*,CPooledResource*,long);
 virtual unsigned long GetBlockSize();
};
unsigned long __fastcall GetStreamBlockOffset(CStreamBlockReader*,unsigned long);
long __fastcall GetStreamBlockTail(CStreamBlockReader*,long,long);
#ifdef FABLE_RETAIL_STREAMED_BLOCK_CALLEES_PASS23
// Actual C1E970/C1E990 bodies: quotient/product offset, then ceiling to the next full block.
inline unsigned long __fastcall GetStreamBlockOffset(CStreamBlockReader*stream,unsigned long position){unsigned long block=stream->GetBlockSize();return position-(position/block)*block;}
inline long __fastcall GetStreamBlockTail(CStreamBlockReader*stream,long position,long size){unsigned long used=GetStreamBlockOffset(stream,position)+size;unsigned long block=stream->GetBlockSize();return ((used+block-1)/block)*block-used;}
#endif
#endif
