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
#ifndef FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CVERTEXSHADER
#endif
#include "engine/C3DVector.h"
#include "engine/CArray.h"
#include "engine/CVertexShader.h"
#endif
class CEngineInit;
class CEngineOcclusionManager : public CEngineComponent {
public:
    CEngineOcclusionManager();                 // 0x00B8D780 ??0CEngineOcclusionManager@@QAE@XZ
    virtual unsigned long PeekSceneFilterFlags() const;
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
    struct COccluderEntry { unsigned long PixelCount; unsigned long SlotID : 30; unsigned long Completed : 1; unsigned long Testing : 1; };
    struct COccluderFlareEntry {
        C3DVector     Position;                 // +0x00
        float         Radius;                   // +0x0c
        unsigned long CircleSegments;           // +0x10
        unsigned long MaxPixelCount;            // +0x14
        unsigned long OccluderHandle;           // +0x18
    };
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
    unsigned char   HandleMapper[0xc];                               // +0x2403c vector<pair<unsigned long,unsigned long> >
    unsigned char   PrevFrameHandleMapper[0xc];                      // +0x24048 vector<pair<unsigned long,unsigned long> >
    unsigned char   FlareVertexBuffers[0xc];                         // +0x24054 vector<pair<unsigned long,CCountedPointer<CVertexBufferWin32> > >
    CVertexShader   VertexShaderFlareTest;                           // +0x24060
    unsigned char   UnitBoxVB[0x34];                                 // +0x24068 CVertexBufferWin32
    unsigned char   UnitBoxIB[0x14];                                 // +0x2409c CIndexBuffer
    unsigned char   OcclusionBoxVShader[0x8];                        // +0x240b0 CVertexShader
    unsigned char   OcclusionBoxPixelShader[0x8];                    // +0x240b8 CPixelShader
#else
private:
    unsigned char _pad_0xc[0x240b4];            // +0xc members not modelled yet (Ego_r.pdb names; retail STLport sizes)
#endif
};
FABLE_STATIC_ASSERT(sizeof(CEngineOcclusionManager) == 0x240c0);
#else // generated flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineOcclusionManager = 0x240d4.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CEngineOcclusionManager = 0x240c0.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

struct COccluderEntry;

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
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEngineOcclusionManager) == 0x240c0);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, OccluderSlots) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, NextSlot) == 0x400c);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, CurrentFrame) == 0x4010);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, Entries) == 0x4014);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, PrevFrameEntries) == 0x4018);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, EntriesSize) == 0x401c);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, PrevFrameEntriesSize) == 0x4020);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, EntriesBuffer1) == 0x4024);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, EntriesBuffer2) == 0x14024);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, FlareTests) == 0x24024);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, PrevFrameFlareTests) == 0x24030);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, HandleMapper) == 0x2403c);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, PrevFrameHandleMapper) == 0x24048);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, FlareVertexBuffers) == 0x24054);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, VertexShaderFlareTest) == 0x24060);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, UnitBoxVB) == 0x24068);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, UnitBoxIB) == 0x2409c);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, OcclusionBoxVShader) == 0x240b0);
FABLE_STATIC_ASSERT(offsetof(CEngineOcclusionManager, OcclusionBoxPixelShader) == 0x240b8);

#endif // FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#endif // FABLE_ENGINE_CENGINEOCCLUSIONMANAGER_H
