// CGameScriptInterface::CameraUseCameraPoint(const CCharString&, const CScriptThing&, float, long, long) const -- look the camera point up by script name and forward to the CScriptThing overload
// Leaf: ?CameraUseCameraPoint@CGameScriptInterface@@UBEXABVCCharString@@ABVCScriptThing@@MJJ@Z
// Retail: Fable.exe @ 0x008922F0, 132 bytes, `ret 14h`; Ego_r.pdb same symbol.
// Body: CScriptThing point = GetThingWithScriptName(name) (vtable slot 72, +0x120, 0x008A7D60, hidden result);
// CameraUseCameraPoint(point, target, time, mode1, mode2) (slot 413, +0x674, 0x00891070, the CScriptThing overload);
// then point's inline destructor: vptr = 0x01238C8C, ~CCountedPointer (if Info: --RefCount, at zero
// DeleteFunc(Data) __fastcall + operator delete 0x00BFE9BC; Data = Info = 0), out-of-line ~CBaseClass 0x0099A2E0.
// Layout: engine/CScriptThing.h (vptr, PImp CCountedPointer<CScriptThing> +4/+8) flattens the vptr into a data member,
// so the polymorphic classes are declared here as in the landed 0x00F35B40 model; the CGameScriptInterface slots come
// from slots.py on its retail vtable (PDB order). VC7.1 lays an overload group out in reverse declaration order, so
// this overload (slot 414) is declared before the CScriptThing one (slot 413).
// Shape: /O2 (16-aligned entry, no frame).
class CCharString;
struct CCPPointerInfo {
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
    virtual void _v410(); virtual void _v411(); virtual void _v412();
    virtual void CameraUseCameraPoint(const CCharString& name, const CScriptThing& target, float time,
                                      long mode1, long mode2) const;                       // slot 414 (+0x678)
    virtual void CameraUseCameraPoint(const CScriptThing& point, const CScriptThing& target, float time,
                                      long mode1, long mode2) const;                       // slot 413 (+0x674)
};

void CGameScriptInterface::CameraUseCameraPoint(const CCharString& name, const CScriptThing& target, float time,
                                                long mode1, long mode2) const
{
    CScriptThing point = GetThingWithScriptName(name);
    CameraUseCameraPoint(point, target, time, mode1, mode2);
}