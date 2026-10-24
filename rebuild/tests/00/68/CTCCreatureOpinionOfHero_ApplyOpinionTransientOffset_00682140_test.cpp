// Behaviour test for 0x00682140 CTCCreatureOpinionOfHero::ApplyOpinionTransientOffset: with a personality def (and
// not told to ignore it) the queued offset is the plain offset adapted by the traits of that opinion; without a def,
// or when ignorePersonality is set, the plain offset is queued.  Each call appends exactly one entry.
#include <cstdio>
#include <list>

typedef int EOpinion;
class COpinionPersonalityTraits { public: float Scale; };

class COpinionTransientOffset {
public:
    EOpinion Opinion;
    float    Peak;
    int      FramesToRunIn;
    int      FramesToRunOut;
    float    Final;
    COpinionTransientOffset(EOpinion opinion, float peakOffset, int framesToRunIn, int framesToRunOut, float finalOffset)
        : Opinion(opinion), Peak(peakOffset), FramesToRunIn(framesToRunIn), FramesToRunOut(framesToRunOut),
          Final(finalOffset) { }
    // stand-in for the personality adaptation 0x00681680 (scales the peak and the persisting offset)
    COpinionTransientOffset(const COpinionTransientOffset& offset, const COpinionPersonalityTraits& traits)
        : Opinion(offset.Opinion), Peak(offset.Peak * traits.Scale), FramesToRunIn(offset.FramesToRunIn),
          FramesToRunOut(offset.FramesToRunOut), Final(offset.Final * traits.Scale) { }
};

class COpinionPersonalityDef { public: COpinionPersonalityTraits* PersonalityTraits; };
template <class T> class CDefPointer {
public:
    T* Object;
    T* operator->() const { return Object; }
    operator T*() const { return Object; }
};

class CTCCreatureOpinionOfHero {
public:
    CDefPointer<COpinionPersonalityDef> PPersonalityDef;
    std::list<COpinionTransientOffset>  TransientOffsets;
    void ApplyOpinionTransientOffset(EOpinion opinion, float peakOffset, int framesToRunIn, int framesToRunOut,
                                     float finalOffset, bool ignorePersonality)
    {
        const COpinionPersonalityDef* personality;
        if (!ignorePersonality && (personality = PPersonalityDef) != 0) {
            COpinionTransientOffset offset(opinion, peakOffset, framesToRunIn, framesToRunOut, finalOffset);
            TransientOffsets.push_back(COpinionTransientOffset(offset, personality->PersonalityTraits[opinion]));
        } else
            TransientOffsets.push_back(
                COpinionTransientOffset(opinion, peakOffset, framesToRunIn, framesToRunOut, finalOffset));
    }
};

int main()
{
    COpinionPersonalityTraits traits[5] = { { 1.0f }, { 2.0f }, { 0.5f }, { 3.0f }, { 4.0f } };
    COpinionPersonalityDef def;
    def.PersonalityTraits = traits;
    CTCCreatureOpinionOfHero tc;
    tc.PPersonalityDef.Object = &def;

    tc.ApplyOpinionTransientOffset(1, 10.0f, 5, 7, 2.0f, false);    // scaled by traits[1] (x2)
    tc.ApplyOpinionTransientOffset(3, 10.0f, 5, 7, 2.0f, true);     // ignored personality: plain
    tc.PPersonalityDef.Object = 0;
    tc.ApplyOpinionTransientOffset(2, 8.0f, 1, 2, 4.0f, false);     // no def: plain
    if (tc.TransientOffsets.size() != 3) { std::printf("00682140_TEST FAIL\n"); return 1; }
    std::list<COpinionTransientOffset>::const_iterator it = tc.TransientOffsets.begin();
    if (it->Opinion != 1 || it->Peak != 20.0f || it->Final != 4.0f || it->FramesToRunIn != 5 || it->FramesToRunOut != 7) {
        std::printf("00682140_TEST FAIL\n"); return 1;
    }
    ++it;
    if (it->Opinion != 3 || it->Peak != 10.0f || it->Final != 2.0f) { std::printf("00682140_TEST FAIL\n"); return 1; }
    ++it;
    if (it->Opinion != 2 || it->Peak != 8.0f || it->Final != 4.0f || it->FramesToRunIn != 1) {
        std::printf("00682140_TEST FAIL\n"); return 1;
    }
    std::printf("00682140_TEST PASS\n");
    return 0;
}