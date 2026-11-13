# OUTLINE 0x00ACA830 COptimisedPrimitive::Initialise(const COptimisationParameters& parameters)

## Frame facts
- Prologue: `push ebp; mov ebp,esp; and esp,0FFFFFFF8h; sub esp,0D4h; push ebx; push esi; push edi`. Saved order: ebx, esi, edi. The 8-aligned frame comes from the CQuadric (double) local. No EH, no /GS cookie.
- `ret 4`. `this` is spilled to [esp+1Ch] right away and is mostly in edi from 0x00ACAA75 on. The argument is at [ebp+8].
- Offsets below are esp-relative with no outstanding pushes, so add 4 per pushed argument when reading the listing.

## Frame map (slots are heavily reused; each line lists the main tenants)
| slot | tenants |
|---|---|
| +0C | longest edge (float, pass 1); face byte-offset (pass 2, edge passes); unique-vertex index (unsplit loop); sub index i (final loop); sub_a.Index1 (edge loop) |
| +10 | edge length a (pass 1); face counter (pass 3); u_min (edge loop) and then sub_b.Index1; map iterator node (contraction loop) |
| +14 | the insert_unique key temp (unsigned long / long copy of the index); edge counter (edge loop); *it1 (unsplit); SubContractions[i]* (final loop) |
| +18 | &Vertices[VertexIndices[2]] (pass 2); edge byte-offset (edge loop); it2 (unsplit); multimap iterator (final loop) |
| +1C | this |
| +20..+28 | C3DVector (normal / mid / perp); key pair<long,long> (+20,+24); the unsplit CCountedPointer local (Info at +24) |
| +2C,+30 | std::set<long> vertices1 (header ptr, node count) (contraction loop); &uv.VertexIndices (unsplit); overflow buffer ptr |
| +38 | scaling_squared = parameters.Scaling*parameters.Scaling (float, live through pass 4); later the overflow `1`/len scratch; contraction Data (final loop) |
| +40..+48 | face normal (pass 3) / edge vector (pass 4); CCountedPointer& from operator[] (+40, edge loop); *it2 (+40, unsplit); sub count (+40, final loop) |
| +4C,+50 | face counter (passes 1 and 4); dot (pass 3); u_max / sub_b.Index2 (edge loop); std::set<long> vertices2 (header +4C, count +50); it1 (unsplit) |
| +58..+60 | pair<const float,CCountedPointer> temp (+58 key, +5C Data, +60 Info) (contraction loop); edge slot e (passes 3 and 4); unique-vertex byte offset (unsplit) |
| +60 / +64 | fast-1/sqrt bit pattern (+60); edge length b (+64, pass 1); sub_a.Index2 (+64, edge loop); overflow size |
| +6C..+74 | other face normal (pass 3); face normal (pass 4); pair<const float,CCountedPointer> temp (unsplit: +6C key, +70 Data, +74 Info) |
| +78..+CF | CQuadric quadric (11 doubles; Weight at +C8) |
| +D4..+DF | std::map<std::pair<long,long>, CCountedPointer<CVertexPairContraction> > contractions (_M_header +D4, _M_node_count +D8, compare +DC) |

## Body (retail order)

