// Grade: DIFFER(942v990) -- register allocation only: retail keeps the running CRC in edi and the string chars in ebx/esi (iterator esi then ebx), ours CRC in ebx, chars in edi; 63 masked lines
// CMeshBankStateBlock::CreateCRC() -- CRC-32 of the mesh bank state: the plain state words, every bone LOD (name + level), every excluded bone name and the optional XYZ range box
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A25040, 992 bytes.
// Callees: CCharString copy ctor @ 0x0099EC30, CCharString dtor @ 0x0099EAE0; reads the CRC-32 table @ 0x0129A168.
// Layout (Ego_r.pdb, retail 12-byte STLport map/set): State +0x04 (16 bytes), LODState.BoneLODs map<CCharString,long>
// header +0x14, LODState.MinXYZRange CCountedPointer<C3DBoundingBox> Data +0x20, AnimationState.ExcludedBones
// set<CCharString> header +0x28. Modelled locally (no generated header for this class).
// Shape: CCRC::Calc (0x004014A0) is inlined everywhere; the constant-length runs (16, 4, 12, 12 bytes) are unrolled.
extern const unsigned long g_crcTable_0129a168[256];   // 0x0129A168: CRC-32 table
extern const char g_emptyLiteral_0129aaf4[];           // 0x0129AAF4: ""

struct CCharStringData {
    const char*   m_pChars;
    unsigned long m_length;
};
struct CCharString {
    CCharStringData* m_pStringData;
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetChars() const { return m_pStringData == 0 ? g_emptyLiteral_0129aaf4 : m_pStringData->m_pChars; }
};

__forceinline unsigned long CRCCalc(unsigned long crc, const void* data, unsigned long length)
{
    const unsigned char* bytes = (const unsigned char*)data;
    if (length != 0)
        for (unsigned long i = 0; i < length; ++i)
            crc = g_crcTable_0129a168[bytes[i] ^ (crc & 0xff)] ^ (crc >> 8);
    return crc;
}

struct C3DVector { float X, Y, Z; };
struct C3DBoundingBox { C3DVector Min; C3DVector Max; };

struct SRbNodeBase {
    long         _M_color;
    SRbNodeBase* _M_parent;
    SRbNodeBase* _M_left;
    SRbNodeBase* _M_right;
};
__forceinline SRbNodeBase* RbIncrement(SRbNodeBase* node)
{
    if (node->_M_right != 0) {
        node = node->_M_right;
        while (node->_M_left != 0)
            node = node->_M_left;
    } else {
        SRbNodeBase* y = node->_M_parent;
        while (node == y->_M_right) {
            node = y;
            y = y->_M_parent;
        }
        if (node->_M_right != y)
            node = y;
    }
    return node;
}
struct SBoneLODNode : SRbNodeBase {
    CCharString first;
    long        second;
};
struct SBoneNode : SRbNodeBase {
    CCharString value;
};

struct CStateBlock {
    bool EnableLOD;
    bool PreserveMaterials;
    bool GenericAnimation;
    bool UnpackedPositions;
    long MaxLODs;
    long ResourcePhysicsMeshIndex;
    long AnimationVersion;
};

struct CMeshBankStateBlock {
    void*           _vfptr;               // +0x00
    CStateBlock     State;                // +0x04
    SBoneLODNode*   BoneLODsHeader;       // +0x14 map<CCharString,long> header
    void*           BoneLODsAlloc;        // +0x18
    long            BoneLODsCount;        // +0x1c
    C3DBoundingBox* MinXYZRange;          // +0x20 CCountedPointer Data
    void*           MinXYZRangeInfo;      // +0x24
    SBoneNode*      ExcludedBonesHeader;  // +0x28 set<CCharString> header
    void*           ExcludedBonesAlloc;   // +0x2c
    long            ExcludedBonesCount;   // +0x30
    unsigned long CreateCRC();
};

unsigned long CMeshBankStateBlock::CreateCRC()
{
    unsigned long crc = CRCCalc(0, &State, sizeof(State));
    for (SBoneLODNode* it = (SBoneLODNode*)BoneLODsHeader->_M_left; it != BoneLODsHeader;
         it = (SBoneLODNode*)RbIncrement(it)) {
        CCharString name(it->first);
        crc = CRCCalc(crc, name.GetChars(), name.GetLength());
        long level = it->second;
        crc = CRCCalc(crc, &level, sizeof(level));
    }
    for (SBoneNode* bone = (SBoneNode*)ExcludedBonesHeader->_M_left; bone != ExcludedBonesHeader;
         bone = (SBoneNode*)RbIncrement(bone)) {
        CCharString name(bone->value);
        crc = CRCCalc(crc, name.GetChars(), name.GetLength());
    }
    if (MinXYZRange != 0) {
        crc = CRCCalc(crc, &MinXYZRange->Min, sizeof(C3DVector));
        crc = CRCCalc(crc, &MinXYZRange->Max, sizeof(C3DVector));
    }
    return crc;
}
