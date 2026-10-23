// 00868b40 Row00868b40::Thing::Damage
// Flags: /O2 /Oy /W3 
// Leaf: Damage
namespace Row00868b40 {
struct Component;struct Scale{float Get(Component*);};
struct Def {virtual void Dummy()=0;virtual void Release()=0;int refs;char gap[0xb4];float high,low;Scale scale;};
struct Component{char pad[0x58];Def*def;int total,current;char gap[0x60];Scale scale;float A();float B();float C();};
struct Entry{int key;Component*component;};
struct Map{Entry*first,*last,*limit;__declspec(noinline) Entry*Lower(const int& key)const {Entry* b=first;int n=last-first;while(n>0){int half=n/2;Entry* m=b+half;if(m->key<key){b=m+1;n-=half+1;}else n=half;}return b;}};
// Layout: this is thing interface owner, flags+2c/+30, map+44. Component and def offsets taken from retail, no shared matching headers.
struct Thing{char pad[0x2c];unsigned flags;unsigned flags2;char gap[0x10];Map map;float Damage();
 template<int Key> __forceinline Component*Get(){const Map*m=&map;int key=Key;Entry*p=m->Lower(key);Entry*end=m->last;return (p==end||Key<p->key?end:p)->component;}
};
float Thing::Damage(){float value=1.0f;if(flags&0x100){Component*c=Get<0x68>();Def*d=c->def;if(d)++d->refs;float scale=d->scale.Get(c);const float low=d->low;value=(d->high-low)*scale+low;if(--d->refs==0)d->Release();}
 if(flags2&0x10)value*=Get<0x84>()->A();
 if(flags2&0x20){Component*c=Get<0x85>();if(c->current<c->total)value*=c->B();}
 if(flags2&0x40){Component*c=Get<0x86>();if(c->current<c->total)return c->C()*value;}
 return value;
}
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
namespace Row00868b40{float Scale::Get(Component*){return 0.5f;}float Component::A(){return 2;}float Component::B(){return 4;}float Component::C(){return 8;}struct TestDef:Def{int released;void Dummy(){}void Release(){++released;}};}
int main(){using namespace Row00868b40;Thing t;Component c;Entry e[5];TestDef d;memset(&t,0,sizeof(t));memset(&c,0,sizeof(c));memset(e,0,sizeof(e));d.refs=1;d.high=3;d.low=1;d.released=0;c.def=&d;c.total=2;c.current=1;for(int i=0;i<4;++i){e[i].key=i?0x83+i:0x68;e[i].component=&c;}t.map.first=e;t.map.last=e+4;t.map.limit=e+5;assert(t.Damage()==1);t.flags=0x100;t.flags2=0x70;assert(t.Damage()==128&&d.refs==1&&d.released==0);c.current=2;assert(t.Damage()==4);d.refs=0;t.flags2=0;assert(t.Damage()==2&&d.refs==0&&d.released==1);d.refs=1;e[4].component=&c;e[0].key=0x69;assert(t.Damage()==2&&d.refs==1&&d.released==1);puts("00868b40_TEST PASS");}
