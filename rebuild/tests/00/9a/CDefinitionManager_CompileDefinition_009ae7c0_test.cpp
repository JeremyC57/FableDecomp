#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
namespace fixture {
void check(bool value){if(!value)abort();}
struct CCharString:std::string {
 CCharString(const char* v=""):std::string(v){} CCharString(const std::string& v):std::string(v){}
 char GetChar(long n)const{return (*this)[n];}void SetChar(long n,char c){(*this)[n]=c;}
};
struct CDefString {CCharString value;CDefString(const CCharString& v):value(v){}operator CCharString()const{return value;}};
enum {PARSED_ITEM_STRING=2};
struct CParsedItem {int type;CCharString word;int GetType()const{return type;}const CCharString& GetString()const{return word;}};
struct CStringParser {
 CCharString text;int line,mode,tokens;size_t pos;std::string first,second;
 CStringParser(const CCharString& s,int,const char*,long l):text(s),line(l),mode(0),tokens(0),pos(0){}
 void Init(const CCharString& s,int,const char*,long l){check(l==line);text=s;pos=0;}
 long GetLine()const{return line;}long GetNoCharsIn()const{return (long)pos;}
 bool PeekNextItem(CParsedItem& o)const{o.type=mode==1?1:PARSED_ITEM_STRING;o.word=first;return mode!=0;}
 CCharString ReadAsString(){++tokens;return tokens==1?first:second;}
 CCharString ReadAsStringUntilString(const CCharString& v){size_t n=text.find(v,pos),start=pos;pos=(n==text.npos)?text.size():n;return text.substr(start,pos-start);}
 bool SkipPastString(const CCharString& v){size_t n=text.find(v,pos);if(n==text.npos)return false;pos=n+v.size();return true;}
 bool SkipPastNextItemIfIs(const CCharString& v){if(text.compare(pos,v.size(),v))return false;pos+=v.size();return true;}
};
struct CDefinitionManager;
struct CPersistContext {bool defaults;CCharString scanned;int start;bool* unknown;
 CPersistContext(CStringParser&,int,bool,CDefinitionManager*,int,const CCharString*);
 void CheckUnknownSymbols(){*unknown=true;}
};
struct CDefClassBase {
 long references;bool setup,is_template,applied,transferred,used_defaults;int payload;CCharString name;
 CDefClassBase():references(0),setup(false),is_template(false),applied(false),transferred(false),used_defaults(false),payload(0){}
 bool IsSetup()const{return setup;}bool IsTemplate()const{return is_template;}bool AreDefaultValsApplied()const{return applied;}
 CDefString GetInstantiationName()const{return CDefString(name);}void SetInstantiationName(CDefString n){name=n.value;}
 void SetAsSetup(bool b){setup=b;}void SetAsTemplate(bool b){is_template=b;}void SetDefaultValsApplied(bool b){applied=b;}
 void Copy(CDefClassBase* d){check(references==1&&d->references==1);long held=references;*this=*d;references=held;}void Transfer(CPersistContext& c){transferred=true;used_defaults=c.defaults;}
};
template<class T>struct CDefPointer {T* p;CDefPointer(T* ptr=0):p(ptr){if(p)++p->references;}CDefPointer(const CDefPointer& o):p(o.p){if(p)++p->references;}~CDefPointer(){if(p)check(--p->references>=0);}T* Get()const{return p;}T* operator->()const{return p;}};
struct Symbols {bool present;bool FindSymbol(const CCharString& n,long& id){check(n=="target");if(present)id=3;return present;}int Get()const{return 9;}};
struct CDefinitionManager {
 Symbols symbols,PSymbolMap;Symbols* PDefinitionSymbolMap;CDefClassBase dest,parent;long parent_id;int ErrorCallback;
 std::vector<CCharString> messages,subtext,subname;CCharString scanned;bool checked;int line;
 CDefinitionManager():PDefinitionSymbolMap(&symbols),parent_id(4),ErrorCallback(2),checked(false),line(-1){}
 long GetDefGlobalIndexFromName(const CCharString& n){check(n=="parent");return parent_id;}
 CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long i){check(i==3||i==4);return i==3?&dest:&parent;}
 void Error(CStringParser&,const CCharString& text){messages.push_back(text);}
 void CompileSubDefinition(const CCharString& text,const CCharString& name,long i){check(i==3);subtext.push_back(text);subname.push_back(name);}
 void CompileDefinition(CStringParser&,const CCharString&,const CCharString&);
 CDefPointer<CDefClassBase> GetPDefFromInstantiationName(const CCharString&);
};
CPersistContext::CPersistContext(CStringParser& p,int cb,bool use,CDefinitionManager* m,int map,const CCharString* s):defaults(use),scanned(*s),start(p.line),unknown(&m->checked) {check(cb==2&&map==9&&p.pos==0&&p.text==*s);check(m->dest.references==1&&m->parent.references==0);m->scanned=*s;m->line=start;}
void CDefinitionManager::CompileDefinition(CStringParser& in_parser, const CCharString& def_type_name, const CCharString& def_instantiation_name)
{
    long def_index;
    if (PDefinitionSymbolMap->FindSymbol(def_instantiation_name, def_index)) {
        CDefPointer<CDefClassBase> def = GetPDefFromGlobalIndex(def_index);
        long start_line = in_parser.GetLine();
        CParsedItem parsed_item;
        bool specialised = false;
        if (in_parser.PeekNextItem(parsed_item) && parsed_item.GetType() == PARSED_ITEM_STRING && parsed_item.GetString() == "specialises") {
            in_parser.ReadAsString();
            CCharString specialise_class_name = in_parser.ReadAsString();
            CDefPointer<CDefClassBase> specialise_class = GetPDefFromInstantiationName(specialise_class_name);
            if (specialise_class.Get()) {
                if (specialise_class->IsSetup()) {
                    specialised = true;
                    bool originally_template = def->IsTemplate();
                    CCharString original_name(static_cast<CCharString>(def->GetInstantiationName()));
                    def->Copy(specialise_class.Get());
                    def->SetAsSetup(false);
                    def->SetAsTemplate(originally_template);
                    def->SetInstantiationName(original_name);
                } else {
                    Error(in_parser, CCharString("Def ") + specialise_class_name + " is not setup to specialise.");
                }
            } else
                Error(in_parser, CCharString("Def ") + specialise_class_name + " does not exist.");
        }
        CCharString def_text = in_parser.ReadAsStringUntilString("#end_definition");
        CStringParser def_parser(def_text, ErrorCallback, "", start_line);
        bool use_defaults = true;
        if (specialised || def->AreDefaultValsApplied()) use_defaults = false;
        while (def_parser.SkipPastString("<")) {
            long begin_pos = def_parser.GetNoCharsIn() - 1;
            CCharString sub_def_name = def_parser.ReadAsStringUntilString(">");
            bool success = def_parser.SkipPastNextItemIfIs(">");
            CCharString end_marker = "<\\" + sub_def_name + ">";
            CCharString sub_text = def_parser.ReadAsStringUntilString(end_marker);
            success = def_parser.SkipPastString(end_marker);
            long end_pos = def_parser.GetNoCharsIn();
            for (long c1 = begin_pos; c1 < end_pos; ++c1)
                if (def_text.GetChar(c1) != '\n')
                    def_text.SetChar(c1, ' ');
            CompileSubDefinition(sub_text, sub_def_name, def_index);
        }
        def_parser.Init(def_text, ErrorCallback, "", start_line);
        CPersistContext def_persist_context(def_parser, ErrorCallback, use_defaults, this, PSymbolMap.Get(), &def_text);
        def->Transfer(def_persist_context);
        def->SetAsSetup(true);
        def->SetDefaultValsApplied(true);
        def_persist_context.CheckUnknownSymbols();
    } else
        Error(in_parser, "Unable to find def class called " + def_instantiation_name);
}
CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromInstantiationName(const CCharString& instantiation_name)
{
    long global_index = GetDefGlobalIndexFromName(instantiation_name);
    if (global_index > 0)
        return GetPDefFromGlobalIndex(global_index);
    return CDefPointer<CDefClassBase>();
}


}
int main(){
 using namespace fixture;
 for(int known=0;known<2;++known)for(int mode=0;mode<4;++mode)for(int parent=0;parent<4;++parent)
 for(int prior=0;prior<2;++prior)for(int tmpl=0;tmpl<2;++tmpl)for(int fragments=0;fragments<3;++fragments){
  CDefinitionManager m;m.symbols.present=known!=0;m.parent_id=parent==0?-1:parent==1?0:4;m.parent.setup=parent==3;
  m.parent.name="parent";m.parent.payload=27;m.parent.is_template=!tmpl;m.dest.name="target";m.dest.payload=8;m.dest.applied=prior!=0;m.dest.is_template=tmpl!=0;
  std::string raw="unused";if(fragments)raw+="<alpha>abc\nd<\\alpha>";if(fragments==2)raw+="&<bravo>word<\\bravo>";raw+=" tail";
  CStringParser p(CCharString(raw+"#end_definitionOTHER"),0,"",64);p.mode=mode;p.first=mode==3?"specialises":"other";p.second="parent";
  m.CompileDefinition(p,"model","target");
  check(m.dest.references==0&&m.parent.references==0);
  if(!known){check(m.messages.size()==1&&m.messages[0]=="Unable to find def class called target"&&!m.checked&&!m.dest.transferred&&p.tokens==0);continue;}
  bool wants=mode==3,ok=wants&&parent==3;check(p.tokens==(wants?2:0));check(m.dest.transferred&&m.checked&&m.line==64&&m.dest.setup&&m.dest.applied);
  check(m.dest.name=="target"&&m.dest.is_template==(tmpl!=0)&&m.dest.payload==(ok?27:8)&&m.dest.used_defaults==(!ok&&!prior));
  check(m.messages.size()==(wants&&!ok?1u:0u));if(wants&&!ok)check(m.messages[0]==(parent<2?"Def parent does not exist.":"Def parent is not setup to specialise."));
  check(m.subname.size()==(size_t)fragments);if(fragments)check(m.subname[0]=="alpha"&&m.subtext[0]=="abc\nd");if(fragments==2)check(m.subname[1]=="bravo"&&m.subtext[1]=="word");
  std::string expected=raw;bool in=false;for(size_t i=0;i<raw.size();++i){if(raw[i]=='<')in=true;if(in&&raw[i]!='\n')expected[i]=' ';if(raw[i]=='>'&&i>0&&(raw[i-1]=='a'||raw[i-1]=='o')&&i>5&&raw.rfind("<\\",i)==raw.rfind("<",i))in=false;}
  check(m.scanned==expected);
 }
 puts("009ae7c0_TEST PASS");return 0;
}