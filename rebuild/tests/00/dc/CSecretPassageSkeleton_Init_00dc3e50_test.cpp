// Behaviour test for CSecretPassageSkeleton::Init (0x00DC3E50): returned[] and collected_all cleared, the four object names assigned.
#include <cstdio>
#include <cstring>
struct CCharString {
    const char* m_text;
    CCharString& operator=(const char* text) { m_text = text; return *this; }
};
struct CSecretPassageSkeleton {
    bool returned[4];
    CCharString skeleton_object[4];
    bool collected_all;
    virtual ~CSecretPassageSkeleton() {}
    void Init()
    {
        for (long i = 0; i < 4; ++i)
            returned[i] = false;
        skeleton_object[0] = "OBJECT_CEREMONIAL_SKELETON_SWORD";
        skeleton_object[1] = "OBJECT_CEREMONIAL_SKELETON_SHIELD";
        skeleton_object[2] = "OBJECT_CEREMONIAL_SKELETON_ARMOUR";
        skeleton_object[3] = "OBJECT_CEREMONIAL_SKELETON_HELMET";
        collected_all = false;
    }
};
int main()
{
    CSecretPassageSkeleton s;
    for (int i = 0; i < 4; ++i) { s.returned[i] = true; s.skeleton_object[i].m_text = ""; }
    s.collected_all = true;
    s.Init();
    if (s.returned[0] || s.returned[1] || s.returned[2] || s.returned[3] || s.collected_all ||
        std::strcmp(s.skeleton_object[0].m_text, "OBJECT_CEREMONIAL_SKELETON_SWORD") != 0 ||
        std::strcmp(s.skeleton_object[1].m_text, "OBJECT_CEREMONIAL_SKELETON_SHIELD") != 0 ||
        std::strcmp(s.skeleton_object[2].m_text, "OBJECT_CEREMONIAL_SKELETON_ARMOUR") != 0 ||
        std::strcmp(s.skeleton_object[3].m_text, "OBJECT_CEREMONIAL_SKELETON_HELMET") != 0) { std::printf("00dc3e50_TEST FAIL\n"); return 1; }
    std::printf("00dc3e50_TEST PASS\n");
    return 0;
}