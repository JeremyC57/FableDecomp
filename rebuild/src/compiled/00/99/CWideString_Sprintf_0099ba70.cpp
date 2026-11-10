// CWideString::Sprintf(string,...) -- format wide text using a fixed buffer or dynamic array.
// Inline-budget: real SetString is called at base settings; /d2inlT200 expands original string cleanup as native.
// Leaf: ?Sprintf@CWideString@@QAAXPBGZZ
// Retail:0099ba70,265 bytes,variadic cdecl member
// Flags: /GS /d2inlT200
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_SPRINTF_ORIGINAL
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#include <stdarg.h>
#include <wchar.h>
#include "fable_string.h"
#include "engine/CArray.h"
void CWideString::Sprintf(const wchar_t* string,...)
{
 const long HALF_STATIC_BUFFER_SIZE=4000;
 wchar_t sbuffer[HALF_STATIC_BUFFER_SIZE*2];
 CArray<wchar_t> dbuffer;
 wchar_t* temp_str=0;
 const long fmtlen=wcslen(string);
 if(fmtlen>HALF_STATIC_BUFFER_SIZE){
  dbuffer.resize(fmtlen+HALF_STATIC_BUFFER_SIZE);
  temp_str=&dbuffer[0];
 }else temp_str=sbuffer;
 long buffer_char_length=fmtlen+HALF_STATIC_BUFFER_SIZE;
 va_list arg_ptr;
 va_start(arg_ptr,string);
 long no_chars=_vsnwprintf(temp_str,buffer_char_length-1,string,arg_ptr);
 temp_str[buffer_char_length-1]=0;
 va_end(arg_ptr);
 if(no_chars<=buffer_char_length)SetString(temp_str);
}