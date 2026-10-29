#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
// Retail CStringParser is0x28, proven by landed00404720: Comments one pointer+14, ErrorCallback+18, CursorPos[3]+1c.
// Generated parser header is0x30 because donor Comments occupies12 bytes; its explicit vptr also cannot supply virtual cleanup.
// CAnimComponentCombatHandedness::Initialise(CPersistContext&, const CCharString&) -- parse and store start/end handedness.
// Leaf: ?Initialise@CAnimComponentCombatHandedness@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z
// Retail: Fable.exe @ 0x005DC550, 252-byte virtual method (the row's final 4 bytes are padding). Reset is vtable slot 8;
// parser/string/list/base cleanup callees are 0x0099EBF0, 0x00404720, 0x0099EAE0,
// 0x004046B0, and 0x0099A2E0. The PDB header is data-only (size 0x10); it confirms
// StartHandedness +8 and EndHandedness +0xc, while the local virtual model supplies slots.
class CPersistContext;
class CCharString { public: void* m_pData; CCharString(const char*, long); ~CCharString(); };
class CBaseClass {
public:  virtual ~CBaseClass(); };
struct CStringParserCommentList { void* head; ~CStringParserCommentList(); }; // retail list is one sentinel pointer (4 bytes)
class CStringParser : public CBaseClass { public:
 char* String; unsigned long BaseLineOffset; CCharString Filename; CCharString TempCharString;
 CStringParserCommentList Comments; void (*ErrorCallback)(CCharString*); long CursorPos[3];
 CStringParser(const CCharString&, void (*)(CCharString*), const CCharString&, long);
 bool SkipPastNextItemIfIs(const CCharString&); // 009B9E00
};
void __fastcall TransferIn(CPersistContext*, CStringParser*, long*); // 009E2590
inline long ParseLong(CPersistContext& c, CStringParser& p) { long v; TransferIn(&c,&p,&v); return v; }
class CAnimComponentCombatHandedness { public:
 virtual ~CAnimComponentCombatHandedness(); virtual void v01(); virtual void v02();
 virtual bool Initialise(CPersistContext&, const CCharString&); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void Reset();
 long IVRefCount; long StartHandedness; long EndHandedness;
};
bool CAnimComponentCombatHandedness::Initialise(CPersistContext& context, const CCharString& text) {
 Reset();
 CStringParser parser(text, 0, CCharString("", -1), 0); // pooled empty filename at 0x0122D70E
 long start=ParseLong(context, parser);
 bool notFound;
 {
  CCharString marker(",", -1); // marker literal at 0x0122E024
  notFound=!parser.SkipPastNextItemIfIs(marker);
 }
 if (notFound) return false;
 long end=ParseLong(context, parser);
 StartHandedness=start;
 EndHandedness=end;
 return true;
}

class CPersistContext { public: long values[2]; int calls; };
static bool bad,found;static char events[48];static int eventCount;
static CPersistContext* gContext;static const CCharString* gText;static CStringParser* gParser;
static CAnimComponentCombatHandedness* gOwner;
static void event(char c) {if(eventCount>=47){bad=true;return;}events[eventCount++]=c;events[eventCount]=0;}
static bool same(const char* s){return std::strcmp(events,s)==0;}
CCharString::CCharString(const char* text,long length):m_pData((void*)text) {
 if(!text)return;
 if(!text[0]) {if(length!=-1 || !same("R"))bad=true;event('E');}
 else if(text[0]==','&&!text[1]) {if(length!=-1 || !same("REPFA"))bad=true;event('M');}
}
CCharString::~CCharString() {
 if(gParser && this==&gParser->TempCharString){if(!same(found?"REPFAMSDBL":"REPFAMSDL"))bad=true;event('T');}
 else if(gParser && this==&gParser->Filename){event('N');}
 else if(m_pData && !((const char*)m_pData)[0]){if(!same("REP"))bad=true;event('F');}
 else if(m_pData && ((const char*)m_pData)[0]==',' && !((const char*)m_pData)[1]){if(!same("REPFAMS"))bad=true;event('D');}
}
CBaseClass::~CBaseClass(){if(this!=(CBaseClass*)gParser)bad=true;event('Z');}
CStringParserCommentList::~CStringParserCommentList(){if(this!=&gParser->Comments)bad=true;event('L');}
CStringParser::CStringParser(const CCharString& text,void(*cb)(CCharString*),const CCharString& filename,long mode)
 :String(0),BaseLineOffset(0),Filename(0,-1),TempCharString(0,-1),Comments(),ErrorCallback(cb) {
 if(&text!=gText || cb || mode || !filename.m_pData || ((const char*)filename.m_pData)[0] || !same("RE"))bad=true;
 gParser=this;Comments.head=0;CursorPos[0]=CursorPos[1]=CursorPos[2]=0;event('P');
}
bool CStringParser::SkipPastNextItemIfIs(const CCharString& marker){
 if(this!=gParser || !marker.m_pData || std::strcmp((const char*)marker.m_pData,",") || !same("REPFAM"))bad=true;
 event('S');return found;
}
static void transfer(CPersistContext& c,CStringParser& p,long* dest) {
 if(&c!=gContext || &p!=gParser || c.calls>1 || (c.calls==0 && !same("REPF")) || (c.calls==1 && !same("REPFAMSD")))bad=true;
 
 event(c.calls==0?'A':'B'); *dest=c.values[c.calls++];
}
void __fastcall TransferIn(CPersistContext* c,CStringParser* p,long* dest){transfer(*c,*p,dest);}
CAnimComponentCombatHandedness::~CAnimComponentCombatHandedness(){}
void CAnimComponentCombatHandedness::v01(){}
void CAnimComponentCombatHandedness::v02(){}
void CAnimComponentCombatHandedness::v04(){}
void CAnimComponentCombatHandedness::v05(){}
void CAnimComponentCombatHandedness::v06(){}
void CAnimComponentCombatHandedness::v07(){}

void CAnimComponentCombatHandedness::Reset(){StartHandedness=0; EndHandedness=0;}
struct TestComponent:CAnimComponentCombatHandedness{void Reset(){if(eventCount)bad=true;event('R');CAnimComponentCombatHandedness::Reset();}};
static bool run(bool marker,long first,long second){
 TestComponent component;CCharString text("input",-1);CPersistContext context;context.values[0]=first;context.values[1]=second;context.calls=0;
 component.StartHandedness=1;component.EndHandedness=1;gOwner=&component;gText=&text;gContext=&context;gParser=0;eventCount=0;events[0]=0;bad=false;found=marker;
 bool got=component.Initialise(context,text);
 if(got!=marker || bad || !same(marker?"REPFAMSDBLTNZ":"REPFAMSDLTNZ") || context.calls!=(marker?2:1))return false;
 if(component.StartHandedness!=(marker?first:0) || component.EndHandedness!=(marker?second:0))return false;
 
 gParser=0;return true;
}
int main(){
#ifdef _MSC_VER
 if(sizeof(CStringParser)!=0x28)return 2;
#endif
 bool ok=run(true,3,7)&&run(true,-4,0)&&run(false,9,11);std::puts(ok?"005dc550_TEST PASS":"005dc550_TEST FAIL");return ok?0:1;
}