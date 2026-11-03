// Self-contained behavior fixture: selected parser constructor and target body,
// with observable file/parser/resource dependencies. Byte fidelity is graded separately.
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#ifndef _MSC_VER
#define __fastcall
#endif
static void Check(bool value){if(!value)abort();}
class CASuspendableProcess{};
class CAFile;
class CCharString {
public:
 std::string Text;
 CCharString(){}
 CCharString(const char* p):Text(p){}
 CCharString(const std::string& p):Text(p){}
 CCharString(const CCharString& p):Text(p.Text){}
 const CCharString& operator=(const CCharString& p){Text=p.Text;return *this;}
 const char* GetAsCharArray()const{return Text.c_str();}
 void LoadFromFile(CAFile&,CASuspendableProcess*,long);
};
class CAFile {
public:
 std::string Contents;unsigned Reads;
 explicit CAFile(const std::string& s):Contents(s),Reads(0){}
 static CCharString __fastcall GetAbsoluteFilePath(const CCharString& from,const CCharString& to){
  Check(from.Text=="effects/source.par");
  if(!to.Text.empty()&&to.Text[0]=='/')return CCharString(to);
  return CCharString(std::string("effects/")+to.Text);
 }
};
void CCharString::LoadFromFile(CAFile& file,CASuspendableProcess* process,long length){Check(!process&&length==-1);Text=file.Contents;++file.Reads;}
class CStringParser {
public:
 CCharString TempCharString;std::string String;size_t Position;
 CStringParser(const CCharString&,void (__fastcall*)(const CCharString&)=0,const CCharString& filename=CCharString(""),long base_line_offset=0);
 void Init(const char* text,void (__fastcall* callback)(const CCharString&),const CCharString& filename,long base_line_offset){Check(!callback&&filename.Text.empty()&&base_line_offset==0);String=text;Position=0;}
 bool SkipPastString(const CCharString& token){size_t at=String.find(token.Text,Position);if(at==std::string::npos)return false;Position=at+token.Text.size();return true;}
 bool SkipPastWholeString(const CCharString& token){return SkipPastString(token);}
 CCharString ReadAsStringUntilString(const CCharString& token){size_t at=String.find(token.Text,Position);Check(at!=std::string::npos);CCharString result(String.substr(Position,at-Position));Position=at;return result;}
 void SetStringPos(unsigned long position){Check(position<=String.size());Position=position;}
 long SkipWhitespace(){size_t first=Position;while(Position<String.size()&&(String[Position]==' '||String[Position]=='\n'||String[Position]=='\t'))++Position;return (long)(Position-first);}
 bool SkipPastNextItemIfIs(const CCharString& token){if(String.compare(Position,token.Text.size(),token.Text)!=0)return false;Position+=token.Text.size();return true;}
};
class CDefinitionManager{};class CSymbolMap{};
class CPersistContext {
public:CStringParser& Parser;bool Defaults;
 CPersistContext(CStringParser& parser,void (__fastcall* error)(const CCharString&),bool defaults,const CDefinitionManager* defs,const CSymbolMap* symbols,CCharString* clean):Parser(parser),Defaults(defaults){Check(!error&&!defs&&!symbols&&!clean);}
};
namespace NParticleEngine {
class CEmitterDatabaseSerialiseInfo {public:const CSymbolMap& GraphicSymbols;const CSymbolMap& MeshSymbols;CEmitterDatabaseSerialiseInfo(const CSymbolMap& a,const CSymbolMap& b):GraphicSymbols(a),MeshSymbols(b){}};
class CParticleEmitter {
public:
 std::string Loaded;const CEmitterDatabaseSerialiseInfo* Info;unsigned Loads;
 CParticleEmitter():Info(0),Loads(0){}
 void DatabaseSerialise(CPersistContext& context,const CEmitterDatabaseSerialiseInfo& info){Check(context.Defaults);Loaded=context.Parser.String;Info=&info;++Loads;}
};
class CParticleEmitterDatabase {
public:
 std::vector<CParticleEmitter*> Emitters;long CurrentHandle;CCharString ParFilename,HeaderFilename;std::vector<long> Handles;unsigned Deleted;
 CParticleEmitterDatabase():CurrentHandle(99),ParFilename("effects/source.par"),HeaderFilename("unchanged.h"),Deleted(0){}
 ~CParticleEmitterDatabase(){for(size_t i=0;i<Emitters.size();++i)delete Emitters[i];}
 void DeleteAllEmitterTemplates(){for(size_t i=0;i<Emitters.size();++i){delete Emitters[i];++Deleted;}Emitters.clear();Handles.clear();CurrentHandle=1;}
 CParticleEmitter* CreateEmitterTemplate(long& handle){CParticleEmitter* p=new CParticleEmitter;handle=CurrentHandle++;Handles.push_back(handle);Emitters.push_back(p);return p;}
 void Load(CAFile&,const CEmitterDatabaseSerialiseInfo&);
};
}
CStringParser::CStringParser(const CCharString& string,void (__fastcall* callback)(const CCharString&),const CCharString& filename,long base_line_offset)
{
 TempCharString=string;
 Init(string.GetAsCharArray(),callback,filename,base_line_offset);
}

