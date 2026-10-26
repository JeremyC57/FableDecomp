// Behaviour test (0x006c7960): CPersistTraits<COpinionDeed*>::TransferOut writes the deed tuple; the frame is the current frame while the deed is still happening, and a constant FALSE stands in the fifth field
#include <cstdio>
#include <cstring>
#include <cmath>
// Text-builder model: a fixed-buffer CCharString with operator+= and the scalar traits' TransferOut formatting.
class CCharString { public:
    char Text[256];
    CCharString() { Text[0] = 0; }
    CCharString(const char* s) { std::strcpy(Text, s); }
    CCharString& operator+=(const char* s) { std::strcat(Text, s); return *this; }
    bool Equals(const char* s) const { return std::strcmp(Text, s) == 0; }
};
class CPersistContext { public: int Unused; };
static void TransferOutLong(CPersistContext*, CCharString* out, const int* value) { char b[32]; std::sprintf(b, "%d", *value); *out += b; }
static void TransferOutFloat(CPersistContext*, CCharString* out, const float* value) { char b[32]; std::sprintf(b, "%g", (double)*value); *out += b; }
static void TransferOutString(CPersistContext*, CCharString* out, const CCharString* value) { *out += "\""; *out += value->Text; *out += "\""; }
static int g_currentFrame = 500;
static int GFGetCurrentFrame() { return g_currentFrame; }
static void TransferOutBool(CPersistContext*, CCharString* out, const bool* value) { if (*value) *out += "TRUE"; else *out += "FALSE"; }
class C3DVector { public: float X, Y, Z; };
static void TransferOutVector(CPersistContext* c, CCharString* out, const C3DVector* v)
{
    *out += "C3DCoordF("; TransferOutFloat(c, out, &v->X); *out += ","; TransferOutFloat(c, out, &v->Y); *out += ","; TransferOutFloat(c, out, &v->Z); *out += ")";
}
struct CThingRef { int UID; };
static void TransferOutPointer(CPersistContext* c, CCharString* out, const CThingRef* p) { TransferOutLong(c, out, &p->UID); }
class COpinionDeed { public:
    int DeedType; int DeedHandle; C3DVector Position; CThingRef PTarget; int FrameHappened;
    bool IsStillHappening; bool IsCrimeReportedToVillage; bool IsCrime; int ExtraData;
};
static void TransferOut(CPersistContext* context, CCharString* out, COpinionDeed* const* value)
{
    *out += "(";
    TransferOutLong(context, out, &(*value)->DeedType); *out += ",";
    TransferOutLong(context, out, &(*value)->DeedHandle); *out += ",";
    TransferOutVector(context, out, &(*value)->Position); *out += ",";
    int frame = (*value)->IsStillHappening ? GFGetCurrentFrame() : (*value)->FrameHappened;
    TransferOutLong(context, out, &frame); *out += ",";
    bool searching = false;
    TransferOutBool(context, out, &searching); *out += ",";
    TransferOutPointer(context, out, &(*value)->PTarget); *out += ",";
    TransferOutBool(context, out, &(*value)->IsCrimeReportedToVillage); *out += ",";
    TransferOutBool(context, out, &(*value)->IsCrime); *out += ",";
    TransferOutLong(context, out, &(*value)->ExtraData);
    *out += ")";
}
int main()
{
    CPersistContext ctx; ctx.Unused = 0;
    COpinionDeed d; d.DeedType = 4; d.DeedHandle = 12; d.Position.X = 1.0f; d.Position.Y = 2.0f; d.Position.Z = 3.0f; d.PTarget.UID = 77;
    d.FrameHappened = 100; d.IsStillHappening = false; d.IsCrimeReportedToVillage = true; d.IsCrime = false; d.ExtraData = -1;
    COpinionDeed* p = &d;
    CCharString a; TransferOut(&ctx, &a, &p);
    if (!a.Equals("(4,12,C3DCoordF(1,2,3),100,FALSE,77,TRUE,FALSE,-1)")) { std::printf("006c7960_TEST FAIL\n"); return 1; }
    d.IsStillHappening = true; d.IsCrime = true;
    CCharString b; TransferOut(&ctx, &b, &p);
    if (!b.Equals("(4,12,C3DCoordF(1,2,3),500,FALSE,77,TRUE,TRUE,-1)")) { std::printf("006c7960_TEST FAIL\n"); return 1; }
    std::printf("006c7960_TEST PASS\n");
    return 0;
}