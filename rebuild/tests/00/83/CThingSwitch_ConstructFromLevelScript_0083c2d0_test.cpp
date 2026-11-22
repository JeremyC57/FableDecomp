#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
// Original script/name/scriptname locals and exact shared owner/parser/string APIs.
// Leaf: ?ConstructFromLevelScript@CThingSwitch@@MAE_NPBD@Z
// Flags: (none)
#define FABLE_RETAIL_CTHINGSWITCH_LEVELSCRIPT_PASS26
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSTRINGPARSER
#include "fable_string.h"
#include "engine/CThingSwitch.h"
#include "engine/CStringParser.h"
#pragma intrinsic(strcmp)


#include <cstdio>
#include <cstring>
#include <cstdlib>
static int failures=0,parser_constructions=0,parser_destructions=0,reads=0,skips=0;static bool base_ok=true;
#define CHECK(x) do{if(!(x)){++failures;std::printf("FAIL %d\n",__LINE__);}}while(0)
struct FixtureString{char text[100];FixtureString(const char*s=""){CHECK(std::strlen(s)<100);std::strcpy(text,s);}bool operator==(const char*s)const{return std::strcmp(text,s)==0;}};
struct FixtureParser{const char*text;unsigned pos;FixtureParser(const char*s):text(s),pos(0){++parser_constructions;}~FixtureParser(){++parser_destructions;}void White(){while(text[pos]==' '||text[pos]=='\n'||text[pos]=='\t')++pos;}bool NextItemExists(){White();return text[pos]!=0;}FixtureString ReadAsString(){White();++reads;char out[100];unsigned n=0;bool quote=text[pos]=='"';if(quote)++pos;while(text[pos]&&n<99){char c=text[pos];if(quote?c=='"':(c==' '||c=='\n'||c==';'))break;out[n++]=c;++pos;}if(quote&&text[pos]=='"')++pos;out[n]=0;return FixtureString(out);}float ReadAsFloat(){FixtureString s=ReadAsString();return(float)std::atof(s.text);}bool SkipPastString(const FixtureString&s){++skips;CHECK(s==";");while(text[pos]&&text[pos]!=';')++pos;if(text[pos]==';'){++pos;return true;}return false;}};
struct FixtureBase{int calls;FixtureBase():calls(0){}bool ConstructFromLevelScript(const char*){++calls;return base_ok;}};
struct FixtureSwitch:FixtureBase{float TriggerRadius,TimeToChangeEnvironmentDef;bool Triggered;FixtureString EnvironmentDef,TriggerableObjectScriptName;FixtureSwitch():TriggerRadius(99),TimeToChangeEnvironmentDef(88),Triggered(true),EnvironmentDef("oldenv"),TriggerableObjectScriptName("oldname"){}bool ConstructFromLevelScript(const char*);};
#define CThingSwitch FixtureSwitch
#define CThingGameObject FixtureBase
#define CStringParser FixtureParser
#define CCharString FixtureString
bool CThingSwitch::ConstructFromLevelScript(const char* script)
{
    if (CThingGameObject::ConstructFromLevelScript(script)) {
        Triggered = false;
        CStringParser parser(script);
        while (parser.NextItemExists()) {
            CCharString name = parser.ReadAsString();
            if (name == "TriggerRadius")
                TriggerRadius = parser.ReadAsFloat();
            else if (name == "TimeToChangeEnvironmentDef")
                TimeToChangeEnvironmentDef = parser.ReadAsFloat();
            else if (name == "TriggeredByThing") {
                CCharString scriptname = parser.ReadAsString();
                TriggerableObjectScriptName = scriptname == "NULL" ? CCharString("") : scriptname;
            }
            else if (name == "EnvironmentDef") {
                CCharString scriptname = parser.ReadAsString();
                EnvironmentDef = scriptname == "NULL" ? CCharString("") : scriptname;
            }
            else if (name == "EndThing")
                break;
            parser.SkipPastString(";");
        }
        return true;
    }
    return false;
}

#undef CThingSwitch
#undef CThingGameObject
#undef CStringParser
#undef CCharString
int main(){for(int mode=0;mode<8;++mode){FixtureSwitch s;base_ok=mode!=0;parser_constructions=parser_destructions=reads=skips=0;const char*script=mode==0?"TriggerRadius 2;":mode==1?"":mode==2?"TriggerRadius -2.5; TimeToChangeEnvironmentDef 4.25;":mode==3?"TriggeredByThing hero; EnvironmentDef env;":mode==4?"TriggeredByThing NULL; EnvironmentDef NULL;":mode==5?"EndThing; TriggerRadius 2;":mode==6?"Unknown ignore this; TriggerRadius 3;": "TriggerRadius 2; TriggerRadius 7; EndThing;";bool yes=s.ConstructFromLevelScript(script);CHECK(yes==base_ok&&s.calls==1);CHECK(parser_constructions==(base_ok?1:0)&&parser_destructions==parser_constructions);CHECK(s.Triggered==(base_ok?false:true));if(mode==0||mode==1||mode==5)CHECK(s.TriggerRadius==99&&s.TimeToChangeEnvironmentDef==88&&s.EnvironmentDef=="oldenv"&&s.TriggerableObjectScriptName=="oldname");if(mode==2)CHECK(s.TriggerRadius==-2.5f&&s.TimeToChangeEnvironmentDef==4.25f&&skips==2);if(mode==3)CHECK(s.TriggerableObjectScriptName=="hero"&&s.EnvironmentDef=="env"&&skips==2);if(mode==4)CHECK(s.TriggerableObjectScriptName==""&&s.EnvironmentDef==""&&skips==2);if(mode==5)CHECK(reads==1&&skips==0);if(mode==6)CHECK(s.TriggerRadius==3&&skips==2);if(mode==7)CHECK(s.TriggerRadius==7&&skips==2);}
 std::puts(failures?"0083c2d0_TEST FAIL":"0083c2d0_TEST PASS");return failures?1:0;}