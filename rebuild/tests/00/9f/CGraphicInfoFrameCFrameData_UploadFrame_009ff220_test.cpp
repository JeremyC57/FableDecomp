// Behaviour test for 0x009FF220 CGraphicInfoFrame::CFrameData::UploadFrame: a copy of the row's logic over small stand-in
// objects (bank entry types, load results, frame-data reference counts, file-data release, resource size / registration).
#include <cstdio>
struct FrameData { int rc; bool initialised; int uninit_calls; };
static int g_deleted=0;
struct Frame {
    FrameData* fd;
    Frame():fd(0){}
    ~Frame(){ FrameData* d=fd; if(d){ --d->rc; if(d->rc==0){ if(d->initialised)d->uninit_calls++; g_deleted++; } } }
    Frame& operator=(const Frame& o){ FrameData* d=fd; if(d){ if(--d->rc==0){ if(d->initialised)d->uninit_calls++; g_deleted++; } } fd=o.fd; if(fd)++fd->rc; return *this; }
};
struct Bank { unsigned long type; bool load_ok; FrameData* produced; int added; unsigned long last_level; bool last_bump; bool last_force; int vol_calls; int frame_calls; };
struct Info { Bank* bank; bool flat; };
struct Obj {
    Info* info; Frame frame; int managed, loading; int file_refs; void* file_data;
    signed char cur, loading_level; unsigned long size;
};
static unsigned long SizeofObj(const Obj& o){ unsigned long s=0x44; s+=o.frame.fd?0x1b+4:4; if(o.loading)s+=100; return s; }
static bool UploadFrame(Obj& o,bool force)
{
    bool result=false;
    Bank* b=o.info->bank;
    if(b->type==4) {
        b->vol_calls++; b->last_level=o.loading_level; b->last_bump=(b->type==2);
        if(b->load_ok){ o.managed=o.loading; o.loading=0; o.cur=o.loading_level; result=true; }
    } else {
        Frame f;
        b->frame_calls++; b->last_level=o.loading_level; b->last_force=force;
        bool ok=b->load_ok;
        if(ok){ f.fd=b->produced; if(f.fd)f.fd->rc++; }
        if(ok){
            if(!o.info->flat){ o.frame=f; o.managed=o.loading; o.loading=0; o.cur=o.loading_level; }
            result=true;
        }
    }
    if(o.file_refs){ --o.file_refs; }
    o.file_data=0;
    o.size=SizeofObj(o);
    b->added++;
    return result;
}
int main()
{
    int fail=0;
    // volume texture, load succeeds: only LoadVolumeGraphic runs, and `force` (passed only to LoadGraphicFrame) is not seen
    { Bank b={4,true,0,0,0,false,false}; Info i={&b,false}; Obj o; o.info=&i; o.managed=1; o.loading=7; o.file_refs=1; o.file_data=&b; o.cur=3; o.loading_level=1; o.size=0;
      if(!UploadFrame(o,true)||b.vol_calls!=1||b.frame_calls!=0||b.last_force||b.last_bump||o.managed!=7||o.loading!=0||o.cur!=1||o.file_data||b.added!=1||b.last_level!=1||o.size!=0x48) fail|=1; }
    // plain texture, frame adopted; old frame data released
    { FrameData old={1,true,0}, nw={1,true,0}; Bank b={0,true,&nw,0,0,false,false}; Info i={&b,false}; Obj o; o.info=&i; o.frame.fd=&old; o.managed=1; o.loading=9; o.file_refs=1; o.file_data=&b; o.cur=3; o.loading_level=2;
      g_deleted=0;
      if(!UploadFrame(o,true)||b.frame_calls!=1||b.vol_calls!=0||o.frame.fd!=&nw||nw.rc!=2||old.uninit_calls!=1||g_deleted!=1||o.cur!=2||!b.last_force||o.size!=0x44+0x1f) fail|=2;
      o.frame.fd=0; }
    // flat sequence: success, but nothing adopted
    { FrameData nw={1,false,0}; Bank b={0,true,&nw,0,0,false,false}; Info i={&b,true}; Obj o; o.info=&i; o.managed=1; o.loading=9; o.file_refs=0; o.file_data=0; o.cur=3; o.loading_level=2;
      if(!UploadFrame(o,false)||o.frame.fd||o.managed!=1||o.cur!=3||nw.rc!=1||b.added!=1||o.size!=0x44+4+100) fail|=4; }
    // failed load
    { Bank b={0,false,0,0,0,false,false}; Info i={&b,false}; Obj o; o.info=&i; o.managed=1; o.loading=0; o.file_refs=0; o.file_data=0; o.cur=3; o.loading_level=2;
      if(UploadFrame(o,false)||o.cur!=3||b.added!=1) fail|=8; }
    if(fail){ std::printf("009ff220_TEST FAIL\n"); return 1; }
    std::printf("009ff220_TEST PASS\n");
    return 0;
}