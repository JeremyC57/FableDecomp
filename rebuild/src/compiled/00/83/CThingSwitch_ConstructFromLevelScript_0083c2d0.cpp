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