// CAFile::GetRelativeFilePath(CCharString const &,CCharString const &) -- construct the file path relative to the supplied base path.
// Leaf: ?GetRelativeFilePath@CAFile@@SI?AVCCharString@@ABV2@0@Z
// Flags: /GS /Gr
#define FABLE_RETAIL_RELATIVE_PATH
#include <stdlib.h>
#include "engine/CCharString.h"
#include "engine/CAFile.h"
#include "engine/CStringParser.h"
#include "engine/CParsedItem.h"
CCharString __fastcall operator+(const CCharString&,const CCharString&);
CCharString __fastcall operator+(const CCharString&,const char*);
void __fastcall CAFile::SplitPath(const CCharString& path,CCharString* drive,CCharString* directory,CCharString* filename,CCharString* extension)
{
 char drive_buffer[_MAX_DRIVE],directory_buffer[_MAX_DIR],filename_buffer[_MAX_FNAME],extension_buffer[_MAX_EXT];
 _splitpath(path,drive_buffer,directory_buffer,filename_buffer,extension_buffer);
 if(drive)*drive=drive_buffer;
 if(directory)*directory=directory_buffer;
 if(filename)*filename=filename_buffer;
 if(extension)*extension=extension_buffer;
}
CCharString __fastcall CAFile::GetRelativeFilePath(const CCharString& path1_absolute,const CCharString& path2_absolute)
{
 if(path1_absolute==path2_absolute)return CCharString("");
 CCharString path1_drive,path1_directory;
 SplitPath(path1_absolute,&path1_drive,&path1_directory,0,0);
 CCharString path2_drive,path2_directory,path2_filename,path2_extension;
 SplitPath(path2_absolute,&path2_drive,&path2_directory,&path2_filename,&path2_extension);
 CCharString path_relative;
 bool path_completed=false;
 CStringParser parser1(path1_directory),parser2(path2_directory);
 while(!path_completed) {
  CParsedItem parsed_item1,parsed_item2;
  bool directory_name_found_1=parser1.ReadAsStringUntilString("\\",parsed_item1);
  bool directory_name_found_2=parser2.ReadAsStringUntilString("\\",parsed_item2);
  if(!directory_name_found_1&&!directory_name_found_2) {
   path_relative=path2_filename+path2_extension;
   path_completed=true;
  } else {
   parser1.SkipPastString("\\");
   parser2.SkipPastString("\\");
   if(!directory_name_found_1||!directory_name_found_2||parsed_item1.GetString()!=parsed_item2.GetString()) {
    if(directory_name_found_1) {
     do {
      parser1.SkipPastNextItemIfIs("\\");
      path_relative+="..\\";
      directory_name_found_1=parser1.SkipUntilString("\\");
     } while(directory_name_found_1);
    }
    if(directory_name_found_2) {
     do {
      parser2.SkipPastNextItemIfIs("\\");
      if(parsed_item2.GetString()!="")path_relative+=parsed_item2.GetString()+"\\";
      directory_name_found_2=parser2.ReadAsStringUntilString("\\",parsed_item2);
     } while(directory_name_found_2);
    }
    path_relative+=path2_filename+path2_extension;
    path_completed=true;
   }
  }
 }
 return path_relative;
}
