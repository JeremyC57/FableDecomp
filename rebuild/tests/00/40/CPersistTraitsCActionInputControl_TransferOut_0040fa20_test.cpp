// Behaviour test (0x0040fa20): CPersistTraits<CActionInputControl>::TransferOut writes "CActionInputControl(action,controller,key[,dir])", the key chosen by controller type, the direction only for analogue actions 0x3C/0x72 with a non-zero direction
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
class C2DVector { public: float X, Y; float GetLengthSquared() const { return X * X + Y * Y; } };
static const float g_Epsilon = 1e-4f;
static void TransferOutVector(CPersistContext* c, CCharString* out, const C2DVector* v)
{
    *out += "C2DCoordF("; TransferOutFloat(c, out, &v->X); *out += ","; TransferOutFloat(c, out, &v->Y); *out += ")";
}
class CActionInputControl { public: int GameAction; int ControllerType; int KeyboardKey; int XboxButton; int MouseButton; C2DVector ControlDirection; };
static void TransferOut(CPersistContext* context, CCharString* out, const CActionInputControl* value)
{
    *out += "CActionInputControl";
    *out += "(";
    TransferOutLong(context, out, &value->GameAction);
    *out += ",";
    TransferOutLong(context, out, &value->ControllerType);
    *out += ",";
    switch (value->ControllerType) {
    case 1: TransferOutLong(context, out, &value->XboxButton); break;
    case 2: TransferOutLong(context, out, &value->KeyboardKey); break;
    case 3: TransferOutLong(context, out, &value->MouseButton); break;
    }
    if (value->ControlDirection.GetLengthSquared() > g_Epsilon * g_Epsilon && (value->GameAction == 0x3C || value->GameAction == 0x72)) {
        *out += ",";
        TransferOutVector(context, out, &value->ControlDirection);
    }
    *out += ")";
}
int main()
{
    CPersistContext ctx; ctx.Unused = 0;
    CActionInputControl a; a.GameAction = 5; a.ControllerType = 2; a.KeyboardKey = 30; a.XboxButton = 4; a.MouseButton = 1;
    a.ControlDirection.X = 1.0f; a.ControlDirection.Y = 0.0f;
    CCharString s1; TransferOut(&ctx, &s1, &a);
    if (!s1.Equals("CActionInputControl(5,2,30)")) { std::printf("0040fa20_TEST FAIL\n"); return 1; }
    a.GameAction = 0x3C; a.ControllerType = 1;
    CCharString s2; TransferOut(&ctx, &s2, &a);
    if (!s2.Equals("CActionInputControl(60,1,4,C2DCoordF(1,0))")) { std::printf("0040fa20_TEST FAIL\n"); return 1; }
    a.GameAction = 0x72; a.ControllerType = 4; a.ControlDirection.X = 0.0f;
    CCharString s3; TransferOut(&ctx, &s3, &a);
    if (!s3.Equals("CActionInputControl(114,4,)")) { std::printf("0040fa20_TEST FAIL\n"); return 1; }
    std::printf("0040fa20_TEST PASS\n");
    return 0;
}