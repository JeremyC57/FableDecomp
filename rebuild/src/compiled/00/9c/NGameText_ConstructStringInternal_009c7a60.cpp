// 009c7a60 NGameText::ConstructStringInternal
// Flags: /O2 /Oy /W3 /GS
// Leaf: ConstructStringInternal
#include <stdarg.h>
#include <stdlib.h>
#include <wchar.h>
namespace NGameText {
// Layout: wide strings one dword; integer conversion writes 64-byte stack buffer.
// Shape: index is initialised before the va_list and the first-argument test is the while condition itself (no
//   enclosing if); va_arg is read inside the body before ++index, while key is still alive. The default CWideString
//   constructor is only declared here (retail calls 0x0099AED0 in another TU; an in-TU body lets VC reuse ecx).
struct CWideStringData{unsigned short*text;unsigned length,capacity;int refs;};
extern int g_CWideStringInstanceCount_013BCA20;
struct CWideString {CWideStringData* data;CWideString();CWideString(const CWideString&);CWideString(const char*);CWideString(const unsigned short*);CWideString&operator+=(const CWideString&);~CWideString();void UnassignString();void __cdecl Format(const unsigned short*,...);bool Contains(const CWideString&)const;void Replace(const CWideString&,const CWideString&);};
__declspec(noinline) CWideString::CWideString(const CWideString&other):data(0){++g_CWideStringInstanceCount_013BCA20;if(&other!=this&&other.data!=data){if(data)UnassignString();if(other.data){data=other.data;++data->refs;}}}
__declspec(noinline) CWideString::~CWideString(){CWideStringData*storage=data;if(storage){--storage->refs;if(storage->refs<=0){storage=data;if(storage){if(storage->text)free(storage->text);delete storage;}}data=0;}--g_CWideStringInstanceCount_013BCA20;}
extern const unsigned short Empty[]; // retail129a8e0 UTF16 empty string
__declspec(noinline) bool CWideString::Contains(const CWideString&rhs)const{const unsigned short*needle=rhs.data?rhs.data->text:Empty;const unsigned short*haystack=data?data->text:Empty;return wcsstr((const wchar_t*)haystack,(const wchar_t*)needle)!=0;}
__declspec(noinline) CWideString __fastcall operator+(const unsigned short*left,const CWideString&right){CWideString result(left);result+=right;return result;}
void __fastcall FormatNumber_AddThousandsSeparators(int,unsigned short*);
extern const unsigned short FormatNumber[],Prefix[];
__forceinline CWideString FormatIndex(int index){CWideString n;n.Format(FormatNumber,index);return n;}
bool __cdecl ConstructStringInternal(CWideString&output,int first,...){bool result=true;int index=1;va_list args;va_start(args,first);int number=first;while(number!=-1){CWideString key=Prefix+FormatIndex(index);if(!output.Contains(key)){result=false;break;}unsigned short buffer[32];FormatNumber_AddThousandsSeparators(number,buffer);output.Replace(key,CWideString(buffer));number=va_arg(args,int);++index;}va_end(args);return result;}

}