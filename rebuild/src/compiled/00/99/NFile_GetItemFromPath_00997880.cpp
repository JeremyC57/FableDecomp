// NFile::GetItemFromPath(const wchar_t* pathname, unsigned long directory_number, wchar_t* directory_name) -- copy out the
// path component that ends at the (directory_number+1)-th backslash; a root ("\" at the start or after a drive colon) gives
// "\"; false when the path has too few separators.
// Leaf: ?GetItemFromPath@NFile@@YI_NPBGKPAG@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00997880, 168 bytes, __fastcall (pathname ecx, directory_number edx, directory_name on the
//   stack, `ret 4`).
// Identity: Ego_r.pdb / FableWin.pdb ?GetItemFromPath@NFile@@YI_NPB_WKPA_W@Z (the free __fastcall twin of the landed
//   CDriveManager::GetItemFromPath 0x009E4CB0, the same body as a member: this row is that source as the NFile function;
//   the parameter names are the landed twin's).
// Callees: wcscat (MSVCR71 import thunk 0x00BFED5E) for the root case; memcpy is the intrinsic `rep movsd / movsb`.
//   Globals: the wide literal L"\\" 0x0122DE78.
#include <string.h>
#include <wchar.h>
namespace NFile {
bool __fastcall GetItemFromPath(const wchar_t* pathname, unsigned long directory_number, wchar_t* directory_name);
}

bool __fastcall NFile::GetItemFromPath(const wchar_t* pathname, unsigned long directory_number, wchar_t* directory_name)
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