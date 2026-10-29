// Behaviour test (0x00614080): CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferOut writes "(region,nameLength,name,x,y,graphic,angle)" with the name copied into a temporary string first
#include <cstdio>
#include <cstring>
#include <cmath>
// Text-builder model: a fixed-buffer CCharString with operator+= and the scalar traits' TransferOut formatting.
class CCharString { public:
    char Text[256];
    CCharString() { Text[0] = 0; }
    CCharString(const char* s) { std::strcpy(Text, s); }
    const CCharString& operator+=(const char* s) { std::strcat(Text, s); return *this; }
    bool Equals(const char* s) const { return std::strcmp(Text, s) == 0; }
};
class CPersistContext { public: int Unused; };
static void TransferOutLong(CPersistContext*, CCharString* out, const int* value) { char b[32]; std::sprintf(b, "%d", *value); *out += b; }
static void TransferOutFloat(CPersistContext*, CCharString* out, const float* value) { char b[32]; std::sprintf(b, "%g", (double)*value); *out += b; }
static void TransferOutString(CPersistContext*, CCharString* out, const CCharString* value) { *out += "\""; *out += value->Text; *out += "\""; }
class C2DVector { public: float X, Y; };
struct CUIMiniMapMarker { int Region; CCharString UIDefName; C2DVector Position; int DefNameLength; int GraphicIndex; float Angle; };
static void TransferOut(CPersistContext* context, CCharString* out, const CUIMiniMapMarker* value)
{
    *out += "(";
    TransferOutLong(context, out, &value->Region);
    *out += ",";
    TransferOutLong(context, out, &value->DefNameLength);
    *out += ",";
    CCharString name(value->UIDefName);
    TransferOutString(context, out, &name);
    *out += ",";
    TransferOutFloat(context, out, &value->Position.X);
    *out += ",";
    TransferOutFloat(context, out, &value->Position.Y);
    *out += ",";
    TransferOutLong(context, out, &value->GraphicIndex);
    *out += ",";
    TransferOutFloat(context, out, &value->Angle);
    *out += ")";
}
int main()
{
    CPersistContext ctx; ctx.Unused = 0;
    CUIMiniMapMarker m; m.Region = 3; m.UIDefName = CCharString("MARKER_X"); m.Position.X = 1.5f; m.Position.Y = -2.0f;
    m.DefNameLength = 8; m.GraphicIndex = 12; m.Angle = 0.25f;
    CCharString out; TransferOut(&ctx, &out, &m);
    if (!out.Equals("(3,8,\"MARKER_X\",1.5,-2,12,0.25)")) { std::printf("00614080_TEST FAIL\n"); return 1; }
    std::printf("00614080_TEST PASS\n");
    return 0;
}