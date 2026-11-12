// Behaviour test for NPlayerGui::CDrawQuickAccessIcons::DrawEffectAtPos (0x0064546F): a copy of the model with a counting
// emitter manager. Checks create-at-offset, keep-when-unmoved, kill-and-recreate when moved, and kill-and-clear at the zero position.
#include <cstdio>
#include <vector>
static const float DELTA_NOTIONAL_ZERO = 1e-4f;
class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator+(const C2DVector& v) const { return C2DVector(X + v.X, Y + v.Y); }
    float GetSquaredMagnitude() const { float squared = Y * Y; squared += X * X; return squared; }
    bool IsNonZero() const { return GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO; }
};
class C2DCoordI { public:
    long X, Y;
    C2DCoordI() {}
    C2DCoordI(long x, long y) : X(x), Y(y) {}
    C2DVector ToC2DCoordF() const { return C2DVector((float)X, (float)Y); }
    bool operator!=(const C2DCoordI& coord) const { return X != coord.X || Y != coord.Y; }
};
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
class CRightHandedSet { public:
    C3DVector Up, Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {}
};
enum EEngine2DLayer { ENGINE_2D_LAYER_0 = 0x43 };
struct CParticleEmitter {
    long Type; C2DVector Position; int Layer; bool Start; bool Killed; float UpZ, ForwardY;
    void Kill() { Killed = true; }
};
static int g_created = 0, g_kills = 0;
static CParticleEmitter g_pool[8];
struct CParticleEmitterManager {
    CParticleEmitter* Create2DEmitter(long handle, const C2DVector& position, EEngine2DLayer layer, bool start,
                                      const CRightHandedSet& orientation)
    {
        CParticleEmitter* e = &g_pool[g_created++];
        e->Type = handle; e->Position = position; e->Layer = layer; e->Start = start; e->Killed = false;
        e->UpZ = orientation.Up.Z; e->ForwardY = orientation.Forward.Y;
        return e;
    }
};
static CParticleEmitterManager g_manager;
static CParticleEmitterManager& GetParticleEmitterManager() { return g_manager; }
struct CIntelligentPointer {
    CParticleEmitter* PData;
    CIntelligentPointer() : PData(0) {}
    CIntelligentPointer(CParticleEmitter* p) : PData(p) {}
    CParticleEmitter* GetPItem() const { return PData; }
    CParticleEmitter* operator->() const { return PData; }
};
class CPlayerGuiDef { public: C2DVector QuickAccessEffectOffset; };
static CPlayerGuiDef g_def;
static const CPlayerGuiDef& PeekGuiDef() { return g_def; }
struct CDrawQuickAccessIcons {
    std::vector<CIntelligentPointer> Effects;
    std::vector<C2DCoordI> EffectPositions;
    void DrawEffectAtPos(const C2DCoordI& pos, long effect, long index)
    {
        if (pos.ToC2DCoordF().IsNonZero()) {
            if (Effects[index].GetPItem() && pos != EffectPositions[index]) {
                Effects[index]->Kill(); ++g_kills;
                Effects[index] = 0;
            }
            if (!Effects[index].GetPItem()) {
                const C2DVector& offset = PeekGuiDef().QuickAccessEffectOffset;
                C2DVector position(pos.ToC2DCoordF() + offset);
                Effects[index] = GetParticleEmitterManager().Create2DEmitter(
                    effect, position, ENGINE_2D_LAYER_0, true,
                    CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f)));
                EffectPositions[index] = pos;
            }
        } else {
            if (Effects[index].GetPItem()) {
                Effects[index]->Kill(); ++g_kills;
                Effects[index] = 0;
                EffectPositions[index] = C2DCoordI(0, 0);
            }
        }
    }
};
int main()
{
    bool ok = true;
    g_def.QuickAccessEffectOffset = C2DVector(1.5f, -2.5f);
    CDrawQuickAccessIcons icons;
    icons.Effects.resize(3);
    icons.EffectPositions.resize(3, C2DCoordI(0, 0));
    // zero position, empty slot: nothing happens
    icons.DrawEffectAtPos(C2DCoordI(0, 0), 7, 1);
    ok = ok && g_created == 0 && g_kills == 0 && icons.Effects[1].GetPItem() == 0;
    // non-zero position, empty slot: create at pos + offset, remember the position
    icons.DrawEffectAtPos(C2DCoordI(10, 20), 7, 1);
    CParticleEmitter* first = icons.Effects[1].GetPItem();
    ok = ok && g_created == 1 && first == &g_pool[0] && first->Type == 7 && first->Position.X == 11.5f && first->Position.Y == 17.5f;
    ok = ok && first->Layer == 0x43 && first->Start && first->UpZ == 1.0f && first->ForwardY == 1.0f;
    ok = ok && icons.EffectPositions[1].X == 10 && icons.EffectPositions[1].Y == 20;
    ok = ok && icons.Effects[0].GetPItem() == 0 && icons.Effects[2].GetPItem() == 0;
    // same position again: kept, nothing created or killed
    icons.DrawEffectAtPos(C2DCoordI(10, 20), 7, 1);
    ok = ok && g_created == 1 && g_kills == 0 && icons.Effects[1].GetPItem() == first;
    // only Y moved: the old emitter is killed and a new one created at the new position
    icons.DrawEffectAtPos(C2DCoordI(10, 21), 9, 1);
    CParticleEmitter* second = icons.Effects[1].GetPItem();
    ok = ok && g_created == 2 && g_kills == 1 && first->Killed && second == &g_pool[1] && second->Type == 9;
    ok = ok && second->Position.X == 11.5f && second->Position.Y == 18.5f && icons.EffectPositions[1].Y == 21;
    // only X moved
    icons.DrawEffectAtPos(C2DCoordI(-4, 21), 9, 1);
    ok = ok && g_created == 3 && g_kills == 2 && second->Killed && icons.EffectPositions[1].X == -4;
    // zero position with a live emitter: killed, slot and position cleared
    icons.DrawEffectAtPos(C2DCoordI(0, 0), 9, 1);
    ok = ok && g_created == 3 && g_kills == 3 && g_pool[2].Killed && icons.Effects[1].GetPItem() == 0;
    ok = ok && icons.EffectPositions[1].X == 0 && icons.EffectPositions[1].Y == 0;
    // a position with only Y set counts as non-zero; another slot is independent
    icons.DrawEffectAtPos(C2DCoordI(0, 3), 2, 2);
    ok = ok && g_created == 4 && icons.Effects[2].GetPItem() == &g_pool[3] && icons.EffectPositions[2].Y == 3 && icons.Effects[1].GetPItem() == 0;
    if (!ok) { std::printf("0064546f_TEST FAIL\n"); return 1; }
    std::printf("0064546f_TEST PASS\n");
    return 0;
}