// 009bd460 CMovieSubtitles::ParseSubtitles
// Flags: none
#define FABLE_RETAIL_ANIMATION_EVENTS_LOAD_PASS29
#define FABLE_RETAIL_PASS22_CCHARSTRING_GETASCHARARRAY_ORIGINAL_IFELSE
// Leaf: ?ParseSubtitles@CMovieSubtitles@@QAEXABVCWideString@@@Z
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_FONTMANAGER
#define FABLE_RETAIL_SUBTITLE_TEXT_DRAW_PASS29
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CFONTBANK_SHAREDATA_SIZE_BODY
#define FABLE_RETAIL_CCOUNTEDPOINTER_CFONTBANK_ASSIGN_SIZE_BODY
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CCRC_ORIGINAL_TYPED_LOOP_PASS27
#define FABLE_RETAIL_CRC32TAB_PASS29
#define FABLE_RETAIL_MOVIE_PARSE_PASS29
#define FABLE_RETAIL_CMOVIESUBTITLES_GETCHARWIDTHW
#define FABLE_RETAIL_CSUBTITLE_STRUCT
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CCHARSTRING_GETCRC_BYTELENGTH_PASS27
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_CAFILE_MAKEPATH
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC
#include "engine/CCountedPointer.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CStringParser.h"
#include "engine/CMovieSubtitles.h"
#include "engine/CFontManager.h"
#include "engine/CSystemManager.h"
#include "engine/SubtitleDrawGlobals.h"
template<> inline void CAFile::ReadVar<CCharString>(CCharString& var,CASuspendableProcess*) { var.LoadFromFile(*this,0,-1); }
void CMovieSubtitles::ParseSubtitles(const CWideString& path_name)
{
    unsigned int flags = 1;
    WordWrap_SetOption(0, flags);
    WordWrap_SetCallback(GetCharWidthW, 0);
    FrameCounter = 0;
    IsSubtitleTime = false;
    CWideString drive;
    CWideString directory;
    CWideString file_name;
    CAFile::SplitPath(path_name, &drive, &directory, &file_name, 0);
    file_name = CAFile::MakePath(drive, directory, file_name, L"txt");
    SubTitles.clear();
    if (!pTextBank) return;
    if (!CAFile::PathExists(file_name)) return;
    CDiskFileWin32 fl(file_name, FILE_ACCESS_READ, 2);
    CCharString text;
    fl.ReadVar(text, 0);
    fl.Close();
    CStringParser line_parser(text, 0, CCharString(""), 0);
    CParsedItem itm;
    while (line_parser.NextItemExists()) {
        CCharString SymbolName = line_parser.ReadAsString();
        long smins = line_parser.ReadAsInteger();
        line_parser.ReadNextItem(itm);
        float ssecs = line_parser.ReadAsFloat();
        long emins = line_parser.ReadAsInteger();
        line_parser.ReadNextItem(itm);
        float esecs = line_parser.ReadAsFloat();
        unsigned long text_index = pTextBank->FindIndexBySymbolCRC(SymbolName.GetCRC());
        if (text_index) {
          const NGameText::CDataBankTextEntry* Entry = pTextBank->GetTextInfo(text_index);
          if (Entry) {
            CSubTitle sub;
            sub.TextEntry = pTextBank->GetPTextEntry(text_index, true);
            sub.FrameStart=GetFrameNumber(smins,ssecs);
            sub.FrameEnd=GetFrameNumber(emins,esecs);
            sub.StartTime=smins*60+ssecs;
            sub.EndTime=emins*60+esecs;
            long total = sub.TextEntry->GetText().GetLength();
            SubTitles.push_back(sub);
          }
        }
    }
    PFont = GFGetSystemManager()->GetFontManager()->GetFont("ENG_ARIAL_18");
    PSoundManager = GFGetSystemManager()->GetRetailSoundManager007C();

}