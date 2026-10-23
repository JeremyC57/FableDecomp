#include <assert.h>
#include <stdio.h>
// BEGIN exact source snapshot SHA256 f3bd976708433293644334e00a5e025b511de5a999666e7effddb2060f120cd8
// 009263f0 _Med3<CIntelligentPointer<const CThing>>
// Flags: /O2 /Oy /Gr
// Leaf: _Med3
// Layout: PDB32592 confirms virtual base destructor; retail stride 8, vfptr +0, link +4; CBaseIntelligentPointer callees A01B10/A01B50/A01B90/A01C10.
class CThing;
class CBaseObject;
struct PointerLink {CBaseObject* item;int refs;};
class CBaseIntelligentPointer {public:
 PointerLink* link;
 CBaseIntelligentPointer();virtual ~CBaseIntelligentPointer();
 CBaseObject* GetPItem()const;void SetPItem(CBaseObject*);
};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {public:
 T* GetPItem()const{return (T*)CBaseIntelligentPointer::GetPItem();}
 void SetPItem(T*p){CBaseIntelligentPointer::SetPItem((CBaseObject*)p);}
 CIntelligentPointer(){}
 CIntelligentPointer(const CIntelligentPointer&p){SetPItem(p.GetPItem());}
 CIntelligentPointer& operator=(const CIntelligentPointer&p){SetPItem(p.GetPItem());return *this;}
};
typedef CIntelligentPointer<const CThing> ThingPointer;
typedef bool (__fastcall *ThingCompare)(ThingPointer,ThingPointer);
template<class T,class Compare> const T& __fastcall _Med3(const T&a,const T&b,const T&c,Compare comp){if(comp(a,b)){if(comp(b,c))return b;else if(comp(a,c))return c;else return a;}else if(comp(a,c))return a;else if(comp(b,c))return c;else return b;}
template const ThingPointer& __fastcall _Med3(const ThingPointer&,const ThingPointer&,const ThingPointer&,ThingCompare);

// END exact source snapshot
class CThing{public:int key;};
class CBaseObject{public:int key;};
static int baseDestroyed=0,derivedDestroyed=0;
struct DestructionProbe:ThingPointer{~DestructionProbe(){++derivedDestroyed;}};
CBaseIntelligentPointer::CBaseIntelligentPointer():link(0){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){++baseDestroyed;if(link&&!--link->refs)delete link;}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{return link?link->item:0;}
void CBaseIntelligentPointer::SetPItem(CBaseObject*p){if(link&&link->item==p)return;if(link&&!--link->refs)delete link;link=0;if(p){link=new PointerLink;link->item=p;link->refs=1;}}
bool __fastcall compare(ThingPointer a,ThingPointer b){return a.GetPItem()->key<b.GetPItem()->key;}
int main(){CThing x[3]={{1},{2},{3}};ThingPointer a[3];for(int i=0;i<3;++i)a[i].SetPItem(x+i);int permutations[6][3]={{0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}};for(int i=0;i<6;++i)assert(_Med3(a[permutations[i][0]],a[permutations[i][1]],a[permutations[i][2]],compare).GetPItem()->key==2);assert(&_Med3(a[0],a[0],a[0],compare)==&a[0]);CBaseIntelligentPointer* probe=new DestructionProbe;int prior=baseDestroyed;delete probe;assert(derivedDestroyed==1&&baseDestroyed==prior+1);puts("009263f0_TEST PASS");}