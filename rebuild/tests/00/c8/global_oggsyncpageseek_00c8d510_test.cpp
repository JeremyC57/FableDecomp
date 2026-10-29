#include <stdio.h>
#include <stdlib.h>
// 00c8d510 ogg_sync_pageseek
// Leaf: @ogg_sync_pageseek@8
// Flags: /Gr
// Original libogg 1.1 framing algorithm and public structure layouts.
#ifdef _MSC_VER
#pragma intrinsic(memcmp,memcpy,memset)
#else
#define __fastcall
#define __cdecl
#define __declspec(x)
#endif
extern "C" int __cdecl memcmp(const void*,const void*,unsigned int);
extern "C" void* __cdecl memcpy(void*,const void*,unsigned int);
extern "C" void* __cdecl memset(void*,int,unsigned int);
extern "C" __declspec(dllimport) void* __cdecl memchr(const void*,int,unsigned int);
typedef struct {
  unsigned char *header;
  long header_len;
  unsigned char *body;
  long body_len;
} ogg_page;
typedef struct {
  unsigned char *data;
  int storage;
  int fill;
  int returned;

  int unsynced;
  int headerbytes;
  int bodybytes;
} ogg_sync_state;
extern "C" void __fastcall ogg_page_checksum_set(ogg_page*);
extern "C" long __fastcall ogg_sync_pageseek(ogg_sync_state *oy,ogg_page *og){
  unsigned char *page=oy->data+oy->returned;
  unsigned char *next;
  long bytes=oy->fill-oy->returned;
  
  if(oy->headerbytes==0){
    int headerbytes,i;
    if(bytes<27)return(0); /* not enough for a header */
    
    /* verify capture pattern */
    if(memcmp(page,"OggS",4))goto sync_fail;
    
    headerbytes=page[26]+27;
    if(bytes<headerbytes)return(0); /* not enough for header + seg table */
    
    /* count up body length in the segment table */
    
    for(i=0;i<page[26];i++)
      oy->bodybytes+=page[27+i];
    oy->headerbytes=headerbytes;
  }
  
  if(oy->bodybytes+oy->headerbytes>bytes)return(0);
  
  /* The whole test page is buffered.  Verify the checksum */
  {
    /* Grab the checksum bytes, set the header field to zero */
    char chksum[4];
    ogg_page log;
    
    memcpy(chksum,page+22,4);
    memset(page+22,0,4);
    
    /* set up a temp page struct and recompute the checksum */
    log.header=page;
    log.header_len=oy->headerbytes;
    log.body=page+oy->headerbytes;
    log.body_len=oy->bodybytes;
    ogg_page_checksum_set(&log);
    
    /* Compare */
    if(memcmp(chksum,page+22,4)){
      /* D'oh.  Mismatch! Corrupt page (or miscapture and not a page
	 at all) */
      /* replace the computed checksum with the one actually read in */
      memcpy(page+22,chksum,4);
      
      /* Bad checksum. Lose sync */
      goto sync_fail;
    }
  }
  
  /* yes, have a whole page all ready to go */
  {
    unsigned char *page=oy->data+oy->returned;
    long bytes;

    if(og){
      og->header=page;
      og->header_len=oy->headerbytes;
      og->body=page+oy->headerbytes;
      og->body_len=oy->bodybytes;
    }

    oy->unsynced=0;
    oy->returned+=(bytes=oy->headerbytes+oy->bodybytes);
    oy->headerbytes=0;
    oy->bodybytes=0;
    return(bytes);
  }
  
 sync_fail:
  
  oy->headerbytes=0;
  oy->bodybytes=0;
  
  /* search for possible capture */
  next=(unsigned char*)memchr(page+1,'O',bytes-1);
  if(!next)
    next=oy->data+oy->fill;

  oy->returned=next-oy->data;
  return(-(next-page));
}
typedef char page_size[sizeof(ogg_page)==16?1:-1];
typedef char sync_size[sizeof(ogg_sync_state)==28?1:-1];

