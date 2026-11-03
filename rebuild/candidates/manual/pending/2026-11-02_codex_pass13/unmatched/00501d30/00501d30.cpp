// CWorldMap::GetLevelFilesFromWorldFile(const CWideString&, std::vector<CWideString>&) -- collect level filenames from world text.
// Leaf: ?GetLevelFilesFromWorldFile@CWorldMap@@IAEXABVCWideString@@AAV?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@@Z
#define FABLE_RETAIL_STLPORT_CHAR_FILL
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_PARSER_HELPERS
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CSTRINGPARSER
#include <string.h>
#include "engine/CWorldMap.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CStringParser.h"
#include "engine/CGenericException.h"
namespace NProgressDisplay { void __fastcall DisplayProgress(const CCharString&,float,bool,bool); }
namespace NResourceDirectoryNames { CWideString __fastcall GetLevelDir(); }
CCharString __fastcall operator+(const CCharString&,const CCharString&);
template<> void std::vector<CWideString>::resize(size_type);
inline CCharString::operator const char*() const
{ if(!PStringData) return ""; return PStringData->Data.PeekData(); }
inline bool CCharString::EqualsCaseInsensitive(const CCharString& other) const
{ return _stricmp(*this,other)==0; }
inline CStringParser::CStringParser(const char* text,void (__fastcall* error_callback)(const CCharString&),const CCharString& filename,long base_line_offset)
{ Init(text,error_callback,filename,base_line_offset); }

void CWorldMap::GetLevelFilesFromWorldFile(const CWideString& world_filename,std::vector<CWideString>& level_files)
{
    CDiskFileWin32 file(world_filename,FILE_ACCESS_READ,2);
    long file_length=file.GetLength();
    std::vector<char> text(file_length+1);
    file.Read(&text[0],file_length,0);
    text[file.GetLength()]=0;
    CParsedItem parsed_item;
    CStringParser parser(&text[0]);
    CCharString level_name;
    long map_id=0;
    while(parser.NextItemExists()) {
        if(parser.PeekNextItemType()!=PARSED_ITEM_STRING) { parser.ReadNextItem(parsed_item); continue; }
        {
            CCharString name=parser.ReadAsString();
            if(name=="NewMap") {
                NProgressDisplay::DisplayProgress("Getting level files from world map",-1.0f,false,false);
                map_id=parser.ReadAsInteger();
            } else if(name=="EndMap") {
                if(map_id) {
                    if(level_files.size()<(unsigned long)(map_id+1)) level_files.resize(map_id+1);
                    level_files[map_id]=level_name.ToWideString();
                }
            } else if(name=="LoadedOnPlayerProximity") {
                CCharString t=parser.ReadAsString();
                t.EqualsCaseInsensitive("TRUE");
            } else if(name=="LevelName") {
                if(!(parser.ReadAsSymbol()=="\"")) throw CGenericException();
                level_name=NResourceDirectoryNames::GetLevelDir().ToCharString()+parser.ReadAsStringUntilString("\"");
            }
        }
    }
}
