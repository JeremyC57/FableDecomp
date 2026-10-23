#include <assert.h>
#include <stdio.h>
// BEGIN exact source snapshot SHA256 714ac7a57ca11397aabda0cc9857205bf609ee6e499ab7dba44e785215a2cd7b
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

// END exact source snapshot
CProgressDisplay::CProgressDisplay(){__vftable=0;}
void __fastcall destroyDisplay(CAProgressDisplay*p){delete (CProgressDisplay*)p;}
ProgressPointer::ProgressPointer(CAProgressDisplay*p):data(p),info(0){if(p){info=new ProgressControl;info->refs=1;info->data=p;info->destroy=destroyDisplay;}}
bool gAutoSaveProgress_013b89c1=false;ProgressPointer gPreviousProgress_013b89e0;ProgressPointer current;int starts=0;
namespace NProgressDisplay{ProgressPointer __fastcall GetProgressDisplay(){return ProgressPointer(current);}void __fastcall SetProgressDisplay(const ProgressPointer&p){current=p;}void __fastcall StartProgress(bool a,bool b,bool c,bool d,bool e,bool f){assert(a&&b&&c&&!d&&!e&&!f);++starts;}}
int main(){current=ProgressPointer(new CProgressDisplay);CAProgressDisplay*old=current.data;AutoSave_StartProgress();assert(gAutoSaveProgress_013b89c1&&starts==1&&gPreviousProgress_013b89e0.data==old&&current.data!=old);AutoSave_StartProgress();assert(starts==1);puts("0049e7c0_TEST PASS");}