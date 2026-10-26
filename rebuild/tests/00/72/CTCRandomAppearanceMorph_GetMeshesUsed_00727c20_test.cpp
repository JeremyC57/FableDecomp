// Behaviour test for 00727c20: src/00727c20.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCRandomAppearanceMorph::GetMeshesUsed(vector<NGraphicAppearance::CMeshInfo>& meshes) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCRandomAppearanceMorph::GetMeshesUsed(vector<NGraphicAppearance::CMeshInfo>& meshes) const -- report the meshes this appearance uses: the main graphic's mesh (primary) and the mesh of every part of the current morph set, all at the main graphic's render size
// Leaf: GetMeshesUsed
// Retail: Fable.exe (TLC, VC7.1) @ 0x00727C20, 214 bytes (`ret 4`). CTCRandomAppearanceMorph's CAGraphicAppearanceComponent vtable
//   0x01266520 slot 2 (catalog "CTCRandomAppearanceMorph::GetMeshesUsed"; PDB ?GetMeshesUsed@CTCRandomAppearanceMorph@@UBEXAAV?$vector@VCMeshInfo@NGraphicAppearance@@...@Z):
//   `this` is the component subobject at +0x0c (ParentThing is [ecx-8], PMeshMorphSet [ecx+4]).
// Callees: STLport vector<CMeshInfo>::_M_insert_overflow(pos, const T&, const __true_type&, 1, true) @ 0x007091A0.
// Layout: Ego_r.pdb NGraphicAppearance::CMeshInfo {Index, SizeX, PrimaryMesh:1} (12); CTCGraphicAppearance MainGraphic +0x44 (as
//   landed, CEngineGraphic BankIndex +0, RenderSizeX +8; pack(2), 14 bytes); CMeshMorphSet MeshParts {first, last, end} with
//   16-byte retail parts (MeshIndex +0).
// Tried: one named CMeshInfo reused (loads in the wrong order), the same with SizeX/flag/Index store order, constructor
//   temporaries (this source: everything but the copy schedule). Next: lane gap: copy scheduling of a 12-byte POD in placement new.
class CThing;
inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() { } };
namespace NGraphicAppearance {
    class CMeshInfo { public:                                  // 12 bytes
        long          Index;                            // +0x00
        float         SizeX;                            // +0x04
        unsigned char PrimaryMesh : 1;                  // +0x08
        CMeshInfo(long index, float sizeX, bool primaryMesh) : Index(index), SizeX(sizeX) { PrimaryMesh = primaryMesh; }
    };
    class CPrimitive;
}
template <class T> struct vector {                      // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __true_type& tag, unsigned long fillLength, bool atEnd);   // 0x007091A0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);

#pragma auto_inline(off)
// Same-TU body of the vector growth member (_M_insert_overflow, instantiated in retail's TU): the new storage receives the value
// read through its const reference; the empty tag is never touched.
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* position, const T& x, const __true_type& tag, unsigned long fillLength, bool atEnd)
{
    T* p = (T*)GrowVectorStorage_s7(this, fillLength);
    if (p)
        new (p) T(x);
}
#pragma auto_inline(on)
#pragma pack(push, 2)
class CEngineGraphic { public:                                 // 14 bytes (PDB)
    unsigned long BankIndex;                            // +0x00
    float         AnimStep;                             // +0x04
    float         RenderSizeX;                          // +0x08
    bool          AdditiveAlpha;                        // +0x0c
    unsigned char Type;                                 // +0x0d
};
#pragma pack(pop)
struct CTCGraphicAppearance {
    unsigned char  _pad00[0x44];
    CEngineGraphic MainGraphic;                         // +0x44
};
class CThing {
public:
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64 (retail)
};
class CEngineTextureMorphSet { public: void* _M_start; void* _M_finish; void* _M_end_of_storage; };
class CTCAppearanceMorphBase { public:
    class CMeshMorphSet { public:
        class CMeshMorphSetParts { public:                     // 16 bytes in retail
            unsigned long          MeshIndex;           // +0x00
            CEngineTextureMorphSet TextureMorphs;       // +0x04
        };
        struct CPartArray {                             // CArray<CMeshMorphSetParts> {first, last, end}
            CMeshMorphSetParts* m_pFirst;
            CMeshMorphSetParts* m_pLast;
            CMeshMorphSetParts* m_pEnd;
        };
        CPartArray MeshParts;                           // +0x00
    };
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
};
namespace NGraphicAppearance {
    class CAGraphicAppearanceComponent { public:               // vtable 0x0125A170
        virtual void v00();
        virtual void v01();
        virtual void GetMeshesUsed(vector<CMeshInfo>& meshes) const;   // slot 2
    };
}
template <class T> class CCountedPointer { public:             // {object, count block}
    T*    Object;
    void* Info;
    T* operator->() const { return Object; }
};
class CTCRandomAppearanceMorph : public CTCBase, public NGraphicAppearance::CAGraphicAppearanceComponent { public:
    CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet> PMeshMorphSet;   // +0x10
    unsigned long     Seed;                             // +0x18
    virtual void GetMeshesUsed(vector<NGraphicAppearance::CMeshInfo>& meshes) const;
};

