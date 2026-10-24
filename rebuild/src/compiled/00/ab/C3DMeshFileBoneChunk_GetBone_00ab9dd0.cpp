// C3DMeshFileBoneChunk::GetBone() const -- build the run-time bone record from the chunk: name CRC, parent index, no children counted yet, the localization matrix; returned by value
// Leaf: ?GetBone@C3DMeshFileBoneChunk
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB9DD0, length 82 bytes (+14 int3), thiscall with the hidden result
//   pointer, `ret 4`.
// Callees: C3DBone2::SetName(const CCharString&) @ 0x00ADD650 (stores the name's CRC in NameCRC).
// Identity: Ego_r.pdb `?GetBone@C3DMeshFileBoneChunk@@QBE?BVC3DBone2@@XZ` and C3DBone2 (60 bytes: NameCRC, Parent,
//   OriginalNoChildren, LocalizationMatrix); the row sits right after the C3DMeshFileBoneChunk constructors
//   (0x00AB9D50 / 0x00AB9DA0) and reads the chunk's ParentIndex (+0x30), LocalizationMatrix (+0x3C) and Name (+0x6C).
//   The manifest's "swap<CDebugSphere>" is propagated and wrong.
// Layout: retail chunk members sit 8 bytes below the Ego_r offsets (12-byte retail CArrays in CChunkedFileSuperChunk),
//   as the landed sibling chunk sources; local model.
// Shape: VC7.1 has no NRVO -- the named local is built (inline constructor: Parent = -1, OriginalNoChildren = 0, both
//   stored before the SetName call), Parent then overwritten, the 48-byte matrix block-copied (`rep movsd` x12), and the
//   whole 60-byte record block-copied into the hidden result (`rep movsd` x15).
class CCharString {
public:
    void* m_pData;                                 // CBasicString<char> data block
};

class CMatrix3x4 {                                 // 48 bytes: three rows of four floats
public:
    float m_row[3][4];
};

class C3DBone2 {                                   // 60 bytes (Ego_r.pdb)
public:
    unsigned long NameCRC;                         // +0x00
    long          Parent;                          // +0x04
    long          OriginalNoChildren;              // +0x08
    CMatrix3x4    LocalizationMatrix;              // +0x0c
    C3DBone2() : Parent(-1), OriginalNoChildren(0) { }
    void SetName(const CCharString& name);         // 0x00ADD650
};

class C3DMeshFileBoneChunk {
public:
    void*         __vftable;                       // +0x00
    unsigned char _superChunk[0x28];               // +0x04 CChunkedFileSuperChunk members (retail 12-byte arrays)
    long          BoneIndex;                       // +0x2c
    long          ParentIndex;                     // +0x30
    long          FirstChildIndex;                 // +0x34
    long          NextSiblingIndex;                // +0x38
    CMatrix3x4    LocalizationMatrix;              // +0x3c
    CCharString   Name;                            // +0x6c
    const C3DBone2 GetBone() const;
};

const C3DBone2 C3DMeshFileBoneChunk::GetBone() const
{
    C3DBone2 bone;
    bone.SetName(Name);
    bone.Parent = ParentIndex;
    bone.LocalizationMatrix = LocalizationMatrix;
    return bone;
}