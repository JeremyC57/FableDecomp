// Behaviour test for NFile::GetItemFromPath (0x00997880): the component that ends at the (n+1)-th backslash, "\" for a root.
#include <cstdio>
#include <cstring>
#include <wchar.h>
static bool GetItemFromPath(const wchar_t* pathname, unsigned long directory_number, wchar_t* directory_name)
{
 long copy_start=-1;
 long no_directories=0;
 directory_name[0]=0;
 long c1=0;
 wchar_t cb=pathname[c1];
 for(;cb!=0;cb=pathname[++c1]){
  if(cb==L'\\'){
   ++no_directories;
   if(no_directories>(long)directory_number){
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
       std::memcpy(directory_name,pathname+copy_start,(c1-copy_start)*sizeof(wchar_t));
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
static bool Eq(const wchar_t* a, const wchar_t* b) { return wcscmp(a, b) == 0; }
int main()
{
    wchar_t out[64];
    if (!GetItemFromPath(L"C:\\Games\\Fable\\save.dat", 0, out) || !Eq(out, L"\\")) { std::printf("00997880_TEST FAIL\n"); return 1; }
    if (!GetItemFromPath(L"C:\\Games\\Fable\\save.dat", 1, out) || !Eq(out, L"Games")) { std::printf("00997880_TEST FAIL\n"); return 1; }
    if (!GetItemFromPath(L"C:\\Games\\Fable\\save.dat", 2, out) || !Eq(out, L"Fable")) { std::printf("00997880_TEST FAIL\n"); return 1; }
    if (GetItemFromPath(L"C:\\Games\\Fable\\save.dat", 3, out) || out[0] != 0) { std::printf("00997880_TEST FAIL\n"); return 1; }
    if (!GetItemFromPath(L"\\root\\x", 0, out) || !Eq(out, L"\\")) { std::printf("00997880_TEST FAIL\n"); return 1; }
    std::printf("00997880_TEST PASS\n");
    return 0;
}