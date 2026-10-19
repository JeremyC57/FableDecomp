#include <cstdio>
struct Ptr { void* p; void Init(){p=0;} void* Get() const{return p;} void Set(void* q){p=q;} };
struct Event { unsigned long base_ref; unsigned long payload[16]; Ptr sender; Ptr target; };
static void copy_event(Event* d, const Event* s) {
 d->base_ref=s->base_ref;
 for (int i=0;i<16;++i) d->payload[i]=s->payload[i];
 d->sender.Init(); d->sender.Set(s->sender.Get());
 d->target.Init(); d->target.Set(s->target.Get());
}
int main(){ Event a={}; Event b={}; a.base_ref=7; a.sender.p=(void*)0x1234; a.target.p=(void*)0x5678; for(int i=0;i<16;++i)a.payload[i]=0x100+i; copy_event(&b,&a); if(b.base_ref!=7||b.sender.p!=a.sender.p||b.target.p!=a.target.p) {std::printf("00687310_TEST FAIL\n");return 1;} for(int i=0;i<16;++i)if(b.payload[i]!=a.payload[i]){std::printf("00687310_TEST FAIL\n");return 1;} std::printf("00687310_TEST PASS\n"); return 0; }