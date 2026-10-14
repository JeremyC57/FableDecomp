// CLocalDetailObjectCollectionType::PeekFadeEnd() -- the fade-end distance at +0x20 passed through the local-detail generator's video-option adjustment (global 0x01436E88, ModifyFadeDistanceForVideoOptions @ 0x00B597E0) for this collection's detail class bit (1 << the index at +0x44)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE2740, length 27 bytes.
// Callees: CEngineLocalDetailGenerator::ModifyFadeDistanceForVideoOptions @ 0x00B597E0. Shape: no frame, the shift through cl, `ret`.
struct CEngineLocalDetailGenerator {
    float ModifyFadeDistanceForVideoOptions(unsigned long detailClassMask, float distance);   // 0x00B597E0
};
extern CEngineLocalDetailGenerator* g_pLocalDetailGenerator_01436e88;   // 0x01436E88

struct CLocalDetailObjectCollectionType {
    unsigned char _pad00[0x20];
    float         m_fadeEnd;                       // +0x20
    unsigned char _pad24[0x20];
    unsigned long m_detailClass;                   // +0x44
    float PeekFadeEnd();
};

float CLocalDetailObjectCollectionType::PeekFadeEnd()
{
    return g_pLocalDetailGenerator_01436e88->ModifyFadeDistanceForVideoOptions(1 << m_detailClass, m_fadeEnd);
}
