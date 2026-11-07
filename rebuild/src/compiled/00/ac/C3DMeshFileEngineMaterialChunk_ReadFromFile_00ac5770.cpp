// C3DMeshFileEngineMaterialChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read an EMTL chunk: version 4 files carry the map flags and an optional map-file-name set; older files always get a name set and recompute the flags
// Leaf: ?ReadFromFile@C3DMeshFileEngineMaterialChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC5770, 2568 bytes, virtual (vtable 0x0129EFC8 slot 7), `ret 0Ch`.
// Identity: Ego_r.pdb ?ReadFromFile@C3DMeshFileEngineMaterialChunk@@UAEXAAVCDataInputStream@@KAAVCChunkedFileLoader@@@Z; the
//   mirror of WriteToFile 0x00AC52F0 (MaterialIndex, Name, MapFlags, the four bank indices, ...).
// Callees: the stream's virtual GetPosition (slot 2) and ReadNullTerminatedString (slot 6), CDataInputStream::ReadWithSrcChunkOverflow
//   0x00993CA0 (slow path of the inline Read), CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0, operator new 0x00BFEA1A,
//   CMapFilenames::CMapFilenames 0x00AC5190 (called in the version-4 arm, expanded in the old-version arm), CCharString() 0x0099E4B0,
//   CChunkedFileSuperChunk::ReadFromFile 0x00AA8830 (the base reader, given the bytes left), CalculateMapFlags 0x00AC5260.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_C3DMESHFILEENGINEMATERIALCHUNK
#include "fable_string.h"
#include "engine/CDataInputStream.h"
#include "engine/C3DMeshFileEngineMaterialChunk.h"

void C3DMeshFileEngineMaterialChunk::ReadFromFile(CDataInputStream& in, unsigned long chunk_size, CChunkedFileLoader& loader)
{
    unsigned long start_pos = in.GetPosition();
    unsigned long version = in.ReadULONG();
    if (version == 4) {
        MaterialIndex = in.ReadSLONG();
        Name = in.ReadNullTerminatedString();
        MapFlags = in.ReadULONG();
        DiffuseMapBankIndex = in.ReadULONG();
        BumpMapBankIndex = in.ReadULONG();
        ReflectionMapBankIndex = in.ReadULONG();
        IlluminationMapBankIndex = in.ReadULONG();
        if (in.ReadEBOOL()) {
            MapFilenames.Reset(new CMapFilenames);
            MapFilenames->DiffuseMap = in.ReadNullTerminatedString();
            MapFilenames->BumpMap = in.ReadNullTerminatedString();
            MapFilenames->ReflectionMap = in.ReadNullTerminatedString();
            MapFilenames->IlluminationMap = in.ReadNullTerminatedString();
        }
        else
            MapFilenames.Reset(0);
        SelfIllumination = in.ReadULONG();
        TwoSided = in.ReadEBOOL();
        Transparent = in.ReadEBOOL();
        BooleanAlpha = in.ReadEBOOL();
        DegenerateTriangles = in.ReadEBOOL();
        unsigned long header_size = in.GetPosition() - start_pos;
        CChunkedFileSuperChunk::ReadFromFile(in, chunk_size - header_size, loader);
    }
    else {
        MaterialIndex = in.ReadSLONG();
        Name = in.ReadNullTerminatedString();
        DiffuseMapBankIndex = in.ReadULONG();
        BumpMapBankIndex = in.ReadULONG();
        ReflectionMapBankIndex = in.ReadULONG();
        IlluminationMapBankIndex = in.ReadULONG();
        MapFilenames.Reset(new CMapFilenames);
        MapFilenames->DiffuseMap = in.ReadNullTerminatedString();
        MapFilenames->BumpMap = in.ReadNullTerminatedString();
        MapFilenames->ReflectionMap = in.ReadNullTerminatedString();
        MapFilenames->IlluminationMap = in.ReadNullTerminatedString();
        SelfIllumination = in.ReadULONG();
        TwoSided = in.ReadEBOOL();
        Transparent = in.ReadEBOOL();
        BooleanAlpha = in.ReadEBOOL();
        if (version > 2)
            DegenerateTriangles = in.ReadEBOOL();
        unsigned long header_size = in.GetPosition() - start_pos;
        CChunkedFileSuperChunk::ReadFromFile(in, chunk_size - header_size, loader);
        CalculateMapFlags();
    }
}