// CDiskFileWin32::Write -- write requested bytes, update position/length, throw typed file exceptions.
// Retail0099AA90 length314. Actual disk layout; null buffer checked before count; requested count updates position even on short success.
#include "engine/CDiskFileWin32.h"
#include "fable_string.h"
// Field-free adapter adds actual99EC30 copy API absent in donor fable_string.h; storage/ownership stays in actual base.
struct CStringFileArgument:CCharString {
 CStringFileArgument(const char* text,long length):CCharString(text,length){}
 CStringFileArgument(const CStringFileArgument&); //0099EC30 native narrowcopy
};
// Actual PDB ctorB04820: CBBBFileException(CCharString by value,EFileExceptionType).
class CExceptionBase {
public:
 CExceptionBase(){}
 CExceptionBase(const char*){}
};
class CBBBFileException : public CExceptionBase {
public:
 enum EFileExceptionType {NullBuffer=1,WriteFailed=5,NotOpenForWrite=8};
 EFileExceptionType Type;
 CBBBFileException(CStringFileArgument text,EFileExceptionType type):CExceptionBase(static_cast<const char*>(text)),Type(type){} //99E4C0 conversion, not logging
};
class CASuspendableProcess;
extern int (__stdcall* WriteFileImport)(void*,const void*,unsigned long,unsigned long*,void*); //IAT0143FDA4 kernel32.WriteFile
extern void (__fastcall* DebugMessage)(const char*); //013BCA00
struct CDiskFileWriter:CDiskFileWin32 {void Write(const void*,long,CASuspendableProcess*);};
void CDiskFileWriter::Write(const void* data,long size,CASuspendableProcess*){
 if(!data){CStringFileArgument message("Null dst parameter",-1);throw CBBBFileException(message,CBBBFileException::NullBuffer);}
 if(size==0)return;
 if(*reinterpret_cast<const fable_i32*>(AccessType)!=2){CStringFileArgument message("File is not open for writing",-1);throw CBBBFileException(message,CBBBFileException::NotOpenForWrite);}
 unsigned long written;
 if(WriteFileImport(WinFileHandle,data,size,&written,0)!=1){if(DebugMessage)DebugMessage("");CStringFileArgument message("Error writing file",-1);throw CBBBFileException(message,CBBBFileException::WriteFailed);}
 Position+=size;if(Position>Length)Length=Position;
}