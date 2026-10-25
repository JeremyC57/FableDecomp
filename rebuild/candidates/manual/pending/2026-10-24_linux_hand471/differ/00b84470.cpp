// CEngineSubPrimitiveRenderer::CEngineSubPrimitiveRenderer() -- construct the engine's sub-primitive renderer: the class root and engine-component bases, fifteen empty render lists (sorted + enabled), the empty sort buffer, the sort distance range, a cleared static sort scratch table, each list's render pass (and the stable-sort / bounding-volume options), and the global frame-pool pointer
// Grade: DIFFER(270v270) -- store scheduling only: VC7.1 (RTM and QFE 4035 alike) loads eax=2 once and emits the six RenderPass=2 stores (lists 0,1,11,12,2,3) as one group; retail keeps source order (0x12c/0x144 after list 10, 0x54/0x6c after the list-7/list-2 byte stores) and clears eax after the float stores
// Leaf: ??0CEngineSubPrimitiveRenderer@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B84470, 270 bytes (+ int3 padding), plain `ret` (no parameters, no EH frame).
// Label correction: catalogued "NHeroInformationScreens::CBase::CBase" (propagated); the two vtables stored are
//   CEngineSubPrimitiveRenderer's (rtti-vtables.tsv: 0x012A3A60 primary, 0x012A3A28 for the CEngineComponent base at +4),
//   and the body fills exactly Ego_r.pdb's CEngineSubPrimitiveRenderer members (CSubPrimitiveList[15], SortBuffer).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CEngineComponent::CEngineComponent @ 0x00B59710
//   (manifest label "CNavigationLayer" propagated; vptr, GlobalPointerAddress, RenderEnabled = true, KeepAlive = false).
// Data: the 16 KB static scratch table @ 0x014378C8 (cleared by the intrinsic memset, `rep stosd` x 0x1000), the engine
//   singleton @ 0x01436EA4 (its FrameMemoryPool at retail +0x130, PDB +0x138) and the pool pointer global @ 0x0143BAC4
//   (descriptive names).
// Layout (retail): CBaseClassNonCopyable +0, CEngineComponent +4 (0xC bytes), MaxSortDistance +0x10, SortDistanceMultiplier
//   +0x14, SubPrimitiveLists[15] +0x18 (0x18 each: the 12-byte STLport CArray Elements, RenderPass +0xC, PreRenderPass
//   +0x10, BoundingVolumePass / RequiresStableSort / Sorted / Enable bytes +0x14..+0x17), SortBuffer +0x180,
//   EnableSolidPass / EnableAlphaPass +0x18C. The generated engine/CEngineSubPrimitiveRenderer.h is the Ego_r layout
//   (16-byte CArrays, list stride 0x1C) and keeps the lists opaque, so the class is modelled locally.
// Shape: the list array is the inline vector-constructor loop (counter 15, the 0 / 1 stores from hoisted ecx / edx);
//   the distance range, the scratch clear and the per-list setup are ctor-body assignments in retail's store order
//   (lists 0, 1 (+ pre-pass), 4..14, then list 7's bounding-volume flag and the stable-sorted lists 2 and 3);
//   SortDistanceMultiplier is 2^30 / 10000 folded to its constant.
extern "C" void* __cdecl memset(void* dest, int value, unsigned int count);
#pragma intrinsic(memset)

class CBaseClassNonCopyable {
public:
    CBaseClassNonCopyable();                            // 0x0099A2F0 (vtable 0x0129A7C4)
    virtual ~CBaseClassNonCopyable();                   // 0x0099A300
};

class CEngineComponent {
public:
    CEngineComponent();                                 // 0x00B59710 (vtable 0x012A2544)
    virtual ~CEngineComponent();
    void**        GlobalPointerAddress;                 // +0x04
    bool          RenderEnabled;                        // +0x08
    unsigned char KeepAlive;                            // +0x09
};

enum ERenderPassType { RENDER_PASS_NONE = 0 };

