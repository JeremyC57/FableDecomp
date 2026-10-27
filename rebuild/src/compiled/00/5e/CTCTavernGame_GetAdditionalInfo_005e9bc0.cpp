// CTCTavernGame::GetAdditionalInfo() -- an empty text (this game shows nothing here)
// Leaf: ?GetAdditionalInfo@CTCTavernGame@@UAE?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E9BC0, 44 bytes, thiscall, `ret 4` (CWideString by hidden pointer).
// Identity (proven): slot 46 of CTCTavernGame's vtable 0x012560BC, inherited unchanged by CTCTavernGameCoinBase / CardPairs / CardBase; the landed CTCTavernGameShoveHaPenny::GetAdditionalInfo 0x007CFF40 overrides the same slot 46; Ego_r.pdb ?GetAdditionalInfo@CTCTavernGame@@UAE?AVCWideString@@XZ. The manifest's
//   "CTCTavernGame::GetAdditionalInfo" is propagated across byte-identical bodies.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (the pooled "" 0x0122D70E), CCharString::ToWideString() const
//   0x0099E480, CCharString::~CCharString 0x0099EAE0.
// Shape (family M161, representative 0x005E9BC0): `return CCharString("").ToWideString();` -- the temporary string lives in
//   the dead hidden-result argument slot once the result pointer is in esi.
class CWideString {
public:
    void* m_data;
    ~CWideString();                                       // 0x0099B510
};
class CCharString {
public:
    void* m_data;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    CWideString ToWideString() const;                     // 0x0099E480
};
class CTCTavernGame {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual CWideString GetAdditionalInfo();                    // slot 46
};
CWideString CTCTavernGame::GetAdditionalInfo()
{
    return CCharString("").ToWideString();
}