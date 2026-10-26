// CUserProfileManager::ClampVideoOption(float* out, float value, float fallback, float limit, float secondaryLimit) -- pick a video option value: the explicit value unless it is -1 (unset), else the fallback clamped to the first set limit, else 0
// Leaf: ClampVideoOption
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004068D0, length 202 bytes (`ret 14h`, `this` unused).
// Constant: 0x0122DEE0 = -1.0f (the "unset" marker; `fucompp ; test ah,44h ; jnp` is `!=`).
// Shape: one `__forceinline` ClampToLimit helper expanded in both fallback arms: the stored arm forwards the fallback
// into the compare, the other reads `*out`, and VC7.1 cross-jumps the two copies' compare/store tails; each clamp is
// `if (*out > limit) *out = limit;` (an integer move of the limit).
__forceinline void ClampToLimit(float* out, float limit, float secondaryLimit)
{
    if (limit != -1.0f) {
        if (*out > limit)
            *out = limit;
    }
    else if (secondaryLimit != -1.0f) {
        if (*out > secondaryLimit)
            *out = secondaryLimit;
    }
    else
        *out = 0.0f;
}

class CUserProfileManager { public:
    void ClampVideoOption(float* out, float value, float fallback, float limit, float secondaryLimit);
};

void CUserProfileManager::ClampVideoOption(float* out, float value, float fallback, float limit, float secondaryLimit)
{
    if (value != -1.0f)
        *out = value;
    else if (fallback != -1.0f) {
        *out = fallback;
        ClampToLimit(out, limit, secondaryLimit);
    }
    else
        ClampToLimit(out, limit, secondaryLimit);
}