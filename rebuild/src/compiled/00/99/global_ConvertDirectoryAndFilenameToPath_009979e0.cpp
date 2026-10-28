// 009979e0 CAFile::ConvertDirectoryAndFilenameToPath (catalog; bsim .99 static CAFile member; CWideString method names from FableWin.pdb)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: ConvertDirectoryAndFilenameToPath
// Layout: string object is one pointer; retail stack locals +8 and +12.
class CWideString { public:void* data;CWideString();CWideString(const CWideString&);~CWideString();const unsigned short* GetAsCharArray()const;long GetLength()const;void SetAsSubString(const unsigned short*,long);const CWideString& operator=(const CWideString&);};
CWideString __fastcall operator+(const CWideString&,const unsigned short*);
CWideString __fastcall operator+(const CWideString&,const CWideString&);
extern const unsigned short g_Slash_0122de78[];
class CAFile { public:static CWideString __fastcall CAFile::ConvertDirectoryAndFilenameToPath(const CWideString&,const CWideString&);};
CWideString __fastcall CAFile::ConvertDirectoryAndFilenameToPath(const CWideString& directory,const CWideString& filename){
 CWideString dir(directory),file(filename);
 int length; while((length=dir.GetLength(),dir.GetAsCharArray()[length])=='\\'){CWideString temp;temp.SetAsSubString(dir.GetAsCharArray(),dir.GetLength()-1);dir=temp;}
 while(file.GetAsCharArray()[0]=='\\'){CWideString temp;temp.SetAsSubString(file.GetAsCharArray()+1,file.GetLength()-1);file=temp;}
 return (dir+g_Slash_0122de78)+file;
}
