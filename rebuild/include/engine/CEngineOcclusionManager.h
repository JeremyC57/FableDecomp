#ifndef FABLE_ENGINE_CENGINEOCCLUSIONMANAGER_H
#define FABLE_ENGINE_CENGINEOCCLUSIONMANAGER_H
#ifdef FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
// Retail branch (chunk 731, 2026-11-03): RTTI .?AVCEngineOcclusionManager@@ (vtable 0x012A3CD0) : CEngineComponent (+0x0); sizeof 0x240C0 from
// CEngine::Initialise 0x00B26360 (`new` of 0x240C0 bytes before the constructor call). Constructor ??0CEngineOcclusionManager@@QAE@XZ 0x00B8D780 and
// the PeekSceneFilterFlags override (?PeekSceneFilterFlags@CEngineOcclusionManager@@UBEKXZ): Ego_r.pdb / FableWin.pdb. Own members are
// not modelled yet (one opaque block; the generated Ego_r model below lists their names).
#ifndef FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECOMPONENT
#endif
#include "rebuild_abi.h"
#include "engine/CEngineComponent.h"
#ifdef FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_MEMBERS
// needs FABLE_RETAIL_CVERTEXSHADER
#include "engine/C3DVector.h"
#include "engine/CArray.h"
#include "engine/CVertexShader.h"
#ifdef FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_PASS22_FLAREBUFFER
#include "engine/CCountedPointer.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CIndexBuffer.h"
#include "engine/CPixelShader.h"
#endif
#endif
class CEngineInit;
#ifdef FABLE_RETAIL_WATER_OCCLUSION_ORIGINAL_CHAIN_PASS22
class CMatrix3x4;
#endif
class CEngineOcclusionManager : public CEngineComponent {
public:
    CEngineOcclusionManager();                 // 0x00B8D780 ??0CEngineOcclusionManager@@QAE@XZ
    virtual unsigned long PeekSceneFilterFlags() const;
#if defined(FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_OCCLUSIONTEST) || defined(FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_TESTS)
    // Opt-in (chunk 2060, row 0x00BA5260): Ego_r.pdb ?StartOcclusionTest@CEngineOcclusionManager@@QAEXXZ (Ego_r 0x00A347A0, 1 byte `ret`)
    // and ?EndOcclusionTest@CEngineOcclusionManager@@QAEXK@Z (0x00A347B0, 3 bytes `ret 4`), FableWin.pdb the same names: the empty PC
    // bodies at retail 0x00B8D090 / 0x00B8D0A0 (landed as BeginQuery / EndQuery: NAME_FIXES.tsv).
    // FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_TESTS (wfA5, row 0x00B9AC30) declares the same two members.
    void StartOcclusionTest();                                           // 0x00B8D090
    void EndOcclusionTest(unsigned long handle);                         // 0x00B8D0A0
#endif
#ifdef FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_FLARE
    // Opt-in (chunk 2048, RenderSunFlare 0x00B64550): FableWin.pdb ?GetPrevFrameFlareTestResultFromRetrivalID@CEngineOcclusionManager@@QAEMK@Z
    // (landed bound address 0x00B8D3C0, reloc_bindings).
    float GetPrevFrameFlareTestResultFromRetrivalID(unsigned long id);   // 0x00B8D3C0
#endif
#ifdef FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_MEMBERS
    // Retail opt-in (chunk 2028, row 0x00B8DF50 FrameEnd): the own members with their Ego_r.pdb names and types at the retail
    // offsets (Ego_r - 4 per 12-byte STLport CArray / vector: FlareTests +0x24024, VertexShaderFlareTest +0x24060 read by
    // 0x00B8DF81 / 0x00B8DFB6; landed GetFlareTestResult 0x00B8D280 / FrameStart 0x00B8DBE0 use the same offsets), the nested
    // records from fable_types.h (COccluderFlareEntry 0x1C: Position, Radius, CircleSegments +0x10, MaxPixelCount +0x14,
    // OccluderHandle +0x18; COccluderSlot 8; COccluderEntry 8, U-keyed in pdb_class_keys.tsv), and the virtual FrameStart / FrameEnd
    // overrides plus the private SelectFlareTestVertexBuffer (Ego_r.pdb / FableWin.pdb
    // ?FrameEnd@CEngineOcclusionManager@@UAEXABVCEngineComponentParams@@@Z, ?SelectFlareTestVertexBuffer@CEngineOcclusionManager@@AAEXK@Z).
    // Containers whose element types no row reads yet stay opaque (their real type in the comment). Same size.
    struct COccluderSlot { long EntryIndex; unsigned long Frame; };
    struct COccluderEntry {
#ifdef FABLE_RETAIL_WATER_OCCLUSION_ORIGINAL_CHAIN_PASS22
        // Both original types and donor02CDD6B0: zero every original field.
        COccluderEntry():PixelCount(0),SlotID(0),Completed(0),Testing(0) {}
#endif
        unsigned long PixelCount; unsigned long SlotID : 30; unsigned long Completed : 1; unsigned long Testing : 1;
    };
    struct COccluderFlareEntry {
        C3DVector     Position;                 // +0x00
        float         Radius;                   // +0x0c
        unsigned long CircleSegments;           // +0x10
        unsigned long MaxPixelCount;            // +0x14
        unsigned long OccluderHandle;           // +0x18
    };
#ifdef FABLE_RETAIL_WATER_OCCLUSION_ORIGINAL_CHAIN_PASS22
    // Both PDB exact PUBLIC names plus donor02D4E9D0/native00B6E260 call chain.
    unsigned long GetResult(unsigned long handle,bool prev_frame);
    unsigned long GetPrevFrameResult(unsigned long handle) { return GetResult(handle,true); }
    void StartOcclusionTest();
    void EndOcclusionTest(unsigned long handle);
    unsigned long RenderOcclusionBox(const CMatrix3x4& matrix);
    unsigned long AllocateOccluderHandle() {
        if (EntriesSize<8192) {
            unsigned long handle=EntriesSize;
            Entries[handle]=COccluderEntry();
            ++EntriesSize;
            return handle;
        }
        return (unsigned long)-1;
    }
#endif
    virtual void FrameStart(const CEngineComponentParams& params);   // 0x00B8DBE0
    virtual void FrameEnd(const CEngineComponentParams& params);     // 0x00B8DF50
private:
    void SelectFlareTestVertexBuffer(unsigned long segments);        // 0x00B8DCD0
    COccluderSlot   OccluderSlots[2048];                             // +0x0c
    unsigned long   NextSlot;                                        // +0x400c
    unsigned long   CurrentFrame;                                    // +0x4010
    COccluderEntry* Entries;                                         // +0x4014
    COccluderEntry* PrevFrameEntries;                                // +0x4018
    unsigned long   EntriesSize;                                     // +0x401c
    unsigned long   PrevFrameEntriesSize;                            // +0x4020
    COccluderEntry  EntriesBuffer1[8192];                            // +0x4024
    COccluderEntry  EntriesBuffer2[8192];                            // +0x14024
    CArray<COccluderFlareEntry> FlareTests;                          // +0x24024
    CArray<COccluderFlareEntry> PrevFrameFlareTests;                 // +0x24030
#ifdef FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_PASS22_FLAREBUFFER
    // Both complete PDB classes; native constructor/destructor and00B8DCD0 strides.
    std::vector<std::pair<unsigned long,unsigned long> > HandleMapper;
    std::vector<std::pair<unsigned long,unsigned long> > PrevFrameHandleMapper;
    std::vector<std::pair<unsigned long,CCountedPointer<CVertexBufferWin32> > > FlareVertexBuffers;
    CVertexShader VertexShaderFlareTest;
    CVertexBufferWin32 UnitBoxVB;
    CIndexBuffer UnitBoxIB;
    CVertexShader OcclusionBoxVShader;
    CPixelShader OcclusionBoxPixelShader;
#else
    unsigned char   HandleMapper[0xc];                               // +0x2403c vector<pair<unsigned long,unsigned long> >
    unsigned char   PrevFrameHandleMapper[0xc];                      // +0x24048 vector<pair<unsigned long,unsigned long> >
    unsigned char   FlareVertexBuffers[0xc];                         // +0x24054 vector<pair<unsigned long,CCountedPointer<CVertexBufferWin32> > >
    CVertexShader   VertexShaderFlareTest;                           // +0x24060
    unsigned char   UnitBoxVB[0x34];                                 // +0x24068 CVertexBufferWin32
    unsigned char   UnitBoxIB[0x14];                                 // +0x2409c CIndexBuffer
    unsigned char   OcclusionBoxVShader[0x8];                        // +0x240b0 CVertexShader
    unsigned char   OcclusionBoxPixelShader[0x8];                    // +0x240b8 CPixelShader
#endif
#else
private:
    unsigned char _pad_0xc[0x240b4];            // +0xc members not modelled yet (Ego_r.pdb names; retail STLport sizes)
#endif
};
FABLE_STATIC_ASSERT_AT(75, sizeof(CEngineOcclusionManager) == 0x240c0);
#else // generated flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineOcclusionManager = 0x240d4.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CEngineOcclusionManager = 0x240c0.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

