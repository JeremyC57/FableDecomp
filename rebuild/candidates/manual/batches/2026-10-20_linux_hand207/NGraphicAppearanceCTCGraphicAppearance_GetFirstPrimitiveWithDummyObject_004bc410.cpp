// NGraphicAppearance::CTCGraphicAppearance::GetFirstPrimitiveWithDummyObject(const CCharString& name) const -- the first mesh primitive whose mesh carries the named dummy object, or 0
// Leaf: GetFirstPrimitiveWithDummyObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BC410, length 224 bytes (+ int3 padding). __thiscall, `ret 4`.
//         Ego_r.pdb `?GetFirstPrimitiveWithDummyObject@CTCGraphicAppearance@NGraphicAppearance@@QBEPBVCPrimitive@2@ABVCCharString@@@Z`.
// Callees: Calc @ 0x004014A0 (`__fastcall(0, chars, length)` CRC of the name; the empty literal 0x0129AAF4 for a
//          null string), the mesh bank's GetMeshInfo @ 0x00A24270 on g_pMeshDataBank @ 0x013B8A04,
//          C3DMeshInfo::GetMeshStats() const @ 0x00A25530 (counted pointer by hidden pointer into an 8-byte frame
//          temporary), C3DMeshStats::HasDummyObject(unsigned long) const @ 0x00A2EA60, operator delete @ 0x00BFE9BC.
// Layout: retail CTCGraphicAppearance Primitives +0x38 (STLport list: sentinel node pointer; PDB +0x40, the two
//         maps ahead are 12-byte retail maps); CPrimitive (PDB, 0x10): Type +0, PrimitiveHandle +4, BankIndex +0xC.
// Shape: the bank's mesh info is a named local fetched before the hidden-return temporary; mesh primitives only (types 1 and 2) with a bank index; the counted temporary is released inline after the
//        query (count decremented in place, block re-read from the frame, both words zeroed) before the result test.
void __cdecl operator delete(void* p);                      // 0x00BFE9BC

struct CCharStringData {
    char*         m_pData;                                  // +0
    unsigned long m_length;                                 // +4
};
struct CCharString {
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetPString() const { return m_pStringData == 0 ? "" : m_pStringData->m_pData; }
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const char* data, unsigned long length);   // 0x004014A0: CRC

struct CCPPointerInfo {                                     // PDB: RefCount +0, DeleteFunc +4, Data +8
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template<class T> struct CCountedPointer {
    T*              Data;                                   // +0
    CCPPointerInfo* Info;                                   // +4
    T* operator->() const { return Data; }
    ~CCountedPointer()
    {
        if (Info != 0) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};

struct C3DMeshStats {
    bool HasDummyObject(unsigned long key) const;           // 0x00A2EA60
};
struct C3DMeshInfo {
    CCountedPointer<const C3DMeshStats> GetMeshStats() const;   // 0x00A25530
};
struct CMeshDataBank {
    const C3DMeshInfo* GetMeshInfo(long index);             // 0x00A24270
};
extern CMeshDataBank* g_pMeshDataBank;                      // 0x013B8A04

struct CEnginePrimitiveHandle {
    long  m_header;
    void* Data;
};

namespace NGraphicAppearance {

enum EPrimitiveType { PRIMITIVE_TYPE_0, PRIMITIVE_TYPE_MESH_1, PRIMITIVE_TYPE_MESH_2 };

struct CPrimitive {
    EPrimitiveType         Type;                            // +0x0
    CEnginePrimitiveHandle PrimitiveHandle;                 // +0x4
    long                   BankIndex;                       // +0xC
};

struct CPrimitiveListNode {
    CPrimitiveListNode* _M_next;
    CPrimitiveListNode* _M_prev;
    CPrimitive          _M_data;
};

struct CTCGraphicAppearance {
    unsigned char       _pad00[0x38];
    CPrimitiveListNode* Primitives;                         // +0x38 STLport list<CPrimitive> (sentinel)
    const CPrimitive* GetFirstPrimitiveWithDummyObject(const CCharString& name) const;
};

const CPrimitive* CTCGraphicAppearance::GetFirstPrimitiveWithDummyObject(const CCharString& name) const
{
    unsigned long key = Calc_004014a0(0, name.GetPString(), name.GetLength());
    for (CPrimitiveListNode* node = Primitives->_M_next; node != Primitives; node = node->_M_next) {
        const CPrimitive& primitive = node->_M_data;
        if ((primitive.Type == PRIMITIVE_TYPE_MESH_1 || primitive.Type == PRIMITIVE_TYPE_MESH_2) && primitive.BankIndex != 0) {
            const C3DMeshInfo* meshInfo = g_pMeshDataBank->GetMeshInfo(primitive.BankIndex);
            if (meshInfo->GetMeshStats()->HasDummyObject(key))
                return &primitive;
        }
    }
    return 0;
}

}
