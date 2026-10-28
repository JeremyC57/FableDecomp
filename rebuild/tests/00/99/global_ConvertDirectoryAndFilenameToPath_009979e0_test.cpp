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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
extern "C++" {
// 0099b220 computes(end-begin)/2;0099b720/0099b510 retain/release refs+0x0c.
struct Buffer {unsigned short*first,*last,*limit;int refs;};
static int live,copies;const unsigned short g_Slash_0122de78[]={92,0};static const unsigned short Empty[]={0};
static void Drop(void*p){if(p){Buffer*b=(Buffer*)p;assert(b->refs>0);if(!--b->refs){free(b->first);delete b;--live;}}}
CWideString::CWideString():data(0){}
CWideString::CWideString(const CWideString&x):data(x.data){if(data)++((Buffer*)data)->refs;++copies;}
CWideString::~CWideString(){Drop(data);data=0;}
const unsigned short*CWideString::GetAsCharArray()const{return data?((Buffer*)data)->first:Empty;}
long CWideString::GetLength()const{return data?((Buffer*)data)->last-((Buffer*)data)->first:0;}
void CWideString::SetAsSubString(const unsigned short*p,long n){assert(n>=0);Buffer*b=0;if(p&&n){b=new Buffer;b->first=(unsigned short*)malloc((n+1)*2);memcpy(b->first,p,n*2);b->first[n]=0;b->last=b->first+n;b->limit=b->last+1;b->refs=1;++live;}Drop(data);data=b;}
const CWideString&CWideString::operator=(const CWideString&x){if(data!=x.data){Drop(data);data=x.data;if(data)++((Buffer*)data)->refs;}return *this;}
CWideString __fastcall operator+(const CWideString&a,const unsigned short*b){unsigned short text[256];int n=a.GetLength();assert(n<128);memcpy(text,a.GetAsCharArray(),n*2);while(*b){assert(n<255);text[n++]=*b++;}CWideString out;out.SetAsSubString(text,n);return CWideString(out);}
CWideString __fastcall operator+(const CWideString&a,const CWideString&b){return a+b.GetAsCharArray();}
}
int main(){assert(sizeof(CWideString)==4&&offsetof(Buffer,refs)==0x0c);
 {unsigned short d[]={65,0},f[]={92,92,66,0};CWideString dir,file;dir.SetAsSubString(d,1);file.SetAsSubString(f,3);Buffer*db=(Buffer*)dir.data,*fb=(Buffer*)file.data;
 {CWideString copy(dir);assert(copy.data==dir.data&&db->refs==2);CWideString assigned;assigned=copy;assert(assigned.data==dir.data&&db->refs==3);assigned=assigned;assert(db->refs==3);}assert(db->refs==1);
 {CWideString result=CAFile::ConvertDirectoryAndFilenameToPath(dir,file);assert(result.GetLength()==3&&result.GetAsCharArray()[0]==65&&result.GetAsCharArray()[1]==92&&result.GetAsCharArray()[2]==66);assert(result.data!=dir.data&&result.data!=file.data);assert(dir.data==db&&file.data==fb&&db->refs==1&&fb->refs==1&&file.GetLength()==3&&file.GetAsCharArray()[0]==92);}
 unsigned short trailing[]={65,92,0};dir.SetAsSubString(trailing,2);
 {CWideString dup=CAFile::ConvertDirectoryAndFilenameToPath(dir,file);assert(dup.GetLength()==4&&dup.GetAsCharArray()[1]==92&&dup.GetAsCharArray()[2]==92&&dup.GetAsCharArray()[3]==66);assert(dir.GetLength()==2&&file.GetLength()==3);}
 {CWideString empty;CWideString e=CAFile::ConvertDirectoryAndFilenameToPath(empty,empty);assert(!empty.data&&e.GetLength()==1&&e.GetAsCharArray()[0]==92);}
 unsigned short slashOnly[]={92,92,0};file.SetAsSubString(slashOnly,2);
 {CWideString e=CAFile::ConvertDirectoryAndFilenameToPath(dir,file);assert(e.GetLength()==3&&e.GetAsCharArray()[2]==92&&file.GetLength()==2);}
 assert(live==2&&((Buffer*)dir.data)->refs==1&&((Buffer*)file.data)->refs==1&&copies>0);
 }
 assert(live==0);puts("009979e0_TEST PASS");
}