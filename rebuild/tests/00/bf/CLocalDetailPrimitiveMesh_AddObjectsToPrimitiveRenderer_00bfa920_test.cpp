// Behaviour test for CLocalDetailPrimitiveMesh::AddObjectsToPrimitiveRenderer (0x00BFA920): a standalone copy of the model
// (counted primitive handle, collection type, renderer that records its arguments) run through the same statement sequence.
#include <cstdio>
#include <cstring>
struct Handle;
struct Primitive {
    long refCount; int notified; int deleted;
    Primitive() : refCount(0), notified(0), deleted(0) { }
    void NotifyReleased(const Handle&) { ++notified; }
    void Delete() { ++deleted; }
};
struct CountedPointer {
    Primitive* Object;
    CountedPointer() : Object(0) { }
    CountedPointer(const CountedPointer& o) : Object(o.Object) { if (Object) ++Object->refCount; }
    ~CountedPointer() { Release(); }
    void Release() { Primitive* object = Object; if (object) { if (--object->refCount == 0) object->Delete(); Object = 0; } }
    void Reset(Primitive* object) { Release(); Object = object; }
    CountedPointer& operator=(const CountedPointer& o) {
        if (Object != o.Object) { Release(); Object = o.Object; if (Object) ++Object->refCount; }
        return *this;
    }
    unsigned long GetRefCount() const { return Object ? Object->refCount : 0; }
};
struct Handle {
    CountedPointer Data;
    Handle() { }
    ~Handle() { Clear(); }
    void Clear() { if (Data.GetRefCount() == 1) Data.Object->NotifyReleased(*this); Data.Reset(0); }
    const Handle& operator=(const Handle& o) { Clear(); Data = o.Data; return *this; }
};
struct MeshInfo { int dummy; };
struct Matrix { float M[12]; };
struct FadeDistance { float FadeBeginDistance, MaxDrawDistance; FadeDistance(float b, float m) : FadeBeginDistance(b), MaxDrawDistance(m) { } };
struct Colour {
    unsigned char B, G, R, A;
    Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct Renderer {
    int calls; const MeshInfo *mesh, *shadow; const Matrix* matrix; float scale, fadeBegin, fadeMax; long alpha, alphaRef;
    unsigned char colour[4]; float mipBias; bool persistent; unsigned long flags; Primitive* next;
    Handle AddStaticMesh(const MeshInfo* m, const MeshInfo* s, const Matrix& mat, float sc, const FadeDistance& f, long a,
                         const Colour& c, long ar, float mb, bool p, unsigned long fl) {
        ++calls; mesh = m; shadow = s; matrix = &mat; scale = sc; fadeBegin = f.FadeBeginDistance; fadeMax = f.MaxDrawDistance;
        alpha = a; std::memcpy(colour, &c, 4); alphaRef = ar; mipBias = mb; persistent = p; flags = fl;
        Handle h; h.Data.Object = next; ++next->refCount; return h;
    }
};
static Renderer* g_renderer;
// Polymorphic like retail (CBaseClassNonCopyable base, vptr at +0); the out-of-line PeekFadeStart / PeekFadeEnd apply the
// video-option modifier (modelled as a x2 scale) and must NOT be what the row reads -- it uses the raw *BeforeVideoOptionModifier pair.
struct CollectionType {
    MeshInfo *Mesh, *ShadowMesh; float FadeStart, FadeEnd; long AlphaRef; float AlphaMipBias; bool AlphaIsBoolean, ReceivesShadows;
    CollectionType(MeshInfo* m, MeshInfo* s, float fs, float fe, long ar, float mb, bool ab, bool rs)
        : Mesh(m), ShadowMesh(s), FadeStart(fs), FadeEnd(fe), AlphaRef(ar), AlphaMipBias(mb), AlphaIsBoolean(ab), ReceivesShadows(rs) { }
    virtual ~CollectionType() { }
    const MeshInfo* PeekMesh() const { return Mesh; }
    const MeshInfo* PeekShadowMesh() const { return ShadowMesh; }
    float PeekFadeStartBeforeVideoOptionModifier() const { return FadeStart; }
    float PeekFadeEndBeforeVideoOptionModifier() const { return FadeEnd; }
    float PeekFadeStart() const { return FadeStart * 2.0f; }
    float PeekFadeEnd() const { return FadeEnd * 2.0f; }
    long PeekAlphaRef() const { return AlphaRef; }
    float PeekAlphaMipBias() const { return AlphaMipBias; }
    bool PeekAlphaIsBoolean() const { return AlphaIsBoolean; }
    float PeekReceivesShadows() const { return ReceivesShadows; }
};
struct PrimitiveMesh {
    const CollectionType* Type; Matrix Mat; float Scale; Handle PrimitiveHandle;
    void AddObjectsToPrimitiveRenderer() {
        if (!Type->PeekMesh())
            return;
        FadeDistance fade(Type->PeekFadeStartBeforeVideoOptionModifier(), Type->PeekFadeEndBeforeVideoOptionModifier());
        unsigned long flags = 1;
        if (Type->PeekAlphaIsBoolean())
            flags = 0x101;
        if (Type->PeekReceivesShadows() == 0.0f)
            flags |= 0x1000;
        PrimitiveHandle = g_renderer->AddStaticMesh(Type->PeekMesh(), Type->PeekShadowMesh(), Mat, Scale, fade, 255,
                                                    Colour(255, 255, 255, 255), Type->PeekAlphaRef(), Type->PeekAlphaMipBias(),
                                                    true, flags);
    }
};
static int Fail(int n) { std::printf("00bfa920_TEST FAIL (%d)\n", n); return 1; }
int main()
{
    Renderer r; std::memset(&r, 0, sizeof(r)); g_renderer = &r;
    MeshInfo mesh, shadow;
    CollectionType t(&mesh, &shadow, 10.0f, 25.0f, 128, -0.5f, false, true);
    Primitive first, second;
    {
        PrimitiveMesh m; m.Type = &t; m.Scale = 2.0f;
        // 1. no mesh: nothing is added, the handle stays empty.
        t.Mesh = 0; r.next = &first; m.AddObjectsToPrimitiveRenderer();
        if (r.calls != 0 || m.PrimitiveHandle.Data.Object != 0) return Fail(1);
        // 2. plain alpha, receives shadows: flags 1; arguments forwarded; the handle holds the only reference.
        t.Mesh = &mesh; m.AddObjectsToPrimitiveRenderer();
        if (r.calls != 1 || r.mesh != &mesh || r.shadow != &shadow || r.matrix != &m.Mat || r.scale != 2.0f) return Fail(2);
        if (r.fadeBegin != 10.0f || r.fadeMax != 25.0f || r.alpha != 255 || r.alphaRef != 128 || r.mipBias != -0.5f) return Fail(3);
        if (!r.persistent || r.flags != 1) return Fail(4);
        if (r.colour[0] != 255 || r.colour[1] != 255 || r.colour[2] != 255 || r.colour[3] != 255) return Fail(5);
        if (m.PrimitiveHandle.Data.Object != &first || first.refCount != 1 || first.notified != 0 || first.deleted != 0) return Fail(6);
        // 3. boolean alpha, no shadow receiver: flags 0x1101; the old primitive is notified and deleted, the new one kept once.
        t.AlphaIsBoolean = true; t.ReceivesShadows = false; r.next = &second; m.AddObjectsToPrimitiveRenderer();
        if (r.calls != 2 || r.flags != 0x1101) return Fail(7);
        if (first.refCount != 0 || first.notified != 1 || first.deleted != 1) return Fail(8);
        if (m.PrimitiveHandle.Data.Object != &second || second.refCount != 1 || second.notified != 0) return Fail(9);
        // 4. the two remaining flag combinations.
        t.AlphaIsBoolean = false; second.refCount = 2; m.AddObjectsToPrimitiveRenderer();
        if (r.flags != 0x1001) return Fail(10);
        // a second holder (count 2 before the call): released once without a notify, then re-added.
        if (second.notified != 0 || second.deleted != 0 || second.refCount != 2) return Fail(11);
        t.AlphaIsBoolean = true; t.ReceivesShadows = true; m.AddObjectsToPrimitiveRenderer();
        if (r.flags != 0x101 || second.refCount != 2) return Fail(12);
        second.refCount = 1;
    }
    // the mesh object's own handle dies with it: last reference, notify + delete.
    if (second.refCount != 0 || second.notified != 1 || second.deleted != 1) return Fail(13);
    std::printf("00bfa920_TEST PASS\n");
    return 0;
}