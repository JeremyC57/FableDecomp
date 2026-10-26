// CUserProfileManager::GetMaxRefreshRate() -- highest refresh-rate option index (0..3) the display's maximum refresh rate allows; 3 when the maximum is unknown (-1)
// Retail: Fable.exe @ 0x00406890, 51 bytes (+ int3 padding), `this` unused. No callees.
// Data: the display's maximum refresh rate @ 0x01375088 (-1 = unknown), the option table {60, 75, 85, 100} Hz @ 0x0122DE30.
// Shape: the known-maximum case is the `if` body (the `return 3` fall-through is placed last); the index runs from -1 and is incremented for every table entry not above the maximum (`jg` = signed
// compare, the counter strength-reduced to a byte offset with `jb`); `best == -1 ? 0 : best` is the branch-free
// `cmp ecx,-1 ; sete al ; dec eax ; and eax,ecx`.
extern long g_maxDisplayRefreshRate;            // 0x01375088
extern const long g_refreshRateOptions[4];      // 0x0122DE30

class CUserProfileManager { public:
    unsigned long GetMaxRefreshRate();
};

unsigned long CUserProfileManager::GetMaxRefreshRate()
{
    if (g_maxDisplayRefreshRate != -1) {
        long best = -1;
        for (unsigned long i = 0; i < 4; ++i) {
            if (g_refreshRateOptions[i] > g_maxDisplayRefreshRate)
                break;
            ++best;
        }
        return best == -1 ? 0 : best;
    }
    return 3;
}