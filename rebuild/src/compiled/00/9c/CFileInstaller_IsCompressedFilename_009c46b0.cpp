// CFileInstaller::IsCompressedFilename(const CWideString&,CWideString&) -- replace recognized compressed extensions.
// Leaf: IsCompressedFilename
// Retail009C46B0,240bytes. Donor CFileInstaller CompressedFileTypes+b4 -> native+94 (smaller STL).
// Calls CAFile::SplitPath997620, wide ctors99AED0/dtors99B510, conversion99B2C0,
// lower99ED10, treeFind9AB5D0, char destructor99EAE0, charToWide99E480,widePlus99BE70,assign99B7D0.
// Missing conversion/lower declarations are field-free ABI method views over actual shared storage.
#define CCharString CCharStringCore
#define CWideString CWideStringCore
#include "fable_string.h"
#undef CWideString
#undef CCharString
class CWideString;
class CCharString:public CCharStringCore { public:CCharString();CCharString(const char*,long);~CCharString();CCharString ToLower() const;CWideString ToWideString() const;};
class CWideString:public CWideStringCore { public:CWideString();~CWideString();CCharString ConvertToCharString() const;CWideString& operator=(const CWideString&);};
CWideString __fastcall operator+(const CWideString&,const CWideString&);
struct CExtensionNode {unsigned char TreeLinks[0x10];CCharString first,second;};
struct CExtensionTree { CExtensionNode* Head;CExtensionNode* Find(const CCharString&); };
struct CExtensionMap:CExtensionTree { CExtensionNode* find(const CCharString& key){return Find(key);} };
class CAFile { public:static void __fastcall SplitPath(const CWideString&,CWideString*,CWideString*,CWideString*,CWideString*);};
class CFileInstaller { public:unsigned char EarlierState[0x94];CExtensionMap CompressedFileTypes;bool IsCompressedFilename(const CWideString&,CWideString&);};
bool CFileInstaller::IsCompressedFilename(const CWideString& filename,CWideString& output){
 bool result;
 { CWideString stem,extension;
 CAFile::SplitPath(filename,0,0,&stem,&extension);
 CExtensionNode* found=CompressedFileTypes.find(extension.ConvertToCharString().ToLower());
 if(found!=CompressedFileTypes.Head){
 output=stem+found->second.ToWideString();result=true;
 }else{output=stem+extension;result=false;}
 }return result;
}