// Behaviour test for 007e1990: src/007e1990.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes::GetMeshesUsed(vector<CMeshInfo>& meshes) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes::GetMeshesUsed(vector<CMeshInfo>& meshes) const -- add every mesh of the multi-static-mesh def, each sized like the main graphic and flagged primary
// Leaf: GetMeshesUsed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E1990, length 212 bytes (+ int3 padding). __fastcall, `ret 4`; `this` is the
//         CAGraphicAppearanceComponent subobject at +0x0C (ParentThing read as [ecx-8], PDef as [ecx+10h]).
//         Ego_r.pdb: `?GetMeshesUsed@CTCGraphicAppearanceMultipleStaticMeshes@NGraphicAppearance@@UBEXAAV?$vector@...`.
// Callees: vector<CMeshInfo>::_M_insert_overflow(pos, const T&, const __true_type&, size_type, bool) @ 0x007091A0.
// Shape: an unsigned index loop over PDef->Meshes (56-byte CMultiStaticMeshEntryDef, `size()` = the signed difference
//        divided by 56 through the 0x92492493 multiply, re-evaluated each pass; the byte offset kept in ebx); the main
//        graphic is a reference kept in the frame; each CMeshInfo is filled member by member and pushed through the
//        inline STLport push_back (placement copy, else _M_insert_overflow with the tag in the dead argument slot).
// Layout: Ego_r.pdb CTCGraphicAppearanceMultipleStaticMeshes: PrimHandles +0x10 (12-byte STLport vector in retail), PDef
//         +0x1C; CMultiStaticMeshDef::Meshes +0x28, CMultiStaticMeshEntryDef::Mesh +0x28 (PDB); retail CThing
//         PTCGraphicAppearance +0x64, CTCGraphicAppearance::MainGraphic +0x44 (RenderSizeX +0x4C).
namespace NGraphicAppearance {

class CMeshInfo {
public:
    long Index;                                     // +0x00
    float SizeX;                                    // +0x04
    unsigned char PrimaryMesh : 1;                  // +0x08
};

}

#pragma pack(push, 2)
class CEngineGraphic {                              // 14 bytes
public:
    unsigned long BankIndex;                        // +0x00
    float AnimStep;                                 // +0x04
    float RenderSizeX;                              // +0x08
    bool AdditiveAlpha;                             // +0x0C
    unsigned char Type;                             // +0x0D
};
#pragma pack(pop)

struct __true_type { __true_type() {} };           // empty tag with a user constructor (no byte store)
inline void* operator new(unsigned int, void* where) { return where; }

template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    const T& operator[](unsigned long n) const { return *(begin() + n); }
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend);   // 0x007091A0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (_M_insert_overflow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
template <class T>
__declspec(noinline) void CRetailVector<T>::_M_insert_overflow(T* position, const T& x, const __true_type& p2, unsigned long fill_len, bool atend)
{
    T* p = (T*)GrowVectorStorage_s7(this, fill_len);
    if (p)
        *p = x;
    
}
#pragma auto_inline(on)

class CMultiStaticMeshEntryDef {                    // 56 bytes
public:
    unsigned char _pad00[0x28];
    long Mesh;                                      // +0x28
    unsigned char _pad2c[0x38 - 0x2c];
};
class CMultiStaticMeshDef {
public:
    unsigned char _pad00[0x28];
    CRetailVector<CMultiStaticMeshEntryDef> Meshes; // +0x28
};

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    unsigned char _pad00[0x44];
    CEngineGraphic MainGraphic;                     // +0x44
};
}

class CThing {
public:
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
};

class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
};

namespace NGraphicAppearance {

class CAGraphicAppearanceComponent {
public:
    virtual void GetMeshesUsed(CRetailVector<CMeshInfo>& meshes) const;
};

class CTCGraphicAppearanceMultipleStaticMeshes : public CTCBase, public CAGraphicAppearanceComponent {
public:
    CRetailVector<void*> PrimHandles;               // +0x10 vector<CPrimitive*>
    const CMultiStaticMeshDef* PDef;                // +0x1C CDefPointer<const CMultiStaticMeshDef>::Object
    virtual void GetMeshesUsed(CRetailVector<CMeshInfo>& meshes) const;
};

void CTCGraphicAppearanceMultipleStaticMeshes::GetMeshesUsed(CRetailVector<CMeshInfo>& meshes) const
{
    const CEngineGraphic& graphic = ParentThing->PTCGraphicAppearance->MainGraphic;
    for (unsigned long i = 0; i < PDef->Meshes.size(); ++i) {
        CMeshInfo info;
        info.Index = PDef->Meshes[i].Mesh;
        info.SizeX = graphic.RenderSizeX;
        info.PrimaryMesh = true;
        meshes.push_back(info);
    }
}

}

// ---- stubs and driver ----
using namespace NGraphicAppearance;
static CMeshInfo g_store[8]; static int g_grow = 0;
void* GrowVectorStorage_s7(void* vector, unsigned long count)
{
    ++g_grow; CRetailVector<CMeshInfo>* v = (CRetailVector<CMeshInfo>*)vector; CMeshInfo* p = v->_M_finish; v->_M_finish += count; if (v->_M_end_of_storage < v->_M_finish) v->_M_end_of_storage = v->_M_finish; return p;
}
void CTCBase::v00() {}
void CAGraphicAppearanceComponent::GetMeshesUsed(CRetailVector<CMeshInfo>&) const {}
int main()
{
    CMultiStaticMeshEntryDef defs[3]; memset(defs, 0, sizeof(defs)); defs[0].Mesh = 11; defs[1].Mesh = 22; defs[2].Mesh = 33;
    CMultiStaticMeshDef def; def.Meshes._M_start = defs; def.Meshes._M_finish = defs + 3; def.Meshes._M_end_of_storage = defs + 3;
    CTCGraphicAppearance app; memset(&app, 0, sizeof(app)); app.MainGraphic.RenderSizeX = 2.5f;
    CThing thing; thing.PTCGraphicAppearance = &app;
    CTCGraphicAppearanceMultipleStaticMeshes tc; tc.ParentThing = &thing; tc.PDef = &def;
    CRetailVector<CMeshInfo> meshes; meshes._M_start = g_store; meshes._M_finish = g_store; meshes._M_end_of_storage = g_store + 2;
    memset(g_store, 0, sizeof(g_store));
    tc.GetMeshesUsed(meshes);
    CHECK(meshes.size() == 3); CHECK(g_grow == 1);
    CHECK(g_store[0].Index == 11 && g_store[1].Index == 22 && g_store[2].Index == 33);
    CHECK(g_store[0].SizeX == 2.5f && g_store[2].SizeX == 2.5f); CHECK(g_store[0].PrimaryMesh == 1 && g_store[1].PrimaryMesh == 1 && g_store[2].PrimaryMesh == 1);
    def.Meshes._M_finish = defs; tc.GetMeshesUsed(meshes); CHECK(meshes.size() == 3 && g_grow == 1);
    if (fails == 0) printf("007e1990_TEST PASS\n"); else printf("007e1990_TEST FAIL\n");
    return fails;
}