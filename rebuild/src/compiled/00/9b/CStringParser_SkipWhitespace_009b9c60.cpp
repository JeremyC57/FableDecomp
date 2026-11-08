// 009b9c60 CStringParser::SkipWhitespace
// Leaf: ?SkipWhitespace@CStringParser@@QAEJXZ
// Flags:
#include "fable_compiler.h"
#define CStringParser CStringParserFields
#include "engine/CStringParser.h"
#undef CStringParser
#include "engine/CBaseClass.h"
class CCharString{protected:CCharStringData*PStringData;};
class CStringParserCommentBoundaries;
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
struct _List_node_base{_List_node_base*_M_next,*_M_prev;};
template<class T>struct _List_node:public _List_node_base{T _M_data;};
template<class T,class A>class _List_base{protected:_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > >_M_node;};
template<class T,class A=allocator<T> >class list:public _List_base<T,A>{};
}
extern "C" int __cdecl isspace(int);
// Layout: shared flattened parser is restored to original public CBaseClass and typed fields.
// Native String4, two strings c/10, STLport list14, fastcall callback18, cursor1c; sizeof28.
// Original nested CCursorPos class has InQuotes +0, StringPos +4, Line +8.
class CStringParser:public CBaseClass {
public:
 class CCursorPos {public:bool InQuotes;unsigned long StringPos,Line;};
protected:
 const char*String;unsigned long BaseLineOffset;CCharString Filename,TempCharString;
 std::list<CStringParserCommentBoundaries>Comments;
 void (__fastcall*ErrorCallback)(const CCharString&);CCursorPos CursorPos;
public:virtual~CStringParser();
 long SkipWhitespace();
 FABLE_FORCEINLINE const char* PeekString(){return static_cast<const char*>(String)+CursorPos.StringPos;}
 FABLE_FORCEINLINE unsigned long GetStringPos()const{return CursorPos.StringPos;}
protected:
 FABLE_FORCEINLINE bool CalcInQuotes(bool inQuotes,const char*from,const char*to)const {
  for(const char*p=from;p!=to;++p)if(*p=='"'||*p=='\'')inQuotes=!inQuotes;return inQuotes;
 }
 FABLE_FORCEINLINE void MoveStringPos(unsigned long n){if(n){CursorPos.InQuotes=CalcInQuotes(CursorPos.InQuotes,static_cast<const char*>(String)+CursorPos.StringPos,static_cast<const char*>(String)+CursorPos.StringPos+n);CursorPos.StringPos+=n;}}
};
FABLE_STATIC_ASSERT(sizeof(CStringParser)==sizeof(CStringParserFields));
long CStringParser::SkipWhitespace(){
 if(*PeekString()){const char*p=PeekString();for(;;){if(!isspace((unsigned char)*p)&&*p!='\n')break;if(!*p)break;++p;}
 unsigned long oldPosition=GetStringPos();
 MoveStringPos((p-static_cast<const char*>(String))-oldPosition);
 return (p-static_cast<const char*>(String))-oldPosition;}
 return 0;
}