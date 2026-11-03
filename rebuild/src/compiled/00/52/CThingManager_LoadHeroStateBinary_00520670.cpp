// CThingManager::LoadHeroStateBinary(CPersistContext&) -- loads the saved hero entity sections: for each saved script a
// zlib-compressed block (level id, section name, section, entity) is decompressed and handed to LoadSingleThing, then the
// trailing script text is transferred.
// Leaf: ?LoadHeroStateBinary@CThingManager@@QAEXAAVCPersistContext@@@Z
// Retail: Fable.exe @ 0x00520670, 1524 bytes (ret 4 at 0x00520C61, int3 padding to 0x00520C64). Ego_r.pdb has it at 0x0052A690 in
// thing_manager.obj between LoadHeroStateScript and OnThingKill; local names (world_map, no_scripts, c1, script_size,
// script_is, script_context, num_sections, data_size, uncompressed_size, uncompressed_data, compressed_data,
// compressor_zlib, output_size, is, level_id, name, section_size, section_is, entity_size, entity_is, entity_context,
// script) follow the Codex pass-11 Ego_r locals extraction (not re-read in this lane); its debug-only start_pos is absent.
// Callees: PeekWorldMap (CWorld vtable slot 13, +0x34), CheckCRC 0x00404500, WriteSLONG 0x00993EB0,
// CPersistTraits<long>/<unsigned long>::TransferBinaryIn 0x0040FE60/0x0040F8A0, CCharString(const char*,long) 0x0099EBF0,
// NProgressDisplay::DisplayProgress 0x009E9F40, ~CCharString 0x0099EAE0, CDataInputStream() 0x00993B20,
// SetPositionAssumeWithinSourceChunk 0x00993C10, OpenMemorySource 0x00994570, CPersistContext(CMemoryDataInputStream&,bool)
// 0x009BAE30, malloc/free 0x00BFEA0E/0x00BFEA14, CCompressorZlib::GetRequiredWorkspaceSize 0x00A3A7D0, CACompressor(long)
// 0x00A3AEA0, CCompressorZlib::Initialise 0x00A3AA10, InternalDecompress 0x00A3AB00, CMemoryDataInputStream(const void*,
// unsigned long) 0x00994930, ReadWithSrcChunkOverflow 0x00993CA0, ReadNullTerminatedString 0x00995D30, LoadSingleThing
// 0x0051FD80, CCharString() 0x0099E4B0, CPersistContext::Transfer<CCharString> 0x00431143, ~CDataStream 0x00993B10 (both
// derived stream destructors are implicit), ~CCompressorZlib 0x00A3ACC0.
// Shape: the long / unsigned long Transfer instances are inline here (out of line elsewhere, e.g. 0x00431102 /
// 0x00431020); the two traits readers are COMDAT bodies the TU sees above the caller (GOTCHAS: same-TU callee bodies under
// `#pragma auto_inline(off)`) -- declared-only they leave a 1604-byte register-allocation variant, an ordinary visible
// template body gets inlined (1450 bytes); a single generic Transfer(T&) template with an explicit Transfer<CCharString>
// declaration crashes VC7.1 RTM (C1001).
// Headers: rebuild/include + the Codex pass 11 consolidation 2 overlay (cons11b hdr/, retry11a merged): new CThingManager / CPersistTraits /
// NThingManager_CEntityRuntimePersistence models; opt-in branches FABLE_RETAIL_CDATAOUTPUTSTREAM, CCLASSFACTORY, CACOMPRESSOR,
// CCOMPRESSORZLIB, CARRAY (vector(n) ctor), SCOPED_PTR (accessors); CPERSISTCONTEXT and CDATAINPUTSTREAM extended; CGameComponent /
// CMainGameComponent from fable_game.h (FABLE_RETAIL_CGAMECOMPONENT / FABLE_RETAIL_CMAINGAMECOMPONENT) (hdr/README.md).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CACOMPRESSOR
#define FABLE_RETAIL_CCOMPRESSORZLIB
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#include "stlport/fable_containers.h"
#include "engine/CThingManager.h"
#include "fable_game.h"
#include "engine/CWorld.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistTraits.h"
#include "engine/CArray.h"
#include "engine/CCompressorZlib.h"
namespace NProgressDisplay {
    void __fastcall DisplayProgress(const CCharString&,float,bool,bool);
}
#pragma auto_inline(off)
// 0x0040FE60 / 0x0040F8A0: the scalar traits readers (an inline ReadType<T> of 4 bytes each).
template<> void CPersistTraits<long>::TransferBinaryIn(CMemoryDataInputStream& stream,long& val) { val=stream.ReadSLONG(); }
template<> void CPersistTraits<unsigned long>::TransferBinaryIn(CMemoryDataInputStream& stream,unsigned long& val) { val=stream.ReadULONG(); }
#pragma auto_inline(on)
template<> inline void CPersistContext::Transfer<long>(long& val) {
    CheckCRC("");
    CPersistTraits<long> traits;
    switch(Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream,val);break;
    case MODE_SAVE_BINARY: traits.TransferBinaryOut(*PSaveStream,val);break;
    }
}
template<> inline void CPersistContext::Transfer<unsigned long>(unsigned long& val) {
    CheckCRC("");
    CPersistTraits<unsigned long> traits;
    switch(Mode) {
    case MODE_LOAD_BINARY: traits.TransferBinaryIn(*PLoadStream,val);break;
    case MODE_SAVE_BINARY: traits.TransferBinaryOut(*PSaveStream,val);break;
    }
}
void CThingManager::LoadHeroStateBinary(CPersistContext& context) {
    const CWorldMap& world_map=Component.PeekWorld().PeekWorldMap();
    long no_scripts=0;
    context.Transfer(no_scripts);
    for(long c1=0;c1<no_scripts;++c1) {
        long script_size=0;
        context.Transfer(script_size);
        if(script_size>0) {
            NProgressDisplay::DisplayProgress("Loading entities from script 1",-1.0f,false,false);
            CMemoryDataInputStream script_is;
            script_is.OpenMemorySource(context.GetPLoadStream()->ReadConstData(script_size),script_size);
            CPersistContext script_context(script_is,true);
            unsigned long num_sections=0;
            script_context.Transfer(num_sections);
            unsigned long data_size=0;
            script_context.Transfer(data_size);
            unsigned long uncompressed_size=0;
            script_context.Transfer(uncompressed_size);
            CArray<unsigned char> uncompressed_data(uncompressed_size);
            const unsigned char* compressed_data=script_context.GetPLoadStream()->ReadConstData(data_size);
            CCompressorZlib compressor_zlib;
            unsigned long output_size=compressor_zlib.InternalDecompress(compressed_data,data_size,uncompressed_data.GetAsCArray(),uncompressed_size);
            CMemoryDataInputStream is(uncompressed_data.GetAsCArray(),uncompressed_size);
            unsigned long level_id=is.ReadULONG();
            CCharString name=is.ReadNullTerminatedString();
            unsigned long section_size=is.ReadULONG();
            CMemoryDataInputStream section_is(is.ReadConstData(section_size),section_size);
            unsigned long entity_size=section_is.ReadULONG();
            CMemoryDataInputStream entity_is(section_is.ReadConstData(entity_size),entity_size);
            CPersistContext entity_context(entity_is,true);
            NProgressDisplay::DisplayProgress("Loading entities from script 2",-1.0f,false,false);
            LoadSingleThing(entity_context,1);
            NProgressDisplay::DisplayProgress("Loading entities from script 3",-1.0f,false,false);
            CCharString script;
            script_context.Transfer(script);
        }
    }
}