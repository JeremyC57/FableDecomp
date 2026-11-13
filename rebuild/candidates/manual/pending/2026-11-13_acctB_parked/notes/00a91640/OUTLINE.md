# OUTLINE 0x00A91640 C3DPrimitive2::InitialiseFromMeshFileData
Offsets are B+X (B = esp after the prologue; see $W/dis_norm.txt). Args: chunk B+0xB4, b B+0xB8, reps B+0xBC,
initFlags B+0xC0, memoryHandle B+0xC4. No EH, no cookie, no frame pointer, `ret 14h`.

## Frame map (0xA0 bytes of locals, B+0x10..B+0xAF)
| slot | local (type) | ctor / dtor sites |
|---|---|---|
| B+0x10/0x14 | (a) `verts` CCountedPointer<const C3DMeshFileVerticesChunk> (else-branch scope); (b) result temp of every FindNext*() call | dtor 0xA91983; temp dtors 0xA91A29, 0xA91DAD, 0xA91FF6 (TRIS temp), 0xA923D7, 0xA924E3 |
| B+0x1B | STLport `__false_type` tag byte (destroy_aux arg) | |
| B+0x1C/0x20 | result temp of FindFirstSubChunkOfType for XVBD/XAPR/XUVT/XAPB/XBDS/XMIF; B+0x1C..0x24 also C3DVector temp (GetVTangent(i), GetCenter()); B+0x20 also Info of `newGroup` and of `xapbTris` | |
| B+0x28/0x2C | conversion temp CCountedPointer<const CChunkedFileChunk>(xapb) for FindNext arg; `mteh` local (MTEH if-scope); `1` for max() in push_back overflow; loop induction (-0x14-list) | |
| B+0x30/0x34 | `tris` CCountedPointer<const C3DMeshFileTrianglesChunk> (function scope, 2nd declared) | dtor 0xA9266A |
| B+0x38/0x3C | vertexSize (ulong) / vertex loop counter / new capacity in overflow / FindNext temp in XAPB loop | |
| B+0x40 | float estimate (fast inv sqrt) / `C3DGroup2*` new result (newGroup.Data) / XAPB byte offset i*0x30 | |
| B+0x44 | xmif.Info (Data in esi) | dtor 0xA925C6 |
| B+0x48/0x4C | `group` CCountedPointer<const C3DMeshFileGroupChunk> (function scope) | dtor 0xA92622 |
| B+0x50/0x54 | `cloth` CCountedPointer<const C3DMeshFileClothChunk> (function scope, last declared) | dtor 0xA925A1 |
| B+0x58 | this spill | |
| B+0x5C | `bool hasVertexBuffer` (byte, =0 at 0xA916AF, =1 at 0xA917AF); later newCloth Data; MTEH conv temp B+0x5C/0x60 | |
| B+0x64/0x68 | `xapr` CCountedPointer<const C3DMeshFileXAnimatedPrimitiveChunk>; B+0x64 also float sum in normalise | dtor 0xA92647 |
| B+0x6C..0x74 | C3DVector temp GetUTangent(i) | |
| B+0x70 | xbds.Info (Data esi) | dtor 0xA925E9 |
| B+0x78..0x97 | C3DVertex2 (resize fill temp, uninitialised) AND the per-vertex copy `v` | |
| B+0x9C | vbd.Info (Data in ebp) -- `vbd` CCountedPointer<const C3DMeshFileXVertexDataChunk>, first declared | dtor 0xA9268F / 0xA91740 / 0xA92714 / 0xA9272? |
| B+0xA0 | `long handle` (AllocateResource out) | |
| B+0xA4 | C3DVector() temp for resize(n) (uninitialised, overlaps) | |
| B+0xA8 | xuvt.Info (Data esi) | dtor 0xA92604 |
Declaration order (from reverse dtor order at 0xA925A1..0xA926B2): vbd, tris, xapr, group, xuvt, xbds, xmif, cloth.