#ifdef FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_ALLOCATE
// Opt-in (bwf1 / agentC2, row 0x00BB3E80): Ego_r.pdb COccluderEntry (8 bytes: PixelCount, SlotID:30, Testing:1, Completed:1).
struct COccluderEntry {
    unsigned long PixelCount;
    unsigned long SlotID : 30;
    unsigned long Testing : 1;
    unsigned long Completed : 1;
};
#else
struct COccluderEntry;
#endif

#pragma pack(push, 1)
class CEngineOcclusionManager {
public:
    void*           __vftable;                               // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char   _pad_0x04[0x4];                          // +0x04
    unsigned char   _pad_0x08[0x4];                          // +0x08
    unsigned char   OccluderSlots[0x4000];                   // +0x0c COccluderSlot[2048]
    unsigned long   NextSlot;                                // +0x400c
    unsigned long   CurrentFrame;                            // +0x4010
    COccluderEntry* Entries;                                 // +0x4014
    COccluderEntry* PrevFrameEntries;                        // +0x4018
    unsigned long   EntriesSize;                             // +0x401c
    unsigned long   PrevFrameEntriesSize;                    // +0x4020
    unsigned char   EntriesBuffer1[0x10000];                 // +0x4024 COccluderEntry[8192]
    unsigned char   EntriesBuffer2[0x10000];                 // +0x14024 COccluderEntry[8192]
    unsigned char   FlareTests[0xc];                         // +0x24024 CArray<CEngineOcclusionManager::COccluderFlareEntry>
    unsigned char   PrevFrameFlareTests[0xc];                // +0x24030 CArray<CEngineOcclusionManager::COccluderFlareEntry>
    unsigned char   HandleMapper[0xc];                       // +0x2403c vector<std::pair<unsigned_long,unsigned_long>,std::allocator<std::pair<unsigned_long,unsigned_long>_>_>
    unsigned char   PrevFrameHandleMapper[0xc];              // +0x24048 vector<std::pair<unsigned_long,unsigned_long>,std::allocator<std::pair<unsigned_long,unsigned_long>_>_>
    unsigned char   FlareVertexBuffers[0xc];                 // +0x24054 vector<std::pair<unsigned_long,CCountedPointer<CVertexBufferWin32>_>,std::allocator<std::pair<unsigned_long,CCountedPointer<CVertexBufferWin32>_>_>_>
    unsigned char   VertexShaderFlareTest[0x8];              // +0x24060 CVertexShader
    unsigned char   UnitBoxVB[0x34];                         // +0x24068 CVertexBufferWin32
    unsigned char   UnitBoxIB[0x14];                         // +0x2409c CIndexBuffer
    unsigned char   OcclusionBoxVShader[0x8];                // +0x240b0 CVertexShader
    unsigned char   OcclusionBoxPixelShader[0x8];            // +0x240b8 CPixelShader
#ifdef FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_GETRESULT
    // Opt-in (NF44 round 6, row 0x00B2F8E0 CEnginePrimitiveOcclusionManager::CheckVisibilityResult): Ego_r.pdb
    // ?GetResult@CEngineOcclusionManager@@QAEKK_N@Z (retail 0x00B8D0B0, `xor eax,eax ; ret 8`). Declaration only.
    unsigned long GetResult(unsigned long handle, bool wait);            // 0x00B8D0B0
#endif
#ifdef FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_ADDFLARETEST
    // NF45 opt-in (row 0x00B662F0 CEngineSkyRenderer::Render): Ego_r.pdb ?AddFlareTest@CEngineOcclusionManager@@QAEXKABVC3DVector@@MJ@Z
    // (Ego_r 0x00A34750) = retail 0x00B8D040, the empty PC stub `ret 10h`: the same Ego_r -> retail delta 0x1588F0 as GetResult 0x00B8D0B0
    // (Ego_r 0x00A347C0) and Start/EndOcclusionTest 0x00B8D090 / 0x00B8D0A0, and the four-argument void overload (the other overload
    // 0x00A34760 returns K). Declaration only.
    void AddFlareTest(unsigned long, const class C3DVector&, float, long);   // 0x00B8D040
#endif
#ifdef FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_RENDEROCCLUSIONBOX
    // NF45 opt-in (row 0x00BF7100 CLandscapeBackgroundTreeNode::RenderOcclusionBox): Ego_r.pdb
    // ?RenderOcclusionBox@CEngineOcclusionManager@@QAEKABVCMatrix3x4@@@Z (Ego_r 0x00A34B30 + 0x1588F0) = retail 0x00B8D420, the landed self
    // row's leaf. Declaration only.
    unsigned long RenderOcclusionBox(const class CMatrix3x4& transform);   // 0x00B8D420
#endif
#ifdef FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_ALLOCATE
    // Opt-in (bwf1 / agentC2, row 0x00BB3E80 CEngineClothRenderer::RenderClothPrimitive): the FableWin.pdb inline
    // ?AllocateOccluderHandle@CEngineOcclusionManager@@QAEKXZ (expanded at 0x00BB3EFD..0x00BB3F34; text of landed 0x00B91140) and the
    // Ego_r.pdb out-of-line ?StartOcclusionTest@CEngineOcclusionManager@@QAEXXZ 0x00B8D090 / ?EndOcclusionTest@CEngineOcclusionManager@@QAEXK@Z 0x00B8D0A0.
    FABLE_FORCEINLINE unsigned long AllocateOccluderHandle()
    {
        if (EntriesSize < 0x2000) {
            unsigned long index = EntriesSize;
            COccluderEntry empty;
            empty.PixelCount = 0;
            *(unsigned long*)((char*)&empty + 4) = 0;
            Entries[index] = empty;
            ++EntriesSize;
            return index;
        }
        return 0xffffffff;
    }
    void StartOcclusionTest();                                           // 0x00B8D090
    void EndOcclusionTest(unsigned long handle);                         // 0x00B8D0A0
#endif
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(116, sizeof(CEngineOcclusionManager) == 0x240c0);
FABLE_STATIC_ASSERT_AT(117, offsetof(CEngineOcclusionManager, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(118, offsetof(CEngineOcclusionManager, OccluderSlots) == 0xc);
FABLE_STATIC_ASSERT_AT(119, offsetof(CEngineOcclusionManager, NextSlot) == 0x400c);
FABLE_STATIC_ASSERT_AT(120, offsetof(CEngineOcclusionManager, CurrentFrame) == 0x4010);
FABLE_STATIC_ASSERT_AT(121, offsetof(CEngineOcclusionManager, Entries) == 0x4014);
FABLE_STATIC_ASSERT_AT(122, offsetof(CEngineOcclusionManager, PrevFrameEntries) == 0x4018);
FABLE_STATIC_ASSERT_AT(123, offsetof(CEngineOcclusionManager, EntriesSize) == 0x401c);
FABLE_STATIC_ASSERT_AT(124, offsetof(CEngineOcclusionManager, PrevFrameEntriesSize) == 0x4020);
FABLE_STATIC_ASSERT_AT(125, offsetof(CEngineOcclusionManager, EntriesBuffer1) == 0x4024);
FABLE_STATIC_ASSERT_AT(126, offsetof(CEngineOcclusionManager, EntriesBuffer2) == 0x14024);
FABLE_STATIC_ASSERT_AT(127, offsetof(CEngineOcclusionManager, FlareTests) == 0x24024);
FABLE_STATIC_ASSERT_AT(128, offsetof(CEngineOcclusionManager, PrevFrameFlareTests) == 0x24030);
FABLE_STATIC_ASSERT_AT(129, offsetof(CEngineOcclusionManager, HandleMapper) == 0x2403c);
FABLE_STATIC_ASSERT_AT(130, offsetof(CEngineOcclusionManager, PrevFrameHandleMapper) == 0x24048);
FABLE_STATIC_ASSERT_AT(131, offsetof(CEngineOcclusionManager, FlareVertexBuffers) == 0x24054);
FABLE_STATIC_ASSERT_AT(132, offsetof(CEngineOcclusionManager, VertexShaderFlareTest) == 0x24060);
FABLE_STATIC_ASSERT_AT(133, offsetof(CEngineOcclusionManager, UnitBoxVB) == 0x24068);
FABLE_STATIC_ASSERT_AT(134, offsetof(CEngineOcclusionManager, UnitBoxIB) == 0x2409c);
FABLE_STATIC_ASSERT_AT(135, offsetof(CEngineOcclusionManager, OcclusionBoxVShader) == 0x240b0);
FABLE_STATIC_ASSERT_AT(136, offsetof(CEngineOcclusionManager, OcclusionBoxPixelShader) == 0x240b8);

#endif // FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#endif // FABLE_ENGINE_CENGINEOCCLUSIONMANAGER_H
