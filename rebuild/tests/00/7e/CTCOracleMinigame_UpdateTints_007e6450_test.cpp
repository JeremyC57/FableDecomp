#include <cstdio>
#include <math.h>
#include <string.h>
// Model of UpdateTints' logic: pending-input handling and the tint fade.
struct Model {
    long SymbolsFiring[4]; long FramesFired[4]; long Output[32]; long NumOutputsFired;
    long Pending[8]; long head, tail; bool hasExpression; long frame; long fps; float TimeBetweenOutputs;
    unsigned long TintRed, TintGreen, TintBlue; int quits, removes; long lastExpr; unsigned char R,G,B;
    bool OutputsFiring() { for (int i = 0; i < 4; i++) { if (SymbolsFiring[i] != 0) return true; } return false; }  // as landed 0x007E5D00
    void Update() {
        if (!OutputsFiring() && !hasExpression && (tail - head)) {
            long input = Pending[head]; long e = -1;
            switch (input) { case 0: e = 0; break; case 2: e = 1; break; case 3: e = 2; break; case 1: e = 3; break; }
            lastExpr = e; ++head;
            if (Output[NumOutputsFired++] != input) ++quits;
            FramesFired[input] = frame; ++SymbolsFiring[input];
        }
        for (long i = 0; i < 4; ++i) {
            if (!SymbolsFiring[i]) continue;
            float t = (float)(frame - FramesFired[i]) / (fps * TimeBetweenOutputs);
            if (t > 1.0f) { if (--SymbolsFiring[i]) FramesFired[i] = frame; ++removes; }
            else { float s = (float)sin(t * 0.5f * (double)6.2831855f);
                R = (unsigned char)(255.0f - TintRed * s); G = (unsigned char)(255.0f - TintGreen * s); B = (unsigned char)(255.0f - TintBlue * s); }
        }
    }
};
int main()
{
    Model m; memset(&m, 0, sizeof(m));
    m.fps = 30; m.TimeBetweenOutputs = 1.0f; m.TintRed = 200; m.TintGreen = 100; m.TintBlue = 0;
    m.Output[0] = 2; m.Output[1] = 1; m.Pending[m.tail++] = 2; m.Pending[m.tail++] = 3;
    m.frame = 100; m.Update();
    // Retail jump table 0x007E6734: input 2 posts expression 1, input 3 posts expression 2 (input 1 posts 3).
    bool ok = m.lastExpr == 1 && m.SymbolsFiring[2] == 1 && m.quits == 0 && m.R == 255 && (m.tail - m.head) == 1;
    m.frame = 115; m.Update();           // t = 0.5 -> sin(pi/2) = 1
    ok = ok && m.R == 55 && m.G == 155 && m.B == 255 && (m.tail - m.head) == 1;
    m.frame = 131; m.Update();           // t > 1 -> removed
    ok = ok && m.removes == 1 && m.SymbolsFiring[2] == 0;
    m.frame = 132; m.Update();           // next input 3 != Output[1] -> quit
    ok = ok && m.quits == 1 && m.lastExpr == 2 && m.SymbolsFiring[3] == 1;
    if (!ok) { std::printf("007e6450_TEST FAIL\n"); return 1; }
    std::printf("007e6450_TEST PASS\n");
    return 0;
}