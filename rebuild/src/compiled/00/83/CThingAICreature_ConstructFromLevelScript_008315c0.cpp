// CThingAICreature::ConstructFromLevelScript(const char* script) -- base creature construct step, then health from the def, Drawable, a fresh RelationshipsInit, home / work building cleared, and the AI creature's own level-script keys parsed up to EndThing (InitialPos defaults to the current position)
// Leaf: ?ConstructFromLevelScript@CThingAICreature@@UAE_NPBD@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008315C0, 2488 bytes, public virtual, `ret 4` (last instruction at 0x00831F75).
// Identity: CThingAICreature vtable 0x0127293C slot 4 (catalog placeholder vfunc_4); slot 2 is Save 0x008304A0 and CThing's slot 4 is
//   ConstructFromLevelScript (the CThingPlayerCreature / CThingTrackNode overrides 0x006A9960 / 0x00837E40 sit in the same slot); Ego_r.pdb /
//   FableWin.pdb ?ConstructFromLevelScript@CThingAICreature@@UAE_NPBD@Z; the first call is ?ConstructFromLevelScript@CThingCreatureBase@@UAE_NPBD@Z
//   (0x00662930, by name). The keys are exactly the Save 0x008304A0 vocabulary read back.
// Callees: CCountedPointer<CThingAICreature::CRelationshipsInit>::Reset 0x00834D30 (out of line in retail: declared, no body),
//   ::operator new 0x00BFEA1A, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10,
//   CStringParser(const char*, cb, const CCharString&, long) 0x00414E30 / NextItemExists 0x009BA4F0 / ReadAsString 0x009BA6F0 /
//   ReadAsUHUGE 0x009BA5B0 / ReadAsFloat 0x009BA650 / SkipPastString 0x009B9BA0, CPersistContext(CStringParser&, ...) 0x009BADD0,
//   CPersistTraits<bool>::TransferIn 0x00403FF0, CBasicString<char>::operator==(const char*) 0x004115A0 (the inline CCharString compare's
//   non-null arm), CCharString 0x0099EBF0 / operator= 0x0099EFB0 / operator=(const char*) 0x0099EFE0 / ~ 0x0099EAE0, CThing::GetPos
//   0x004C73D0, ~list<CStringParserCommentBoundaries> 0x004046B0 and ~CBaseClass 0x0099A2E0 (the generated ~CStringParser, expanded).
// Layout: engine/CThingAICreature.h retail branch + FABLE_RETAIL_CTHINGAICREATURE_LEVELSCRIPT (hdr/): RelationshipsInit +0x160 (Ego_r
//   CCountedPointer<CRelationshipsInit>; CRelationshipsInit = five unsigned __int64 UIDs, 0x28 bytes, inline zeroing ctor), HomeBuilding
//   +0x168, WorkBuilding +0x170, OverridingBrainName +0x190, flag bytes +0x1C8 / +0x1C9; CThingCreatureBase PDef +0xE0 (CThingBaseDef::Health
//   +0x50), InitialPos +0xE8; CThingPhysical MaxHealth +0xB0 / Health +0xB4; CThing Drawable = bit 1 of +0x92.
// Shape: the CThingTrackNode::ConstructFromLevelScript 0x00837E40 parser (VC7.1 hoists the key's null test out of the compare chain: one
//   `repe cmpsb` ladder against "" and one Compare ladder); `MaxHealth = Health = ...` stores Health first. Each flag key reads a bool through
//   its own block-scope CPersistContext (ten frame slots) and a block-scope named CPersistTraits<bool> into one function-scope bool, then
//   writes the bitfield. The CPersistTraits<bool>::TransferIn body is in the TU (header-inline in retail, an early COMDAT called out of line;
//   the landed 0x006C9490 precedent): VC7.1 then sees the traits `this` unused and puts every traits object in the shared temporary slot
//   [esp+14h], as retail does (with only a declaration each named traits takes its own slot, frame 0x21C; as a value-initialised temporary
//   it is zero-filled).
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CSTRINGPARSER_READASUHUGE
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"

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


// The bool reader's body is header-inline in retail (the early COMDAT 0x00403FF0, called out of line): visible here, VC7.1 sees the
// traits `this` unused (the landed 0x006C9490 precedent), so the traits temporary takes no slot of its own.
template<> inline void CPersistTraits<bool>::TransferIn(CPersistContext& context, CStringParser& parser, bool& value)
{
    CCharString token = parser.ReadAsString();
    if (token.EqualsCaseInsensitive("TRUE") || token.EqualsCaseInsensitive("BTRUE"))
        value = true;
    else if (token.EqualsCaseInsensitive("FALSE") || token.EqualsCaseInsensitive("BFALSE"))
        value = false;
    else
        context.PersistError(parser, "Unknown boolean value entry " + token);
}

