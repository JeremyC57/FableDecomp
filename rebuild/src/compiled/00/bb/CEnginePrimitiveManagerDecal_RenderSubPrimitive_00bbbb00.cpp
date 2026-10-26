// CEnginePrimitiveManagerDecal::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType) -- when the decal primitive (at sub+0xc) is a landscape decal (byte +0x48), render it through the landscape path
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBBB00, length 23 bytes.
// Callees: RenderLandscapeDecal @ 0x00BBAC60 (on `this`, ecx passed straight through; the primitive). Shape: the byte is tested through dl, the pass argument is unused.
class CEngineInternalPrimitiveDecal { public:
    unsigned char _pad[0x48];
    bool          m_landscape;                     // +0x48
};

class CEngineSubPrimitiveBase { public:
    unsigned char                  _pad[0xc];
    CEngineInternalPrimitiveDecal* m_pPrimitive;   // +0x0c
};

class CEnginePrimitiveManagerDecal { public:
    void RenderLandscapeDecal(CEngineInternalPrimitiveDecal* decal);   // 0x00BBAC60
    void RenderSubPrimitive(CEngineSubPrimitiveBase& sub, long pass);
};

void CEnginePrimitiveManagerDecal::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, long pass)
{
    CEngineInternalPrimitiveDecal* decal = sub.m_pPrimitive;
    if (decal->m_landscape)
        RenderLandscapeDecal(decal);
}