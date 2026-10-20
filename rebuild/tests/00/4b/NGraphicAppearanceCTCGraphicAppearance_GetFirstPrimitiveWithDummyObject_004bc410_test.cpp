#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __int64 long long
#endif
#include <cstdio>
#include <cstring>
static int g_armed, g_deletes, g_destroys;
void __cdecl operator delete(void* p) throw() { if (g_armed) ++g_deletes; }   // test spy (never frees)


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

// ---- test stubs ----
static unsigned long g_lastSeed = 99, g_calcCalls;
unsigned long __fastcall Calc_004014a0(unsigned long seed, const char* data, unsigned long length)
{
    g_lastSeed = seed; ++g_calcCalls;
    return length * 1000 + (unsigned char)data[0];
}
static C3DMeshStats g_stats[4];
static C3DMeshInfo g_infos[8];
static CCPPointerInfo g_block;
static CCountedPointer<const C3DMeshStats> g_ret;
static int g_statsCalls, g_queryCalls;
static unsigned long g_hasKey; static const C3DMeshStats* g_hasStats;
static void __fastcall DestroyStats(void*) { ++g_destroys; }
CCountedPointer<const C3DMeshStats> C3DMeshInfo::GetMeshStats() const
{
    ++g_statsCalls;
    g_ret.Data = &g_stats[(this - g_infos) % 4];
    g_ret.Info = &g_block;
    ++g_block.RefCount;
    return g_ret;   // copy of a global: no local destructor on either compiler
}
bool C3DMeshStats::HasDummyObject(unsigned long key) const
{
    ++g_queryCalls;
    return key == g_hasKey && this == g_hasStats;
}
static int g_lastIndex = -1;
const C3DMeshInfo* CMeshDataBank::GetMeshInfo(long index) { g_lastIndex = (int)index; return &g_infos[index]; }
static CMeshDataBank g_bank;
CMeshDataBank* g_pMeshDataBank = &g_bank;

using namespace NGraphicAppearance;

static int fail(int n) { printf("004bc410_TEST FAIL %d\n", n); return 1; }

int main()
{
    g_block.DeleteFunc = DestroyStats;
    CPrimitiveListNode head, n[5];
    long types[5] = { 0, 1, 2, 2, 1 };
    long banks[5] = { 3, 0, 5, 6, 7 };
    CPrimitiveListNode* prev = &head;
    for (int i = 0; i < 5; ++i) { n[i]._M_data.Type = (EPrimitiveType)types[i]; n[i]._M_data.BankIndex = banks[i]; prev->_M_next = &n[i]; n[i]._M_prev = prev; prev = &n[i]; }
    prev->_M_next = &head; head._M_prev = prev;
    CTCGraphicAppearance ga; ga.Primitives = &head;
    CCharStringData data = { (char*)"Spine", 5 };
    CCharString name; name.m_pStringData = &data;
    g_hasKey = 5000 + 'S'; g_hasStats = &g_stats[6 % 4];  // the bank-6 primitive (n[3])
    g_armed = 1;
    const CPrimitive* r = ga.GetFirstPrimitiveWithDummyObject(name);
    g_armed = 0;
    if (r != &n[3]._M_data) return fail(1);
    if (g_lastSeed != 0 || g_calcCalls != 1) return fail(2);
    if (g_statsCalls != 2 || g_queryCalls != 2 || g_lastIndex != 6) return fail(3);   // banks 5 and 6 only
    if (g_block.RefCount != 0 || g_destroys != 2 || g_deletes != 2) return fail(4);   // temporary released each time
    // not found, null name -> "" and length 0
    CCharString empty; empty.m_pStringData = 0;
    g_hasKey = 0 + 0; g_hasStats = 0; g_statsCalls = 0;
    g_armed = 1;
    r = ga.GetFirstPrimitiveWithDummyObject(empty);
    g_armed = 0;
    if (r != 0) return fail(5);
    if (g_statsCalls != 3 || g_destroys != 5 || g_deletes != 5) return fail(6);   // banks 5, 6, 7
    // empty list
    head._M_next = &head; head._M_prev = &head; g_statsCalls = 0;
    if (ga.GetFirstPrimitiveWithDummyObject(name) != 0 || g_statsCalls != 0) return fail(7);
    g_ret.Info = 0;
    printf("004bc410_TEST PASS\n");
    return 0;
}