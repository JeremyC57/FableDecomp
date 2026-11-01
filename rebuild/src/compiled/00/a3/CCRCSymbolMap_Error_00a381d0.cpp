// CCRCSymbolMap::Error(const CWideString&,long,const char*,...) -- build and release a compiler-style diagnostic
// Leaf: ?Error@CCRCSymbolMap@@IAAXABVCWideString@@JPBDZZ
// Retail0x00A381D0,263 bytes. Native member varargs ABI puts this on stack. Buffers are line[12] and message[256].
// Literal0x0129C14C="(%d)",0x0129C138=" : error C2065 : ",0x0122D70E="". /GS cookie is present.
// Naming (naming wave 24): Ego_r.pdb / FableWin.pdb ?Error@CCRCSymbolMap@@IAAXABVCWideString@@JPBDZZ (protected; labels_chunk691) on the
//   engine CWideString, whose 0x0099B2C0 is ?ToCharString@CWideString@@QBE?AVCCharString@@XZ (both PDBs; DECISIONS wave 1). The row had
//   modelled the file name as a CRetailWideString view with a ConvertToCharString member. fable_string.h's CWideString does not declare
//   ToCharString, so the two string classes are declared here (same members as fable_string.h).
// Callees: CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / ~ 0x0099EAE0 / operator= 0x0099EFB0, CWideString::ToCharString
//   0x0099B2C0, operator+(const CCharString&, const CCharString&) 0x0099F570 / (const CCharString&, const char*) 0x0099F600 (__fastcall),
//   sprintf / vsprintf.
// Flags: /GS
#include <stdio.h>
#include <stdarg.h>
class CCharString {
public:
    CCharString();
    CCharString(const char* text, long length);
    ~CCharString();
    const CCharString& operator=(const CCharString& other);
private:
    void* storage_;
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const CCharString& left, const char* right);         // 0x0099F600
class CWideString {
public:
    CCharString ToCharString() const;                   // 0x0099B2C0
private:
    void* storage_;
};
class CCRCSymbolMap {
protected:
    void Error(const CWideString& file, long line, const char* format, ...);
};
void CCRCSymbolMap::Error(const CWideString& file,long line,const char* format,...) {
    CCharString result;
    char lineText[12]; char message[256];
    sprintf(lineText,"(%d)",line+1);
    va_list args; va_start(args,format); vsprintf(message,format,args);va_end(args);
    result=CCharString("",-1)+file.ToCharString()+lineText+" : error C2065 : "+message;
}