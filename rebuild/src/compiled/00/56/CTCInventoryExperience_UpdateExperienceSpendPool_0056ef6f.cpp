// CTCInventoryExperience::UpdateExperienceSpendPool(long pool, long amount) -- moves `amount` of pending experience spend between a category pool and the general pool (index 3): a positive amount is taken from the category first, then from the general pool, then the remainder is split; a negative amount refunds the general pool up to its initial value and pushes the overflow back into the category
// Retry 10b (combined sweeps): the pool vector is read through an inline `begin()` accessor (FIXES B-15) and the category entry is a const REFERENCE to its address (`ExperienceSpendInfo* const& category = &pool.begin()[index];`, B-12): retail's read-modify-write keeps (`mov ebx,[edi] ; cmp ; ... sub [edi]`) instead of the forwarded compare value.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056EF6F, length 206 bytes. Size TU (`#pragma optimize("s", on)`: `and dword ptr [edx+eax],0`).
// Callees: none. Model: the retail ExperienceSpendValuesPool vector at +0x1E0 (12-byte STLport vector of 12-byte {current, initial,
// levelUp} records, as the landed ResetExperienceSpendValues 0x0056F03D; the engine header carries the PDB's 16-byte containers).
// Shape: the amount (second argument) is loaded first and kept in eax; the pool index is strength-reduced once to a byte offset
// (`lea edx,[edx+edx*2] ; shl edx,2`) and the vector start is re-read after every store through it (the element store may alias
// the start pointer). The negative arm takes `abs(amount)` (`cdq ; xor ; sub`). Every `if` arm ends in the shared epilogue.
#pragma optimize("s", on)
#include <stdlib.h>
enum { EXPERIENCE_POOL_GENERAL = 3 };
struct ExperienceSpendInfo {
    long currentSpend;      // +0x0
    long initialSpend;      // +0x4
    long levelUpSpend;      // +0x8
};
struct SSpendInfoVector {
    ExperienceSpendInfo* _M_start;
    ExperienceSpendInfo* begin() const { return _M_start; }
    ExperienceSpendInfo* _M_finish;
    ExperienceSpendInfo* _M_end_of_storage;
};
class CTCInventoryExperience { public:
    unsigned char _pad_000[0x1E0];
    SSpendInfoVector ExperienceSpendValuesPool;     // +0x1E0
    void UpdateExperienceSpendPool(long pool, long amount);
};
void CTCInventoryExperience::UpdateExperienceSpendPool(long pool, long amount)
{
    if (amount > 0) {
        ExperienceSpendInfo* const& category = &ExperienceSpendValuesPool.begin()[pool];
        long categorySpend = category->currentSpend;
        if (categorySpend >= amount) {
            category->currentSpend -= amount;
        } else if (categorySpend == 0 && ExperienceSpendValuesPool.begin()[EXPERIENCE_POOL_GENERAL].currentSpend >= amount) {
            ExperienceSpendValuesPool.begin()[EXPERIENCE_POOL_GENERAL].currentSpend -= amount;
        } else if (ExperienceSpendValuesPool.begin()[EXPERIENCE_POOL_GENERAL].currentSpend
                   > amount - ExperienceSpendValuesPool.begin()[pool].currentSpend) {
            ExperienceSpendValuesPool.begin()[EXPERIENCE_POOL_GENERAL].currentSpend
                += ExperienceSpendValuesPool.begin()[pool].currentSpend - amount;
            ExperienceSpendValuesPool.begin()[pool].currentSpend = 0;
        }
    } else {
        amount = abs(amount);
        if (ExperienceSpendValuesPool.begin()[EXPERIENCE_POOL_GENERAL].currentSpend + amount
            <= ExperienceSpendValuesPool.begin()[EXPERIENCE_POOL_GENERAL].initialSpend) {
            ExperienceSpendValuesPool.begin()[EXPERIENCE_POOL_GENERAL].currentSpend += amount;
        } else if (ExperienceSpendValuesPool.begin()[EXPERIENCE_POOL_GENERAL].initialSpend + amount
                   > ExperienceSpendValuesPool.begin()[EXPERIENCE_POOL_GENERAL].currentSpend) {
            long overflow = ExperienceSpendValuesPool.begin()[EXPERIENCE_POOL_GENERAL].currentSpend
                          - ExperienceSpendValuesPool.begin()[EXPERIENCE_POOL_GENERAL].initialSpend;
            overflow += amount;
            ExperienceSpendValuesPool.begin()[pool].currentSpend += overflow;
            ExperienceSpendValuesPool.begin()[EXPERIENCE_POOL_GENERAL].currentSpend
                = ExperienceSpendValuesPool.begin()[EXPERIENCE_POOL_GENERAL].initialSpend;
        } else if (ExperienceSpendValuesPool.begin()[EXPERIENCE_POOL_GENERAL].currentSpend
                   == ExperienceSpendValuesPool.begin()[EXPERIENCE_POOL_GENERAL].initialSpend) {
            ExperienceSpendValuesPool.begin()[pool].currentSpend += amount;
        }
    }
}