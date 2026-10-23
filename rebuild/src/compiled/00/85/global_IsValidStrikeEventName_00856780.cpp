// 00856780 IsValidStrikeEventName
// Flags: /O2 /Oy /W3 
// Leaf: IsValidStrikeEventName
#include <string.h>
namespace ComboStrikes {
// Layout: CCharString one-pointer handle; CBasicString<char> data+0,length+4. Globals13bac64..84 are string handles.
template<class T>struct CBasicString {T*Data;unsigned Length;static int __fastcall Compare(const T*,const T*);};
extern const char EmptyString[];
struct CCharString{CBasicString<char>*Data;
 __forceinline const char*c_str()const{return !Data?EmptyString:Data->Data;}
 __forceinline unsigned size()const{return !Data?0:Data->Length;}
 __forceinline bool StartsWith(const CCharString&prefix)const{return strncmp(c_str(),prefix.c_str(),prefix.size())==0;}
 __forceinline bool operator==(const CCharString&v)const{if(Data==v.Data)return true;if(!Data||!v.Data)return false;if(Data->Length!=v.Data->Length)return false;return CBasicString<char>::Compare(Data->Data,v.Data->Data)==0;}
};
extern CCharString PrimaryPrefix,SecondaryPrefix,PrimaryAlternative,SecondaryAlternative;
extern CCharString BothFlags,SecondFlag,FirstFlag,NeitherFlag;
__forceinline bool Equal(CBasicString<char>*a,CBasicString<char>*b){if(a==b)return true;if(!a||!b)return false;if(a->Length!=b->Length)return false;return CBasicString<char>::Compare(a->Data,b->Data)==0;}
bool __fastcall IsValidStrikeEventName(const CCharString&name,bool&first,bool&prefix,bool&named,bool&second){
 prefix=false;bool primary=name.StartsWith(PrimaryPrefix);bool secondary=name.StartsWith(SecondaryPrefix);
 if(!primary&&!secondary){primary=name.StartsWith(PrimaryAlternative);secondary=name.StartsWith(SecondaryAlternative);}else prefix=true;
 if(primary||secondary){named=false;first=primary;return true;}
 named=true;CBasicString<char>*cached=name.Data;
 if(Equal(cached,BothFlags.Data)){first=true;second=true;return true;}
 if(Equal(cached,FirstFlag.Data)){first=true;second=false;return true;}
 if(Equal(cached,SecondFlag.Data)){first=false;second=true;return true;}
 if(Equal(cached,NeitherFlag.Data)){first=false;second=false;return true;}
 return false;
}
}