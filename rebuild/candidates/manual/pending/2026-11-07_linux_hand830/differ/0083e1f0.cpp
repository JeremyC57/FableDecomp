// CThingScriptUIDReplacer::CThingScriptUIDReplacer(const CCharString&) -- copy a level-script fragment, swap every NewThing UID for a placeholder token, then each token for a freshly allocated UID on the hero's map
// Grade: DIFFER(1984v1973) -- register colouring in the second replace loop (retail: index in ebx, iterator in [esp+24h]; ours the reverse, index at [esp+14h]) plus a 15-byte loop-head alignment pad; the call sequence and every callee match
// Next idea: a loop form whose index outlives the iterator (retail destroys the result before index++), or the landed CStringParser sibling's exact Find/replace spelling; ten loop-shape variants (v6-v10) left the colouring unchanged
// Leaf: ??0CThingScriptUIDReplacer@@QAE@ABVCCharString@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
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
#include <string.h>
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

CCharString FABLE_FASTCALL GFUHUGEToCharString(unsigned __int64);   // 0x0099F930

class CThingScriptUIDReplacer {
public:
    CThingScriptUIDReplacer(const CCharString& script);
private:
    CPlayerManager& GetPlayerManager() { return CMainGameComponent::Get()->GetPlayerManager(); }
    CThingManager& GetThingManager() { return CMainGameComponent::Get()->GetWorld().GetThingManager(); }
    CWorldMap& GetWorldMap() { return CMainGameComponent::Get()->GetWorld().GetWorldMap(); }
    CCharString GetSwapUniqueIdentifier(long index) const
    {
        CCharString id = "__UID_SWAP_NO_" + GFIntToCharString(index) + "__";
        return id;
    }
    CCharString ReplacedUIDScript;
};

inline CStringParser::CStringParser(const CCharString& text, void (__fastcall* errorCallback)(const CCharString&),
                                    const CCharString& filename, long baseLineOffset)
    : CBaseClass(), Filename(), TempCharString(), Comments()
{
    TempCharString = text;
    Init(text.GetAsCharArray(), errorCallback, filename, baseLineOffset);
}

template <> inline CCharString __fastcall CPersistContext::GetDefaultVal<CCharString>() { CCharString value; return value; }
template <> inline unsigned __int64 __fastcall CPersistContext::GetDefaultVal<unsigned __int64>() { return 0; }

#pragma auto_inline(off)
template <> void CPersistContext::Transfer<unsigned __int64>(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue)
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
        __assume(0);
    }
}
#pragma auto_inline(on)

CThingScriptUIDReplacer::CThingScriptUIDReplacer(const CCharString& script)
{
    ReplacedUIDScript = "";
    ReplacedUIDScript = script;

    GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    C3DVector pos = GetPlayerManager().GetMainPlayer().GetPControlledCreature()->GetPos();
    long mapNumber = GetWorldMap().GetMapNumberAt(pos);

    std::list<unsigned __int64> uids;
    CStringParser parser(ReplacedUIDScript);
    while (parser.ReadAsStringUntilString("NewThing") != "") {
        parser.SkipPastString("NewThing");
        parser.SkipWhitespace();
        parser.ReadAsStringUntilString(";");
        parser.SkipPastString(";");
        CCharString thing = parser.ReadAsStringUntilString("EndThing");
        CStringParser thingParser(thing);
        CPersistContext context;
        context.Init(thingParser, 0, true, 0, 0, 0);
        CCharString defType;
        context.Transfer("DefinitionType", defType);
        unsigned __int64 uid;
        context.Transfer("UID", uid);
        if (uid != 0)
            uids.push_back(uid);
    }

    {
        long index = 0;
        for (std::list<unsigned __int64>::iterator it = uids.begin(); it != uids.end(); ++it) {
            CCharString find = GFUHUGEToCharString(*it);
            CCharString swap = GetSwapUniqueIdentifier(index);
            CCharString replace = swap;
            {
                CCharString result;
                char* src = const_cast<char*>(ReplacedUIDScript.GetAsCharArray());
                const char* findText = find.GetAsCharArray();
                char* p = strstr(src, findText);
                while (p) {
                    *p = 0;
                    p += strlen(findText);
                    result += CCharString(src);
                    result += replace;
                    src = p;
                    p = strstr(p, findText);
                }
                result += CCharString(src);
                ReplacedUIDScript = result;
            }
            index++;
        }
    }
    {
        long index = 0;
        for (std::list<unsigned __int64>::iterator it = uids.begin(); it != uids.end(); ++it) {
            CCharString swap = GetSwapUniqueIdentifier(index);
            CCharString find = swap;
            CCharString replace = GFUHUGEToCharString(GetThingManager().AllocateNewThingUID(mapNumber, false, false));
            {
                CCharString result;
                char* src = const_cast<char*>(ReplacedUIDScript.GetAsCharArray());
                const char* findText = find.GetAsCharArray();
                char* p = strstr(src, findText);
                while (p) {
                    *p = 0;
                    p += strlen(findText);
                    result += CCharString(src);
                    result += replace;
                    src = p;
                    p = strstr(p, findText);
                }
                result += CCharString(src);
                ReplacedUIDScript = result;
            }
            index++;
        }
    }
}
