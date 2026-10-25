// 00ac7ff0 QuadricOptimiserInternals::COptimisedVertex::GetGroupDifference
// Flags: /O2 /Oy /W3
// Leaf: GetGroupDifference
#include <math.h>
namespace X_00ac7ff0 {
namespace std {
struct _Rb_tree_node{unsigned char Colour;unsigned char Pad[3];_Rb_tree_node*Parent,*Left,*Right;unsigned Key;float Weight;};
struct _Rb_tree_iterator{_Rb_tree_node*Node;__forceinline void increment(){if(Node->Right){Node=Node->Right;while(Node->Left)Node=Node->Left;}else{_Rb_tree_node*p=Node->Parent;while(Node==p->Right){Node=p;p=p->Parent;}if(Node->Right!=p)Node=p;}}};
template<class K,class V>struct map;template<>struct map<unsigned,float>{_Rb_tree_node*Header;unsigned Count;__forceinline _Rb_tree_node*find(unsigned key)const{_Rb_tree_node*y=Header,*x=Header->Parent;while(x){if(!(x->Key<key)){y=x;x=x->Left;}else x=x->Right;}if(y==Header||key<y->Key)y=Header;return y;}};
}
// Layout: retail group-map header at this+0x9c; tree node parent+4,left+8,right+12,key+16,weight+20. No active COptimisedVertex header exists.
// Fidelity: shared keys subtract this node weight from the previously copied same weight, as the retail instructions do.
namespace QuadricOptimiserInternals {
struct COptimisedVertex{char EarlierFields[0x9c];std::map<unsigned,float> Groups;float GetGroupDifference(const COptimisedVertex&)const;};
float COptimisedVertex::GetGroupDifference(const COptimisedVertex&other)const{
 float difference=0.0f;std::_Rb_tree_iterator i;i.Node=Groups.Header->Left;
 for(;i.Node!=Groups.Header;i.increment()){float weight=i.Node->Weight;if(other.Groups.find(i.Node->Key)!=other.Groups.Header)weight=(float)fabs(i.Node->Weight-weight);difference+=weight;}
 i.Node=other.Groups.Header->Left;for(;i.Node!=other.Groups.Header;i.increment()){float weight=i.Node->Weight;if(Groups.find(i.Node->Key)==Groups.Header)difference+=weight;}
 return(float)pow((double)difference,1.0);
}
}
}

#include <assert.h>
#include <stdio.h>
#include <string.h>
using namespace X_00ac7ff0;using namespace X_00ac7ff0::QuadricOptimiserInternals;
static void Init(std::map<unsigned,float>&m,std::_Rb_tree_node&h){memset(&h,0,sizeof(h));h.Left=h.Right=&h;m.Header=&h;m.Count=0;}
static void Add(std::map<unsigned,float>&m,std::_Rb_tree_node&n,unsigned k,float w){memset(&n,0,sizeof(n));n.Colour=1;n.Key=k;n.Weight=w;std::_Rb_tree_node*p=m.Header,*x=p->Parent;while(x){p=x;x=k<p->Key?p->Left:p->Right;}n.Parent=p;if(p==m.Header){p->Parent=p->Left=p->Right=&n;}else if(k<p->Key){p->Left=&n;if(m.Header->Left==p)m.Header->Left=&n;}else{p->Right=&n;if(m.Header->Right==p)m.Header->Right=&n;}++m.Count;}
int main(){COptimisedVertex a,b;std::_Rb_tree_node ah,bh,n[4];Init(a.Groups,ah);Init(b.Groups,bh);assert(a.GetGroupDifference(b)==0);Add(a.Groups,n[0],1,2);Add(a.Groups,n[1],2,3);Add(b.Groups,n[2],2,17);Add(b.Groups,n[3],3,5);assert(a.GetGroupDifference(b)==7);assert(b.GetGroupDifference(a)==7);assert(a.GetGroupDifference(a)==0);Init(b.Groups,bh);assert(a.GetGroupDifference(b)==5&&b.GetGroupDifference(a)==5);puts("00ac7ff0_TEST PASS");}