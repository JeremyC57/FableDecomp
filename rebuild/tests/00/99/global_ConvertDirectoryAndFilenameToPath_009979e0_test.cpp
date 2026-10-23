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
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
namespace NPath009979e0 {
// 0099b220 computes(end-begin)/2;0099b720/0099b510 retain/release refs+0x0c.
struct Buffer {unsigned short*first,*last,*limit;int refs;};
static int live,copies;const unsigned short Slash[]={92,0};static const unsigned short Empty[]={0};
static void Drop(void*p){if(p){Buffer*b=(Buffer*)p;assert(b->refs>0);if(!--b->refs){free(b->first);delete b;--live;}}}
Wide::Wide():data(0){}
Wide::Wide(const Wide&x):data(x.data){if(data)++((Buffer*)data)->refs;++copies;}
Wide::~Wide(){Drop(data);data=0;}
const unsigned short*Wide::Text()const{return data?((Buffer*)data)->first:Empty;}
int Wide::Length()const{return data?((Buffer*)data)->last-((Buffer*)data)->first:0;}
void Wide::Set(const unsigned short*p,int n){assert(n>=0);Buffer*b=0;if(p&&n){b=new Buffer;b->first=(unsigned short*)malloc((n+1)*2);memcpy(b->first,p,n*2);b->first[n]=0;b->last=b->first+n;b->limit=b->last+1;b->refs=1;++live;}Drop(data);data=b;}
Wide&Wide::operator=(const Wide&x){if(data!=x.data){Drop(data);data=x.data;if(data)++((Buffer*)data)->refs;}return *this;}
Wide __fastcall operator+(const Wide&a,const unsigned short*b){unsigned short text[256];int n=a.Length();assert(n<128);memcpy(text,a.Text(),n*2);while(*b){assert(n<255);text[n++]=*b++;}Wide out;out.Set(text,n);return Wide(out);}
Wide __fastcall operator+(const Wide&a,const Wide&b){return a+b.Text();}
}
int main(){using namespace NPath009979e0;assert(sizeof(Wide)==4&&offsetof(Buffer,refs)==0x0c);
 {unsigned short d[]={65,0},f[]={92,92,66,0};Wide dir,file;dir.Set(d,1);file.Set(f,3);Buffer*db=(Buffer*)dir.data,*fb=(Buffer*)file.data;
 {Wide copy(dir);assert(copy.data==dir.data&&db->refs==2);Wide assigned;assigned=copy;assert(assigned.data==dir.data&&db->refs==3);assigned=assigned;assert(db->refs==3);}assert(db->refs==1);
 {Wide result=ConvertDirectoryAndFilenameToPath(dir,file);assert(result.Length()==3&&result.Text()[0]==65&&result.Text()[1]==92&&result.Text()[2]==66);assert(result.data!=dir.data&&result.data!=file.data);assert(dir.data==db&&file.data==fb&&db->refs==1&&fb->refs==1&&file.Length()==3&&file.Text()[0]==92);}
 unsigned short trailing[]={65,92,0};dir.Set(trailing,2);
 {Wide dup=ConvertDirectoryAndFilenameToPath(dir,file);assert(dup.Length()==4&&dup.Text()[1]==92&&dup.Text()[2]==92&&dup.Text()[3]==66);assert(dir.Length()==2&&file.Length()==3);}
 {Wide empty;Wide e=ConvertDirectoryAndFilenameToPath(empty,empty);assert(!empty.data&&e.Length()==1&&e.Text()[0]==92);}
 unsigned short slashOnly[]={92,92,0};file.Set(slashOnly,2);
 {Wide e=ConvertDirectoryAndFilenameToPath(dir,file);assert(e.Length()==3&&e.Text()[2]==92&&file.Length()==2);}
 assert(live==2&&((Buffer*)dir.data)->refs==1&&((Buffer*)file.data)->refs==1&&copies>0);
 }
 assert(live==0);puts("009979e0_TEST PASS");
}