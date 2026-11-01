// 00492fb4 CSubtitleRenderer::SetText
// Leaf: ?SetText@CSubtitleRenderer@@QAEXABVCFontBank@@ABVCCharString@@111KM_N2@Z
// Flags: /O1 /Oy-
#define CSubtitleRenderer CSubtitleRendererLayout
#include "engine/CSubtitleRenderer.h"
#undef CSubtitleRenderer
class CCharString;class CWideStringData;class CCharStringData;
class CWideString{
protected:CWideStringData*PStringData;CWideString(const CCharString&);friend class CCharString;
public:CWideString(const unsigned short*);CWideString(const CWideString&);~CWideString();
};
class CCharString{
protected:CCharStringData*PStringData;
public:CWideString ToWideString()const;
};
class CFontBank;
// Layout: imported original CSubtitleRenderer data; this overload adds no fields.
class CSubtitleRenderer:public CSubtitleRendererLayout{
public:
 void SetText(const CFontBank&,const CWideString&,const CWideString&,const CWideString&,const CWideString&,const CWideString&,unsigned long,float,bool,bool);
 void SetText(const CFontBank&,const CCharString&,const CCharString&,const CCharString&,const CCharString&,unsigned long,float,bool,bool);
};
void CSubtitleRenderer::SetText(const CFontBank&font,const CCharString&message,const CCharString&name,const CCharString&button_1_text,const CCharString&button_2_text,unsigned long flags,float display_time,bool update_while_paused,bool change_player_mode){
 SetText(font,message.ToWideString(),name.ToWideString(),button_1_text.ToWideString(),button_2_text.ToWideString(),L"",flags,display_time,update_while_paused,change_player_mode);
}