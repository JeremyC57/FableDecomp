#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cwchar>
static void check(bool b){if(!b)std::abort();}
class CDriveManager{public:bool GetItemFromPath(const wchar_t*,unsigned long,wchar_t*);};
bool CDriveManager::GetItemFromPath(const wchar_t* pathname,unsigned long directory_number,wchar_t* directory_name)
{
 long copy_start=-1;
 long no_directories=0;
 directory_name[0]=0;
 long c1=0;
 wchar_t cb=pathname[c1];
 for(;cb!=0;cb=pathname[++c1]){
  if(cb==L'\\'){
   ++no_directories;
   if(no_directories>directory_number){
    if(c1==0||pathname[c1-1]==L':'){
     wcscat(directory_name,L"\\");
     return true;
    }else{
     for(long c2=c1-1;;--c2){
      if(c2==-1)copy_start=0;
      else{
       wchar_t cb2=pathname[c2];
       if(cb2==L'\\'||cb2==L':'||c2==0)copy_start=c2+1;
      }
      if(copy_start!=-1){
       memcpy(directory_name,pathname+copy_start,(c1-copy_start)*sizeof(wchar_t));
       directory_name[c1-copy_start]=0;
       return true;
      }
     }
    }
   }
  }
 }
 return false;
}

// Independent delimiter-position oracle, retaining retail's first-character omission on relative paths.
static bool expected(const wchar_t* p,unsigned long requested,wchar_t* out){
 out[0]=0;unsigned long seen=0;int previous=-1;
 for(int end=0;p[end];++end){
  if(p[end]!=L'\\')continue;
  if(seen++!=requested){previous=end;continue;}
  if(end==0||p[end-1]==L':'){out[0]=L'\\';out[1]=0;return true;}
  int start=previous+1;
  for(int k=start;k<end;++k)if(p[k]==L':')start=k+1;
  if(start==0)start=1;
  int n=0;for(int k=start;k<end;++k)out[n++]=p[k];out[n]=0;return true;
 }
 return false;
}
int main(){
 CDriveManager manager;wchar_t path[10],before[10],actual[16],want[16];
 const wchar_t alphabet[5]={L'a',L'b',L':',L'\\',wchar_t(0x03a9)};
 unsigned long cases=0;
 for(int length=0;length<=6;++length){
  unsigned long combinations=1;for(int j=0;j<length;++j)combinations*=5;
  for(unsigned long code=0;code<combinations;++code){
   unsigned long rem=code;for(int j=0;j<length;++j){path[j]=alphabet[rem%5];rem/=5;}path[length]=0;
   std::wcscpy(before,path);
   for(unsigned long index=0;index<8;++index){
    for(int j=0;j<16;++j)actual[j]=wchar_t(0x1111);
    bool reference=expected(path,index,want);bool result=manager.GetItemFromPath(path,index,actual);
    check(result==reference&&std::wcscmp(actual,want)==0);check(std::wcscmp(path,before)==0);
    check(actual[std::wcslen(want)+1]==wchar_t(0x1111));++cases;
   }
  }
 }
 check(cases==156248);std::puts("009e4cb0_TEST PASS");return 0;
}