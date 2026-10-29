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