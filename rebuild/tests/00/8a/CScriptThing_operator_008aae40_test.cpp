#include <cstdio>
#include <cstring>
// Behaviour of 0x008AAE40 (chunk 901 piece): the inverse of operator==
struct CScriptThingImp_Retail {
    virtual long v00() { return 0; }
    virtual long v01() { return 1; }
    virtual long v02() { return 2; }
    virtual long v03() { return 3; }
    virtual long v04() { return 4; }
    virtual long v05() { return 5; }
    virtual long v06() { return 6; }
    virtual long v07() { return 7; }
    virtual long v08() { return 8; }
    virtual long v09() { return 9; }
    virtual long v10() { return 10; }
    virtual long v11() { return 11; }
    virtual long v12() { return 12; }
    virtual long v13() { return 13; }
    virtual long v14() { return 14; }
    virtual long v15() { return 15; }
    virtual long v16() { return 16; }
    virtual long v17() { return 17; }
    virtual long v18() { return 18; }
    virtual long v19() { return 19; }
    virtual long v20() { return 20; }
    virtual long v21() { return 21; }
    virtual long v22() { return 22; }
    virtual long v23() { return 23; }
    virtual long v24() { return 24; }
    virtual long v25() { return 25; }
    virtual long v26() { return 26; }
    virtual long v27() { return 27; }
    virtual long v28() { return 28; }
    virtual long v29() { return 29; }
    virtual long v30() { return 30; }
    virtual long v31() { return 31; }
    virtual long v32() { return 32; }
    virtual long v33() { return 33; }
    virtual long v34() { return 34; }
    virtual long v35() { return 35; }
    virtual long v36() { return 36; }
    virtual long v37() { return 37; }
    virtual long v38() { return 38; }
    virtual long v39() { return 39; }
    virtual long v40() { return 40; }
    virtual long v41() { return 41; }
    virtual long v42() { return 42; }
    virtual long v43() { return 43; }
    virtual long v44() { return 44; }
    virtual long v45() { return 45; }
    virtual long v46() { return 46; }
    virtual long v47() { return 47; }
    virtual long v48() { return 48; }
    virtual long v49() { return 49; }
    virtual long v50() { return 50; }
    virtual long v51() { return 51; }
    virtual long v52() { return 52; }
    virtual long v53() { return 53; }
    virtual long v54() { return 54; }
    virtual long v55() { return 55; }
    virtual long v56() { return 56; }
    virtual long v57() { return 57; }
    virtual long v58() { return 58; }
    virtual long v59() { return 59; }
    virtual long v60() { return 60; }
    virtual long v61() { return 61; }
    virtual long v62() { return 62; }
    virtual long v63() { return 63; }
    virtual long v64() { return 64; }
    virtual long v65() { return 65; }
    virtual long v66() { return 66; }
    virtual long v67() { return 67; }
    virtual long v68() { return 68; }
    virtual long v69() { return 69; }
    virtual long v70() { return 70; }
    virtual long v71() { return 71; }
    virtual long v72() { return 72; }
    virtual long v73() { return 73; }
    virtual long v74() { return 74; }
    virtual long v75() { return 75; }
    virtual long v76() { return 76; }
    virtual long v77() { return 77; }
    long m_id;
    virtual bool IsSameThing(CScriptThingImp_Retail* other) { return other->m_id == m_id; }
};
class CScriptThing { public: void* __vftable; CScriptThing* PImp_Data; void* PImp_Info; };
struct CScriptThing_Methods : CScriptThing {
    CScriptThingImp_Retail* GetPImp() const { return reinterpret_cast<CScriptThingImp_Retail*>(PImp_Data); }
    bool operator==(const CScriptThing_Methods& other) const;
    bool operator!=(const CScriptThing_Methods& other) const;
};
bool CScriptThing_Methods::operator==(const CScriptThing_Methods& other) const { return GetPImp()->IsSameThing(other.GetPImp()); }
bool CScriptThing_Methods::operator!=(const CScriptThing_Methods& other) const { return !GetPImp()->IsSameThing(other.GetPImp()); }
static int setup(CScriptThing_Methods& a, CScriptThing_Methods& b, CScriptThing_Methods& c, CScriptThingImp_Retail& x, CScriptThingImp_Retail& y, CScriptThingImp_Retail& z)
{
    x.m_id = 1; y.m_id = 1; z.m_id = 2;
    a.PImp_Data = reinterpret_cast<CScriptThing*>(&x); b.PImp_Data = reinterpret_cast<CScriptThing*>(&y); c.PImp_Data = reinterpret_cast<CScriptThing*>(&z);
    return 0;
}
int main()
{
    CScriptThing_Methods a, b, c; CScriptThingImp_Retail x, y, z; setup(a, b, c, x, y, z);
    if ((a != b) || !(a != c) || (a != a)) { std::printf("008aae40_TEST FAIL\n"); return 1; }
    std::printf("008aae40_TEST PASS\n"); return 0;
}