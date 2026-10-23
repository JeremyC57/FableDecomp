// 0049e7c0 AutoSave_StartProgress
// Flags: /O2 /Oy
#include "engine/CAProgressDisplay.h"
#include <new>
// Layout: CProgressDisplay retail allocation136; counted info refs+0/deleter+4/data+8, pointer pair8bytes.
struct CProgressDisplay:CAProgressDisplay{char rest[132];CProgressDisplay();};
struct ProgressControl{int refs;void (__fastcall*destroy)(CAProgressDisplay*);CAProgressDisplay*data;};
struct ProgressPointer{CAProgressDisplay*data;ProgressControl*info;ProgressPointer():data(0),info(0){}ProgressPointer(CAProgressDisplay*);ProgressPointer(const ProgressPointer&p):data(p.data),info(p.info){if(info)++info->refs;}~ProgressPointer(){if(info){--info->refs;if(info->refs==0){info->destroy(info->data);operator delete(info);}}}ProgressPointer&operator=(const ProgressPointer&p){ProgressControl* incoming=p.info;CAProgressDisplay* object=p.data;if(info!=incoming){if(info){--info->refs;if(info->refs==0){info->destroy(info->data);operator delete(info);}}data=object;info=incoming;if(info)++info->refs;}return *this;}};
namespace NProgressDisplay{ProgressPointer __fastcall GetProgressDisplay();void __fastcall SetProgressDisplay(const ProgressPointer&);void __fastcall StartProgress(bool,bool,bool,bool,bool,bool);}
extern bool gAutoSaveProgress_013b89c1;extern ProgressPointer gPreviousProgress_013b89e0;
void AutoSave_StartProgress(){if(!gAutoSaveProgress_013b89c1){gPreviousProgress_013b89e0=NProgressDisplay::GetProgressDisplay();ProgressPointer display(new CProgressDisplay);NProgressDisplay::SetProgressDisplay(display);gAutoSaveProgress_013b89c1=true;NProgressDisplay::StartProgress(true,true,true,false,false,false);}}