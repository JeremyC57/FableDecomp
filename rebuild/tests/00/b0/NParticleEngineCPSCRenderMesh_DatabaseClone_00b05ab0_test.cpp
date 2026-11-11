// Behaviour test for 0x00B05AB0 NParticleEngine::CPSCRenderMesh::DatabaseClone() const (agentFAM05 respell; the catalog test linked
// the old symbol DrawIsDrawable). VC7.1: links the row object; this file supplies CPSCRenderMesh::Alloc (fastcall static) and a
// CPSCRenderMesh whose slot 18 (DatabaseCopy) records its argument. The virtuals keep the TLC slot order of the row's model with
// their parameters dropped (only the slot positions matter to the row). g++: the row body as a portable twin.
#include <cstdio>
#ifdef _MSC_VER
#define FABLE_TEST_FASTCALL __fastcall
#else
#define FABLE_TEST_FASTCALL
#endif
namespace NParticleEngine {
class CPSCBase {
public:
    virtual ~CPSCBase() {}                                   // slot 0
    virtual void GetUpdateParam() {}                         // slot 1
    virtual CPSCBase* DatabaseClone() const = 0;             // slot 2
    virtual void GetClassTypeName() const {}                 // slot 3
    virtual void GetComponentType() const {}                 // slot 4
    virtual void GetComponentSubType() const {}              // slot 5
    virtual void GetGUIName() const {}                       // slot 6
    virtual void OnCreate() {}                               // slot 7
    virtual void StartUpdate() {}                            // slot 8
    virtual void Update() {}                                 // slot 9
    virtual void CollectUpdateParameters() {}                // slot 10
    virtual void TransferUpdateParametersToDialog() {}       // slot 11
    virtual void UpdateAddParticle() {}                      // slot 12
    virtual void PreparePrimitives() {}                      // slot 13
    virtual void InterpolateTo() {}                          // slot 14
    virtual void ReadBinary() {}                             // slot 15
    virtual void WriteBinary() const {}                      // slot 16
    virtual void DatabaseSerialise() {}                      // slot 17
    virtual void DatabaseCopy(const CPSCBase&) {}            // slot 18
};
class CPSCRenderMesh : public CPSCBase {
public:
    CPSCRenderMesh() : CopiedFrom(0) {}
    static CPSCBase* FABLE_TEST_FASTCALL Alloc();
    virtual CPSCBase* DatabaseClone() const;
    virtual void DatabaseCopy(const CPSCBase& other) { CopiedFrom = &other; }
    const CPSCBase* CopiedFrom;
};
static CPSCRenderMesh AllocatedMesh;
CPSCBase* FABLE_TEST_FASTCALL CPSCRenderMesh::Alloc() { return &AllocatedMesh; }
#ifndef _MSC_VER
CPSCBase* CPSCRenderMesh::DatabaseClone() const
{
    CPSCBase* clone = Alloc();
    clone->DatabaseCopy(*this);
    return clone;
}
#endif
}
int main()
{
    NParticleEngine::CPSCRenderMesh source;
    NParticleEngine::CPSCBase* clone = source.NParticleEngine::CPSCRenderMesh::DatabaseClone();
    if (clone != &NParticleEngine::AllocatedMesh || NParticleEngine::AllocatedMesh.CopiedFrom != &source) {
        std::printf("00b05ab0_TEST FAIL\n");
        return 1;
    }
    std::printf("00b05ab0_TEST PASS\n");
    return 0;
}