void CTCRandomAppearanceMorph::GetMeshesUsed(vector<NGraphicAppearance::CMeshInfo>& meshes) const
{
    const CEngineGraphic& graphic = ParentThing->PTCGraphicAppearance->MainGraphic;
    meshes.push_back(NGraphicAppearance::CMeshInfo(graphic.BankIndex, graphic.RenderSizeX, true));
    for (const CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts* part = PMeshMorphSet->MeshParts.m_pFirst;
         part != PMeshMorphSet->MeshParts.m_pLast; ++part)
        meshes.push_back(NGraphicAppearance::CMeshInfo(part->MeshIndex, graphic.RenderSizeX, true));
}

// ---- stubs and driver ----
using namespace NGraphicAppearance;
static unsigned char g_raw[8 * sizeof(CMeshInfo)]; static int g_grow = 0;
void* GrowVectorStorage_s7(void* storage, unsigned long count)
{
    ++g_grow; vector<CMeshInfo>* v = (vector<CMeshInfo>*)storage; CMeshInfo* p = v->_M_finish; v->_M_finish += count; if (v->_M_end_of_storage < v->_M_finish) v->_M_end_of_storage = v->_M_finish; return p;
}
void CTCBase::v00() {}
void CAGraphicAppearanceComponent::v00() {} void CAGraphicAppearanceComponent::v01() {}
void CAGraphicAppearanceComponent::GetMeshesUsed(vector<CMeshInfo>&) const {}
int main()
{
    CMeshInfo* store = (CMeshInfo*)g_raw; memset(g_raw, 0, sizeof(g_raw));
    static CTCGraphicAppearance app; app.MainGraphic.BankIndex = 77; app.MainGraphic.RenderSizeX = 3.0f;
    static CThing thing; thing.PTCGraphicAppearance = &app;
    CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts parts[2]; memset(parts, 0, sizeof(parts)); parts[0].MeshIndex = 5; parts[1].MeshIndex = 6;
    CTCAppearanceMorphBase::CMeshMorphSet set; set.MeshParts.m_pFirst = parts; set.MeshParts.m_pLast = parts + 2; set.MeshParts.m_pEnd = parts + 2;
    CTCRandomAppearanceMorph tc; tc.ParentThing = &thing; tc.PMeshMorphSet.Object = &set; tc.PMeshMorphSet.Info = 0; tc.Seed = 0;
    vector<CMeshInfo> meshes; meshes._M_start = store; meshes._M_finish = store; meshes._M_end_of_storage = store + 2;
    tc.GetMeshesUsed(meshes);
    CHECK(meshes._M_finish == store + 3 && g_grow == 1);
    CHECK(store[0].Index == 77 && store[0].SizeX == 3.0f && store[0].PrimaryMesh == 1);
    CHECK(store[1].Index == 5 && store[1].SizeX == 3.0f && store[1].PrimaryMesh == 1);
    CHECK(store[2].Index == 6 && store[2].SizeX == 3.0f && store[2].PrimaryMesh == 1);
    set.MeshParts.m_pLast = parts; meshes._M_end_of_storage = store + 8; tc.GetMeshesUsed(meshes);
    CHECK(meshes._M_finish == store + 4 && store[3].Index == 77 && g_grow == 1);
    if (fails == 0) printf("00727c20_TEST PASS\n"); else printf("00727c20_TEST FAIL\n");
    return fails;
}