#include <cstdio>
class CCard { public: void* Thing; long Name; long Value; float InitialRotation; float FinalRotation; float TurnSpeed; float TurnFraction; float MoveSpeed; float MoveFraction; bool Turning; bool FaceDown; bool Moving;
    void Turn() { bool faceDown = FaceDown; Turning = true; TurnFraction = 0.0f; if (faceDown == true) { InitialRotation = 0.5f; FinalRotation = 0.0f; } else { InitialRotation = 0.0f; FinalRotation = 0.5f; } FaceDown = !faceDown; } };

int main()
{
    CCard card; card.Turning = false; card.FaceDown = true; card.TurnFraction = 0.75f;
    card.Turn();
    if (!card.Turning || card.FaceDown || card.TurnFraction != 0.0f || card.InitialRotation != 0.5f || card.FinalRotation != 0.0f) { std::printf("008f0620_TEST FAIL\n"); return 1; }
    card.TurnFraction = 0.3f; card.Turn();
    if (!card.FaceDown || card.TurnFraction != 0.0f || card.InitialRotation != 0.0f || card.FinalRotation != 0.5f) { std::printf("008f0620_TEST FAIL\n"); return 1; }
    std::printf("008f0620_TEST PASS\n");
    return 0;
}