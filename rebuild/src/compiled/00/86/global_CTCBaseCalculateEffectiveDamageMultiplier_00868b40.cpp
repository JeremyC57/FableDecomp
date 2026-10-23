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