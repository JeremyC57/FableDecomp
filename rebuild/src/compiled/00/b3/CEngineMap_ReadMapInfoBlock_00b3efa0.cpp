// CEngineMap::ReadMapInfoBlock(CStaticMapInfoBlock&, CDataInputStream&) -- read the fixed static-map file header.
// Leaf: ?ReadMapInfoBlock@CEngineMap@@CIXAAUCStaticMapInfoBlock@1@AAVCDataInputStream@@@Z
// Reads: the inline CDataInputStream::ReadSLONG / ReadULONG (Ego_r / FableWin ??$ReadType@J@ / ??$ReadType@K@CDataInputStream), each
//   the bounds-checked inline Read; CameraMapBounds is one raw Read of the struct.
#define FABLE_RETAIL_CDATAINPUTSTREAM
#include "engine/CEngineMap.h"
#include "engine/CDataInputStream.h"
void __fastcall CEngineMap::ReadMapInfoBlock(CStaticMapInfoBlock& block,CDataInputStream& ipstream)
{
    block.VersionID=ipstream.ReadSLONG();
    block.BankFileIndex=ipstream.ReadSLONG();
    block.EdgeHeightFileSize=ipstream.ReadSLONG();
    block.MapWidth=ipstream.ReadSLONG();
    block.MapHeight=ipstream.ReadSLONG();
    block.WorldX=ipstream.ReadSLONG();
    block.WorldY=ipstream.ReadSLONG();
    block.EdgeHeightFilePtr=ipstream.ReadSLONG();
    block.LandscapeMapPtr=ipstream.ReadSLONG();
    block.LocalDetailMapPtr=ipstream.ReadSLONG();
    block.Quality=ipstream.ReadSLONG();
    block.ShorePointArraySize=ipstream.ReadSLONG();
    block.ShorePointArrayStart=ipstream.ReadSLONG();
    block.LevelChecksum=ipstream.ReadULONG();
    block.ChecksumBlockFilePtr=ipstream.ReadSLONG();
    block.ChecksumBlockFileSize=ipstream.ReadSLONG();
    ipstream.Read(&block.CameraMapBounds,sizeof(block.CameraMapBounds));
    block.HeaderEndPtr=ipstream.ReadSLONG();
}