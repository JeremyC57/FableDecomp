// 0055d810 CTCPhotojournalCapture::OnCaptionEntered
// Leaf: ?OnCaptionEntered@CTCPhotojournalCapture@@QAEXABVCWideString@@@Z
// Flags: none
#define FABLE_RETAIL_PHOTOJOURNAL_CAPTION_PASS29
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING_REPLACE_EGOD_PASS24
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES
#define FABLE_RETAIL_CDATEANDTIME_SYSTEMTIME
#include "engine/CTCPhotojournalCapture.h"
#include "engine/CUserProfileManager.h"
#include "engine/CDateAndTime.h"
struct _SYSTEMTIME { unsigned short wYear,wMonth,wDayOfWeek,wDay,wHour,wMinute,wSecond,wMilliseconds; };
extern "C" __declspec(dllimport) void __stdcall GetSystemTime(_SYSTEMTIME*);
namespace NResourceDirectoryNames { CWideString __fastcall GetPhotojournalDir(); }
CWideString __fastcall GetValidProfileName(const CWideString&);
void CTCPhotojournalCapture::OnCaptionEntered(const CWideString& caption_param)
{
    Description=caption_param;
    _SYSTEMTIME systime; GetSystemTime(&systime);
    CDateAndTime date(systime);
    CCharString date_amended=date.ToString();
    date_amended.Replace("\\","-");
    date_amended.Replace("/","-");
    date_amended.Replace(":",".");
    FileName=NResourceDirectoryNames::GetPhotojournalDir()+GetValidProfileName(CUserProfileManager::Get().GetProfileName())+L"\\"+date_amended.ToWideString()+L".tga";
    ClosePhotojournalCapture();
}