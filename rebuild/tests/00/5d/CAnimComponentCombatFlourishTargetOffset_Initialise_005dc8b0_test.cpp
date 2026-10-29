#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
// Retail CStringParser is0x28, proven by landed00404720: Comments one pointer+14, ErrorCallback+18, CursorPos[3]+1c.
// Generated parser header is0x30 because donor Comments occupies12 bytes; its explicit vptr also cannot supply virtual cleanup.
// CAnimComponentCombatFlourishTargetOffset::Initialise(CPersistContext&, const CCharString&) -- enable target offset and parse x/y
// Leaf: ?Initialise@CAnimComponentCombatFlourishTargetOffset@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z
// Retail: Fable.exe @ 0x005DC8B0, 248 bytes (row includes 8 padding bytes). Reset is slot 8; parser, trait, string,
// list and base cleanup callees are 0x00404720, 0x0040F7E0, 0x0099EBF0, 0x0099EAE0,
// 0x004046B0, 0x0099A2E0. The generated data-only header gives size 0x11 and members
// TargetOffset +8, Vector_X +9, Vector_Y +0xd. Its virtual slots are absent, so this
// packed local polymorphic model uses the header's field layout and supplies slots 3/8.
class CPersistContext;
class CCharString { public: void* data; CCharString(const char*, long); ~CCharString(); };
class CBaseClass {
public:  virtual ~CBaseClass(); };
struct CStringParserCommentList { void* head; ~CStringParserCommentList(); }; // retail sentinel-only list, 4 bytes
class CStringParser : public CBaseClass { public:
 char* String; unsigned long BaseLineOffset; CCharString Filename; CCharString TempCharString;
 CStringParserCommentList Comments; void (*ErrorCallback)(CCharString*); long CursorPos[3];
 CStringParser(const CCharString&, void (*)(CCharString*), const CCharString&, long);
 bool SkipPastNextItemIfIs(const CCharString&); // 009B9E00
};
template<class T> struct CPersistTraits { CPersistTraits() {} void TransferIn(CPersistContext&, CStringParser&, T*); };
#pragma pack(push, 1)
class CAnimComponentCombatFlourishTargetOffset { public:
 virtual ~CAnimComponentCombatFlourishTargetOffset(); virtual void v1(); virtual void v2(); virtual bool Initialise(CPersistContext&, const CCharString&);
 virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7(); virtual void Reset();
 long base; bool TargetOffset; float TargetOffsetVector_X; float TargetOffsetVector_Y;
};
#pragma pack(pop)
bool CAnimComponentCombatFlourishTargetOffset::Initialise(CPersistContext& context, const CCharString& text) {
 Reset(); TargetOffset=true;
 CStringParser parser(text, 0, CCharString("", -1), 0);
 CPersistTraits<float>().TransferIn(context, parser, &TargetOffsetVector_X);
 bool notFound;
 { CCharString marker(",", -1); notFound=!parser.SkipPastNextItemIfIs(marker); }
 if (notFound) return false;
 CPersistTraits<float>().TransferIn(context, parser, &TargetOffsetVector_Y);
 return true;
}