template<> void CCountedPointer<CThingAICreature::CRelationshipsInit>::Reset(CThingAICreature::CRelationshipsInit* data);   // 0x00834D30

bool CThingAICreature::ConstructFromLevelScript(const char* script)
{
    if (CThingCreatureBase::ConstructFromLevelScript(script)) {
        MaxHealth = Health = PDef->Health;
        Drawable = true;
        RelationshipsInit.Reset(new CRelationshipsInit);
        HomeBuilding = CIntelligentPointer<CThingBuilding>(0);
        WorkBuilding = CIntelligentPointer<CThingBuilding>(0);

        bool initialPosSet = false;
        bool value;
        CStringParser parser(script);
        while (parser.NextItemExists()) {
            CCharString key = parser.ReadAsString();
            if (key == "HomeBuildingUID") {
                RelationshipsInit->HomeBuildingUID = parser.ReadAsUHUGE();
            } else if (key == "WorkBuildingUID") {
                RelationshipsInit->WorkBuildingUID = parser.ReadAsUHUGE();
            } else if (key == "FatherCreatureUID") {
                RelationshipsInit->FatherCreatureUID = parser.ReadAsUHUGE();
            } else if (key == "MotherCreatureUID") {
                RelationshipsInit->MotherCreatureUID = parser.ReadAsUHUGE();
            } else if (key == "SpouseCreatureUID") {
                RelationshipsInit->SpouseCreatureUID = parser.ReadAsUHUGE();
            } else if (key == "OverridingBrainName") {
                CCharString name = parser.ReadAsString();
                if (name == "NULL")
                    OverridingBrainName = "";
                else
                    OverridingBrainName = name;
            } else if (key == "EnableCreatureAutoPlacing") {
                CPersistContext context(parser, 0, true, 0, 0, 0);
                value = false;
                CPersistTraits<bool> traits;
                traits.TransferIn(context, parser, value);
                EnableCreatureAutoPlacing = value;
            } else if (key == "HasInformation") {
                CPersistContext context(parser, 0, true, 0, 0, 0);
                value = false;
                CPersistTraits<bool> traits;
                traits.TransferIn(context, parser, value);
                HasInformation = value;
            } else if (key == "WanderWithInformation") {
                CPersistContext context(parser, 0, true, 0, 0, 0);
                value = false;
                CPersistTraits<bool> traits;
                traits.TransferIn(context, parser, value);
                WanderWithInformation = value;
            } else if (key == "WaveWithInformation") {
                CPersistContext context(parser, 0, true, 0, 0, 0);
                value = false;
                CPersistTraits<bool> traits;
                traits.TransferIn(context, parser, value);
                WaveWithInformation = value;
            } else if (key == "ContinueAIWithInformation") {
                CPersistContext context(parser, 0, true, 0, 0, 0);
                value = false;
                CPersistTraits<bool> traits;
                traits.TransferIn(context, parser, value);
                ContinueAIWithInformation = value;
            } else if (key == "AllowedToFollowHero") {
                CPersistContext context(parser, 0, true, 0, 0, 0);
                value = false;
                CPersistTraits<bool> traits;
                traits.TransferIn(context, parser, value);
                AllowedToFollowHero = value;
            } else if (key == "RegionFollowingOverriddenFromScript") {
                CPersistContext context(parser, 0, true, 0, 0, 0);
                value = false;
                CPersistTraits<bool> traits;
                traits.TransferIn(context, parser, value);
                RegionFollowingOverriddenFromScript = value;
            } else if (key == "RespondingToFollowAndWait") {
                CPersistContext context(parser, 0, true, 0, 0, 0);
                value = true;
                CPersistTraits<bool> traits;
                traits.TransferIn(context, parser, value);
                RespondingToFollowAndWait = value;
            } else if (key == "CanBeCourted") {
                CPersistContext context(parser, 0, true, 0, 0, 0);
                value = false;
                CPersistTraits<bool> traits;
                traits.TransferIn(context, parser, value);
                CanBeCourted = value;
            } else if (key == "CanBeMarried") {
                CPersistContext context(parser, 0, true, 0, 0, 0);
                value = false;
                CPersistTraits<bool> traits;
                traits.TransferIn(context, parser, value);
                CanBeMarried = value;
            } else if (key == "InitialPosX") {
                InitialPos.X = parser.ReadAsFloat();
                initialPosSet = true;
            } else if (key == "InitialPosY") {
                InitialPos.Y = parser.ReadAsFloat();
                initialPosSet = true;
            } else if (key == "InitialPosZ") {
                InitialPos.Z = parser.ReadAsFloat();
                initialPosSet = true;
            } else if (key == "EndThing") {
                break;
            }
            parser.SkipPastString(";");
        }
        if (!initialPosSet)
            InitialPos = GetPos();
        return true;
    }
    return false;
}