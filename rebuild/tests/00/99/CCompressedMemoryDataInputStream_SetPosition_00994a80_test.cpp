// 00994a80 exact selected seek body and original chunk helpers.
// Controlled decompressor and base-seek service boundaries; no zlib algorithm claim.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static unsigned long reads[256],read_count,total_read,base_pos;
static unsigned errors,base_count;
static int fail_mode;
static void* expected_buffer;
static int bad;
class CCompressorZlib {public: unsigned long DecompressStream(void* dest,unsigned long bytes,bool& finished) {
 if(dest!=expected_buffer||finished||read_count>=256)bad=1;
 if(read_count<256)reads[read_count++]=bytes;
 total_read+=bytes;finished=(read_count&1)!=0;
 return fail_mode&&read_count%2?bytes-1:bytes;
}};
namespace boost {template<class T>class scoped_ptr {T* ptr;public:explicit scoped_ptr(T* p=0):ptr(p){} T* operator->()const{return ptr;} };}
class CAFile {public:
#ifdef _MSC_VER
 typedef void (__fastcall *ErrorCallbackType)();
 static void __fastcall DoErrorCallback(){if(GetErrorCallback())CallErrorCallback();}
 static ErrorCallbackType __fastcall GetErrorCallback(){return m_errorCallback;}
 static void __fastcall CallErrorCallback(){m_errorCallback();}
#else
 typedef void (*ErrorCallbackType)();
 static void DoErrorCallback(){if(GetErrorCallback())CallErrorCallback();}
 static ErrorCallbackType GetErrorCallback(){return m_errorCallback;}
 static void CallErrorCallback(){m_errorCallback();}
#endif
 static ErrorCallbackType m_errorCallback;
};
CAFile::ErrorCallbackType CAFile::m_errorCallback=0;
#ifdef _MSC_VER
static void __fastcall on_error(){++errors;}
#else
static void on_error(){++errors;}
#endif
class CDataInputStream {
public:
 virtual ~CDataInputStream(){}
 virtual void SetPosition(unsigned long pos){base_pos=pos;++base_count;}
 virtual unsigned long GetPosition()const{return StreamPos;}
 unsigned long StreamPos;long StreamSize;const unsigned char* CurrentSourcePtr;
 unsigned long SourceChunkStreamPos;long SourceChunkBytesRemaining;
    bool CanSeekToPosWithinSourceChunk(long pos) {
        return (unsigned long)pos>=SourceChunkStreamPos && (unsigned long)pos<=StreamPos+SourceChunkBytesRemaining;
    }
    void SetPositionAssumeWithinSourceChunk(unsigned long pos) {
        CurrentSourcePtr+=(long)(pos-StreamPos);
        SourceChunkBytesRemaining+=(long)(StreamPos-pos);
        StreamPos=pos;
    }
};
class CCompressedMemoryDataInputStream:public CDataInputStream {
public:
 CCompressedMemoryDataInputStream(unsigned char* buffer,long size,CCompressorZlib* compressor):Buffer(buffer),BufferSize(size),Compressor(compressor){}
 virtual void SetPosition(unsigned long pos);
 unsigned char* Buffer;long BufferSize;boost::scoped_ptr<CCompressorZlib> Compressor;
};
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

int main(){
 unsigned char storage[256],buffer[128];CCompressorZlib compressor;
 unsigned long positions[]={0,40,49,50,55,60,70,80,81,82,90,127,160,200};
 unsigned count=0;
 for(unsigned initial=0;initial<3;++initial)for(unsigned rem=0;rem<3;++rem)for(unsigned bsize=1;bsize<=17;bsize+=4)for(unsigned p=0;p<14;++p)for(unsigned mode=0;mode<3;++mode){
  unsigned long current=50+initial*10;long remaining=rem*10;
  CCompressedMemoryDataInputStream stream(buffer,bsize,&compressor);
  stream.StreamPos=current;stream.StreamSize=256;stream.CurrentSourcePtr=storage+80;stream.SourceChunkStreamPos=40;stream.SourceChunkBytesRemaining=remaining;
  read_count=total_read=errors=base_count=bad=0;base_pos=0;fail_mode=mode!=0;CAFile::m_errorCallback=mode==2?0:on_error;expected_buffer=buffer;
  unsigned long pos=positions[p];stream.SetPosition(pos);
  if(bad)return 1;
  if(pos==current){if(read_count||base_count||errors||stream.StreamPos!=current||stream.CurrentSourcePtr!=storage+80||stream.SourceChunkBytesRemaining!=remaining)return 2;}
  else if(pos>=40&&pos<=current+(unsigned long)remaining){
   long delta=(long)pos-(long)current;
   if(read_count||base_count||errors||stream.StreamPos!=pos||stream.CurrentSourcePtr!=storage+80+delta||stream.SourceChunkBytesRemaining!=remaining-delta)return 3;
  }else if(pos>current){
   unsigned long left=pos-current,expected_count=(left+bsize-1)/bsize;
   if(read_count!=expected_count||total_read!=left||base_count!=1||base_pos!=pos)return 4;
   for(unsigned q=0;q<read_count;++q){unsigned long amount=left<bsize?left:bsize;if(reads[q]!=amount)return 5;left-=amount;}
   if(errors!=(mode==1?(read_count+1)/2:0))return 6;
   if(stream.StreamPos!=current||stream.CurrentSourcePtr!=storage+80||stream.SourceChunkBytesRemaining!=remaining)return 7;
  }else if(read_count||base_count||errors||stream.StreamPos!=current||stream.CurrentSourcePtr!=storage+80||stream.SourceChunkBytesRemaining!=remaining)return 8;
  ++count;
 }
 if(count!=1890)return 9;
 std::puts("00994a80_TEST PASS");return 0;
}