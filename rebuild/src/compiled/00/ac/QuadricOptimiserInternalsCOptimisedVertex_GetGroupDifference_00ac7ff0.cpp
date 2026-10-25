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