// 009ac5d0 StringMap::Index
// Flags: /O2 /Oy /W3
// Leaf: Index
// Layout: tree header0,count4; node key+16,value+20; mapped value four WideString words and a zero dword. Retail calls confirm.
namespace N009ac5d0 {

#pragma pack(push,1)
struct Basic {char* Data;int length;unsigned capacity;unsigned char flags;int refs;bool Less(const Basic&)const;};
struct WideData {unsigned short* text;int length;unsigned capacity;int refs;};
#pragma pack(pop)
struct String {Basic* Data;String();String(const char*);String(const String&);~String();String& operator=(const String&); __forceinline bool operator<(const String& o)const {if(o.Data==Data)return false;if(!Data)return true;if(!o.Data)return false;return Data->Less(*o.Data);}};
struct Wide {WideData* Data;Wide();Wide(const unsigned short*);Wide(const Wide&);~Wide();Wide& operator=(const Wide&);};
struct Value {Wide A,B,C,D;unsigned int State;Value():State(0){}};
struct Pair {String Key;Value Mapped;Pair(const String& k,const Value& v):Key(k),Mapped(v){}};
struct Node {unsigned colour;Node* parent;Node* left;Node* right;String Key;Value Mapped;};
struct Iterator {Node* Ptr;Iterator(Node* p):Ptr(p){}__forceinline Iterator(const Iterator& o):Ptr(o.Ptr){}__forceinline bool operator==(const Iterator&o)const{return Ptr==o.Ptr;}};
struct StringMap {Node* Head;unsigned int Count;Node* LowerBound(const String&);Iterator Insert(Iterator,const Pair&);__forceinline Iterator end()const{return Iterator(Head);}Value& Index(const String&);};
Value& StringMap::Index(const String& key){
 Node* p=LowerBound(key);
 if(Iterator(p)==end() || key<p->Key) {Value value;Pair pair(key,value);Iterator result=Insert(p,pair);p=result.Ptr;}
 return p->Mapped;
}
}