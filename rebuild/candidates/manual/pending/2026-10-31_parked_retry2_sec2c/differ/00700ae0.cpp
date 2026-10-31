// COpinionReactionManager::GetOpinions(...) const -- HEAD DRAFT (first loop of 1322 bytes): with no override and a source
// Grade: DIFFER(234v1322) 385 diff lines (triage-only 0v1322/415): head draft of the guards and the five-axis clamp loop (pointer-select clamps to +-2 / +-1, GetHeroStatsInOpinionFormat 0x007009D0, source modifier 0x006817C0); TC 0x23/0x22 blocks and the override/no-source paths not drafted (retry wave 2 sec2c)
// creature, clamp each of the five opinion axes to +-2 (pointer-select ternaries through bound locals), add the source's
// modified hero-stat term for the axis and store the sum clamped to +-1; the CTCGuard/TC 0x23/0x22 modifier blocks, the
// no-source path (0x00700E77) and the recursive override path (0x00700EDF) are not drafted.
// Leaf: GetOpinions
// Retail: Fable.exe (TLC, VC7.1) @ 0x00700AE0, 1322 bytes, `ret 0Ch`.
struct COpinionSource { unsigned char _pad[0x48]; float* Opinions; float Modify(long axis, float value) const; };   // 0x006817C0
struct COpinionVector { float* Data; };
template <class T> inline const T& GFClampRef(const T& value, const T& low, const T& high)
{
    return value < low ? low : (high < value ? high : value);
}
class COpinionReactionManager {
public:
    float GetHeroStatsInOpinionFormat(long axis) const;                  // 0x007009D0
    void GetOpinions(const COpinionSource* source, COpinionVector* out, const void* override) const;
};
void COpinionReactionManager::GetOpinions(const COpinionSource* source, COpinionVector* out, const void* override) const
{
    if (override)
        return;
    if (!source)
        return;
    const float minus2 = -2.0f, plus2 = 2.0f, minus1 = -1.0f, plus1 = 1.0f;
    for (long i = 0; i < 5; ++i) {
        float value = source->Opinions[i];
        const float& clamped = GFClampRef(value, minus2, plus2);
        float sum = source->Modify(i, GetHeroStatsInOpinionFormat(i)) + clamped;
        out->Data[i] = GFClampRef(sum, minus1, plus1);
    }
}
