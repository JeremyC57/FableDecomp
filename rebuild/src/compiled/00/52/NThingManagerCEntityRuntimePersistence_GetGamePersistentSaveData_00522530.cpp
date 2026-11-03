// NThingManager::CEntityRuntimePersistence::GetGamePersistentSaveData(long index, CArray<unsigned char>& out) const --
//   validate a game-persistent entity script and decompress its saved data into `out`.
// Leaf: ?GetGamePersistentSaveData@CEntityRuntimePersistence@NThingManager@@QBE_NJAAV?$CArray@E@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00522530, 194 bytes; name in Ego_r (0x005297A0) and FableWin (0x01D437D0).
// Callees: vector<unsigned char>::resize 0x00486189, CACompressor::CACompressor(long) 0x00A3AEA0,
//   CCompressorZlib::GetRequiredWorkspaceSize 0x00A3A7D0, CCompressorZlib::Initialise 0x00A3AA10,
//   CCompressorZlib::InternalDecompress 0x00A3AB00, CCompressorZlib::~CCompressorZlib 0x00A3ACC0; vtable 0x01245974.
// Shape: a local CCompressorZlib (inline ctor: base ctor with the required workspace size, then Initialise); the
//   decompress call is non-virtual. Rebased (2026-11-03) from a row-local CCompressorZlibNative model onto the shared
//   engine/CCompressorZlib.h, engine/CACompressor.h and NThingManager_CEntityRuntimePersistence.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CACOMPRESSOR
#define FABLE_RETAIL_CCOMPRESSORZLIB
#define FABLE_RETAIL_CARRAY
#include "stlport/fable_containers.h"
#include "engine/NThingManager_CEntityRuntimePersistence.h"
#include "engine/CCompressorZlib.h"

bool NThingManager::CEntityRuntimePersistence::GetGamePersistentSaveData(long index, CArray<unsigned char>& out) const
{
    if ((unsigned long)index < GamePersistentEntityScripts.size()) {
        const CEntityScriptInfo& info = GamePersistentEntityScripts[index];
        if (info.Valid) {
            out.resize(info.UncompressedSize, 0);
            if (info.UncompressedSize > 0) {
                CCompressorZlib compressor;
                compressor.InternalDecompress(info.SaveData.GetAsCArray(), info.SaveData.size(), out.GetAsCArray(), out.size());
            }
            return true;
        }
    }
    return false;
}