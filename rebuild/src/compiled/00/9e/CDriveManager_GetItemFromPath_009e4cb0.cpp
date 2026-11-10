// CDriveManager::GetItemFromPath -- resolve a path through drive directory items.
// Retail: 0x009E4CB0, 179 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ?GetItemFromPath@CDriveManager@@AAE_NPBGKPAG@Z
#define FABLE_RETAIL_CDRIVEMANAGER_PATH
#include <string.h>
#include <wchar.h>
#include "engine/CDriveManager.h"
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