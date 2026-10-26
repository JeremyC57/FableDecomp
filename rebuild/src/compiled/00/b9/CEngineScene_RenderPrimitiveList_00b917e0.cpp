// CEngineScene::RenderPrimitiveList(CEngineInternalPrimitiveBase* first, unsigned long layerMask, long, long) -- walk the primitive chain: a primitive whose render-layer mask (+0x30) meets the mask is handed to its type's manager (slot 8) and the walk continues through its +0x38 link, otherwise through its +0x40 link
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B917E0, length 55 bytes.
// Callees: virtual slot 8 (+0x20) of the primitive's manager (`g_pEngine->m_managers[type]` @ 0x01436E84 + 0x10). Shape: `test esi,esi ; je` skips an empty chain, the
// mask stays in edi, the loop head is aligned with `mov edi,edi`; the two trailing arguments and `this` are unused (`ret 10h`).
class CEngineInternalPrimitiveBase;

struct CEnginePrimitiveManagerBase_00b917e0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void RenderPrimitive(CEngineInternalPrimitiveBase* primitive);   // slot 8 (+0x20)
};

struct CEngine_01436e84 {
    unsigned char                        _pad[0x10];
    CEnginePrimitiveManagerBase_00b917e0* m_managers[1];   // +0x10, indexed by primitive type
};
extern CEngine_01436e84* g_pEngine_01436e84;       // 0x01436E84

class CEngineInternalPrimitiveBase { public:
    unsigned char                 _pad0[8];
    long                          m_type;          // +0x08
    unsigned char                 _pad0c[0x24];
    unsigned long                 RenderLayerMask; // +0x30
    unsigned char                 _pad34[4];
    CEngineInternalPrimitiveBase* m_pNextRendered; // +0x38
    unsigned char                 _pad3c[4];
    CEngineInternalPrimitiveBase* m_pNextSkipped;  // +0x40
};

class CEngineScene { public:
    void RenderPrimitiveList(CEngineInternalPrimitiveBase* first, unsigned long layerMask, long a, long b);
};

void CEngineScene::RenderPrimitiveList(CEngineInternalPrimitiveBase* first, unsigned long layerMask, long a, long b)
{
    CEngineInternalPrimitiveBase* primitive = first;
    while (primitive) {
        if (primitive->RenderLayerMask & layerMask) {
            g_pEngine_01436e84->m_managers[primitive->m_type]->RenderPrimitive(primitive);
            primitive = primitive->m_pNextRendered;
        } else {
            primitive = primitive->m_pNextSkipped;
        }
    }
}