```
// 0x00ACA845
Parameters = parameters;                                    // rep movsd x10 into this+5Ch
float scaling_squared = parameters.Scaling * parameters.Scaling;   // fld [eax]; fld st0; fmul st1 -> [+38]; the pop of the second copy follows
// 0x00ACA883 PASS 0: clear the vertex quadrics (loop bound recomputed each pass from Vertices.size())
for (unsigned long i = 0; i < Vertices.size(); ++i) {       // esi = byte offset, edi = i
    Vertices[i].PositionError.Clear();                      // CQuadric::Clear INLINED: 11x `fld [0.0]; fstp` (+28..+78)
    Vertices[i].Cost = 0.0f;                                // mov dword [eax+80h],0
}
// 0x00ACA94E PASS 1: face membership. [+0C] = face byte offset, esi = i
for (unsigned long i = 0; i < Faces.size(); ++i) {
    COptimisedFace& face = Faces[i];
    COptimisedVertex& v0 = Vertices[face.VertexIndices[0]];   // edi
    COptimisedVertex& v1 = Vertices[face.VertexIndices[1]];   // ebx
    COptimisedVertex& v2 = Vertices[face.VertexIndices[2]];   // [+18]
    v0.FaceList.AddIndex(i); v1.FaceList.AddIndex(i); v2.FaceList.AddIndex(i);      // 0x514C10 x3, key temp [+14]
    UniqueVertices[v0.UniqueIndex].FaceList.AddIndex(i);    // 0x00ACA9F9 (0x1C stride, FaceList +10h)
    UniqueVertices[v1.UniqueIndex].FaceList.AddIndex(i);
    UniqueVertices[v2.UniqueIndex].FaceList.AddIndex(i);    // 0x00ACAA43
}
// 0x00ACAA75
FindAdjacentFaces();        // 0x00AC9A60
PaintSegments();            // 0x00AC8540
MeasureSegments();          // 0x00AC9D50
// 0x00ACAAB3 PASS 2: face plane quadrics. [+4C] = i, [+2C] = byte offset
for (unsigned long i = 0; i < Faces.size(); ++i) {
    COptimisedFace& face = Faces[i];
    COptimisedVertex& v0 = Vertices[face.VertexIndices[0]];   // edi
    COptimisedVertex& v1 = Vertices[face.VertexIndices[1]];   // ebx
    COptimisedVertex& v2 = Vertices[face.VertexIndices[2]];   // esi
    float a = (v1.Position - v0.Position).GetMagnitude();     // fsqrt, sum X,Z,Y -> [+10]
    float b = (v2.Position - v1.Position).GetMagnitude();     // -> [+64]
    float c = (v0.Position - v2.Position).GetMagnitude();     // stays on FPU
    float longest = MAX(MAX(a, b), c);                        // 0x00ACAB78..BBF: `a > b` is evaluated TWICE (once for the
        // compare with c, once to pick the stored value via integer moves), so it is a macro-like nested ternary
        // `((a>b?a:b) > c) ? (a>b?a:b) : c`; equal values pick the second operand (test ah,41h / jne). Try
        // std::max(std::max(b,a),c) only if the macro form fails.
    C3DVector normal = GetFaceNormal(face);                   // 0x00AC7CB0 -> [+20]
    if (normal.IsNonZero()) normal.Normalise();               // 0x00ACABD2: X,Z,Y loads; jp 0x00ACAEF5 (cleanup fstp x3, jmp 0x00ACAEFB)
    quadric.Initialise(normal, v2.Position, 1.0f);            // 0x00ACAC94 (push 3F800000h, push esi, push &normal)
    quadric *= longest * longest * 0.5f;                      // 0x00ACAC99 operator*= INLINED (fld q.m; fmul st1; fstp)
    v0.PositionError += quadric;                              // 0x00ACAD4F, edi (inline +=: fld q.m; fadd v.m; fstp v.m)
    v1.PositionError += quadric;                              // ebx
    v2.PositionError += quadric;                              // esi, ends 0x00ACAEF3 jmp 0x00ACAEFB
}
// 0x00ACAF3A PASS 3: weight normalisation. ebx = byte offset, esi = i
for (unsigned long i = 0; i < Vertices.size(); ++i) {
    COptimisedVertex& vertex = Vertices[i];                   // ecx
    if (vertex.PositionError.Weight > 0.0)                    // fcomp qword [0.0]; test ah,41h; jne
        vertex.PositionError *= scaling_squared / vertex.PositionError.Weight;   // double factor; inline *= (fld st0; fmul m; fstp m)
}
// 0x00ACAFFF PASS 4: crease quadrics on adjacent faces. [+10] = i, [+0C] = byte offset
for (unsigned long i = 0; i < Faces.size(); ++i) {
    COptimisedFace& face = Faces[i];                          // esi, [+64]
    C3DVector normal = GetFaceNormal(face);                   // 0x00ACB03B -> [+40]
    for (unsigned long e = 0; e < 3; ++e) {                   // ebx / [+58]
        if (face.Edges[e] == -1) continue;                    // 0x00ACB054 -> 0x00ACB38C
        C3DVector other = GetFaceNormal(Faces[face.Edges[e]]);// 0x00ACB06E -> [+6C]
        float dot = other.X*normal.X + other.Z*normal.Z + other.Y*normal.Y;  // `other.Dot(normal)`, sum X,Z,Y -> fst [+4C]
        if (dot < 0.0f) {                                     // fcomp [0122DEDC]; test ah,5; jp skip
            C3DVector mid = other + normal;                   // X stored [+20], Y and Z on FPU
            if (mid.IsNonZero()) mid.Normalise();             // jp 0x00ACB382 (cleanup fstp x5)
            COptimisedVertex& va = Vertices[face.VertexIndices[e]];          // esi
            COptimisedVertex& vb = Vertices[face.VertexIndices[(e+1)%3]];    // edi (div by 3)
            quadric.Initialise(mid, va.Position, 1.0f);       // 0x00ACB1A1
            quadric *= -dot;                                  // 0x00ACB1A6 fld [+4C]; fchs
            va.PositionError += quadric;                      // [esi+28h..]
            vb.PositionError += quadric;                      // via eax = &vb.PositionError (0x00ACB245 lea eax,[edi+ebx+28h])
        }
    }
}
// 0x00ACB3D1 PASS 5: border-edge quadrics. [+4C] = i, [+0C] = byte offset; ebx = &face
for (unsigned long i = 0; i < Faces.size(); ++i) {
    COptimisedFace& face = Faces[i];
    C3DVector normal = GetFaceNormal(face);                   // 0x00ACB411 -> [+6C]
    for (unsigned long e = 0; e < 3; ++e) {                   // eax / [+58]
        if (face.Edges[e] != -1) continue;                    // jne 0x00ACB786
        COptimisedVertex& va = Vertices[face.VertexIndices[e]];          // esi
        COptimisedVertex& vb = Vertices[face.VertexIndices[(e+1)%3]];    // edi
        C3DVector edge = vb.Position - va.Position;           // [+40,+44,+48]
        C3DVector perp = normal ^ edge;                       // operator^ (Y*o.Z-Z*o.Y, Z*o.X-X*o.Z, X*o.Y-Y*o.X) -> [+20..+28]
        if (perp.IsNonZero()) perp.Normalise();               // squares loaded Z,Y,X here (0x00ACB4B2); jp 0x00ACB560
        quadric.Initialise(perp, va.Position, 0.0f);          // 0x00ACB575 (push 0)
        quadric *= edge.GetSquaredMagnitude() * parameters.EdgeWeight * scaling_squared;  // Z,Y,X sum at 0x00ACB57A; fmul [eax+4]; fmul [+38]
        va.PositionError += quadric;
        vb.PositionError += quadric;
        va.Cost += parameters.EdgeCost;                       // 0x00ACB760 (fld [esi+80h]; fadd [eax+8])
        vb.Cost += parameters.EdgeCost;
    }
}
// 0x00ACB7C9
for (unsigned long i = 0; i < Faces.size(); ++i) FindFaceTextureTangents(Faces[i]);    // 0x00AC7E10
// 0x00ACB816
for (unsigned long i = 0; i < Vertices.size(); ++i) ComputeVertexCost(Vertices[i]);   // 0x00AC8C80
// 0x00ACB86A edge contractions: the local map is constructed here (header malloc(20h) at +D4, count +D8 = 0)
std::map<std::pair<long,long>, CCountedPointer<CVertexPairContraction> > contractions;
for (unsigned long i = 0; i < Edges.size(); ++i) {           // [+14] = i, [+18] = byte offset, sizes >>4
    COptimisedEdge& edge = Edges[i];                          // [+58]
    unsigned long u_min = edge.UniqueVertex1, u_max = edge.UniqueVertex2;   // [+10], [+4C]
    if (u_min > u_max) swap(u_min, u_max);                    // unsigned `jbe` (inline std::swap or a manual swap)
    CCountedPointer<CVertexPairContraction>& contraction = contractions[std::pair<long,long>(u_min, u_max)];  // 0x00AD6740 -> esi/[+40]
    if (contraction.IsNull())                                 // cmp [esi],0
        contraction.Reset(new CVertexPairContraction);        // inline: new(28h) + inline ctor; ReleaseData of the old Info; Info=0;
                                                              // Data=p; if (p) Info=new CCPPointerInfo(1, DeleteData 0x00AD5ED0, Data)
    const COptimisedFace& face_a = Faces[edge.FaceA];         // 0x00ACB9AA ([+38])
    long a1 = UniqueToActualIndex(u_min, face_a);             // inline loop over 3 -> [+0C] or -1
    long a2 = UniqueToActualIndex(u_max, face_a);             // -> [+64] or -1
    contraction->SubContractions.push_back(CSubContraction(a1, a2));   // inline push_back + overflow (0x00ACBA16..0x00ACBAEE)
    const COptimisedFace& face_b = Faces[edge.FaceB];         // 0x00ACBAF1
    long b1 = UniqueToActualIndex(u_min, face_b);             // [+10]
    long b2 = UniqueToActualIndex(u_max, face_b);             // eax/[+4C] (`or eax,-1` form here)
    if (b1 != a1 || b2 != a2)                                 // 0x00ACBB61 (maybe `CSubContraction(b1,b2) != CSubContraction(a1,a2)`)
        contraction->SubContractions.push_back(CSubContraction(b1, b2));  // 0x00ACBB79..0x00ACBC4D
}
// 0x00ACBC77 keep only contractions whose sub-contractions use distinct vertices
for (iterator it = contractions.begin(); it != contractions.end(); ++it) {   // node in [+10]; inline ++ at 0x00ACBEAA
    std::set<long> vertices1;                                 // header malloc(14h) -> [+2C]
    std::set<long> vertices2;                                 // -> [+4C]
    unsigned long count = it->second->SubContractions.size(); // hoisted into esi before the loop
    for (unsigned long s = 0; s < count; ++s) {               // ebx
        const CSubContraction& sub = it->second->SubContractions[s];   // re-read through the node each pass
        vertices1.insert(sub.Index1);                         // 0x512DA0 (long temp [+14])
        vertices2.insert(sub.Index2);                         // 0x512DA0
    }
    if (vertices1.size() == count && vertices2.size() == count) {      // 0x00ACBD54
        ComputeContraction(*it->second);                      // 0x00AC92C0
        VertexPairContractions.insert(std::pair<const float, CCountedPointer<CVertexPairContraction> >(it->second->Cost, it->second));
            // the pair temp at +58; insert_equal INLINE (0x00ACBD94 walk: fld key; fcomp [x+10h]; test ah,5; jp -> right);
            // _M_insert 0x00AD4540 (x, y, v, 0); the result is DISCARDED; ~pair (release, then Data=Info=0 at 0x00ACBDEA)
    }
    // ~vertices2 (0x00ACBDF6), then ~vertices1 (0x00ACBE50): if (count) { inline _M_erase loop calling 0x4A9CB0 on the right
    // child, free(node) }, reset the header, then free(header)
}
// 0x00ACBEF5 unsplit (same-position) contractions over UniqueVertices. [+0C] = index, [+58] = byte offset
for (unsigned long u = 0; u < UniqueVertices.size(); ++u) {
    CUniqueVertex& unique = UniqueVertices[u];
    if (unique.VertexIndices.GetSize() > 1) {                 // cmp [eax+4],1; jbe
        for (it1 = unique.VertexIndices.Begin(); it1 != End; ++it1) {      // [+4C]
            unsigned long vertex1 = *it1;                     // [+14]
            iterator it2 = it1; ++it2;                        // [+18]
            for (; it2 != End; ++it2) {                       // inner ++ at 0x00ACC180
                unsigned long vertex2 = *it2;                 // [+40]
                CCountedPointer<CVertexPairContraction> contraction(new CVertexPairContraction);   // Data in esi, Info [+24]
                contraction->Unsplit = true;                  // mov byte [esi+1],1
                contraction->SubContractions.push_back(CSubContraction(vertex1, vertex2));   // inline overflow 0x00ACC02A..0x00ACC0E5
                ComputeSelfContraction(*contraction);         // 0x00AC7EB0
                VertexPairContractions.insert(std::pair<const float, CCountedPointer<CVertexPairContraction> >(contraction->Cost, contraction));
                    // pair temp at +6C; insert_equal inline (0x00ACC109); _M_insert 0x00AD4540; ~pair
            }   // ~contraction (0x00ACC161)
        }
    }
}
// 0x00ACC24F register each contraction with both vertices
for (iterator it = VertexPairContractions.begin(); it != VertexPairContractions.end(); ++it) {   // [+18]; inline ++ 0x00ACC3D4
    CCountedPointer<CVertexPairContraction> contraction = it->second;   // Data [+38], Info esi (inc)
    unsigned long count = contraction->SubContractions.size();          // [+40]
    for (unsigned long s = 0; s < count; ++s) {               // [+0C] / ebx
        const CSubContraction& sub = contraction->SubContractions[s];   // [+14]
        COptimisedVertex& vertex1 = Vertices[sub.Index1];     // eax
        COptimisedVertex& vertex2 = Vertices[sub.Index2];     // edi
        vertex1.PairContractions[sub.Index2] = CContractionReference(contraction, s);   // temp inc BEFORE operator[] 0x00AD64C0;
                                                              // inline operator= (compare Info, release, copy, inc), SubIndex = s; ~temp
        vertex2.PairContractions[sub.Index1] = CContractionReference(contraction, s);   // 0x00ACC346
    }
    contraction->ListIterator = it;                           // 0x00ACC3BC mov [ecx+24h],edi
}   // ~contraction (0x00ACC3B2)
// 0x00ACC422 ~contractions: if (count) { _M_erase(root) 0x00AD3300; reset the header; count=0 } then free(header)
// 0x00ACC47F epilogue, ret 4
```

