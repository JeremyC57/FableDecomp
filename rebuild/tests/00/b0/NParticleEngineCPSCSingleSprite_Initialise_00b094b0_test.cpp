// Test: CPSCSingleSprite::Initialise -- with an engine and a sprite bank, create one child sprite per crossed sprite (or a
// single sprite), all at the zero position, recording each id at its index (growing the id list); then the trail
// primitive at the next index when a trail bank is set; mark initialised and copy StayWithEmitter to the system.
#include <cstdio>
#include <vector>
struct V3 { float X, Y, Z; V3(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct Engine { int calls; bool zeroOk; long next; Engine() : calls(0), zeroOk(true), next(100) {}
    long CreateSprite(const V3& p) { ++calls; if (p.X != 0 || p.Y != 0 || p.Z != 0) zeroOk = false; return next++; }
    long CreateTrail() { ++calls; return 900; } };
struct Sprite {
    unsigned long SpriteBankIndex, TrailBankIndex; bool CrossedSprites; long NoCrossedSprites; bool Initialised, StayWithEmitter;
    std::vector<long> PrimIDList;
    bool NeedsPrim(unsigned int index) const { return index >= PrimIDList.size() || PrimIDList[index] == -1; }
    bool Initialise(Engine* engine, bool& systemStay)
    {
        if (engine) {
            unsigned int index = 0;
            if (SpriteBankIndex) {
                if (NeedsPrim(index)) {
                    if (CrossedSprites) {
                        long i = 0;
                        if (NoCrossedSprites > 0) {
                            V3 zero(0.0f, 0.0f, 0.0f);
                            do {
                                long id = engine->CreateSprite(zero);
                                if (PrimIDList.size() <= (unsigned int)i) PrimIDList.resize(i + 1);
                                PrimIDList[i] = id;
                            } while (++i < NoCrossedSprites);
                        }
                        index = NoCrossedSprites;
                    } else {
                        long id = engine->CreateSprite(V3(0.0f, 0.0f, 0.0f));
                        if (PrimIDList.size() <= 0) PrimIDList.resize(1);
                        PrimIDList[0] = id; index = 1;
                    }
                } else
                    index = CrossedSprites ? NoCrossedSprites : 1;
            }
            if (TrailBankIndex && NeedsPrim(index)) {
                long id = engine->CreateTrail();
                if (PrimIDList.size() <= index) PrimIDList.resize(index + 1);
                PrimIDList[index] = id;
            }
            Initialised = true;
        }
        systemStay = StayWithEmitter;
        return Initialised;
    }
};
int main()
{
    bool ok = true, stay = false;
    Engine e; Sprite s; s.SpriteBankIndex = 3; s.TrailBankIndex = 4; s.CrossedSprites = true; s.NoCrossedSprites = 3;
    s.Initialised = false; s.StayWithEmitter = true;
    if (!s.Initialise(&e, stay) || !stay || e.calls != 4 || !e.zeroOk) ok = false;
    if (s.PrimIDList.size() != 4 || s.PrimIDList[0] != 100 || s.PrimIDList[2] != 102 || s.PrimIDList[3] != 900) ok = false;
    Engine e2; Sprite t = s; t.PrimIDList.clear(); t.NoCrossedSprites = 0; t.TrailBankIndex = 0;   // empty crossed loop
    t.Initialise(&e2, stay); if (e2.calls != 0 || !t.PrimIDList.empty()) ok = false;
    Engine e3; Sprite u = s; u.PrimIDList.clear(); u.CrossedSprites = false;
    u.Initialise(&e3, stay); if (e3.calls != 2 || u.PrimIDList.size() != 2 || u.PrimIDList[1] != 900) ok = false;
    Sprite w = s; w.Initialised = false; if (w.Initialise(0, stay)) ok = false;
    printf(ok ? "00b094b0_TEST PASS\n" : "00b094b0_TEST FAIL\n");
    return ok ? 0 : 1;
}