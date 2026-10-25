// 00756250 std::_Rb_tree::insert_unique
// Flags: /O2 /Oy /W3
// Leaf: insert_unique
namespace X_00756250 {
// Layout: packed CBasicString<char> data0/length4/capacity8/owns12; CCharString data refcount13, from99ec30.
#pragma pack(push,1)
template<class T>struct CBasicString {void*PStringData;unsigned Length,Capacity;bool OwnsData;__declspec(noinline) bool operator<(const CBasicString&o)const{const T*a=(const T*)PStringData,*b=(const T*)o.PStringData;while(*a||*b){if(*a<*b)return true;if(*a>*b)return false;++a;++b;}return false;}};
struct CCharStringData:CBasicString<char>{unsigned RefCount;};
#pragma pack(pop)
struct CCharString {CCharStringData*PString;CCharString();CCharString(const CCharString&);~CCharString();CCharString&operator=(const CCharString&);bool operator<(const CCharString&o)const{bool result;if(o.PString==PString)result=false;else if(!PString)result=true;else if(!o.PString)result=false;else result=*PString<*o.PString;return result;}};
namespace std {
template<class A,class B>struct pair {A first;B second;pair(){}pair(const A&a,const B&b):first(a),second(b){}pair(const pair&o):first(o.first),second(o.second){}};
typedef pair<CCharString,pair<long,long> > value_type;
// Layout: node colour0,parent4,left8,right12,pair key16,mapped20/24; allocator756150 malloc28.
struct _Rb_tree_node {unsigned char Colour;unsigned char Pad[3];_Rb_tree_node*Parent,*Left,*Right;value_type Value;};
struct _Rb_tree_iterator {_Rb_tree_node*Node;void _M_decrement();};
__declspec(noinline) void _Rb_tree_iterator::_M_decrement(){if(Node->Colour==0&&Node->Parent->Parent==Node){Node=Node->Right;return;}if(Node->Left){Node=Node->Left;while(Node->Right)Node=Node->Right;}else{_Rb_tree_node*p=Node->Parent;while(Node==p->Left){Node=p;p=p->Parent;}Node=p;}}
struct _Rb_tree {_Rb_tree_node*Header;unsigned Count;_Rb_tree_iterator _M_insert(_Rb_tree_node*,_Rb_tree_node*,const value_type&,_Rb_tree_node*);pair<_Rb_tree_iterator,bool> insert_unique(const value_type&);};
pair<_Rb_tree_iterator,bool> _Rb_tree::insert_unique(const value_type&value){_Rb_tree_node*parent=Header,*node=Header->Parent;bool goLeft=true;while(node){parent=node;goLeft=value.first<node->Value.first;node=goLeft?node->Left:node->Right;}_Rb_tree_iterator previous;previous.Node=parent;if(goLeft){if(previous.Node==Header->Left){return pair<_Rb_tree_iterator,bool>(_M_insert(parent,parent,value,0),true);}previous._M_decrement();}if(previous.Node->Value.first<value.first){return pair<_Rb_tree_iterator,bool>(_M_insert(node,parent,value,0),true);}return pair<_Rb_tree_iterator,bool>(previous,false);}
}
}

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static unsigned ReleasedStrings=0;
namespace X_00756250 {
CCharString::CCharString():PString(0){}
CCharString::CCharString(const CCharString&o):PString(o.PString){if(PString)++PString->RefCount;}
CCharString::~CCharString(){if(PString&&--PString->RefCount==0){if(PString->OwnsData)free(PString->PStringData);delete PString;++ReleasedStrings;}}
CCharString&CCharString::operator=(const CCharString&o){if(PString!=o.PString){if(o.PString)++o.PString->RefCount;if(PString&&--PString->RefCount==0){if(PString->OwnsData)free(PString->PStringData);delete PString;++ReleasedStrings;}PString=o.PString;}return *this;}
namespace std {_Rb_tree_iterator _Rb_tree::_M_insert(_Rb_tree_node*x,_Rb_tree_node*parent,const value_type&value,_Rb_tree_node*){bool left=parent==Header||x||value.first<parent->Value.first;_Rb_tree_node*n=new _Rb_tree_node;n->Colour=1;n->Parent=parent;n->Left=n->Right=0;n->Value=value;if(parent==Header)Header->Parent=Header->Left=Header->Right=n;else if(left){parent->Left=n;if(parent==Header->Left)Header->Left=n;}else{parent->Right=n;if(parent==Header->Right)Header->Right=n;}++Count;_Rb_tree_iterator r;r.Node=n;return r;}}
}
using namespace X_00756250;using namespace X_00756250::std;
static void Clear(_Rb_tree_node*n){if(!n)return;Clear(n->Left);Clear(n->Right);delete n;}
static CCharString Make(const char*text){CCharString r;r.PString=new CCharStringData;unsigned n=(unsigned)strlen(text);r.PString->PStringData=malloc(n+1);memcpy(r.PString->PStringData,text,n+1);r.PString->Length=n;r.PString->Capacity=n+1;r.PString->OwnsData=true;r.PString->RefCount=1;return r;}
int main(){{_Rb_tree t;_Rb_tree_node h;h.Colour=0;h.Parent=0;h.Left=h.Right=&h;t.Header=&h;t.Count=0;CCharString str[4];str[0]=Make("m");str[1]=Make("a");str[2]=Make("z");str[3]=Make("m");value_type v;for(int i=0;i<3;++i){v.first=str[i];v.second.first=10+i;v.second.second=20+i;pair<_Rb_tree_iterator,bool>r=t.insert_unique(v);assert(r.second&&r.first.Node->Value.second.first==10+i&&r.first.Node->Value.second.second==20+i);}v.first=str[3];v.second.first=99;assert(!t.insert_unique(v).second&&t.Count==3);v.first=CCharString();assert(t.insert_unique(v).second&&h.Left->Value.first.PString==0);assert(!t.insert_unique(v).second&&t.Count==4);Clear(h.Parent);for(int j=0;j<4;++j)assert(str[j].PString->RefCount==1);}assert(ReleasedStrings==4);puts("00756250_TEST PASS");}