#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif
// Retail CStringParser is0x28, proven by landed00404720: Comments one pointer+14, ErrorCallback+18, CursorPos[3]+1c.
// Generated parser header is0x30 because donor Comments occupies12 bytes; its explicit vptr also cannot supply virtual cleanup.
// CAnimComponentCombatMisc::Initialise(CPersistContext&, const CCharString&) -- parse both combat miscellany flags separated by marker
// Leaf: ?Initialise@CAnimComponentCombatMisc@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z
// Retail: Fable.exe @ 0x005DC7B0, 256 bytes. Reset is slot 8; string/parser/list/base
// cleanup callees are 0x0099EBF0, 0x00404720, 0x0099EAE0, 0x004046B0, 0x0099A2E0.
// Header is data-only (size 0x0a), confirming MeleeFlourish +8 and MeleeKnockdown +9;
// the polymorphic local model supplies Initialise at slot 3 and Reset at slot 8.
class CPersistContext;
class CCharString { public: void* data; CCharString(const char*, long); ~CCharString(); };
struct CBase { virtual ~CBase(); };
struct CStringParserCommentList { void* head; ~CStringParserCommentList(); }; // retail sentinel-only list, 4 bytes
class CStringParser : public CBase { public:
 char* String; unsigned long BaseLineOffset; CCharString Filename; CCharString TempCharString;
 CStringParserCommentList Comments; void (*ErrorCallback)(CCharString*); long CursorPos[3];
 CStringParser(const CCharString&, void (*)(CCharString*), const CCharString&, long);
 bool SkipPastNextItemIfIs(const CCharString&); // 009B9E00
};
// bool TransferIn: 00403FF0 (thiscall).
template<class T> struct CPersistTraits { CPersistTraits() {} void TransferIn(CPersistContext&, CStringParser&, T*); };
class CAnimComponentCombatMisc { public:
 virtual ~CAnimComponentCombatMisc(); virtual void v1(); virtual void v2(); virtual bool Initialise(CPersistContext&, const CCharString&);
 virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7(); virtual void Reset();
 long base; bool MeleeFlourish; bool MeleeKnockdown;
};
bool CAnimComponentCombatMisc::Initialise(CPersistContext& context, const CCharString& text) {
 Reset();
 CStringParser parser(text, 0, CCharString("", -1), 0); // pooled empty filename at 0x0122D70E
 CPersistTraits<bool>().TransferIn(context, parser, &MeleeFlourish);
 bool notFound;
 { CCharString marker(",", -1); notFound=!parser.SkipPastNextItemIfIs(marker); } // literal at 0x0122E024
 if (notFound) return false;
 CPersistTraits<bool>().TransferIn(context, parser, &MeleeKnockdown);
 return true;
}

class CPersistContext { public: bool values[2]; int calls; };
static bool bad,found;static char events[48];static int eventCount;
static CPersistContext* gContext;static const CCharString* gText;static CStringParser* gParser;
static CAnimComponentCombatMisc* gOwner;
static void event(char c) {if(eventCount>=47){bad=true;return;}events[eventCount++]=c;events[eventCount]=0;}
static bool same(const char* s){return std::strcmp(events,s)==0;}
CCharString::CCharString(const char* text,long length):data((void*)text) {
 if(!text)return;
 if(!text[0]) {if(length!=-1 || !same("R"))bad=true;event('E');}
 else if(text[0]==','&&!text[1]) {if(length!=-1 || !same("REPFA"))bad=true;event('M');}
}
CCharString::~CCharString() {
 if(gParser && this==&gParser->TempCharString){if(!same(found?"REPFAMSDBL":"REPFAMSDL"))bad=true;event('T');}
 else if(gParser && this==&gParser->Filename){event('N');}
 else if(data && !((const char*)data)[0]){if(!same("REP"))bad=true;event('F');}
 else if(data && ((const char*)data)[0]==',' && !((const char*)data)[1]){if(!same("REPFAMS"))bad=true;event('D');}
}
CBase::~CBase(){if(this!=(CBase*)gParser)bad=true;event('Z');}
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
static void transfer(CPersistContext& c,CStringParser& p,bool* dest) {
 if(&c!=gContext || &p!=gParser || c.calls>1 || (c.calls==0 && !same("REPF")) || (c.calls==1 && !same("REPFAMSD")))bad=true;
 if(dest!=(c.calls==0?&gOwner->MeleeFlourish:&gOwner->MeleeKnockdown))bad=true;

 event(c.calls==0?'A':'B'); *dest=c.values[c.calls++];
}
template<class T> void CPersistTraits<T>::TransferIn(CPersistContext& c,CStringParser& p,T* dest){transfer(c,p,dest);}
CAnimComponentCombatMisc::~CAnimComponentCombatMisc(){}
void CAnimComponentCombatMisc::v1(){}
void CAnimComponentCombatMisc::v2(){}
void CAnimComponentCombatMisc::v4(){}
void CAnimComponentCombatMisc::v5(){}
void CAnimComponentCombatMisc::v6(){}
void CAnimComponentCombatMisc::v7(){}

void CAnimComponentCombatMisc::Reset(){MeleeFlourish=0; MeleeKnockdown=0;}
struct TestComponent:CAnimComponentCombatMisc{void Reset(){if(eventCount)bad=true;event('R');CAnimComponentCombatMisc::Reset();}};
static bool run(bool marker,bool first,bool second){
 TestComponent component;CCharString text("input",-1);CPersistContext context;context.values[0]=first;context.values[1]=second;context.calls=0;
 component.MeleeFlourish=1;component.MeleeKnockdown=1;gOwner=&component;gText=&text;gContext=&context;gParser=0;eventCount=0;events[0]=0;bad=false;found=marker;
 bool got=component.Initialise(context,text);
 if(got!=marker || bad || !same(marker?"REPFAMSDBLTNZ":"REPFAMSDLTNZ") || context.calls!=(marker?2:1))return false;
 if(component.MeleeFlourish!=(marker?first:first) || component.MeleeKnockdown!=(marker?second:0))return false;
 
 gParser=0;return true;
}
int main(){
#ifdef _MSC_VER
 if(sizeof(CStringParser)!=0x28)return 2;
#endif
 bool ok=run(true,false,false)&&run(true,false,true)&&run(true,true,false)&&run(true,true,true)&&run(false,false,true)&&run(false,true,false);std::puts(ok?"005dc7b0_TEST PASS":"005dc7b0_TEST FAIL");return ok?0:1;
}