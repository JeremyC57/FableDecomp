#pragma optimize("y",off)
// CIEngine::PeekGlobalDiffuseColour @ 00b28b40

class CRGBFloatColour { public:
    float r, g, b, a;
    CRGBFloatColour(float rr, float gg, float bb, float aa) : r(rr), g(gg), b(bb), a(aa) {}
    CRGBFloatColour() {}
};

class CIEngine { public:
    CRGBFloatColour PeekGlobalDiffuseColour();
    char dummy;
};

CRGBFloatColour CIEngine::PeekGlobalDiffuseColour()
{
    return CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f);
}