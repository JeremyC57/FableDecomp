// 009ac5d0 N009ac5d0::bankMap<CCharString,CBankFileManager::CBankInfo>::operator[] (catalog: bankMap<CCharString::operator[]; bsim .69; model kept in N009ac5d0)
// Flags: /O2 /Oy /W3
// Leaf: ??A
// Layout: tree header0,count4; node key+16,value+20; mapped value four WideString words and a zero dword. Retail calls confirm.
namespace N009ac5d0 {

#pragma pack(push,1)
struct CCharStringData {char* Data;int length;unsigned capacity;unsigned char flags;int refs;bool operator<(const CCharStringData&)const;};
struct CWideStringData {unsigned short* text;int length;unsigned capacity;int refs;};
#pragma pack(pop)
struct CCharString {CCharStringData* Data;CCharString();CCharString(const char*);CCharString(const CCharString&);~CCharString();CCharString& operator=(const CCharString&); __forceinline bool operator<(const CCharString& o)const {if(o.Data==Data)return false;if(!Data)return true;if(!o.Data)return false;return *Data<*o.Data;}};
struct CWideString {CWideStringData* Data;CWideString();CWideString(const unsigned short*);CWideString(const CWideString&);~CWideString();CWideString& operator=(const CWideString&);};
struct CBankInfo {CWideString A,B,C,D;unsigned int State;CBankInfo():State(0){}};
struct Pair {CCharString Key;CBankInfo Mapped;Pair(const CCharString& k,const CBankInfo& v):Key(k),Mapped(v){}};
struct Node {unsigned colour;Node* parent;Node* left;Node* right;CCharString Key;CBankInfo Mapped;};
struct Iterator {Node* Ptr;Iterator(Node* p):Ptr(p){}__forceinline Iterator(const Iterator& o):Ptr(o.Ptr){}__forceinline bool operator==(const Iterator&o)const{return Ptr==o.Ptr;}};
struct map {Node* Head;unsigned int Count;Node* lower_bound(const CCharString&);Iterator insert(Iterator,const Pair&);__forceinline Iterator end()const{return Iterator(Head);}CBankInfo& operator[](const CCharString&);};
CBankInfo& map::operator[](const CCharString& key){
 Node* p=lower_bound(key);
 if(Iterator(p)==end() || key<p->Key) {CBankInfo value;Pair pair(key,value);Iterator result=insert(p,pair);p=result.Ptr;}
 return p->Mapped;
}
}