class CPersistContext { public: float values[2]; int calls; };
static bool bad,found;static char events[48];static int eventCount;
static CPersistContext* gContext;static const CCharString* gText;static CStringParser* gParser;
static CAnimComponentCombatFlourishTargetOffset* gOwner;
static void event(char c) {if(eventCount>=47){bad=true;return;}events[eventCount++]=c;events[eventCount]=0;}
static bool same(const char* s){return std::strcmp(events,s)==0;}
CCharString::CCharString(const char* text,long length):data((void*)text) {
 if(!text)return;
 if(!text[0]) {if(length!=-1 || !same("R") || !gOwner->TargetOffset)bad=true;event('E');}
 else if(text[0]==','&&!text[1]) {if(length!=-1 || !same("REPFA"))bad=true;event('M');}
}
CCharString::~CCharString() {
 if(gParser && this==&gParser->TempCharString){if(!same(found?"REPFAMSDBL":"REPFAMSDL"))bad=true;event('T');}
 else if(gParser && this==&gParser->Filename){event('N');}
 else if(data && !((const char*)data)[0]){if(!same("REP"))bad=true;event('F');}
 else if(data && ((const char*)data)[0]==',' && !((const char*)data)[1]){if(!same("REPFAMS"))bad=true;event('D');}
}
CBaseClass::~CBaseClass(){if(this!=(CBaseClass*)gParser)bad=true;event('Z');}
CStringParserCommentList::~CStringParserCommentList(){if(this!=&gParser->Comments)bad=true;event('L');}
CStringParser::CStringParser(const CCharString& text,void(*cb)(CCharString*),const CCharString& filename,long mode)
 :String(0),BaseLineOffset(0),Filename(0,-1),TempCharString(0,-1),Comments(),ErrorCallback(cb) {
 if(&text!=gText || cb || mode || !filename.data || ((const char*)filename.data)[0] || !same("RE"))bad=true;
 gParser=this;Comments.head=0;CursorPos[0]=CursorPos[1]=CursorPos[2]=0;event('P');
}
bool CStringParser::SkipPastNextItemIfIs(const CCharString& marker){
 if(this!=gParser || !marker.data || std::strcmp((const char*)marker.data,",") || !same("REPFAM"))bad=true;
 event('S');return found;
}
static void transfer(CPersistContext& c,CStringParser& p,float* dest) {
 if(&c!=gContext || &p!=gParser || c.calls>1 || (c.calls==0 && !same("REPF")) || (c.calls==1 && !same("REPFAMSD")))bad=true;
 if(dest!=(c.calls==0?&gOwner->TargetOffsetVector_X:&gOwner->TargetOffsetVector_Y))bad=true;

 event(c.calls==0?'A':'B'); *dest=c.values[c.calls++];
}
template<class T> void CPersistTraits<T>::TransferIn(CPersistContext& c,CStringParser& p,T* dest){transfer(c,p,dest);}
CAnimComponentCombatFlourishTargetOffset::~CAnimComponentCombatFlourishTargetOffset(){}
void CAnimComponentCombatFlourishTargetOffset::v1(){}
void CAnimComponentCombatFlourishTargetOffset::v2(){}
void CAnimComponentCombatFlourishTargetOffset::v4(){}
void CAnimComponentCombatFlourishTargetOffset::v5(){}
void CAnimComponentCombatFlourishTargetOffset::v6(){}
void CAnimComponentCombatFlourishTargetOffset::v7(){}

void CAnimComponentCombatFlourishTargetOffset::Reset(){TargetOffsetVector_X=0; TargetOffsetVector_Y=0; TargetOffset=false;}
struct TestComponent:CAnimComponentCombatFlourishTargetOffset{void Reset(){if(eventCount)bad=true;event('R');CAnimComponentCombatFlourishTargetOffset::Reset();}};
static bool run(bool marker,float first,float second){
 TestComponent component;CCharString text("input",-1);CPersistContext context;context.values[0]=first;context.values[1]=second;context.calls=0;
 component.TargetOffsetVector_X=1;component.TargetOffsetVector_Y=1;gOwner=&component;gText=&text;gContext=&context;gParser=0;eventCount=0;events[0]=0;bad=false;found=marker;
 bool got=component.Initialise(context,text);
 if(got!=marker || bad || !same(marker?"REPFAMSDBLTNZ":"REPFAMSDLTNZ") || context.calls!=(marker?2:1))return false;
 if(component.TargetOffsetVector_X!=(marker?first:first) || component.TargetOffsetVector_Y!=(marker?second:0))return false;
 if(!component.TargetOffset)return false;

 gParser=0;return true;
}
int main(){
#ifdef _MSC_VER
 if(sizeof(CStringParser)!=0x28)return 2;
#endif
 bool ok=run(true,1.25f,-2.5f)&&run(true,0.0f,4.0f)&&run(false,3.5f,8.0f);std::puts(ok?"005dc8b0_TEST PASS":"005dc8b0_TEST FAIL");return ok?0:1;
}