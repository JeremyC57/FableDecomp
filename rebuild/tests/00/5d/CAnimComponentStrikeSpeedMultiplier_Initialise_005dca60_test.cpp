// Behaviour test for CAnimComponentStrikeSpeedMultiplier::Initialise (0x005DCA60): reset to the
// default through the virtual SetDefaultValue, parse one float from the text, return true.
#include <cstdio>
#include <cstdlib>

struct CStringParser {
    const char* String;
    CStringParser(const char* text) : String(text) {}
};

template <class T>
struct CPersistTraits {
    void TransferIn(CStringParser& parser, T* value)
    {
        if (parser.String[0] != 0)
            *value = (T)std::atof(parser.String);
    }
};

struct CAnimComponentStrikeSpeedMultiplier {
    float AnimationSpeedMultiplier;
    int resets;
    virtual ~CAnimComponentStrikeSpeedMultiplier() {}
    virtual void SetDefaultValue() { ++resets; AnimationSpeedMultiplier = 1.0f; }
    bool Initialise(const char* text)
    {
        SetDefaultValue();
        CStringParser parser(text);
        CPersistTraits<float>().TransferIn(parser, &AnimationSpeedMultiplier);
        return true;
    }
};

int main()
{
    CAnimComponentStrikeSpeedMultiplier c;
    c.AnimationSpeedMultiplier = 7.0f;
    c.resets = 0;
    if (!c.Initialise("2.5") || c.AnimationSpeedMultiplier != 2.5f || c.resets != 1) {
        std::printf("005dca60_TEST FAIL\n"); return 1;
    }
    if (!c.Initialise("") || c.AnimationSpeedMultiplier != 1.0f || c.resets != 2) {
        std::printf("005dca60_TEST FAIL\n"); return 1;
    }
    std::printf("005dca60_TEST PASS\n");
    return 0;
}