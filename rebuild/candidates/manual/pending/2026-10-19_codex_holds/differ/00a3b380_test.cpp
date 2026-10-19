#include <cstdio>
struct Ref { int* n; virtual void A(){} virtual void B(){} virtual void Release(){ ++*n; } };
struct CVideoSys { Ref* GBuilder; Ref* MControl; Ref* MPosition; Ref* MSeeking; Ref* MEvent; Ref* BasicAudio; Ref* PAsfReader; Ref* PFileSource; void* TexRenderer; unsigned char pad[0x44]; void* pTextBank; void* dbg; long lock; long rec; void* own; void* sem; unsigned long spin; void* RenderEvent; void* texture; long Height; long Width; };
void Close(CVideoSys* self)
{
 Ref* p; p=self->MControl;if(p){p->Release();self->MControl=0;} p=self->MPosition;if(p){p->Release();self->MPosition=0;} p=self->MSeeking;if(p){p->Release();self->MSeeking=0;} p=self->BasicAudio;if(p){p->Release();self->BasicAudio=0;} p=self->PFileSource;if(p){p->Release();self->PFileSource=0;} p=self->PAsfReader;if(p){p->Release();self->PAsfReader=0;} p=self->MEvent;if(p){p->Release();self->MEvent=0;} p=self->GBuilder;if(p){p->Release();self->GBuilder=0;} self->sem=0;
}
int main(){int n=0; Ref r; r.n=&n; CVideoSys v={0,&r,0,&r,0,&r,0,0,0,{0},0,0,0,0,0,0,0,0,0,0}; Close(&v); if(v.MControl||v.MSeeking||v.BasicAudio||v.sem||n!=3){std::printf("00a3b380_TEST FAIL\n");return 1;} std::printf("00a3b380_TEST PASS\n");return 0;}