## Body in retail order (pseudo-C++)
```cpp
bool C3DPrimitive2::InitialiseFromMeshFileData(const C3DMeshFilePrimitiveChunk& chunk, bool b, unsigned long reps,
                                               unsigned long initFlags, CScopedMemoryHandle& memoryHandle)
{
    FreeData();                                                     // 0xA91650 call 0xA901E0
    Data.reset(new N3DPrimitive::CDeviceIndependent);               // 0xA91655 new(0x50) -> 0xA98100; store, then old->slot0(1)
    InitFlags = initFlags;                                          // 0xA91693 (+0x68)
    RepeatingMeshReps = reps;                                       // 0xA91696 (+0x08)
    MaterialIndex = chunk.GetMaterialIndex(0);                      // 0xA916A7 MaterialIndices[0] (+0x04)
    bool hasVertexBuffer = false;                                   // B+0x5C byte
    CCountedPointer<const C3DMeshFileXVertexDataChunk> vbd;         // edi/ebp zeroed 0xA916AA/B4, Info spilled B+0x9C
    vbd.SetStaticCast(chunk.FindFirstSubChunkOfType(C3DMeshFileXVertexDataChunk::GetStaticChunkType()));  // 0xA916BD "XVBD"; temp B+0x1C released 0xA916DB
    if (vbd.Get()) {                                                // 0xA91701 test ebp
        VertexCount = vbd->GetVertexCount();                        // 0xA9170C (+0x58) <- +0x14
        unsigned long vertexSize = vbd->GetVertexSize();            // 0xA91715 -> B+0x38 (must be a local: read once before the calls)
        InitFlags = vbd->GetInitFlags();                            // 0xA91722 (+0x68) <- +0x1C (ecx also = CalculateVertexSize arg1)
        if (!SetupVertexBuffer((EVertexType)0,                      // 0xA91737 call 0xA8EDA0 (push InitFlags, VertexCount, size, 0)
                CalculateVertexSize(InitFlags, b, false),           // 0xA91725 call 0xA910E0 (ecx flags, dl b, push 0)
                VertexCount, InitFlags))
            return false;                                           // 0xA91740: release vbd (edi) -> 0xA92721 delete -> fail epilogue 0xA92729
        VertexBuffer->CompressionParams = vbd->VBCParams;           // 0xA9175C rep movsd x8 (+0x30 -> +0x14) <- vbd+0x20
        const void* src = vbd->PeekVertexData();                    // 0xA9176C begin, or &VertexData when empty
        void* dst = VertexBuffer->Lock(0, 0, 0);                    // 0xA91786 call 0xAC20A0
        memcpy(dst, src, VertexCount * vertexSize);                 // 0xA9178B intrinsic rep movsd/movsb
        VertexBuffer->Unlock();                                     // 0xA917A6 call 0xAC20B0
        VertexBufferStatus |= 1;                                    // 0xA917AB (+0x38)
        hasVertexBuffer = true;                                     // 0xA917AF
    } else {                                                        // 0xA917B9
        CCountedPointer<const C3DMeshFileVerticesChunk> verts = chunk.PeekVertices();   // 0xA917C0 -> B+0x10 (hidden result)
        const C3DVertex2* list = verts->GetList();                  // 0xA917C9 call 0xABC420 -> esi
        unsigned long count = verts->GetCount();                    // 0xA917D4 call 0xABA360 -> ebp
        Data->Vertices.resize(count);                               // 0xA917D9 inline: count<size ? erase(0xA96A90) : _M_fill_insert(0xA97040, temp B+0x78)
        for (unsigned long i = 0; i < count; ++i) {                 // 0xA9181B / loop head 0xA91835, down-counter B+0x38
            C3DVertex2 v = list[i];                                 // B+0x78..0x94
            if (v.Normal.IsNonZero())                               // 0xA918A8 fcompp vs DELTA_NOTIONAL_ZERO^2, jp 0xA91954 (3 pops)
                v.Normal.Normalise();                               // 0xA918C7..0xA9194B (NORMALISE_UNPAREN shape)
            Data->Vertices[i] = v;                                  // 0xA9195A Data reloaded, rep movsd x8
        }
        VertexCount = count;                                        // 0xA91989 (+0x58)
    }                                                               // verts dtor 0xA91983..0xA919A9
    CCountedPointer<const C3DMeshFileTrianglesChunk> tris = chunk.FindFirstTriangles();   // 0xA919BA -> B+0x30 direct
    TriangleCount = 0;                                              // 0xA919C5 (+0x5C) (stored after the call)
    while (tris.Get()) {                                            // 0xA919CC / 0xA91A5F
        Data->AddTriangleList(*tris);                               // 0xA919D6 call 0xA90900
        tris = chunk.FindNextTriangles(tris);                       // 0xA919E7 call 0xABB390 (temp B+0x10), ShareData, temp dtor
    }
    AnimatedBlockCount = 0;                                         // 0xA91A75 (+0x48)
    CCountedPointer<const C3DMeshFileXAnimatedPrimitiveChunk> xapr; // esi/edi zeroed, also B+0x64/0x68
    xapr.SetStaticCast(chunk.FindFirstSubChunkOfType(C3DMeshFileXAnimatedPrimitiveChunk::GetStaticChunkType()));   // 0xA91A80 "XAPR"
    if (xapr.Get())
        AnimatedBlockCount = xapr->CountSubChunksOfType(C3DMeshFileXAnimatedPrimitiveBlockChunk::GetStaticChunkType());  // 0xA91AD0 "XAPB"
    StaticBlockCount = Data->TriangleLists.size();                  // 0xA91AEF (+0x40)
    unsigned long blockBytes = AnimatedBlockCount * sizeof(N3DPrimitive::CAnimatedBlock)   // 0x30
                             + StaticBlockCount * sizeof(N3DPrimitive::CStaticBlock);      // 0x20  (ecx = anim*0x30 + static*0x20)
    long handle;                                                    // B+0xA0
    if (!IsValidSize((__int64)blockBytes))                          // 0xA91AFA..0xA91B1C -> cold 0xA926C4 (release xapr, tris, vbd; fail)
        return false;
    if (!GMeshDataMemoryPool->AllocateResource(&handle, this, blockBytes, 0))   // 0xA91B22 [0x013D2D6C], call 0xA62E40
        return false;                                               // 0xA91B3F: release xapr(edi), tris(B+0x34), -> 0xA92714 vbd
    // (try both: one forceinline AllocateChecked wrapper vs two ifs -- see CContainmentVolume_Initialise_00a755c0)
    memoryHandle.Initialise(GMeshDataMemoryPool, handle);           // 0xA91BA5 call 0xA039F0 (pool reloaded into esi before)
    StaticBlocks = (N3DPrimitive::CStaticBlock*)memoryHandle.GetPointer();   // 0xA91BB0 call 0xA026B0 (ecx Pool, push Handle)
    GFExplicitNew(StaticBlocks, StaticBlockCount);                  // 0xA91BBF call 0xA94150 (fastcall)
    if (AnimatedBlockCount) {                                       // 0xA91BC7
        AnimatedBlocks = (N3DPrimitive::CAnimatedBlock*)(StaticBlocks + StaticBlockCount);   // 0xA91BD5
        GFExplicitNew(AnimatedBlocks, AnimatedBlockCount);          // 0xA91BD8 call 0xA96270
    }
    for (unsigned long i = 0; i < StaticBlockCount; ++i)            // 0xA91BDD; loop 0xA91BF0 (StaticBlocks & count reloaded)
        StaticBlocks[i].MaterialIndex = chunk.GetMaterialIndex(i);  // +0x1C <- chunk.MaterialIndices[i]
    CCountedPointer<const C3DMeshFileGroupChunk> group = chunk.FindFirstGroup();    // 0xA91C0F -> B+0x48 direct
    while (group.Get()) {                                           // 0xA91C18 / 0xA91DF8
        CCountedPointer<C3DGroup2> newGroup(new C3DGroup2(*group)); // 0xA91C20 new(0x10), 0xA91C35 ctor 0xAC4150; Info new(0xC){1,0xA982D0,data} -> ebp; Data B+0x40
        Data->Groups.push_back(newGroup);                           // 0xA91C72 inline; overflow 0xA91CA3 (malloc 0xBFEA0E, copy loop, construct x,
                                                                    //   __destroy_aux 0xA969D0, free 0xBFEA14, _M_set)
        group = chunk.FindNextGroup(group);                         // 0xA91D6B call 0xABB9B0 (temp B+0x10)
    }                                                               // newGroup dtor 0xA91DD4 (ebp) -- AFTER the FindNext assignment
    CCountedPointer<const C3DMeshFileXUVTangentChunk> xuvt;         // esi / B+0xA8
    xuvt.SetStaticCast(chunk.FindFirstSubChunkOfType(C3DMeshFileXUVTangentChunk::GetStaticChunkType()));  // 0xA91E18 "XUVT"
    if (xuvt.Get()) {                                               // 0xA91E5A
        unsigned long count = xuvt->GetCount();                     // 0xA91E62 (end-begin)/12 via imul 2AAAAAABh
        Data->UTangents.resize(count);                              // 0xA91E89 call 0x75C040 (n, &temp B+0xA4)
        Data->VTangents.resize(count);                              // 0xA91E9D call 0x75C040
        for (unsigned long i = 0; i < count; ++i) {                 // 0xA91EA8, down-counter edi, byte index ecx += 0xC
            C3DVector u = xuvt->GetUTangent(i);                     // B+0x6C
            C3DVector v = xuvt->GetVTangent(i);                     // B+0x1C
            Data->UTangents[i] = u;                                 // 0xA91ED7 (Data reloaded)
            Data->VTangents[i] = v;                                 // 0xA91EF7
        }
    }
    if (xapr.Get()) {                                               // 0xA91F19 test B+0x64
        Data->TriangleLists.clear();                                // 0xA91F25: __copy 0xA964C0 (end,end,begin), __destroy_aux 0xA96990, _M_finish=
        CCountedPointer<const C3DMeshFileXAnimatedPrimitiveBlockChunk> xapb;   // esi/ebp registers
        xapb.SetStaticCast(xapr->FindFirstSubChunkOfType(C3DMeshFileXAnimatedPrimitiveBlockChunk::GetStaticChunkType()));  // 0xA91F66 "XAPB" (ecx=xapr.Data)
        for (unsigned long i = 0; xapb.Get(); ++i) {                // 0xA91FA3; i as byte offset B+0x40 (+0x30 at 0xA920DB)
            GetAnimatedBlock(i)->InitialiseFromMeshFileData(*xapb, hasVertexBuffer);   // 0xA91FC6 call 0xA90AE0 (AnimatedBlocks reloaded)
            CCountedPointer<const C3DMeshFileTrianglesChunk> blockTris;                  // edi / B+0x20
            blockTris.SetStaticCast(xapb->FindFirstSubChunkOfType(C3DMeshFileTrianglesChunk::GetStaticChunkType()));  // 0xA91FDD "TRIS"
            Data->AddTriangleList(*blockTris);                      // 0xA9202A call 0xA90900
            xapb = xapr->FindNextSubChunkOfType(xapb);              // 0xA9204C call 0xAA7FF0: arg conv temp B+0x28, result temp B+0x38
        }                                                           // blockTris dtor 0xA920EC
    }                                                               // xapb dtor 0xA9210B
    CCountedPointer<const C3DMeshFileXBoundingSphereChunk> xbds;    // esi / B+0x70
    xbds.SetStaticCast(chunk.FindFirstSubChunkOfType(C3DMeshFileXBoundingSphereChunk::GetStaticChunkType()));   // 0xA9213E "XBDS"
    if (xbds.Get()) {                                               // 0xA9217C
        BoundingSphere.Center = xbds->GetCenter();                  // by value: temp B+0x1C, then copied to +0x0C
        BoundingSphere.Radius = xbds->GetRadius();                  // 0xA921B0 (+0x18)
    }
    AverageTextureStretch = 0.0f;                                   // 0xA921C2 (+0x1C, int 0 from ebp)
    CCountedPointer<const C3DMeshFileXMappingInfoChunk> xmif;       // esi / B+0x44
    xmif.SetStaticCast(chunk.FindFirstSubChunkOfType(C3DMeshFileXMappingInfoChunk::GetStaticChunkType()));      // 0xA921CB "XMIF"
    if (xmif.Get())
        AverageTextureStretch = xmif->GetAverageTextureStretch();   // 0xA9220D
    TriangleCount = Data->GetTotalTriangleCount();                  // 0xA92218 call 0xA8ECE0 (Data in esi once)
    IndexCount = Data->GetTotalIndexCount();                        // 0xA92222 call 0xA8ED30
    ClothPrimitives.reserve(chunk.CountSubChunksOfType(C3DMeshFileClothChunk::GetStaticChunkType()));  // 0xA92234 "CLTH" / 0xA9223C call 0xA975E0
    CCountedPointer<const C3DMeshFileClothChunk> cloth = chunk.FindFirstCloth();    // 0xA92248 -> B+0x50 direct
    while (cloth.Get()) {                                           // 0xA92253 / 0xA92421
        CCountedPointer<C3DClothPrimitive> newCloth(new C3DClothPrimitive(cloth.Get()));   // 0xA92260 new(0x14), ctor 0xAC3E00; Info {1,0xA93A60,data} ebx; Data edi/B+0x5C
        ClothPrimitives.push_back(newCloth);                        // 0xA922AE inline incl. overflow (__destroy_aux 0xA96940)
        cloth = chunk.FindNextCloth(cloth);                         // 0xA92395 call 0xABBC90
    }                                                               // newCloth dtor 0xA923FE
    if (GLoadMirroredTextureHighlights) {                           // 0xA92432 byte [0x013D2D74]
        CCountedPointer<CChunkedFileChunk> mteh = chunk.FindFirstSubChunkOfType(CMirroredTextureHighlightsChunk::GetStaticChunkType());
                                                                    // 0xA9244B "MTEH": result constructed DIRECTLY in B+0x28 (no SetStaticCast)
        while (mteh.Get()) {                                        // 0xA92456 / 0xA92543
            CMirroredTextureHighlightsChunk* m = static_cast<CMirroredTextureHighlightsChunk*>(mteh.Get());
            AddMirroredTextureVertexIndex(m->GetList(), m->GetCount());   // 0xA92464 0xAC4F30, 0xAC4F20, 0xA92476 call 0xA90510
            mteh = chunk.FindNextSubChunkOfType(mteh);              // 0xA924A2: conv temp B+0x5C (copy+inc 0xA9247B), result temp B+0x10
        }
    }                                                               // mteh dtor 0xA9254F
    SetupVertexStreamFlags(InitFlags, b);                           // 0xA92578 inline: VSF=0; if(b) VSF=1; if(flags&2) VSF|=2; if(flags&0x10) VSF|=4
                                                                    //   (flags re-read from +0x68 each test: `mov cl,[edi+68h]`, `test byte [edi+68h],10h`)
    return true;                                                    // dtors 0xA925A1..0xA926B2 (cloth, xmif, xbds, xuvt, group, xapr, tris, vbd)
}
```
Notes
- The dead `Data=0; Info=0` stores after temp releases ARE kept in several places (0xA91A57, 0xA91DD6, 0xA92400,
  0xA9250E, 0xA9253B, 0xA92698): they are the CCountedPointer dtor's zeroing on temps whose slots are reused.
- Cold block 0xA926C4..0xA92726: release edi (xapr Info), B+0x34 (tris), B+0x9C (vbd), zero B+0x30/0x34 -> 0xA92729.
- No jump tables, no switch.
- Open decisions for the author: (1) UTangents/VTangents resize must CALL 0x75C040 while Vertices.resize is expanded;
  (2) IsValidSize/AllocateResource grouping (the size guard has its OWN cold cleanup copy, AllocateResource failure falls
  through into an inline copy); (3) the MTEH local type (CCountedPointer<CChunkedFileChunk> constructed straight from the
  call, unlike the SetStaticCast sites); (4) GMeshDataMemoryPool / GLoadMirroredTextureHighlights are descriptive names.
