// CThing::ConstructFromBinarySave(CMemoryDataInputStream& stream, const CThing::CSerialiseInfo& info) -- rebuild a thing from a binary save: identity (unless only the hero state is being loaded), the length-prefixed game-state block, the map number and the def / editor / map-who components, then every saved component (class name, creatable flag, length-prefixed block) up to the END marker, the script name / data and the two persistence flags
// Leaf: ?ConstructFromBinarySave@CThing@@UAEXAAVCMemoryDataInputStream@@ABVCSerialiseInfo@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CA390, 1430 bytes, thiscall, `ret 8`; CThing vtable 0x01239ABC slot 16 (+0x40).
// Identity (two sources): Ego_r.pdb 0x004CCA00 / FableWin.pdb 0x01CF1F50 ?ConstructFromBinarySave@CThing@@UAEXAAVCMemoryDataInputStream@@ABVCSerialiseInfo@1@@Z;
//   rtti-vtables.tsv puts 0x004CA390 at slot 16 of 0x01239ABC between CreateFromDef (15) and IsToBePlacedInMap (17), the PDB slot order
//   (engine/CThing.h FABLE_RETAIL_CTHING_LOADSINGLE); its progress literals "CThing: Construct From Binary Save 1" / "... 2" (0x01239C30 /
//   0x01239C08) name it, and CThingManager::LoadSingleThing calls slot +0x40 with the stream and the record. Catalog "CThing::vfunc_16".
// Callees: NProgressDisplay::DisplayProgress 0x009E9F40, CCharString (const char*, long) 0x0099EBF0 / copy 0x0099EC30 / () 0x0099E4B0 /
//   operator= 0x0099EFB0 / ~ 0x0099EAE0, CThingManager::ThingTypeToTypeFlags 0x0051E580, CWorld::GetFrame 0x0049D870, CThing::InitFromThingInfo
//   0x004C7F40, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 / SetPositionAssumeWithinSourceChunk 0x00993C10 (inline ReadSLONG and
//   ReadConstData), CMemoryDataInputStream(const void*, unsigned long) 0x00994930 / ~CDataStream 0x00993B10, CPersistContext
//   (CMemoryDataInputStream&, bool) 0x009BAE30, CPersistContext::CheckCRC 0x00404500, CPersistTraits<short / bool>::TransferBinaryIn 0x004CBC70 /
//   0x00403EB0, CDataOutputStream::WriteSLONG 0x00993EB0 / WriteEBOOL 0x00993E30, CPersistContext::Transfer<CCharString> 0x00431143,
//   CThing::AddTC 0x004C9D60, CBasicString<char>::operator==(const char*) 0x004115A0 / Compare 0x00411570, CTCBase::Serialise 0x00686AA0;
//   virtuals SerialiseGameState (slot 3), CreateFromDef (slot 15), CMainGameComponent::IsEditorActive (slot 9), CTCBase::GetClassTypeName
//   (slot 22), CDataInputStream::GetPosition (slot 2).
// Shape: the load-side mirror of the landed CThing::SaveBinary 0x004C84B0 (same TU, same inline one-argument transfers and traits bodies).
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

// The binary readers of this TU's traits (landed 0x004CBC70 / 0x00403EB0), called out of line; their bodies are visible as in the
// landed SaveBinary of this TU.
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
template<> void CPersistTraits<short>::TransferBinaryIn(CMemoryDataInputStream& stream, short& val) { val = (short)stream.ReadSLONG(); }
template<> void CPersistTraits<bool>::TransferBinaryIn(CMemoryDataInputStream& stream, bool& val) { val = stream.ReadEBOOL(); }
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif

// The one-argument binary transfers, inline in retail (CheckCRC(""), then the load / save arm of the mode).
template<> inline void CPersistContext::Transfer<short>(short& val) {
    CheckCRC("");
    CPersistTraits<short> traits;
    switch (Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream, val); break;
    case MODE_SAVE_BINARY: PSaveStream->WriteSLONG(val); break;
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

// The section name every thing starts with: a by-value copy of the global CCharString 0x013BAB0C ("NULL"; descriptive names, as the
// landed CThing::ConstructFromParams 0x004CA010 of this TU spells them).
extern CCharString g_DefaultSerialisationSectionName;   // 0x013BAB0C
inline CCharString GetDefaultSerialisationSectionName() { return g_DefaultSerialisationSectionName; }

// 0x004C7A70 (landed), the same TU: expanded here (the manager's Component +0x1C read in place).
const CMainGameComponent& CThing::PeekComponent() const
{
    return PCurrentThingManager->PeekComponent();
}

void CThing::ConstructFromBinarySave(CMemoryDataInputStream& stream, const CSerialiseInfo& info)
{
    NProgressDisplay::DisplayProgress("CThing: Construct From Binary Save 1", -1.0f, false, false);
    bool hero_state_only = false;
    if ((ThingTypeFlags & 2) && PCurrentThingManager->PeekWorld().IsSerialisingHeroState())
        hero_state_only = true;
    else {
        CreatedBySerialisation = true;
        RenderListPrev = 0;
        RenderListNext = 0;
        UpdateListPrev = 0;
        UpdateListNext = 0;
        SetSerialisationSectionName(GetDefaultSerialisationSectionName());
        ThingTypeFlags = PCurrentThingManager->ThingTypeToTypeFlags(Type);
        WorldFrameCreatedAt = CWorld::GetFrame();
        InitFromThingInfo(info);
    }

    long state_length = stream.ReadSLONG();
    CMemoryDataInputStream state_stream(stream.ReadConstData(state_length), state_length);
    CPersistContext state_context(state_stream, true);
    SerialiseGameState(state_context);

    CPersistContext context(stream, true);
    if (!hero_state_only) {
        context.Transfer(CurrentMapNumber);
        if (!PBaseDef.IsNull())
            CreateFromDef(*PBaseDef);
        if (PeekComponent().IsEditorActive())
            AddTC("CTCEditor", false, 0);
        AddTC("CTCMapwho", false, 0);
    }

    while (!stream.IsEOF()) {
        NProgressDisplay::DisplayProgress("CThing: Construct From Binary Save 2", -1.0f, false, false);
        CCharString class_name;
        context.Transfer(class_name);
        if (class_name == "END")
            break;
        bool creatable;
        context.Transfer(creatable);
        CTCBase* tc = 0;
        if (creatable)
            tc = AddTC(class_name, true, 0);
        else {
            for (unsigned long i = 0; i < Components.size(); ++i) {
                CTCBase* component = Components[i].second;
                if (component->GetClassTypeName() == class_name) {
                    tc = component;
                    break;
                }
            }
        }
        long length = stream.ReadSLONG();
        CMemoryDataInputStream tc_stream(stream.ReadConstData(length), length);
        CPersistContext tc_context(tc_stream, true);
        if (tc && length) {
            AddingTCsPermitted = false;
            tc->Serialise(tc_context, true);
            AddingTCsPermitted = true;
        }
    }

    context.Transfer(ScriptName);
    context.Transfer(ScriptData);
    bool game_state_persisted;
    context.Transfer(game_state_persisted);
    bool level_persistent;
    context.Transfer(level_persistent);
    GameStatePersisted = game_state_persisted;
    LevelPersistent = level_persistent;
}
