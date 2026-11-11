// CThing::SaveBinary(CDataOutputStream& os) -- write the thing to a binary save: its identity record (player, remapped UID, def name), its game-state block, the map number, every serialisable component (class name, creatable flag, length-prefixed block), the END marker, the script name / data and the two persistence flags
// Leaf: ?SaveBinary@CThing@@QAEXAAVCDataOutputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C84B0, 1070 bytes, thiscall, `ret 4`.
// Identity (two sources): Ego_r.pdb 0x004C9E60 / FableWin.pdb 0x01CF18B0 ?SaveBinary@CThing@@QAEXAAVCDataOutputStream@@@Z (the only CThing
//   member taking a CDataOutputStream&); the body builds CPersistContext(CDataOutputStream&, bool) 0x009BAE50 on its argument and its one
//   caller 0x005222B5 passes a CMemoryDataOutputStream. The catalog name "CThing::Serialise" is a propagated label.
// Callees: CPersistContext(CDataOutputStream&, bool) 0x009BAE50, CCharString() 0x0099E4B0 / operator=(const char*) 0x0099EFE0 / operator=
//   0x0099EFB0 / (const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CThingManager::RemapUIDForSerialisation 0x0051E210,
//   CDefinitionManager::GetDefNameFromGlobalIndex 0x009ACCC0, CDefStringTable::GetString 0x009D49B0 (GDefStringTable 0x013CA828),
//   CPersistContext::CheckCRC 0x00404500 and the inline one-argument transfers (CPersistTraits<signed char / unsigned __int64 / short /
//   bool>::TransferBinaryIn 0x004CBD50 / 0x0044B890 / 0x004CBC70 / 0x00403EB0; CDataOutputStream::WriteSLONG 0x00993EB0 / WriteUHUGE
//   0x00993EC0 / WriteEBOOL 0x00993E30), Transfer<CCharString> 0x00431143, CMemoryDataOutputStream ctor 0x00996AF0 / dtor 0x00996B30 /
//   GetLength 0x00994B40 / PeekData 0x00995E70, CDataOutputStream::WriteULONG 0x00993EA0 / Write(const CMemoryDataOutputStream&)
//   0x00995EB0, CTCBase::Serialise 0x00686AA0, CThing::IsLevelPersistent 0x004C82A0; virtual SerialiseGameState (slot 3), CTCBase slots
//   19 IsSerialiseCreatable / 20 IsSerialisableForHeroState / 22 GetClassTypeName, CDataOutputStream::Write (slot 4).
// Shape: IsGameStatePersisted 0x004C8280 is expanded in place (same TU: its landed body is repeated here as an inline); both saved flags
//   are that one value and IsLevelPersistent's result is dropped (0x004C8832..0x004C883A).
// Headers (overlay, FABLE_HDR_OVERLAY=$A/hdr): FABLE_RETAIL_CTHING_SAVEBINARY, FABLE_RETAIL_CTCBASE_SERIALISE,
//   FABLE_RETAIL_CDATAOUTPUTSTREAM_WRITESTREAM.
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
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_PEEKWORLD
#define FABLE_RETAIL_CTHINGMANAGER_LOADSINGLE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM_WRITESTREAM
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTCONTEXT
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

// The binary readers of this TU's traits (landed 0x004CBD50 / 0x004CBC70 / 0x00403EB0 / 0x0044B890), called out of line. Their bodies
// are visible so that VC7.1 knows they never touch the (data-less in use) traits object: retail lets every traits object share the
// frame word of a live local (the last two share the END marker string's, 0x004C8876 / 0x004C88B1).
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
template<> void CPersistTraits<signed char>::TransferBinaryIn(CMemoryDataInputStream& stream, signed char& val) { val = (signed char)stream.ReadSLONG(); }
template<> void CPersistTraits<short>::TransferBinaryIn(CMemoryDataInputStream& stream, short& val) { val = (short)stream.ReadSLONG(); }
template<> void CPersistTraits<bool>::TransferBinaryIn(CMemoryDataInputStream& stream, bool& val) { val = stream.ReadEBOOL(); }
void CPersistTraits<unsigned __int64>::TransferBinaryIn(CMemoryDataInputStream& stream, unsigned __int64& val) { val = stream.ReadType<unsigned __int64>(); }
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif

