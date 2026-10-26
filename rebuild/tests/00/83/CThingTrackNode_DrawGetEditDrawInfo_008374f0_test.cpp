#include <cstdio>
#include <cstring>
class CCharString { public:
    char Text[32];
    CCharString(const char* t, long = -1) { std::strcpy(Text, t); }
    CCharString& operator=(const CCharString& o) { std::strcpy(Text, o.Text); return *this; }
};
CCharString operator+(const CCharString& l, const char* r) { CCharString s(l.Text); std::strcat(s.Text, r); return s; }
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CEditEngineDrawInfo { public:
    float Radius;
    CCharString Fullname;
    CRGBColour Colour;
    bool UsesEffectRadius;
    CEditEngineDrawInfo() : Fullname("x"), Colour(1, 2, 3, 4), UsesEffectRadius(true) {}
};
static bool g_base = true;
class CThingPhysical { public:
    bool DrawGetEditDrawInfo(float, CEditEngineDrawInfo* info) const { info->Radius = 2.0f; return g_base; }
};
class CThingTrackNode : public CThingPhysical { public:
    bool DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const;
};
bool CThingTrackNode::DrawGetEditDrawInfo(float time, CEditEngineDrawInfo* info) const
{
    if (CThingPhysical::DrawGetEditDrawInfo(time, info)) {
        info->Colour = CRGBColour(0x80, 0x80, 0xff, 0xff);
        info->Fullname = CCharString("") + "TrackNode";
        info->UsesEffectRadius = false;
        return true;
    }
    return false;
}
int main()
{
    CThingTrackNode n; CEditEngineDrawInfo info;
    if (!n.DrawGetEditDrawInfo(0.0f, &info) || std::strcmp(info.Fullname.Text, "TrackNode") || info.Colour.R != 0x80
        || info.Colour.G != 0x80 || info.Colour.B != 0xff || info.Colour.A != 0xff || info.UsesEffectRadius) { std::printf("008374f0_TEST FAIL\n"); return 1; }
    CEditEngineDrawInfo info2; g_base = false;
    if (n.DrawGetEditDrawInfo(0.0f, &info2) || std::strcmp(info2.Fullname.Text, "x") || !info2.UsesEffectRadius) { std::printf("008374f0_TEST FAIL\n"); return 1; }
    std::printf("008374f0_TEST PASS\n");
    return 0;
}