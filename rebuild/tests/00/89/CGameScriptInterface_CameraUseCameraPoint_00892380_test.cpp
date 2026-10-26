#include <cstdio>
class CCharString;
class C3DVector;
class CRightHandedSet;
class CCPPointerInfo { public:
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info)
        {
            --Info->RefCount;
            if (Info->RefCount == 0)
            {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CBaseClass {
public:
    virtual ~CBaseClass();                               // 0x0099A2E0
};
class CScriptThing : public CBaseClass {
public:
    CCountedPointer<CScriptThing> PImp;                  // +0x04
    CScriptThing(const CScriptThing& other);
    virtual ~CScriptThing() {}
};
#define V10(p) virtual void p##0(); virtual void p##1(); virtual void p##2(); virtual void p##3(); virtual void p##4(); \
               virtual void p##5(); virtual void p##6(); virtual void p##7(); virtual void p##8(); virtual void p##9();
class CGameScriptInterface {
public:
    V10(_s000)
    V10(_s001)
    V10(_s002)
    V10(_s003)
    V10(_s004)
    V10(_s005)
    V10(_s006)
    virtual void _v070(); virtual void _v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;          // slot 72 (+0x120)
    virtual void _v073(); virtual void _v074(); virtual void _v075(); virtual void _v076(); virtual void _v077();
    virtual void _v078(); virtual void _v079();
    V10(_s008)
    V10(_s009)
    V10(_s010)
    V10(_s011)
    V10(_s012)
    V10(_s013)
    V10(_s014)
    V10(_s015)
    V10(_s016)
    V10(_s017)
    V10(_s018)
    V10(_s019)
    V10(_s020)
    V10(_s021)
    V10(_s022)
    V10(_s023)
    V10(_s024)
    V10(_s025)
    V10(_s026)
    V10(_s027)
    V10(_s028)
    V10(_s029)
    V10(_s030)
    V10(_s031)
    V10(_s032)
    V10(_s033)
    V10(_s034)
    V10(_s035)
    V10(_s036)
    V10(_s037)
    V10(_s038)
    V10(_s039)
    V10(_s040)
    virtual void _v410();
    virtual void CameraUseCameraPoint(const CCharString& name, const CScriptThing& target, float time,
                                      long mode1, long mode2) const;                       // slot 414 (+0x678)
    virtual void CameraUseCameraPoint(const CScriptThing& point, const CScriptThing& target, float time,
                                      long mode1, long mode2) const;                       // slot 413 (+0x674)
    virtual void CameraUseCameraPoint(const CCharString& name, const C3DVector* position,
                                      const CRightHandedSet* orientation, float time,
                                      long mode1, long mode2) const;                       // slot 412 (+0x670)
    virtual void CameraUseCameraPoint(const CScriptThing& point, const C3DVector* position,
                                      const CRightHandedSet* orientation, float time,
                                      long mode1, long mode2) const;                       // slot 411 (+0x66c)
};

void CGameScriptInterface::CameraUseCameraPoint(const CCharString& name, const C3DVector* position,
                                                const CRightHandedSet* orientation, float time,
                                                long mode1, long mode2) const
{
    CScriptThing point = GetThingWithScriptName(name);
    CameraUseCameraPoint(point, position, orientation, time, mode1, mode2);
}

CBaseClass::~CBaseClass() {}
struct RawThing { void* vt; void* data; CCPPointerInfo* info; };
static int g_deleted, g_lookups, g_forwards; static const CCharString* g_name; static void* g_data;
static const C3DVector* g_pos; static const CRightHandedSet* g_ori; static float g_time; static long g_m1, g_m2; static long g_seenRef;
static void __fastcall DeleteData(void* d) { g_data = d; ++g_deleted; }
static CScriptThing* __fastcall Lookup(void* self, int, CScriptThing* ret, const CCharString* name)
{
    ++g_lookups; g_name = name;
    RawThing* r = (RawThing*)ret; r->vt = 0; r->data = (void*)0x1234;
    r->info = new CCPPointerInfo; r->info->RefCount = 1; r->info->DeleteFunc = DeleteData; r->info->Data = (void*)0x5678;
    return ret;
}
static void __fastcall Forward(void* self, int, const CScriptThing* point, const C3DVector* pos, const CRightHandedSet* ori, float time, long m1, long m2)
{
    ++g_forwards; g_pos = pos; g_ori = ori; g_time = time; g_m1 = m1; g_m2 = m2;
    g_seenRef = ((const RawThing*)point)->info->RefCount;
}
int main()
{
    static void* vt[420]; vt[72] = (void*)Lookup; vt[411] = (void*)Forward;
    void* obj = vt; const CGameScriptInterface* gsi = (const CGameScriptInterface*)&obj;
    const CCharString* name = (const CCharString*)0x4444;
    gsi->CGameScriptInterface::CameraUseCameraPoint(*name, (const C3DVector*)0x1111, (const CRightHandedSet*)0x2222, 2.5f, 3, 4);
    bool ok = g_lookups == 1 && g_forwards == 1 && g_name == name && g_pos == (const C3DVector*)0x1111 && g_ori == (const CRightHandedSet*)0x2222
           && g_time == 2.5f && g_m1 == 3 && g_m2 == 4 && g_seenRef == 1 && g_deleted == 1 && g_data == (void*)0x5678;
    std::printf(ok ? "00892380_TEST PASS\n" : "00892380_TEST FAIL\n");
    return ok ? 0 : 1;
}