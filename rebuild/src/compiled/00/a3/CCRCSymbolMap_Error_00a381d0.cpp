// CCRCSymbolMap::Error(const CWideString&,long,const char*,...) -- build and release a compiler-style diagnostic
// Retail0x00A381D0,263 bytes. Native member varargs ABI puts this on stack. Buffers are line[12] and message[256].
// Literal0x0129C14C="(%d)",0x0129C138=" : error C2065 : ",0x0122D70E="". /GS cookie is present.
// Flags: /GS
#include "fable_string.h"
#include <stdio.h>
#include <stdarg.h>
struct CRetailWideString : CWideString {
    CCharString ConvertToCharString() const; // 0x0099B2C0
};
CCharString FABLE_FASTCALL operator+(const CCharString&,const CCharString&); // 0x0099F570
// Existing operator+(CCharString,const char*) is native0x0099F600.
struct CCRCSymbolMap { void Error(const CRetailWideString& file,long line,const char* format,...); };
void CCRCSymbolMap::Error(const CRetailWideString& file,long line,const char* format,...) {
    CCharString result;
    char lineText[12]; char message[256];
    sprintf(lineText,"(%d)",line+1);
    va_list args; va_start(args,format); vsprintf(message,format,args);va_end(args);
    result=CCharString("",-1)+file.ConvertToCharString()+lineText+" : error C2065 : "+message;
}