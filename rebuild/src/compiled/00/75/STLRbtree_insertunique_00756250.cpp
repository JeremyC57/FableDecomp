// 00756250 std::_Rb_tree::insert_unique
// Flags: /O2 /Oy /W3
// Leaf: ?insert_unique@_Rb_tree@std@X_00756250@@QAE?AU?$pair@U_Rb_tree_iterator@std@X_00756250@@_N@23@ABU?$pair@UCCharString@X_00756250@@U?$pair@JJ@std@2@@23@@Z
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
class _Rb_tree {
public:_Rb_tree_node*Header;unsigned Count;private: _Rb_tree_iterator _M_insert(_Rb_tree_node*,_Rb_tree_node*,const value_type&,_Rb_tree_node*); public:pair<_Rb_tree_iterator,bool> insert_unique(const value_type&);};
pair<_Rb_tree_iterator,bool> _Rb_tree::insert_unique(const value_type&value){_Rb_tree_node*parent=Header,*node=Header->Parent;bool goLeft=true;while(node){parent=node;goLeft=value.first<node->Value.first;node=goLeft?node->Left:node->Right;}_Rb_tree_iterator previous;previous.Node=parent;if(goLeft){if(previous.Node==Header->Left){return pair<_Rb_tree_iterator,bool>(_M_insert(parent,parent,value,0),true);}previous._M_decrement();}if(previous.Node->Value.first<value.first){return pair<_Rb_tree_iterator,bool>(_M_insert(node,parent,value,0),true);}return pair<_Rb_tree_iterator,bool>(previous,false);}
}
}