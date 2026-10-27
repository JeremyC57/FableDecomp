// behaviour test for 0x004E9F47 vector<NParticleAttachment::CParticleAttachmentInfo>::resize (model: shrink erases the tail, growth appends copies)
#include <cstdio>
#include <vector>
int main()
{
    std::vector<int> v(5, 1);
    v.resize(2, 7);
    int ok = v.size() == 2 && v[1] == 1;
    v.resize(4, 7);
    ok = ok && v.size() == 4 && v[2] == 7 && v[3] == 7;
    if (!ok) { std::printf("004e9f47_TEST FAIL\n"); return 1; }
    std::printf("004e9f47_TEST PASS\n");
    return 0;
}