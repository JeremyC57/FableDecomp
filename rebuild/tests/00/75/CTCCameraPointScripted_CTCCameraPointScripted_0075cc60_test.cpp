#include <cstdio>

// Model of CTCCameraPointScripted::CTCCameraPointScripted(CThing*) @ 0x0075CC60: StartPos zeroed, the other vectors
// left as they were, UID / acquisition type / transition time zeroed, the three flags cleared.
struct CThing { int id; };
struct CTCCameraPointDefinitionBase { CThing* Owner; CTCCameraPointDefinitionBase(CThing* t) : Owner(t) { } virtual ~CTCCameraPointDefinitionBase() { } };
struct C3DVector { float X, Y, Z; C3DVector() { } C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
struct CTCCameraPointScripted : CTCCameraPointDefinitionBase {
    C3DVector StartPos, EndPos, LookPos, LookPosEnd; unsigned long long ThingUID; long PAT; float TransitionTime;
    bool LookDirSet, StartPosSet, EndPosSet;
    CTCCameraPointScripted(CThing* thing)
        : CTCCameraPointDefinitionBase(thing), StartPos(0.0f, 0.0f, 0.0f), ThingUID(0), PAT(0), TransitionTime(0.0f),
          LookDirSet(false), StartPosSet(false), EndPosSet(false) { }
};

int main()
{
    CThing t; t.id = 9;
    CTCCameraPointScripted* p = new CTCCameraPointScripted(&t);
    bool ok = p->Owner == &t && p->StartPos.X == 0.0f && p->StartPos.Y == 0.0f && p->StartPos.Z == 0.0f &&
              p->ThingUID == 0 && p->PAT == 0 && p->TransitionTime == 0.0f && !p->LookDirSet && !p->StartPosSet && !p->EndPosSet;
    delete p;
    if (!ok) { std::printf("0075cc60_TEST FAIL\n"); return 1; }
    std::printf("0075cc60_TEST PASS\n");
    return 0;
}