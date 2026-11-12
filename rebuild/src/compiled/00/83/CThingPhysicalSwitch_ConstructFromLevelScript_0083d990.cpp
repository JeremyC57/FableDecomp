// CThingPhysicalSwitch::ConstructFromLevelScript(const char*) -- construct the base game object from the level script, clear Triggered, then parse the switch's own key (TriggeredByThing; the value "NULL" means no triggerable object) up to EndThing
// Leaf: ?ConstructFromLevelScript@CThingPhysicalSwitch@@UAE_NPBD@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083D990, 460 bytes (0x0083D990..0x0083DB5C, `ret 4` at both exits).
// Identity: CThingPhysicalSwitch vtable 0x01245644 slot 4 (= ConstructFromLevelScript in every CThing vtable); FableWin.pdb
//   ?ConstructFromLevelScript@CThingPhysicalSwitch@@UAE_NPBD@Z (public virtual, bool, const char*).
// Callees: CThingGameObject::ConstructFromLevelScript(const char*) 0x00838930 (called by name),
//   CStringParser(const char*, cb, const CCharString&, long) 0x00414E30 (header default arguments: 0, CCharString(""), 0),
//   CStringParser::NextItemExists 0x009BA4F0 / ReadAsString 0x009BA6F0 / SkipPastString 0x009B9BA0,
//   CBasicString<char>::operator==(const char*) 0x004115A0 (the inline CCharString compare's non-null arm; the null arm is the
//   strcmp intrinsic against ""), CCharString(const char*, long) 0x0099EBF0 / operator= 0x0099EFB0 / ~CCharString 0x0099EAE0,
//   ~list<CStringParserCommentBoundaries> 0x004046B0 and ~CBaseClass 0x0099A2E0 (the compiler-generated ~CStringParser, expanded).
// Literals: "" 0x0122D70E, "TriggeredByThing" 0x012730B0, "EndThing" 0x01239B8C, "NULL" 0x012393E4, ";" 0x0123542C.
// Members: Triggered +0xD0, TriggerableObjectScriptName +0xD4 (engine/CThingPhysicalSwitch.h).
// Shape: no frame pointer, this in ebp; the key loop is rotated (NextItemExists tested before and after the body); EndThing breaks
//   out into the shared parser teardown; every other key, matched or not, is followed by SkipPastString(";"). The value string
//   lives in the dead `script` argument slot; the conditional "" temporary of the ternary is tracked by a flag in ebx bit 0.
//   The key's PStringData stays in esi across both compares because the TU sees the bodies of CBasicString<T>::Compare /
//   operator==(const T*) (row-local template bodies, the landed 0x00837E40 / 0x009BB110 precedent) yet calls 0x004115A0 out of line.
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGPHYSICALSWITCH_LEVELSCRIPT
#define FABLE_RETAIL_CSTRINGPARSER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingPhysicalSwitch.h"
#include "engine/CStringParser.h"

template<class T> inline long __fastcall CBasicString<T>::Compare(const T* a,const T* b)
{
    while(*a || *b) {
        if(*a<*b)return -1;
        if(*a>*b)return 1;
        ++a; ++b;
    }
    return 0;
}
template<class T> inline bool CBasicString<T>::operator==(const T* text) const
{ return Compare(PData,text)==0; }

bool CThingPhysicalSwitch::ConstructFromLevelScript(const char* script)
{
    if (CThingGameObject::ConstructFromLevelScript(script)) {
        Triggered = false;
        CStringParser parser(script);
        while (parser.NextItemExists()) {
            CCharString key = parser.ReadAsString();
            if (key == "TriggeredByThing") {
                CCharString value = parser.ReadAsString();
                TriggerableObjectScriptName = (value == "NULL") ? CCharString("") : value;
            } else if (key == "EndThing") {
                break;
            }
            parser.SkipPastString(";");
        }
        return true;
    }
    return false;
}