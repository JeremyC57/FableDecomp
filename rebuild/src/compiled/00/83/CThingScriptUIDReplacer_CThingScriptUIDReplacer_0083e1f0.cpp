// CThingScriptUIDReplacer::CThingScriptUIDReplacer
// Leaf: ??0CThingScriptUIDReplacer@@QAE@ABVCCharString@@@Z
// Flags: default
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_REPLACE_EGOD_PASS24
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CSTRINGPARSER_INLINE_CTOR
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_IMPLICIT_DTOR_PASS27
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER_ALLOCATEUID
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTHINGMANAGER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CThingManager.h"
#include "engine/fable_script_text.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CPersistContext_TransferableComponent.h"
#include "engine/CThingScriptUIDReplacer.h"
#include "fable_assert.h"

CCharString FABLE_FASTCALL GFUHUGEToCharString(unsigned __int64);   // 0x0099F930


inline CPlayerManager& CThingScriptUIDReplacer::GetPlayerManager()
{
    return CMainGameComponent::Get()->GetPlayerManager();
}

inline CWorldMap& CThingScriptUIDReplacer::GetWorldMap()
{
    return CMainGameComponent::Get()->GetWorld().GetWorldMap();
}

inline CThingManager& CThingScriptUIDReplacer::GetThingManager()
{
    return CMainGameComponent::Get()->GetWorld().GetThingManager();
}

inline CCharString CThingScriptUIDReplacer::GetSwapUniqueIdentifier(long unique_identifier) const
{
    CCharString id = "__UID_SWAP_NO_" + GFIntToCharString(unique_identifier) + "__";
    return id;
}

template <> inline CCharString __fastcall CPersistContext::GetDefaultVal<CCharString>() { CCharString value; return value; }
template <> inline unsigned __int64 __fastcall CPersistContext::GetDefaultVal<unsigned __int64>() { return 0; }

#pragma auto_inline(off)
template <> inline void CPersistContext::Transfer<unsigned __int64>(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue)
{
    CheckCRC(name);
    switch (Mode) {
    case MODE_CLEAR:
        value = defaultValue;
        break;
    case MODE_SAVE_BINARY:
        PSaveStream->WriteUHUGE(value);
        break;
    case MODE_LOAD_BINARY: {
        CPersistTraits<unsigned __int64> traits;
        traits.TransferBinaryIn(*PLoadStream, value);
        break;
    }
    case MODE_SAVE_TEXT: {
        CPersistContext_TransferableComponent<unsigned __int64> item(value);
        TransferObjectSaveText(name, item);
        break;
    }
    case MODE_LOAD_TEXT: {
        if (UseDefaults)
            value = defaultValue;
        CPersistContext_TransferableComponent<unsigned __int64> item(value);
        TransferObjectLoadText(name, item);
        break;
    }
    default:
        // Inferred: only the five persistence modes reach the switch. Retail 0x0044BB11
        // and Ego_r 0x004462C1 both dispatch directly from Mode without a range test.
#ifdef _MSC_VER
        __assume(0);
#elif defined(__GNUC__)
        __builtin_unreachable();
#endif
    }
}
#pragma auto_inline(on)

CThingScriptUIDReplacer::CThingScriptUIDReplacer(const CCharString& previous_script)
{
    ReplacedUIDScript = "";
    ReplacedUIDScript = previous_script;
    CCharString& script = ReplacedUIDScript;
    CThingPlayerCreature* phero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    FABLE_RETAIL_ASSERT(phero != 0);
    C3DVector pos(GetPlayerManager().GetMainPlayer().GetPControlledCreature()->GetPos());
    long level = GetWorldMap().GetMapNumberAt(pos);

    std::list<unsigned __int64> replaced_uids;
    CStringParser parser(script);
    while (parser.ReadAsStringUntilString("NewThing") != "") {
        parser.SkipPastString("NewThing");
        parser.SkipWhitespace();
        parser.ReadAsStringUntilString(";");
        parser.SkipPastString(";");
        CCharString sub_script = parser.ReadAsStringUntilString("EndThing");
        CStringParser sub_parser(sub_script);
        CPersistContext sub_context;
        sub_context.Init(sub_parser, 0, true, 0, 0, 0);
        CCharString def_type;
        unsigned __int64 old_uid;
        sub_context.Transfer("DefinitionType", def_type);
        sub_context.Transfer("UID", old_uid);
        if (old_uid != 0)
            replaced_uids.push_back(old_uid);
    }

    long unique_identifier = 0;
    for (std::list<unsigned __int64>::iterator it = replaced_uids.begin(); it != replaced_uids.end(); ++it) {
        unsigned __int64 old_uid = *it;
        CCharString old_uid_string = GFUHUGEToCharString(old_uid);
        CCharString delimiter_string = GetSwapUniqueIdentifier(unique_identifier);
        CCharString new_uid_string = delimiter_string;
        script.Replace(old_uid_string, new_uid_string);
        ++unique_identifier;
    }

    unique_identifier = 0;
    for (std::list<unsigned __int64>::iterator it = replaced_uids.begin(); it != replaced_uids.end(); ++it) {
        CCharString delimiter_string = GetSwapUniqueIdentifier(unique_identifier);
        CCharString replace_string = delimiter_string;
        unsigned __int64 new_uid = GetThingManager().AllocateNewThingUID(level, false, false);
        CCharString new_uid_string = GFUHUGEToCharString(new_uid);
        script.Replace(replace_string, new_uid_string);
        ++unique_identifier;
    }
}