void NParticleEngine::CParticleEmitterDatabase::Load(CAFile& file,const CEmitterDatabaseSerialiseInfo& info)
{
 CCharString script;
 DeleteAllEmitterTemplates();
 script.LoadFromFile(file,0,-1);
 CStringParser parser(script);
 while(parser.SkipPastString("BEGIN_EMITTER")) {
  long emitter_handle;
  CParticleEmitter* pemitter=CreateEmitterTemplate(emitter_handle);
  CCharString emitter_script=parser.ReadAsStringUntilString("END_EMITTER");
  CStringParser emitter_parser(emitter_script);
  CPersistContext context(emitter_parser,0,true,0,0,0);
  pemitter->DatabaseSerialise(context,info);
 }
 parser.SetStringPos(0);
 if(parser.SkipPastWholeString("HeaderFilename")) {
  parser.SkipWhitespace();
  parser.SkipPastNextItemIfIs("\"");
  HeaderFilename=parser.ReadAsStringUntilString("\"");
  HeaderFilename=CAFile::GetAbsoluteFilePath(ParFilename,HeaderFilename);
 }
}

int main(){
 unsigned cases=0;
 for(unsigned count=0;count<6;++count)for(unsigned prior=0;prior<3;++prior)for(unsigned mode=0;mode<4;++mode){
  std::string text,expected[6];
  if(mode==1)text="HeaderFilename \"before.h\"\n";
  for(unsigned i=0;i<count;++i){char value[32];sprintf(value,"name=emitter%u;\n",i);expected[i]=value;text+="BEGIN_EMITTER";text+=expected[i];text+="END_EMITTER\n";}
  if(mode==2)text+="\nHeaderFilename\t\"/absolute/after.h\"\n";
  if(mode==3)text+="\nHeaderFilename\t\"\"\n";
  NParticleEngine::CParticleEmitterDatabase database;
  for(unsigned i=0;i<prior;++i)database.Emitters.push_back(new NParticleEngine::CParticleEmitter);
  CSymbolMap graphics,meshes;NParticleEngine::CEmitterDatabaseSerialiseInfo info(graphics,meshes);CAFile file(text);
  database.Load(file,info);
  Check(file.Reads==1&&database.Deleted==prior&&database.Emitters.size()==count&&database.CurrentHandle==(long)count+1);
  for(unsigned i=0;i<count;++i)Check(database.Handles[i]==(long)i+1&&database.Emitters[i]->Loaded==expected[i]&&database.Emitters[i]->Loads==1&&database.Emitters[i]->Info==&info);
  const char* paths[]={"unchanged.h","effects/before.h","/absolute/after.h","effects/"};
  Check(database.HeaderFilename.Text==paths[mode]&&database.ParFilename.Text=="effects/source.par");
  ++cases;
 }
 Check(cases==72);puts("00ae8d50_TEST PASS");return 0;
}
