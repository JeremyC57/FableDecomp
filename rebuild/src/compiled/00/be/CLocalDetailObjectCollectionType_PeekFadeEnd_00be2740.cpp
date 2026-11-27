// Leaf: ?PeekFadeEnd@CLocalDetailObjectCollectionType@@QBEMXZ
// CLocalDetailObjectCollectionType::PeekFadeEnd() -- the fade-end distance at +0x20 passed through the local-detail generator's video-option adjustment (global 0x01436E88, ModifyFadeDistanceForVideoOptions @ 0x00B597E0) for this collection's detail class bit (1 << the index at +0x44)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE2740, length 27 bytes.
// Callees: CEngineLocalDetailGenerator::ModifyFadeDistanceForVideoOptions @ 0x00B597E0. Shape: no frame, the shift through cl, `ret`.
class CEngineLocalDetailGenerator { public:
    float ModifyFadeDistanceForVideoOptions(unsigned long detailClassMask, float distance);   // 0x00B597E0
};
// NF479 (Codex pass 29 name fixes): ?GPLocalDetailGenerator@NEngineInternal@@3PAVCEngineLocalDetailGenerator@@A (raw Ego_r / FableWin /
//   Ego_d PDB publics; was g_pLocalDetailGenerator_01436e88).
namespace NEngineInternal { extern CEngineLocalDetailGenerator* GPLocalDetailGenerator; }   // 0x01436E88

class CLocalDetailObjectCollectionType { public:
    unsigned char _pad00[0x20];
    float         m_fadeEnd;                       // +0x20
    unsigned char _pad24[0x20];
    unsigned long m_detailClass;                   // +0x44
    float PeekFadeEnd() const;
};

float CLocalDetailObjectCollectionType::PeekFadeEnd() const
{
    return NEngineInternal::GPLocalDetailGenerator->ModifyFadeDistanceForVideoOptions(1 << m_detailClass, m_fadeEnd);
}