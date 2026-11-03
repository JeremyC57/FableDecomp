// NParticleEngine::CParticleEmitterDatabase::Load(CAFile&,const CEmitterDatabaseSerialiseInfo&) -- loads emitter templates and their header path.
// Leaf: ?Load@CParticleEmitterDatabase@NParticleEngine@@IAEXAAVCAFile@@ABVCEmitterDatabaseSerialiseInfo@2@@Z
// Flags: /Gr
#define FABLE_RETAIL_EMITTER_DATABASE
#define FABLE_RETAIL_PARTICLE_SERIALISE
#define FABLE_RETAIL_REGION_GRAPH
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_STLPORT_HTTP
#define FABLE_RETAIL_QUEST_COMPLETION
#define FABLE_STLPORT_CONSTRUCTORS
#include "engine/NParticleEngine_CParticleEmitterDatabase.h"
#include "engine/CParticleEmitter.h"
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CAFile.h"

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
