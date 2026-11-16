// CThing::InitFromThingInfo(const CThing::CSerialiseInfo& info) -- copies the unique id and player number from the serialise info, resolves the def name through the definition manager (a "NULL" name means no def; an unknown name kills the thing and fails) and binds the base def pointer when an index was found
// Leaf: ?InitFromThingInfo@CThing@@AAE_NABVCSerialiseInfo@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7F40, 167 bytes (+9 int3). `ret 4`, bool in al.
// Identity: Ego_r.pdb 0x004C9B80 + FableWin.pdb 0x01CEFCE0 ?InitFromThingInfo@CThing@@AAE_NABVCSerialiseInfo@1@@Z (private, nested
//   CSerialiseInfo); ConstructFromBinarySave 0x004CA449 passes its CSerialiseInfo argument (engine/CThing.h CONSTRUCTFROMBINARYSAVE opt-in).
// Callees: CBasicString<char>::operator==(const char*) 0x004115A0 on the name's data block (the null-data case is the inline
//   `repe cmpsb` of "NULL" against "" from the inline CCharString::operator==(const char*)), CDefinitionManager::GetDefGlobalIndexFromName
//   0x009AD410 and GetDef<CThingBaseDef>(long, CDefPointer<const CThingBaseDef>*) const 0x0042AF3C on
//   CThing::GetPCurrentThingManager()->PeekDefinitionManager() (+0x20, global 0x013B8A1C).
// Layout: CThing::CSerialiseInfo UID +0x00, DefinitionType +0x08, Player +0x0C; CThing UniqueID +0x18, PBaseDef +0x70,
//   DefGlobalIndex +0x8C, PlayerNumber +0x90, Dead +0x91 bit 0.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_SERIALISE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_SAVEBINARY
#define FABLE_RETAIL_CTHING_LOADSINGLE
#define FABLE_RETAIL_CTHING_CONSTRUCTFROMBINARYSAVE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_PEEKWORLD
#define FABLE_RETAIL_CTHINGMANAGER_LOADSINGLE
#define FABLE_RETAIL_CTHINGMANAGER_THINGTYPEFLAGS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_TEXT
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CMemoryDataOutputStream.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistTraits.h"
#include "engine/CDefString.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCBase.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CThingManager.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/NProgressDisplay.h"

bool CThing::InitFromThingInfo(const CSerialiseInfo& info)
{
    UniqueID = info.UID;
    PlayerNumber = info.Player;
    if (info.DefinitionType == "NULL") {
        DefGlobalIndex = 0;
    } else {
        DefGlobalIndex = (unsigned short)GetPCurrentThingManager()->PeekDefinitionManager().GetDefGlobalIndexFromName(info.DefinitionType);
        if (DefGlobalIndex == 0) {
            Dead = true;
            return false;
        }
    }
    if (DefGlobalIndex > 0)
        {
        const CDefinitionManager& definitions = GetPCurrentThingManager()->PeekDefinitionManager();
        definitions.GetDef(DefGlobalIndex, &PBaseDef);
    }
    return true;
}
