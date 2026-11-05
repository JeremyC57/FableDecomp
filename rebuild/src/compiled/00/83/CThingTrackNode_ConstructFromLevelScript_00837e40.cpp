// CThingTrackNode::ConstructFromLevelScript(const char*) -- add the village-member TC, construct the base game object from the level script, add the standard physics TC and feed it the script text, then parse the node's own keys (LinkedToUID1/2, LinkedToFileID1/2, Start, End, ScriptName) up to EndThing
// Leaf: ?ConstructFromLevelScript@CThingTrackNode@@UAE_NPBD@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00837E40, 1421 bytes (0x00837E40..0x008383CC, `ret 4`); the 1691-byte ledger row runs on over the
//   int3 padding into CThingTrackNode::Create 0x008383D0 (265 bytes, `ret 4`, __fastcall static) -- a second function.
// Identity: CThingTrackNode vtable 0x01272CE4 slot 4 (CThingObject's slot 4 = ConstructFromLevelScript 0x00703110); Ego_r.pdb
//   ?ConstructFromLevelScript@CThingTrackNode@@UAE_NPBD@Z; the keys are the node's SerialiseGameState / Save vocabulary.
// Callees: CThing::AddTC(const CCharString&, bool, const CTCParamsBase*) 0x004C9D60, CThingGameObject::ConstructFromLevelScript(const char*)
//   0x00838930 (called by name), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inline Find inside the forceinline GetTC),
//   CMainGameComponent::IsEditorActive (vtable 0x0122F180 slot 9, on the static PMainGameComponent 0x013B86A0),
//   CTCBase::SerialiseLevelState(bool, CCharString&, bool) 0x00686E10, CStringParser(const char*, cb, const CCharString&, long) 0x00414E30,
//   CStringParser::NextItemExists 0x009BA4F0 / ReadAsString 0x009BA6F0 / ReadAsUHUGE 0x009BA5B0 / ReadAsInteger 0x009BA540 /
//   SkipPastString 0x009B9BA0, CThing::IsLevelLess 0x004C7930, CThingManager::RemapUIDForSerialisation 0x0051E210 on
//   CThing::PCurrentThingManager 0x013B8A1C, CBasicString<char>::operator==(const char*) 0x004115A0 (the inline CCharString compare's
//   non-null arm; the null arm is the strcmp intrinsic against ""), _stricmp 0x00BFEBA8 (the inline EqualsCaseInsensitive),
//   CCharString(const char*, long) 0x0099EBF0 / operator= 0x0099EFB0 / ~CCharString 0x0099EAE0, ~list<CStringParserCommentBoundaries>
//   0x004046B0 and ~CBaseClass 0x0099A2E0 (the compiler-generated ~CStringParser, expanded).
// Literals: "CTCVillageMember" 0x0123A5D8, "CTCPhysicsStandard" 0x01239F38, "LinkedToUID1" 0x01272ECC, "LinkedToUID2" 0x01272EBC,
//   "LinkedToFileID1" 0x01272EAC, "LinkedToFileID2" 0x01272E9C, "Start" 0x0125BE80, "End" 0x0125BE7C, "ScriptName" 0x01239C8C,
//   "EndThing" 0x01239B8C, "TRUE" 0x0122D6D4, "FALSE" 0x0122D6CC, ";" 0x0123542C, "" 0x0122D70E, CCharString::NullChar 0x0129AAF4.
// Shape: the script text is a named CCharString (SerialiseLevelState takes it by non-const reference); the key loop is rotated
//   (NextItemExists tested before and after the body); EndThing breaks out; every other key, matched or not, is followed by
//   SkipPastString(";").
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQUALSCI
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_SERIALISELEVELSTATE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_LEVELSCRIPT
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGTRACKNODE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_READASUHUGE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingTrackNode.h"
#include "engine/CThingManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CStringParser.h"

// Row-local template bodies (the landed 0x009BB110 precedent): fable_string.h declares CBasicString<T>::Compare 0x00411570 and
// operator==(const T*) 0x004115A0; retail's TU holds their bodies (operator== is called out of line here, the key's PStringData
// kept in esi across the compare chain).
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

bool CThingTrackNode::ConstructFromLevelScript(const char* script)
{
    AddTC("CTCVillageMember", false, 0);
    if (CThingGameObject::ConstructFromLevelScript(script)) {
        AddTC("CTCPhysicsStandard", false, 0);

        CCharString scriptText(script);
        GetTC(TCI_PHYSICS).SerialiseLevelState(false, scriptText, !CMainGameComponent::Get()->IsEditorActive());

        NextNodeUID = 0;
        PreviousNodeUID = 0;
        PreviousNodeThingFileID = 0;
        NextNodeThingFileID = 0;

        CStringParser parser(script);
        while (parser.NextItemExists()) {
            CCharString key = parser.ReadAsString();
            if (key == "LinkedToUID1") {
                PreviousNodeUID = GetPCurrentThingManager()->RemapUIDForSerialisation(false, IsLevelLess(), GetCurrentMapNumber(), parser.ReadAsUHUGE());
            } else if (key == "LinkedToUID2") {
                NextNodeUID = GetPCurrentThingManager()->RemapUIDForSerialisation(false, IsLevelLess(), GetCurrentMapNumber(), parser.ReadAsUHUGE());
            } else if (key == "LinkedToFileID1") {
                PreviousNodeThingFileID = parser.ReadAsInteger();
            } else if (key == "LinkedToFileID2") {
                NextNodeThingFileID = parser.ReadAsInteger();
            } else if (key == "Start") {
                CCharString value = parser.ReadAsString();
                if (value.EqualsCaseInsensitive("TRUE"))
                    IsStart = true;
                else if (value.EqualsCaseInsensitive("FALSE"))
                    IsStart = false;
            } else if (key == "End") {
                CCharString value = parser.ReadAsString();
                if (value.EqualsCaseInsensitive("TRUE"))
                    IsEnd = true;
                else if (value.EqualsCaseInsensitive("FALSE"))
                    IsEnd = false;
            } else if (key == "ScriptName") {
                TrackName = parser.ReadAsString();
            } else if (key == "EndThing") {
                break;
            }
            parser.SkipPastString(";");
        }
        return true;
    }
    return false;
}