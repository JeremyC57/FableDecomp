// Behaviour test for 0x00778610 GFGetValPositionInLimitsLooping(value, start, end, min, max, position&)
#include <cstdio>

static bool GetPos(float value, float start, float end, float minLimit, float maxLimit, float& position)
{
    if (start < end) {
        if (value >= start && value <= end) {
            position = (value - start) / (end - start);
            return true;
        }
        return false;
    }
    float tail = maxLimit - end;
    float offset = (end - minLimit) + tail;
    float range = (end - minLimit) + tail;
    if (value >= minLimit && value < end) {
        position = offset / range;
        return true;
    }
    if (value >= start && value <= maxLimit) {
        position = (value - start) / range;
        return true;
    }
    return false;
}

static bool Near(float a, float b) { float d = a - b; return d < 0.00001f && d > -0.00001f; }

int main()
{
    float p = -1.0f;
    // plain window [6, 18] on a 0..24 clock
    if (!GetPos(12.0f, 6.0f, 18.0f, 0.0f, 24.0f, p) || !Near(p, 0.5f)) { std::printf("00778610_TEST FAIL\n"); return 1; }
    if (!GetPos(6.0f, 6.0f, 18.0f, 0.0f, 24.0f, p) || !Near(p, 0.0f)) { std::printf("00778610_TEST FAIL\n"); return 1; }
    if (!GetPos(18.0f, 6.0f, 18.0f, 0.0f, 24.0f, p) || !Near(p, 1.0f)) { std::printf("00778610_TEST FAIL\n"); return 1; }
    p = -1.0f;
    if (GetPos(19.0f, 6.0f, 18.0f, 0.0f, 24.0f, p) || p != -1.0f) { std::printf("00778610_TEST FAIL\n"); return 1; }
    // wrapping window [20, 4]: range = (4 - 0) + (24 - 4) = 24
    if (!GetPos(2.0f, 20.0f, 4.0f, 0.0f, 24.0f, p) || !Near(p, 1.0f)) { std::printf("00778610_TEST FAIL\n"); return 1; }   // retail: range / range
    if (!GetPos(22.0f, 20.0f, 4.0f, 0.0f, 24.0f, p) || !Near(p, 2.0f / 24.0f)) { std::printf("00778610_TEST FAIL\n"); return 1; }
    p = -1.0f;
    if (GetPos(4.0f, 20.0f, 4.0f, 0.0f, 24.0f, p) || p != -1.0f) { std::printf("00778610_TEST FAIL\n"); return 1; }        // end excluded
    if (GetPos(10.0f, 20.0f, 4.0f, 0.0f, 24.0f, p) || p != -1.0f) { std::printf("00778610_TEST FAIL\n"); return 1; }
    std::printf("00778610_TEST PASS\n");
    return 0;
}