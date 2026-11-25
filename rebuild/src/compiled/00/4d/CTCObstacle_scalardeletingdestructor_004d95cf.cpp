#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004D95EB now binds its PDB spelling ??1CTCObstacle@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCObstacle_Destruct@@YIXPAX@Z.
class CTCObstacle { public: virtual ~CTCObstacle(); };
void* __fastcall CTCObstacle_ScalarDeletingDestructor(void* obstacle, int, unsigned int flags)
{
    reinterpret_cast<CTCObstacle*>(obstacle)->CTCObstacle::~CTCObstacle();
    if (flags & 1)
        operator delete(obstacle);
    return obstacle;
}