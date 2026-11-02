#ifndef FABLE_ENGINE_CENGINESTATICMAPFILEBLOCKMANAGER_H
#define FABLE_ENGINE_CENGINESTATICMAPFILEBLOCKMANAGER_H
#include "engine/CCountedPointer.h"
#include "stlport/fable_map.h"
class CBankFileAsyncEntry;
class CBankFileAsyncData;
// Layout: donor TPI supplies every field/type. Retail's three-word STLport
// tree replaces the donor four-word tree; owning CEngineMap's widths follow
// this member at +0x5c, corroborating the 20-byte retail manager.
class CEngineStaticMapFileBlockManager {
public:
    ~CEngineStaticMapFileBlockManager();
private:
    struct CFileBlock {
        long RefCount;
        bool LowPriority;
        CCountedPointer<CBankFileAsyncData> LoadingData;
    };
    CCountedPointer<CBankFileAsyncEntry> AsyncBankEntry;
    std::map<long,CFileBlock> Blocks;
};
FABLE_STATIC_ASSERT(sizeof(CEngineStaticMapFileBlockManager)==20);
#endif
