// Behaviour test for 00710b80: CAnimSet copy ctor -- words, type byte and the five flag bits are copied.
#include <cstdio>
static int g_vecCopies = 0;
struct TVec { int n; TVec() : n(0) {} TVec(const TVec& o) : n(o.n) { ++g_vecCopies; } };
struct TAnimSet {
    long UpdateMode; TVec Anims; TVec InternalAlphas; float Speed; long Priority; unsigned char Type;
    bool Looping : 1; bool Finished : 1; bool Forwards : 1; bool HadUpdate : 1; bool OwnedByMainAnim : 1;
    TAnimSet() {}
    TAnimSet(const TAnimSet& o) : UpdateMode(o.UpdateMode), Anims(o.Anims), InternalAlphas(o.InternalAlphas),
        Speed(o.Speed), Priority(o.Priority), Type(o.Type)
    { Looping = o.Looping; Finished = o.Finished; Forwards = o.Forwards; HadUpdate = o.HadUpdate; OwnedByMainAnim = o.OwnedByMainAnim; }
};
int main()
{
    TAnimSet a; a.UpdateMode = 2; a.Anims.n = 3; a.InternalAlphas.n = 4; a.Speed = 1.5f; a.Priority = 9; a.Type = 6;
    a.Looping = true; a.Finished = false; a.Forwards = true; a.HadUpdate = false; a.OwnedByMainAnim = true;
    TAnimSet b(a);
    if (b.UpdateMode != 2 || b.Anims.n != 3 || b.InternalAlphas.n != 4 || b.Speed != 1.5f || b.Priority != 9 || b.Type != 6 ||
        !b.Looping || b.Finished || !b.Forwards || b.HadUpdate || !b.OwnedByMainAnim || g_vecCopies != 2)
    { std::printf("00710b80_TEST FAIL\n"); return 1; }
    std::printf("00710b80_TEST PASS\n");
    return 0;
}