template <class T> class CArray {                       // retail CArray: the 12-byte STLport vector
public:
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};

class CSimpleMemoryPool {                              // opaque here
    unsigned char _opaque[0x10];
};
class CEngine {
public:
    unsigned char     _pad000[0x130];
    CSimpleMemoryPool FrameMemoryPool;                  // retail +0x130 (Ego_r.pdb +0x138)
};
extern CEngine*           g_pEngine_01436ea4;           // 0x01436EA4
extern CSimpleMemoryPool* g_pSubPrimitiveFramePool;     // 0x0143BAC4 (descriptive)
extern unsigned long  g_SubPrimitiveSortScratch[0x1000];   // 0x014378C8 (descriptive, 16 KB)

class CEngineSubPrimitiveRenderer : public CBaseClassNonCopyable, public CEngineComponent {
public:
    struct CSortElement { void* SubPrimitive; float Key; };
    struct CSubPrimitiveList {
        CArray<CSortElement> Elements;                  // +0x00
        ERenderPassType      RenderPass;                // +0x0C
        ERenderPassType      PreRenderPass;             // +0x10
        bool                 BoundingVolumePass;        // +0x14
        bool                 RequiresStableSort;        // +0x15
        bool                 Sorted;                    // +0x16
        bool                 Enable;                    // +0x17
        CSubPrimitiveList()
            : RenderPass(RENDER_PASS_NONE), PreRenderPass(RENDER_PASS_NONE), BoundingVolumePass(false),
              RequiresStableSort(false), Sorted(true), Enable(true) {}
    };
    float                MaxSortDistance;               // +0x10
    float                SortDistanceMultiplier;        // +0x14
    CSubPrimitiveList    SubPrimitiveLists[15];         // +0x18
    CArray<CSortElement> SortBuffer;                    // +0x180
    bool                 EnableSolidPass;               // +0x18C
    bool                 EnableAlphaPass;               // +0x18D

    CEngineSubPrimitiveRenderer();
    virtual ~CEngineSubPrimitiveRenderer();
};

CEngineSubPrimitiveRenderer::CEngineSubPrimitiveRenderer()
{
    MaxSortDistance = 10000.0f;
    SortDistanceMultiplier = 107374.1824f;
    memset(g_SubPrimitiveSortScratch, 0, sizeof(g_SubPrimitiveSortScratch));
    SubPrimitiveLists[0].RenderPass = (ERenderPassType)2;
    SubPrimitiveLists[1].RenderPass = (ERenderPassType)2;
    SubPrimitiveLists[1].PreRenderPass = (ERenderPassType)1;
    SubPrimitiveLists[4].RenderPass = (ERenderPassType)5;
    SubPrimitiveLists[5].RenderPass = (ERenderPassType)6;
    SubPrimitiveLists[6].RenderPass = (ERenderPassType)7;
    SubPrimitiveLists[8].RenderPass = (ERenderPassType)8;
    SubPrimitiveLists[9].RenderPass = (ERenderPassType)9;
    SubPrimitiveLists[10].RenderPass = (ERenderPassType)0xC;
    SubPrimitiveLists[11].RenderPass = (ERenderPassType)2;
    SubPrimitiveLists[12].RenderPass = (ERenderPassType)2;
    SubPrimitiveLists[13].RenderPass = (ERenderPassType)0xA;
    SubPrimitiveLists[14].RenderPass = (ERenderPassType)0xD;
    SubPrimitiveLists[7].BoundingVolumePass = true;
    SubPrimitiveLists[2].RenderPass = (ERenderPassType)2;
    SubPrimitiveLists[2].RequiresStableSort = true;
    SubPrimitiveLists[3].RenderPass = (ERenderPassType)2;
    SubPrimitiveLists[3].RequiresStableSort = true;
    g_pSubPrimitiveFramePool = &g_pEngine_01436ea4->FrameMemoryPool;
}
