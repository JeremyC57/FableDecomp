// NThingManager::CEntityRuntimePersistence::SetGamePersistentSaveData(long,const void*,unsigned long) -- grow the game-persistent script array to hold the index, store the uncompressed size and zlib-compress the data into the record
// Leaf: ?SetGamePersistentSaveData@CEntityRuntimePersistence@NThingManager@@QAEXJPBXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00523860, 173 bytes, thiscall, `ret 0Ch`. Callees: vector<CEntityScriptInfo>::resize(size_type) 0x0052C6A0
//   (out of line), CCompressorZlib::GetRequiredWorkspaceSize 0x00A3A7D0, CACompressor(long) 0x00A3AEA0, CCompressorZlib::Initialise 0x00A3AA10
//   (the inline CCompressorZlib() stores vtable 0x01245974 between them), CCompressorZlib::InternalCompress 0x00A3ADD0, ~CCompressorZlib 0x00A3ACC0.
// NF62 (2026-11-17): ported onto engine/NThingManager_CEntityRuntimePersistence.h / CArray.h / CCompressorZlib.h / CACompressor.h (the local
//   CArray, CEntityScriptInfo, CEntityScriptArray, CACompressorNative and CCompressorZlibNative models deleted); InternalCompress now binds
//   ?InternalCompress@CCompressorZlib@@QAEKPBXKAAV?$CArray@E@@@Z (FABLE_RETAIL_CCOMPRESSORZLIB_INTERNALCOMPRESS, the 0x00523CF0 TU opt-in).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_CACOMPRESSOR
#define FABLE_RETAIL_CCOMPRESSORZLIB
#define FABLE_RETAIL_CCOMPRESSORZLIB_INTERNALCOMPRESS
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CArray.h"
#include "engine/CCompressorZlib.h"
#include "engine/NThingManager_CEntityRuntimePersistence.h"

void NThingManager::CEntityRuntimePersistence::SetGamePersistentSaveData(long index, const void* data, unsigned long size)
{
    unsigned long needed = index + 1;
    GamePersistentEntityScripts.resize(needed > GamePersistentEntityScripts.size() ? needed : GamePersistentEntityScripts.size());
    CEntityScriptInfo& info = GamePersistentEntityScripts[index];
    info.UncompressedSize = size;
    if (size > 0)
    {
        CCompressorZlib compressor;
        compressor.InternalCompress(data, size, info.SaveData);
    }
    info.Valid = true;
}
