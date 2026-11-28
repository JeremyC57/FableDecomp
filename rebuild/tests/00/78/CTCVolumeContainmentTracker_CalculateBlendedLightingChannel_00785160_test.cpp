// Standalone behavior cases for the blended lighting selection and byte conversion.
#define FABLE_PORTABLE
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
#include <cassert>
#include <cstdio>
#include <utility>
#include <vector>

namespace {
struct Sample {
    long channel;
    float intersection;
    bool appearance;
    bool physics;
};

void Blend(const Sample* samples, unsigned int count, CEnginePrimitiveThemeChannelDesc& desc)
{
    typedef std::pair<long, float> Theme;
    std::vector<Theme> themes;
    const float epsilon = 1.0f / 255.0f;
    float total = 0.0f;
    for (unsigned int n = 0; n != count; ++n) {
        const Sample& sample = samples[n];
        if (!sample.appearance || !sample.physics)
            continue;
        const float amount = sample.intersection;
        if (amount > epsilon) {
            std::vector<Theme>::iterator existing = themes.begin();
            while (existing != themes.end() && existing->first != sample.channel)
                ++existing;
            if (existing != themes.end())
                existing->second += amount;
            else
                themes.push_back(Theme(sample.channel, amount));
        }
        total += amount;
    }
    if (total < 1.0f - epsilon) {
        themes.push_back(Theme(0, 1.0f - total));
        total = 1.0f;
    }
    if (themes.size() > 2) {
        long chosen[3] = {-1,-1,-1};
        for (int position = 0; position != 3; ++position) {
            for (unsigned int candidate = 0; candidate != themes.size(); ++candidate) {
                bool chosen_earlier = false;
                for (int earlier = 0; earlier != position; ++earlier)
                    chosen_earlier = chosen_earlier || chosen[earlier] == (long)candidate;
                if (!chosen_earlier && (chosen[position] < 0 || themes[candidate].second > themes[chosen[position]].second))
                    chosen[position] = candidate;
            }
        }
        const Theme first = themes[chosen[0]];
        Theme second = themes[chosen[1]];
        second.second -= themes[chosen[2]].second;
        total = first.second + second.second;
        themes[0] = first;
        themes[1] = second;
        themes.resize(2);
    }
    desc.ChannelIndex0 = (unsigned char)themes[0].first;
    if (themes.size() == 1) {
        desc.ChannelBlendFactor = 0;
    } else {
        desc.ChannelIndex1 = (unsigned char)themes[1].first;
        double scaled = themes[1].second * 255.0 / total;
        if (scaled < 0.0)
            scaled = 0.0;
        if (scaled > 255.0)
            scaled = 255.0;
        desc.ChannelBlendFactor = (unsigned char)scaled;
    }
}

void Expect(const Sample* samples, unsigned int count, unsigned int first, unsigned int second, unsigned int blend)
{
    CEnginePrimitiveThemeChannelDesc desc;
    desc.ChannelIndex0 = 99;
    desc.ChannelIndex1 = 98;
    desc.ChannelBlendFactor = 97;
    Blend(samples, count, desc);
    assert(desc.ChannelIndex0==first && desc.ChannelIndex1==second && desc.ChannelBlendFactor==blend);
}
}

int main()
{
    Expect(0,0,0,98,0);
    const Sample missing[]={{4,1.0f,false,true},{5,1.0f,true,false}};
    Expect(missing,2,0,98,0);
    const Sample one[]={{12,1.0f,true,true}};
    Expect(one,1,12,98,0);
    const Sample nearly_full[]={{31,254.0f/255.0f,true,true}};
    Expect(nearly_full,1,31,98,0);
    const Sample epsilon[]={{6,1.0f/255.0f,true,true}};
    Expect(epsilon,1,0,98,0);
    const Sample only_ambient_fill[]={{7,0.25f,true,true}};
    Expect(only_ambient_fill,1,7,0,191);
    const Sample repeats[]={{5,0.5f,true,true},{5,0.5f,true,true}};
    Expect(repeats,2,5,98,0);
    const Sample two[]={{3,0.25f,true,true},{8,0.75f,true,true}};
    Expect(two,2,3,8,191);
    const Sample three[]={{9,0.40f,true,true},{17,0.35f,true,true},{12,0.45f,true,true}};
    Expect(three,3,12,9,25);
    const Sample ties[]={{9,0.4f,true,true},{17,0.4f,true,true},{12,0.4f,true,true}};
    Expect(ties,3,9,17,0);
    std::puts("00785160_TEST PASS");
}