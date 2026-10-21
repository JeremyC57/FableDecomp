// NUISystem::CManager::SetMetaLayer(long metaLayer) -- clamp the meta layer to [0, 4], store it (+0x7c) and rebuild the UI-layer -> engine 2D layer map (+0x70): meta layer 0 gets the fixed table 0x40..0x51 for keys -3..13, any other meta layer maps keys -3..7 onto a run starting at the per-meta-layer base
// Grade: DIFFER(485v485) -- scheduling only: retail hoists the `or [ebp+8],-1` / `and [ebp+8],0` key stores of keys -1 and 0 above the previous value store
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041E1CD, length 485 bytes. Frame-pointer size TU: `#pragma optimize("y", off)` + `("s", on)`.
// Callees: the layer tree's operator[] @ 0x0042D246 (const long& -> long&; see the landed GetLayer 0x0041E3B2). Data: the per-meta-layer
//   base table @ 0x01230118.
// Layout (retail NUISystem::CManager, as in the landed GetLayer 0x0041E3B2): layer tree +0x70, meta layer +0x7c; engine/CManager.h is the
//   PDB view (Layers +0x94, MetaLayer +0xa4), so the class is modelled locally.
// Shape: the clamp constants 4 and 5 are hoisted into edi/ebx (`push 4 ; pop edi`) and reused as the keys 4 and 5; every key is a
//   const-ref temporary homed in the dead argument slot [ebp+8].
#pragma optimize("y", off)
#pragma optimize("s", on)
enum EEngine2DLayer { E2DL_FIRST = 0x40 };
struct CLayerTree {
    EEngine2DLayer& operator[](const long& key);   // 0x0042D246
    unsigned char _d[8];
};
extern long g_metaLayerBase[5];                    // 0x01230118

namespace NUISystem {
struct CManager {
    unsigned char _pad[0x70];
    CLayerTree    m_layers;                        // +0x70
    unsigned char _pad78[4];
    long          m_metaLayer;                     // +0x7c
    void SetMetaLayer(long metaLayer);
};

void CManager::SetMetaLayer(long metaLayer)
{
    if (metaLayer < 0)
        metaLayer = 0;
    else if ((unsigned long)metaLayer >= 5)
        metaLayer = 4;
    m_metaLayer = metaLayer;
    if (metaLayer == 0) {
        m_layers[-3] = (EEngine2DLayer)0x40;
        m_layers[-2] = (EEngine2DLayer)0x41;
        m_layers[-1] = (EEngine2DLayer)0x42;
        m_layers[0] = (EEngine2DLayer)0x43;
        m_layers[1] = (EEngine2DLayer)0x44;
        m_layers[2] = (EEngine2DLayer)0x45;
        m_layers[3] = (EEngine2DLayer)0x46;
        m_layers[4] = (EEngine2DLayer)0x47;
        m_layers[5] = (EEngine2DLayer)0x49;
        m_layers[6] = (EEngine2DLayer)0x4a;
        m_layers[7] = (EEngine2DLayer)0x4b;
        m_layers[8] = (EEngine2DLayer)0x4c;
        m_layers[9] = (EEngine2DLayer)0x4d;
        m_layers[10] = (EEngine2DLayer)0x4e;
        m_layers[11] = (EEngine2DLayer)0x4f;
        m_layers[12] = (EEngine2DLayer)0x50;
        m_layers[13] = (EEngine2DLayer)0x51;
    } else {
        long base = g_metaLayerBase[metaLayer];
        for (long i = -3; i <= 7; i++) {
            long key = i;
            m_layers[key] = (EEngine2DLayer)(base + i + 2);
        }
    }
}
}
