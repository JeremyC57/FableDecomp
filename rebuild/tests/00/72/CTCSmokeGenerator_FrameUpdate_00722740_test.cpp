// Behaviour test for CTCSmokeGenerator::FrameUpdate (0x00722740): a model of the smoke refill -- before the positions exist they
// are created; afterwards every position without a smoke thing gets a new effect thing of the def's particle emitter type there.
#include <cstdio>
#include <vector>
struct Thing { long type; float x; };
struct Def { long ParticleEmitter; int refs; };
struct Pos { float x; Thing* smoke; };
struct Generator {
    bool PositionsCreated; std::vector<Pos> GenerationPositions; Def* def; int cameraReads; int created; Thing pool[8];
    Thing* Create(long type, const float& x) { Thing& t = pool[created++]; t.type = type; t.x = x; return &t; }
    void CreatePositions() { Pos p; p.smoke = 0; p.x = 1.0f; GenerationPositions.push_back(p); p.x = 2.0f; GenerationPositions.push_back(p); PositionsCreated = true; }
    void FrameUpdate()
    {
        if (PositionsCreated) {
            for (std::vector<Pos>::iterator it = GenerationPositions.begin(); it != GenerationPositions.end(); ++it) {
                ++cameraReads;
                if (!it->smoke) {
                    ++def->refs;
                    it->smoke = Create(def->ParticleEmitter, it->x);
                    --def->refs;
                }
            }
        } else
            CreatePositions();
    }
};
int main()
{
    Def d; d.ParticleEmitter = 77; d.refs = 1;
    Generator g; g.PositionsCreated = false; g.def = &d; g.cameraReads = 0; g.created = 0;
    g.FrameUpdate();
    if (!g.PositionsCreated || g.GenerationPositions.size() != 2 || g.created != 0) { std::printf("00722740_TEST FAIL\n"); return 1; }
    g.FrameUpdate();
    if (g.created != 2 || g.GenerationPositions[1].smoke->type != 77 || g.GenerationPositions[1].smoke->x != 2.0f || d.refs != 1 || g.cameraReads != 2)
        { std::printf("00722740_TEST FAIL\n"); return 1; }
    g.GenerationPositions[0].smoke = 0;
    g.FrameUpdate();
    if (g.created != 3 || g.GenerationPositions[0].smoke->x != 1.0f) { std::printf("00722740_TEST FAIL\n"); return 1; }
    std::printf("00722740_TEST PASS\n");
    return 0;
}