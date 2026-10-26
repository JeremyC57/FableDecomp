// GetToughnessDamageReductionFactorForThing(const CThing&) -- the thing's damage multiplier: the toughness def's high/low range scaled by its component, then three optional state multipliers (each only while the component's current count is below its total)
// Flags: /O2 /Oy /W3 
// Leaf: ?GetToughnessDamageReductionFactorForThing@?A0x
// (Leaf is a prefix: the anonymous-namespace hash varies with the file path.)
// Identity (sweep3; was the descriptive "CTCBase_CalculateEffectiveDamageMultiplier"): Ego_r.pdb
//   ?GetToughnessDamageReductionFactorForThing@?A0x23f4dc30@@YIMABVCThing@@@Z, one of the five anonymous-namespace armour
//   helpers of the combat-damage TU (with GetArmourThresholdValueFromArmourWeights 0x008686E0,
//   GetArmourResponseDamageAttributeDamageMultiplier 0x008687A0, GetArmourResponseAugmentationDamageMultiplier 0x00868A80,
//   GetArmourWeightsFromThing 0x00868E50); the only one taking just a const CThing& and returning a float (__fastcall,
//   plain `ret`); its caller CHitParameters::SetAsModifiedByArmour 0x00869020 scales the damage by it. Body unchanged.
namespace Row00868b40 {
struct Component;struct Scale{float Get(Component*);};
struct Def {virtual void Dummy()=0;virtual void Release()=0;int refs;char gap[0xb4];float high,low;Scale scale;};
struct Component{char pad[0x58];Def*def;int total,current;char gap[0x60];Scale scale;float A();float B();float C();};
struct Entry{int key;Component*component;};
struct Map{Entry*first,*last,*limit;__declspec(noinline) Entry*LowerBound(const int& key)const {Entry* b=first;int n=last-first;while(n>0){int half=n/2;Entry* m=b+half;if(m->key<key){b=m+1;n-=half+1;}else n=half;}return b;}};
// Layout: this is thing interface owner, flags+2c/+30, map+44. Component and def offsets taken from retail, no shared matching headers.
}
using namespace Row00868b40;
class CThing{public:char pad[0x2c];unsigned flags;unsigned flags2;char gap[0x10];Map map;
 template<int Key> __forceinline Component*Get()const{const Map*m=&map;int key=Key;Entry*p=m->LowerBound(key);Entry*end=m->last;return (p==end||Key<p->key?end:p)->component;}
};
namespace {
float __fastcall GetToughnessDamageReductionFactorForThing(const CThing& thing){float value=1.0f;if(thing.flags&0x100){Component*c=thing.Get<0x68>();Def*d=c->def;if(d)++d->refs;float scale=d->scale.Get(c);const float low=d->low;value=(d->high-low)*scale+low;if(--d->refs==0)d->Release();}
 if(thing.flags2&0x10)value*=thing.Get<0x84>()->A();
 if(thing.flags2&0x20){Component*c=thing.Get<0x85>();if(c->current<c->total)value*=c->B();}
 if(thing.flags2&0x40){Component*c=thing.Get<0x86>();if(c->current<c->total)return c->C()*value;}
 return value;
}
}
