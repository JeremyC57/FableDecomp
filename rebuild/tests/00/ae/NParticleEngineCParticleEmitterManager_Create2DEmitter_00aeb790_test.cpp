// Behaviour test for CParticleEmitterManager::Create2DEmitter (0x00AEB790): a 2D template is cloned, inserted into the active list,
// placed on the layer and oriented, and returned; a missing or 3D template gives null; a full list gives null.
#include <cstdio>
#include <list>
class C2DVector { public: float X, Y; };
class CRightHandedSet { public: float V[6]; };
struct CParticleEmitter {
    bool Emitter2D; int Layer; C2DVector Pos; CRightHandedSet Orientation; bool Dead; int Priority;
    bool Is2DEmitter() const { return Emitter2D; }
    CParticleEmitter* DatabaseClone() const { return new CParticleEmitter(*this); }
    void Set2DPosition(const C2DVector& p, int layer) { Pos = p; Layer = layer; }
    void SetOrientation(const CRightHandedSet& o) { Orientation = o; }
};
struct CParticleEmitterList {
    std::list<CParticleEmitter*> Active; long MaxActiveEmitters;
    bool Insert(CParticleEmitter* e)
    {
        if (Active.empty()) { Active.push_back(e); return true; }
        if ((long)Active.size() < MaxActiveEmitters) { Active.push_back(e); return true; }
        return false;
    }
};
static CParticleEmitter* Templates[4];
struct CParticleEmitterManager {
    CParticleEmitterList ActiveEmitters;
    CParticleEmitter* Create2DEmitter(long handle, const C2DVector& position, int layer, bool, const CRightHandedSet& orientation)
    {
        CParticleEmitter* emitter_template = Templates[handle];
        CParticleEmitter* result = 0;
        if (emitter_template && emitter_template->Is2DEmitter()) {
            CParticleEmitter* emitter = emitter_template->DatabaseClone();
            if (ActiveEmitters.Insert(emitter)) {
                result = emitter;
                result->Set2DPosition(position, layer);
                result->SetOrientation(orientation);
            } else delete emitter;
        }
        return result;
    }
};
int main()
{
    CParticleEmitter t2d = {}; t2d.Emitter2D = true;
    CParticleEmitter t3d = {}; t3d.Emitter2D = false;
    Templates[1] = &t2d; Templates[2] = &t3d; Templates[0] = 0;
    CParticleEmitterManager m; m.ActiveEmitters.MaxActiveEmitters = 1;
    C2DVector p = { 3.0f, 4.0f }; CRightHandedSet o = { { 0, 0, 1, 0, 1, 0 } };
    CParticleEmitter* e = m.Create2DEmitter(1, p, 5, true, o);
    if (!e || e == &t2d || e->Pos.X != 3.0f || e->Layer != 5 || e->Orientation.V[2] != 1) { std::printf("00aeb790_TEST FAIL\n"); return 1; }
    if (m.Create2DEmitter(2, p, 5, true, o) || m.Create2DEmitter(0, p, 5, true, o)) { std::printf("00aeb790_TEST FAIL\n"); return 1; }
    if (m.Create2DEmitter(1, p, 5, true, o)) { std::printf("00aeb790_TEST FAIL\n"); return 1; }
    std::printf("00aeb790_TEST PASS\n");
    return 0;
}