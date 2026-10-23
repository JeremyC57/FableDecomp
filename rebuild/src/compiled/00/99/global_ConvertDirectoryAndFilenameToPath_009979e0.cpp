// 009979e0 NPath009979e0::ConvertDirectoryAndFilenameToPath
// Flags: /O2 /Oy /W3 /Gr
// Leaf: ConvertDirectoryAndFilenameToPath
namespace NPath009979e0 {
// Layout: string object is one pointer; retail stack locals +8 and +12.
struct Wide {void* data;Wide();Wide(const Wide&);~Wide();const unsigned short* Text()const;int Length()const;void Set(const unsigned short*,int);Wide& operator=(const Wide&);};
Wide __fastcall operator+(const Wide&,const unsigned short*);
Wide __fastcall operator+(const Wide&,const Wide&);
extern const unsigned short Slash[];
Wide __fastcall ConvertDirectoryAndFilenameToPath(const Wide& directory,const Wide& filename){
 Wide dir(directory),file(filename);
 int length; while((length=dir.Length(),dir.Text()[length])=='\\'){Wide temp;temp.Set(dir.Text(),dir.Length()-1);dir=temp;}
 while(file.Text()[0]=='\\'){Wide temp;temp.Set(file.Text()+1,file.Length()-1);file=temp;}
 return (dir+Slash)+file;
}
}