// NThingManager::CEntityRuntimePersistence::SaveToGameStateBinary(CPersistContext&) -- writes the game-persistent entity
// scripts (each as a sub-stream: map UID, compressed save data, uncompressed size, script text) and the UID / section-name
// CRC of every game-persistent dead entity.
// Flags: /Gr
// Leaf: ?SaveToGameStateBinary@CEntityRuntimePersistence@NThingManager@@QAEXAAVCPersistContext@@@Z
// Retail: Fable.exe @ 0x00521620, 1129 bytes; called from 0x0052444F on CThingManager's EntityRuntimePersistence (+0xA0).
// Ego_r.pdb PUBLIC 0x0052C6C0 / FableWin.pdb 0x01D42CC0 give the exact spelling (the manifest label
// CEditWorldMap_LoadRivers is wrong). Local names per the Codex pass-11 Ego_r locals extraction (not re-read here): no_scripts, script_os, script_context, map_uid, data_size,
// script_size, dead_entities, world_map, no_dead_things.
// Callees: PeekWorldMap (CWorld vtable slot 13, +0x34) through CMainGameComponent::PMainGameComponent 0x013B86A0,
// CheckCRC 0x00404500, WriteSLONG 0x00993EB0, WriteUHUGE 0x00993EC0, CPersistTraits<long>/<unsigned long>/
// <unsigned __int64>::TransferBinaryIn 0x0040FE60/0x0040F8A0/0x0044B890, CMemoryDataOutputStream() 0x00996AF0,
// CPersistContext(CDataOutputStream&,bool) 0x009BAE50, CWorldMap::GetMapUID 0x004FCE00, CDataOutputStream::Write
// (vtable +0x10), CPersistContext::Transfer<CCharString> 0x00431143, CMemoryDataOutputStream::GetLength 0x00994B40 /
// PeekData 0x00995E70 / ~CMemoryDataOutputStream 0x00996B30, malloc/free 0x00BFEA0E/0x00BFEA14,
// std::__uninitialized_copy<CPersistedDeadEntity*,CPersistedDeadEntity*> 0x005269B0 (fastcall: STLport free template in
// a /Gr TU).
// Shape: /Gr (the STLport helper 0x005269B0 is YI). The long / unsigned long / unsigned __int64 Transfer instances are
// inline here; the three traits readers and the vector growth copy are same-TU bodies above the caller under
// `#pragma auto_inline(off)` (GOTCHAS: callee clobber knowledge) -- declared-only readers leave a 1156-byte variant.
// Headers: rebuild/include + the Codex pass 11 consolidation 2 overlay (cons11b hdr/, retry11a merged): new CPersistTraits /
// NThingManager_CEntityRuntimePersistence models; opt-in branches FABLE_RETAIL_CDATAOUTPUTSTREAM, CMEMORYDATAOUTPUTSTREAM,
// CWORLDMAP, SCOPED_PTR; CPERSISTCONTEXT (save-side ctor, GetPSaveStream) and CDATAINPUTSTREAM extended; CGameComponent /
// CMainGameComponent from fable_game.h (FABLE_RETAIL_CGAMECOMPONENT / FABLE_RETAIL_CMAINGAMECOMPONENT) (hdr/README.md).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#include "stlport/fable_containers.h"
#include "engine/NThingManager_CEntityRuntimePersistence.h"
#include "engine/CMemoryDataOutputStream.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistTraits.h"
#include "fable_game.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"

#pragma auto_inline(off)
// 0x005269B0: STLport 4.6 __uninitialized_copy (non-POD path) for the dead-entity vector growth.
namespace std {
template<> NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity*
__uninitialized_copy(NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity* first,NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity* last,
                     NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity* result,const __false_type&)
{
    NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity* cur=result;
    for(;first!=last;++first,++cur) _Construct(&*cur,*first);
    return cur;
}
}
// 0x0040FE60 / 0x0040F8A0 / 0x0044B890: the scalar traits readers.
template<> void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStream& stream,long& val) { val=stream.ReadSLONG(); }
template<> void CPersistTraits<unsigned long>::TransferBinaryIn(CMemoryDataInputStream& stream,unsigned long& val) { val=stream.ReadULONG(); }
void CPersistTraits<unsigned __int64>::TransferBinaryIn(CMemoryDataInputStream& stream,unsigned __int64& val) { val=stream.ReadType<unsigned __int64>(); }
#pragma auto_inline(on)
template<> inline void CPersistContext::Transfer<long>(long& val) {
    CheckCRC("");
    CPersistTraits<long> traits;
    switch(Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream,val);break;
    case MODE_SAVE_BINARY: PSaveStream->WriteSLONG(val);break;
    }
}
template<> inline void CPersistContext::Transfer<unsigned long>(unsigned long& val) {
    CheckCRC("");
    CPersistTraits<unsigned long> traits;
    switch(Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream,val);break;
    case MODE_SAVE_BINARY: PSaveStream->WriteSLONG(val);break;
    }
}
template<> inline void CPersistContext::Transfer<unsigned __int64>(unsigned __int64& val) {
    CheckCRC("");
    CPersistTraits<unsigned __int64> traits;
    switch(Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream,val);break;
    case MODE_SAVE_BINARY: PSaveStream->WriteUHUGE(val);break;
    }
}
void NThingManager::CEntityRuntimePersistence::SaveToGameStateBinary(CPersistContext& context)
{
    const CWorldMap& world_map=CMainGameComponent::Get()->PeekWorld().PeekWorldMap();
    long no_scripts=GamePersistentEntityScripts.size();
    context.Transfer(no_scripts);
    for(long c1=0;c1<no_scripts;++c1) {
        const CCharString* script=GetGamePersistentScript(c1);
        CMemoryDataOutputStream script_os;
        CPersistContext script_context(script_os,true);
        if(script) {
            unsigned long map_uid=world_map.GetMapUID(c1);
            script_context.Transfer(map_uid);
            CEntityScriptInfo& info=GamePersistentEntityScripts[c1];
            unsigned long data_size=info.SaveData.size();
            script_context.Transfer(data_size);
            script_context.Transfer(info.UncompressedSize);
            script_context.GetPSaveStream()->Write(info.SaveData.GetAsCArray(),data_size);
            script_context.Transfer(info.Script);
        }
        long script_size=script_os.GetLength();
        context.Transfer(script_size);
        context.GetPSaveStream()->Write(script_os.PeekData(),script_size);
    }
    CArray<CPersistedDeadEntity> dead_entities;
    for(std::map<unsigned __int64,CPersistedDeadEntity>::iterator it=PersistedDeadEntities.begin();it!=PersistedDeadEntities.end();++it)
        if(it->second.GamePersistent) dead_entities.push_back(it->second);
    unsigned long no_dead_things=dead_entities.size();
    context.Transfer(no_dead_things);
    for(unsigned long c2=0;c2<no_dead_things;++c2) {
        context.Transfer(dead_entities[c2].UID);
        context.Transfer(dead_entities[c2].SectionNameCRC);
    }
}