// CRandomAppearanceMorph::TransferBinaryIn(CMemoryDataInputStream* stream) -- read the body-part count; for each body part resize its mesh array to the stored count and read every mesh's index, priority and texture-morph map; read each body part's texture-morph map; resize the skeletal-morph map to the stored count and read every entry's key, group, name and variation flag (marking the map sorted); then read the general texture-morph map and the highest texture group id
// Grade: DIFFER(1307v1307) -- one jump target: on the resize erase path retail jumps past the default CBodyPartMesh
//   temp's free check (it knows the temp is still zero); VC here jumps to the check. Tried default-arg const-ref and
//   by-value resize, named local, else-only temp (forced and unforced).
// Leaf: TransferBinaryIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086BFB0, 1305 bytes (int3 padded to 1312), `ret 4`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, vector<CBodyPartMesh> erase 0x0086E150 / fill insert
//   0x0086E5C0, vector<pair<long, CSkeletalMorph>> erase 0x0086C720 / fill insert 0x0086CD70, GFReadVectorMapMemDump
//   <CTextureMorph> 0x0086DEB0 (`__fastcall`), CDefString::TransferBinaryIn 0x0044FC00, free 0x00BFEA14.
// Layout: Ego_r.pdb CRandomAppearanceMorph with 12-byte retail vectors: BodyParts +0x00, BodyPartTextureMorphs +0x0c,
//   SkeletalMorphs +0x18 (Dirty +0x25), GeneralTextureMorphs +0x28, MaxTextureGroupID +0x38; CBodyPartMesh 0x18 bytes
//   {MeshIndex, Priority, TextureMorphs (16-byte CVectorMap)}; pair<long, CSkeletalMorph {GroupID, Name, AllowVariation}> 16.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
extern "C" void __cdecl free(void* p);              // 0x00BFEA14
struct CMemoryDataInputStream {
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= m_remaining) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline long ReadLong()
    {
        long value;
        Read(&value, sizeof(value));
        return value;
    }
    __forceinline unsigned char ReadByte()
    {
        unsigned char value;
        Read(&value, sizeof(value));
        return value;
    }
};
struct CDefString {
    long Index;
    CDefString() : Index(-1) {}
    void TransferBinaryIn(CMemoryDataInputStream* stream);   // 0x0044FC00
};
struct CRandomAppearanceMorph {
    struct CTextureMorph { unsigned long NewTexture; unsigned long GroupID; };
    struct CCompare { };
    struct CTextureMorphPair { unsigned long first; CTextureMorph second; };
    struct CTextureMorphMap {                       // CVectorMap<unsigned long, CTextureMorph>, 16 bytes
        CTextureMorphPair* _M_start;
        CTextureMorphPair* _M_finish;
        CTextureMorphPair* _M_end_of_storage;
        CCompare Compare;
        bool Dirty;
        CTextureMorphMap() : _M_start(0), _M_finish(0), _M_end_of_storage(0), Dirty(false) {}
        ~CTextureMorphMap() { if (_M_start) free(_M_start); }
    };
    struct CBodyPartMesh {                          // 0x18 bytes
        long MeshIndex;
        long Priority;
        CTextureMorphMap TextureMorphs;
        CBodyPartMesh() : MeshIndex(0), Priority(0) {}
    };
    struct CBodyPartMeshArray {                     // CArray<CBodyPartMesh>
        CBodyPartMesh* _M_start;
        CBodyPartMesh* _M_finish;
        CBodyPartMesh* _M_end_of_storage;
        unsigned int size() const { return _M_finish - _M_start; }
        CBodyPartMesh* erase(CBodyPartMesh* first, CBodyPartMesh* last);   // 0x0086E150
        void _M_fill_insert(CBodyPartMesh* position, unsigned int count, const CBodyPartMesh& value);   // 0x0086E5C0
        void resize(unsigned int count, const CBodyPartMesh& value)
        {
            if (count < size())
                erase(_M_start + count, _M_finish);
            else
                _M_fill_insert(_M_finish, count - size(), value);
        }
        void resize(unsigned int count) { resize(count, CBodyPartMesh()); }
    };
    struct CSkeletalMorph {
        long GroupID;
        CDefString Name;
        bool AllowVariation;
        CSkeletalMorph() : GroupID(0), AllowVariation(false) {}
    };
    struct CSkeletalMorphPair {                     // pair<long, CSkeletalMorph>, 16 bytes
        long first;
        CSkeletalMorph second;
        CSkeletalMorphPair() : first(0) {}
    };
    struct CSkeletalMorphMap {
        CSkeletalMorphPair* _M_start;
        CSkeletalMorphPair* _M_finish;
        CSkeletalMorphPair* _M_end_of_storage;
        CCompare Compare;
        bool Dirty;
        unsigned int size() const { return _M_finish - _M_start; }
        CSkeletalMorphPair* erase(CSkeletalMorphPair* first, CSkeletalMorphPair* last);   // 0x0086C720
        void _M_fill_insert(CSkeletalMorphPair* position, unsigned int count, const CSkeletalMorphPair& value);   // 0x0086CD70
        void resize(unsigned int count, const CSkeletalMorphPair& value)
        {
            if (count < size())
                erase(_M_start + count, _M_finish);
            else
                _M_fill_insert(_M_finish, count - size(), value);
        }
        void resize(unsigned int count) { resize(count, CSkeletalMorphPair()); }
    };
    struct CBodyPartArray {                         // CArray<CArray<CBodyPartMesh>>
        CBodyPartMeshArray* _M_start;
        CBodyPartMeshArray* _M_finish;
        CBodyPartMeshArray* _M_end_of_storage;
    };
    struct CTextureMorphMapArray {
        CTextureMorphMap* _M_start;
        CTextureMorphMap* _M_finish;
        CTextureMorphMap* _M_end_of_storage;
    };
    CBodyPartArray        BodyParts;                // +0x00
    CTextureMorphMapArray BodyPartTextureMorphs;    // +0x0c
    CSkeletalMorphMap     SkeletalMorphs;           // +0x18
    CTextureMorphMap      GeneralTextureMorphs;     // +0x28
    unsigned long         MaxTextureGroupID;        // +0x38
    void TransferBinaryIn(CMemoryDataInputStream* stream);
};
void __fastcall GFReadVectorMapMemDump(CMemoryDataInputStream& stream, CRandomAppearanceMorph::CTextureMorphMap& map);   // 0x0086DEB0

