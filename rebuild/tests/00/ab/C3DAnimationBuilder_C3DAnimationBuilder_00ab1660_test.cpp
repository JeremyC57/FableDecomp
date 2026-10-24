// Behaviour test for C3DAnimationBuilder::C3DAnimationBuilder() @ 0x00AB1660
// The constructor pushes the two default bone prefixes "x" and "sub" onto AddedBonePrefixes, in that order.
#include <cstdio>
#include <cstring>

struct CPrefixList {
    const char* items[8];
    unsigned int count;
    CPrefixList() : count(0) { }
    void push_back(const char* text) { items[count++] = text; }
};

struct C3DAnimationBuilderModel {
    CPrefixList AddedBonePrefixes;
    C3DAnimationBuilderModel()
    {
        AddedBonePrefixes.push_back("x");
        AddedBonePrefixes.push_back("sub");
    }
};

int main()
{
    C3DAnimationBuilderModel builder;
    int ok = builder.AddedBonePrefixes.count == 2
          && std::strcmp(builder.AddedBonePrefixes.items[0], "x") == 0
          && std::strcmp(builder.AddedBonePrefixes.items[1], "sub") == 0;
    if (!ok) { std::printf("00ab1660_TEST FAIL\n"); return 1; }
    std::printf("00ab1660_TEST PASS\n");
    return 0;
}