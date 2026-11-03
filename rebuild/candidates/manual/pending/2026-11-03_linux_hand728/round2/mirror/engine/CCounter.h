#ifndef FABLE_ENGINE_CCOUNTER_H
#define FABLE_ENGINE_CCOUNTER_H
// Retail opt-in (Codex pass 12): Ego_r/FableWin original CCounter, size 4;
// BodyGuard native +0x50 is the RelevantWorldFrame used by its idle logic.
class CCounter {
public:
    CCounter();
    bool operator<=(float) const;
    bool operator>=(float) const;
    bool operator<(float) const;
    bool operator>(float) const;
    float GetCounter() const;
#ifdef FABLE_RETAIL_CCOUNTER
    // Codex pass 13 (row 0x00EFCC50): Ego_r / FableWin ??4CCounter@@QAEAAV0@M@Z (out of line).
    CCounter& operator=(float);
#endif
private:
    long GetFrameDelta() const;
    long RelevantWorldFrame;
};
#endif