void CRandomAppearanceMorph::TransferBinaryIn(CMemoryDataInputStream* stream)
{
    unsigned long numBodyParts = stream->ReadLong();
    for (unsigned long i = 0; i < numBodyParts; ++i) {
        CBodyPartMeshArray& meshes = BodyParts._M_start[i];
        unsigned long numMeshes = stream->ReadLong();
        meshes.resize(numMeshes);
        for (unsigned long j = 0; j < numMeshes; ++j) {
            CBodyPartMesh& mesh = meshes._M_start[j];
            mesh.MeshIndex = stream->ReadLong();
            mesh.Priority = stream->ReadLong();
            GFReadVectorMapMemDump(*stream, mesh.TextureMorphs);
        }
    }
    for (unsigned long k = 0; k < numBodyParts; ++k)
        GFReadVectorMapMemDump(*stream, BodyPartTextureMorphs._M_start[k]);
    unsigned long numSkeletalMorphs = stream->ReadLong();
    SkeletalMorphs.resize(numSkeletalMorphs);
    for (unsigned long m = 0; m < numSkeletalMorphs; ++m) {
        SkeletalMorphs._M_start[m].first = stream->ReadLong();
        CSkeletalMorph& morph = SkeletalMorphs._M_start[m].second;
        morph.GroupID = stream->ReadLong();
        morph.Name.TransferBinaryIn(stream);
        morph.AllowVariation = stream->ReadByte() != 0;
    }
    SkeletalMorphs.Dirty = false;
    GFReadVectorMapMemDump(*stream, GeneralTextureMorphs);
    MaxTextureGroupID = stream->ReadLong();
}