// The one-argument binary transfers, inline in retail (CheckCRC(""), then the load / save arm of the mode).
template<> inline void CPersistContext::Transfer<signed char>(signed char& val) {
    CheckCRC("");
    CPersistTraits<signed char> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream, val); break;
    case MODE_SAVE_BINARY: PSaveStream->WriteSLONG(val); break;
    }
}
template<> inline void CPersistContext::Transfer<short>(short& val) {
    CheckCRC("");
    CPersistTraits<short> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream, val); break;
    case MODE_SAVE_BINARY: PSaveStream->WriteSLONG(val); break;
    }
}
template<> inline void CPersistContext::Transfer<unsigned __int64>(unsigned __int64& val) {
    CheckCRC("");
    CPersistTraits<unsigned __int64> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream, val); break;
    case MODE_SAVE_BINARY: PSaveStream->WriteUHUGE(val); break;
    }
}
template<> inline void CPersistContext::Transfer<bool>(bool& val) {
    CheckCRC("");
    CPersistTraits<bool> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream, val); break;
    case MODE_SAVE_BINARY: PSaveStream->WriteEBOOL(val); break;
    }
}

// Ego_r.pdb / FableWin.pdb ?IsLevelLess@CThing@@QBE_NXZ, inline in retail (the PBaseDef null test, then LevelLess: bit 3 of def +0x41).
inline bool CThing::IsLevelLess() const
{
    bool level_less = false;
    if (!PBaseDef.IsNull() && PBaseDef->LevelLess)
        level_less = true;
    return level_less;
}
// 0x004C8280 (landed), the same TU: SaveBinary expands it.
inline bool CThing::IsGameStatePersisted() const
{
    if (HasTC(TCI_COOP_SPIRIT))
        return false;
    return GameStatePersisted;
}

void CThing::SaveBinary(CDataOutputStream& os)
{
    CPersistContext context(os, true);
    CSerialiseInfo info;
    unsigned __int64 uid = UniqueID;
    info.UID = PCurrentThingManager->RemapUIDForSerialisation(true, IsLevelLess(), CurrentMapNumber, uid);
    info.Player = PlayerNumber;
    info.DefinitionType = "";
    if (DefGlobalIndex > 0)
        info.DefinitionType = PCurrentThingManager->PeekDefinitionManager().GetDefNameFromGlobalIndex(DefGlobalIndex);
    context.Transfer(info.Player);
    context.Transfer(info.UID);
    context.Transfer(info.DefinitionType);

    CMemoryDataOutputStream state_stream;
    CPersistContext state_context(state_stream, true);
    SerialiseGameState(state_context);
    os.WriteULONG(state_stream.GetLength());
    os.Write(state_stream);

    bool hero_state_only = false;
    if ((ThingTypeFlags & 2) && PCurrentThingManager->PeekWorld().IsSerialisingHeroState())
        hero_state_only = true;
    else
        context.Transfer(CurrentMapNumber);

    AddingTCsPermitted = false;
    for (CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::iterator it = Components.begin();
         it != Components.end(); ++it) {
        CTCBase* tc = it->second;
        if (hero_state_only && !tc->IsSerialisableForHeroState())
            continue;
        if (tc->IsToBeRemoved())
            continue;
        CCharString class_name = tc->GetClassTypeName();
        bool creatable = tc->IsSerialiseCreatable();
        context.Transfer(class_name);
        context.Transfer(creatable);
        CMemoryDataOutputStream tc_stream;
        CPersistContext tc_context(tc_stream, true);
        tc->Serialise(tc_context, true);
        os.WriteULONG(tc_stream.GetLength());
        os.Write(tc_stream.PeekData(), tc_stream.GetLength());
    }
    CCharString end_marker("END");
    context.Transfer(end_marker);
    AddingTCsPermitted = true;

    context.Transfer(ScriptName);
    context.Transfer(ScriptData);
    // retail writes the game-state flag twice: IsLevelPersistent's result is dropped (0x004C883A) and the one local is transferred
    // for both the game-state and the level flag (0x004C885D reads the inline's result word, 0x004C8899 the local).
    bool persistent = IsGameStatePersisted();
    IsLevelPersistent();
    context.Transfer(persistent);
    context.Transfer(persistent);
}