#ifdef _MSC_VER
// VC7.1 static test CRT lacks this import slot; supply its exact memchr contract.
extern "C" {
static void* __cdecl test_memchr(const void*buf,int value,unsigned int length){const unsigned char*p=(const unsigned char*)buf;for(unsigned int i=0;i<length;++i)if(p[i]==(unsigned char)value)return (void*)(p+i);return 0;}
void* (__cdecl* _imp__memchr)(const void*,int,unsigned int)=test_memchr;
}
#endif
static void check(bool p){if(!p)abort();}
static unsigned calls;
static unsigned long crcBytes(unsigned long crc,const unsigned char*p,int n){for(int i=0;i<n;++i){crc^=(unsigned long)p[i]<<24;for(int k=0;k<8;++k)crc=(crc&0x80000000UL)?((crc<<1)^0x04c11db7UL):(crc<<1);}return crc;}
// Test dependency implements the checksum contract; its body is not a graded row.
extern "C" void __fastcall ogg_page_checksum_set(ogg_page*p){++calls;check(p->header_len>=27);for(int i=22;i<26;++i)check(p->header[i]==0);unsigned long crc=crcBytes(0,p->header,(int)p->header_len);crc=crcBytes(crc,p->body,(int)p->body_len);for(int i=0;i<4;++i)p->header[22+i]=(unsigned char)(crc>>(i*8));}
static unsigned char bytes[4096],saved[4096];
static unsigned count;
static int makePage(int base,int segments,int shape){for(int i=0;i<4096;++i)bytes[i]=0xA6;unsigned char*p=bytes+base;for(int i=0;i<27+segments;++i)p[i]=0;p[0]='O';p[1]='g';p[2]='g';p[3]='S';p[5]=6;p[14]=0xD1;p[18]=9;p[26]=(unsigned char)segments;int body=0;for(int i=0;i<segments;++i){p[27+i]=(unsigned char)(shape==0?0:(shape==1?1:(i%5==0?255:3)));body+=p[27+i];}for(int i=0;i<body;++i)p[27+segments+i]=(unsigned char)((i*17+11)%251);ogg_page page;page.header=p;page.header_len=27+segments;page.body=p+page.header_len;page.body_len=body;calls=0;ogg_page_checksum_set(&page);check(calls==1);calls=0;return 27+segments+body;}
static void setup(ogg_sync_state&state,int base,int n){state.data=bytes;state.storage=4096;state.fill=base+n;state.returned=base;state.unsynced=1;state.headerbytes=0;state.bodybytes=0;}
static void runGood(int segments,int shape,int base,int cached,int output){int n=makePage(base,segments,shape);int header=27+segments;int body=n-header;for(int i=0;i<4096;++i)saved[i]=bytes[i];ogg_sync_state state;setup(state,base,n);if(cached){state.headerbytes=header;state.bodybytes=body;}ogg_page page;page.header=0;page.body=0;page.header_len=-1;page.body_len=-2;long result=ogg_sync_pageseek(&state,output?&page:0);check(result==n&&calls==1);check(state.returned==base+n&&state.fill==base+n&&state.storage==4096&&state.data==bytes&&state.unsynced==0&&state.headerbytes==0&&state.bodybytes==0);if(output)check(page.header==bytes+base&&page.header_len==header&&page.body==bytes+base+header&&page.body_len==body);else check(page.header==0&&page.body==0&&page.header_len==-1&&page.body_len==-2);for(int i=0;i<4096;++i)check(bytes[i]==saved[i]);++count;}
static void runPartial(int segments,int shape,int base){int n=makePage(base,segments,shape);int header=27+segments;for(int i=0;i<4096;++i)saved[i]=bytes[i];ogg_sync_state state;setup(state,base,0);ogg_page page;page.header=0;page.body=0;page.header_len=-1;page.body_len=-2;for(int length=0;length<n;++length){state.fill=base+length;long result=ogg_sync_pageseek(&state,&page);check(result==0&&calls==0&&state.returned==base&&state.unsynced==1&&state.data==bytes&&state.storage==4096);check(state.headerbytes==(length>=header?header:0));check(state.bodybytes==(length>=header?n-header:0));check(page.header==0&&page.body==0&&page.header_len==-1&&page.body_len==-2);++count;}state.fill=base+n;check(ogg_sync_pageseek(&state,&page)==n&&calls==1&&state.returned==base+n);for(int i=0;i<4096;++i)check(bytes[i]==saved[i]);++count;}
static void runBad(int segments,int shape,int base,int corrupt,int nextKind){int n=makePage(base,segments,shape);unsigned char*p=bytes+base;if(corrupt)p[22]^=1;else p[0]='X';if(nextKind==1)p[10]='O';if(nextKind==2){p[n]='O';p[n+1]='g';n+=2;}for(int i=0;i<4096;++i)saved[i]=bytes[i];int next=n;for(int i=1;i<n;++i)if(p[i]=='O'){next=i;break;}ogg_sync_state state;setup(state,base,n);ogg_page page;page.header=0;page.body=0;page.header_len=-1;page.body_len=-2;long result=ogg_sync_pageseek(&state,&page);check(result==-next&&state.returned==base+next&&state.headerbytes==0&&state.bodybytes==0&&state.unsynced==1&&state.fill==base+n);check(calls==(corrupt?1U:0U));check(page.header==0&&page.body==0&&page.header_len==-1&&page.body_len==-2);for(int i=0;i<4096;++i)check(bytes[i]==saved[i]);++count;}
int main(){check(sizeof(ogg_sync_state)==28&&sizeof(ogg_page)==16);const int segments[]={0,1,2,16,32};for(int s=0;s<5;++s)for(int shape=0;shape<3;++shape)for(int base=0;base<=3;base+=3){for(int cached=0;cached<2;++cached)for(int output=0;output<2;++output)runGood(segments[s],shape,base,cached,output);runPartial(segments[s],shape,base);for(int corrupt=0;corrupt<2;++corrupt)for(int nextKind=0;nextKind<3;++nextKind)runBad(segments[s],shape,base,corrupt,nextKind);}check(count==8406);puts("00c8d510_TEST PASS");return 0;}