## Inline expansions to reproduce (and where the bodies are)
- Tree header ctor: `malloc(node size)`, then color=0, parent=0, left=right=header, count=0. This is STLport _Rb_tree ctor / _M_empty_initialize (stlport/fable_map.h lines 81 and 116).
- Tree dtor: clear() + _Rb_tree_base dtor (fable_map.h lines 82 and 145). For set<long>, _M_erase is expanded one level. For the pair map it is called.
- vector push_back + _M_insert_overflow (STLport 4.6 `_M_insert_overflow(pos, x, __false_type, 1, true)`): len = size + max(size,1) (`max` through references at [+38]/[+2C]), malloc(len*8) or 0, uninitialized_copy with a null-checked placement copy, construct x, free(old) when non-null, then set start/finish/end. See the other landed STLport vector rows (fable_containers.h). Do not use the FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE variant.
- CQuadric::Clear / operator*= / operator+=: copy the landed bodies 0x00AC6CE0 / 0x00AC6EF0 / 0x00AC6E80 as inline members (keep them inline here; retail has out-of-line COMDAT copies too).
- UniqueToActualIndex (FableWin 0x034C7AC0, ABEJKABVCOptimisedFace@2@@Z): `for (k=0;k<3;k++) if (Vertices[face.VertexIndices[k]].UniqueIndex == unique) return face.VertexIndices[k]; return -1;`.
- CVertexPairContraction() : Exists(true), Unsplit(false), (vector empty), ListIterator node 0.
- CCountedPointer: copy (inc), operator= (Info compare first), dtor release with Data=Info=0, Reset(T*) (CCountedPointer.h comment lines 25-36).
