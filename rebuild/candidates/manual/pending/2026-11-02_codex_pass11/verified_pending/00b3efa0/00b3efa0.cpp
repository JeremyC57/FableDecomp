// CEngineMap::ReadMapInfoBlock(CStaticMapInfoBlock&, CDataInputStream&) -- read the fixed static-map file header.
// Leaf: ?ReadMapInfoBlock@CEngineMap@@CIXAAUCStaticMapInfoBlock@1@AAVCDataInputStream@@@Z
#define FABLE_RETAIL_CENGINEMAP_READ
#define FABLE_RETAIL_CDATAINPUTSTREAM
#include "engine/CEngineMap.h"
#include "engine/CDataInputStream.h"
void __fastcall CEngineMap::ReadMapInfoBlock(CStaticMapInfoBlock& block,CDataInputStream& ipstream)
{
    { long val; ipstream.Read(&val,sizeof(val)); block.VersionID=val; }
    { long val; ipstream.Read(&val,sizeof(val)); block.BankFileIndex=val; }
    { long val; ipstream.Read(&val,sizeof(val)); block.EdgeHeightFileSize=val; }
    { long val; ipstream.Read(&val,sizeof(val)); block.MapWidth=val; }
    { long val; ipstream.Read(&val,sizeof(val)); block.MapHeight=val; }
    { long val; ipstream.Read(&val,sizeof(val)); block.WorldX=val; }
    { long val; ipstream.Read(&val,sizeof(val)); block.WorldY=val; }
    { long val; ipstream.Read(&val,sizeof(val)); block.EdgeHeightFilePtr=val; }
    { long val; ipstream.Read(&val,sizeof(val)); block.LandscapeMapPtr=val; }
    { long val; ipstream.Read(&val,sizeof(val)); block.LocalDetailMapPtr=val; }
    { long val; ipstream.Read(&val,sizeof(val)); block.Quality=val; }
    { long val; ipstream.Read(&val,sizeof(val)); block.ShorePointArraySize=val; }
    { long val; ipstream.Read(&val,sizeof(val)); block.ShorePointArrayStart=val; }
    { unsigned long val; ipstream.Read(&val,sizeof(val)); block.LevelChecksum=val; }
    { long val; ipstream.Read(&val,sizeof(val)); block.ChecksumBlockFilePtr=val; }
    { long val; ipstream.Read(&val,sizeof(val)); block.ChecksumBlockFileSize=val; }
    ipstream.Read(&block.CameraMapBounds,sizeof(block.CameraMapBounds));
    { long val; ipstream.Read(&val,sizeof(val)); block.HeaderEndPtr=val; }
}
