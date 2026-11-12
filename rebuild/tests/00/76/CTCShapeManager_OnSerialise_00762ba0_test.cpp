// Behaviour test for 0x00762BA0 CTCShapeManager::OnSerialise: a copy of the function's logic over a small stand-alone model
// (persist context with the five modes, world map with a per-map position, counted shape pointers), checked in save and load modes.
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
class C2DCoordI { public: long X, Y; C2DCoordI() {} C2DCoordI(long x, long y) : X(x), Y(y) {} };
class C3DVector { public: float X, Y, Z; };
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class CPersistContext { public:
    enum EMode { MODE_LOAD_TEXT, MODE_SAVE_TEXT, MODE_LOAD_BINARY, MODE_SAVE_BINARY, MODE_RESET };
    EMode Mode;
    bool StoredRelative; long StoredNum;
    std::vector<std::string> Log;
    bool IsSaving() const { return Mode == MODE_SAVE_TEXT || Mode == MODE_SAVE_BINARY; }
    void Transfer(const char* name, bool& value, const bool& def) { Log.push_back(name); if (IsSaving()) StoredRelative = value; else if (Mode == MODE_RESET) value = def; else value = StoredRelative; }
    void Transfer(const char* name, long& value, const long& def) { Log.push_back(name); if (IsSaving()) StoredNum = value; else if (Mode == MODE_RESET) value = def; else value = StoredNum; }
    void Transfer(const char* name, bool& value) { Transfer(name, value, false); }
    void Transfer(const char* name, long& value) { Transfer(name, value, 0L); }
};
class CWorldMap { public:
    int Calls;
    long GetMapNumberAt(const C3DVector& pos) const { return pos.X < 100.0f ? 3 : 7; }
    C2DCoordI GetMapPos(long map) const { return C2DCoordI(map * 10, map * 20); }
};
class CThing { public: C3DVector Pos; const C3DVector& GetPos() const { return Pos; } };
struct CShape {
    std::vector<long> Indices; std::vector<C2DCoordI> Offsets;
    void OnSerialise(CPersistContext&, long index, const C2DCoordI& offset) { Indices.push_back(index); Offsets.push_back(offset); }
};
static int g_deleted = 0;
struct CInfo { long RefCount; };
struct CCountedPointer {
    CShape* Data; CInfo* Info;
    CCountedPointer(CShape* d) : Data(d), Info(new CInfo) { Info->RefCount = 1; }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { if (Info && --Info->RefCount == 0) { ++g_deleted; delete Data; delete Info; } }
    CShape* operator->() const { return Data; }
    CCountedPointer& operator=(const CCountedPointer& o)
    {
        if (o.Info) ++o.Info->RefCount;
        if (Info && --Info->RefCount == 0) { ++g_deleted; delete Data; delete Info; }
        Data = o.Data; Info = o.Info;
        return *this;
    }
};
class CTCShapeManager { public:
    CThing* ParentThing;
    CWorldMap* Map;
    std::vector<CCountedPointer> PersistantShapes;
    std::vector<CCountedPointer>::iterator SelectedShape;
    C2DBoxF ClosedShapeBounds;
    std::vector<long> Loaded; std::vector<C2DCoordI> LoadedOffsets; int PeekCalls;
    const CWorldMap& PeekWorldMap() { ++PeekCalls; return *Map; }
    static void StatOnSerialise(CTCShapeManager& manager, CPersistContext&, long index, const C2DCoordI& offset, C2DBoxF& bounds)
    {
        manager.Loaded.push_back(index); manager.LoadedOffsets.push_back(offset);
        if ((float)index < bounds.TLX) bounds.TLX = (float)index;
        if ((float)index > bounds.BRX) bounds.BRX = (float)index;
    }
    void OnSerialise(CPersistContext& context, bool)
    {
        bool isCoordsRelativeToMap = true;
        if (context.IsSaving())
            context.Transfer("IsCoordsRelativeToMap", isCoordsRelativeToMap);
        else
            context.Transfer("IsCoordsRelativeToMap", isCoordsRelativeToMap);
        C2DCoordI offset(0, 0);
        if (isCoordsRelativeToMap) {
            C3DVector pos = ParentThing->GetPos();
            offset = PeekWorldMap().GetMapPos(PeekWorldMap().GetMapNumberAt(pos));
        }
        long numShapes;
        if (context.IsSaving())
            numShapes = (long)PersistantShapes.size();
        context.Transfer("NumShapes", numShapes);
        if (!context.IsSaving()) {
            ClosedShapeBounds.BRX = -1e10f; ClosedShapeBounds.BRY = -1e10f; ClosedShapeBounds.TLX = 1e10f; ClosedShapeBounds.TLY = 1e10f;
            for (long i = 0; i != numShapes; ++i)
                StatOnSerialise(*this, context, i, offset, ClosedShapeBounds);
        } else {
            long index = 0;
            for (std::vector<CCountedPointer>::iterator it = PersistantShapes.begin(); it != PersistantShapes.end(); ++it) {
                CCountedPointer shape = *it;
                shape->OnSerialise(context, index, offset);
                ++index;
            }
        }
        SelectedShape = PersistantShapes.end();
    }
};
static int fail(const char* why) { std::printf("00762ba0_TEST FAIL (%s)\n", why); return 1; }
int main()
{
    CThing thing; thing.Pos.X = 50.0f; thing.Pos.Y = 1.0f; thing.Pos.Z = 2.0f;
    CWorldMap map;
    CPersistContext ctx; ctx.StoredRelative = false; ctx.StoredNum = -1;
    {
        // SAVE: three shapes, each sees (context, its index, the owner map's position); the count and the flag are stored.
        CTCShapeManager m; m.ParentThing = &thing; m.Map = &map; m.PeekCalls = 0;
        m.PersistantShapes.reserve(4);
        CShape* s[3];
        for (int i = 0; i < 3; ++i) { s[i] = new CShape; m.PersistantShapes.push_back(CCountedPointer(s[i])); }
        m.SelectedShape = m.PersistantShapes.begin();
        ctx.Mode = CPersistContext::MODE_SAVE_BINARY;
        m.OnSerialise(ctx, false);
        if (ctx.Log.size() != 2 || ctx.Log[0] != "IsCoordsRelativeToMap" || ctx.Log[1] != "NumShapes") return fail("save names");
        if (!ctx.StoredRelative || ctx.StoredNum != 3) return fail("save values");
        if (m.PeekCalls != 2) return fail("PeekWorldMap twice");
        for (int i = 0; i < 3; ++i) {
            if (s[i]->Indices.size() != 1 || s[i]->Indices[0] != i) return fail("save index");
            if (s[i]->Offsets[0].X != 30 || s[i]->Offsets[0].Y != 60) return fail("save offset");
            if (m.PersistantShapes[i].Info->RefCount != 1) return fail("refcount restored");
        }
        if (g_deleted != 0) return fail("no shape freed by the temporary copies");
        if (m.SelectedShape != m.PersistantShapes.end()) return fail("save selected");
        if (!m.Loaded.empty()) return fail("save must not load");
        // SAVE with no shapes (text mode): count 0, selection reset.
        CTCShapeManager e; e.ParentThing = &thing; e.Map = &map; e.PeekCalls = 0;
        e.SelectedShape = e.PersistantShapes.begin();
        CPersistContext c2; c2.Mode = CPersistContext::MODE_SAVE_TEXT; c2.StoredNum = 99; c2.StoredRelative = false;
        e.OnSerialise(c2, true);
        if (c2.StoredNum != 0 || e.SelectedShape != e.PersistantShapes.end()) return fail("empty save");
    }
    if (g_deleted != 3) return fail("shapes freed with the manager");
    {
        // LOAD (binary): flag true -> offset of map 7 (pos.X >= 100); bounds reset then StatOnSerialise for 0..2.
        thing.Pos.X = 150.0f;
        CTCShapeManager m; m.ParentThing = &thing; m.Map = &map; m.PeekCalls = 0;
        m.ClosedShapeBounds.TLX = m.ClosedShapeBounds.TLY = m.ClosedShapeBounds.BRX = m.ClosedShapeBounds.BRY = 5.0f;
        ctx.Mode = CPersistContext::MODE_LOAD_BINARY; ctx.Log.clear();
        m.OnSerialise(ctx, false);
        if (m.Loaded.size() != 3 || m.Loaded[0] != 0 || m.Loaded[1] != 1 || m.Loaded[2] != 2) return fail("load indices");
        if (m.LoadedOffsets[2].X != 70 || m.LoadedOffsets[2].Y != 140) return fail("load offset");
        if (m.ClosedShapeBounds.TLX != 0.0f || m.ClosedShapeBounds.BRX != 2.0f || m.ClosedShapeBounds.TLY != 1e10f || m.ClosedShapeBounds.BRY != -1e10f) return fail("load bounds");
        if (m.SelectedShape != m.PersistantShapes.end()) return fail("load selected");
        // LOAD (text) with the flag stored false and no shapes: offset stays (0,0), no map lookups, bounds are the empty box.
        CTCShapeManager z; z.ParentThing = &thing; z.Map = &map; z.PeekCalls = 0;
        z.ClosedShapeBounds.TLX = z.ClosedShapeBounds.TLY = z.ClosedShapeBounds.BRX = z.ClosedShapeBounds.BRY = 5.0f;
        CPersistContext c3; c3.Mode = CPersistContext::MODE_LOAD_TEXT; c3.StoredRelative = false; c3.StoredNum = 0;
        z.OnSerialise(c3, false);
        if (z.PeekCalls != 0 || !z.Loaded.empty()) return fail("zero load");
        if (z.ClosedShapeBounds.TLX != 1e10f || z.ClosedShapeBounds.TLY != 1e10f || z.ClosedShapeBounds.BRX != -1e10f || z.ClosedShapeBounds.BRY != -1e10f) return fail("zero load bounds");
        // RESET: the defaults (false, 0) come back; one shape count would load with the zero offset.
        CTCShapeManager r; r.ParentThing = &thing; r.Map = &map; r.PeekCalls = 0;
        CPersistContext c4; c4.Mode = CPersistContext::MODE_RESET; c4.StoredRelative = true; c4.StoredNum = 4;
        r.OnSerialise(c4, false);
        if (r.PeekCalls != 0 || !r.Loaded.empty() || r.SelectedShape != r.PersistantShapes.end()) return fail("reset");
    }
    std::printf("00762ba0_TEST PASS\n");
    return 0;
}