// CGameScriptInterface::CloseHouseDoors(const CScriptThing& thing) const -- locks every door of the building the script thing refers to
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088E000, length 48 bytes.
// Callees: CScriptThing virtual slot 75 (+0x12c, IsValid) and slot 11 (+0x2c, GetThing), CThingBuilding::LockAllDoors(bool lock,
// bool) @ 0x0082DB00 (true, false) on the thing when bit 6 of its flag byte at +0x6c is set (it is a building).
// Oracle name: ?CloseHouseDoors@CGameScriptInterface@@UBEXABVCScriptThing@@@Z (`this` unused; written on the PDB-header wrapper).
// Leaf: CloseHouseDoors
#include "engine/CGameScriptInterface.h"

struct CThingBuilding {
    unsigned char _pad[0x6c];
    unsigned char m_typeFlags;                      // +0x6c (bit 6 = building)
    void LockAllDoors(bool lock, bool silent);      // 0x0082DB00
};

struct CScriptThing {
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
    virtual CThingBuilding* GetThing() const;                // slot 11 (+0x2c)
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
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual bool IsValid() const;                   // slot 75 (+0x12c)
    void* m_pImpData;                               // +4 CCountedPointer<CScriptThing>::Data
    void* m_pImpInfo;                               // +8
};

struct CGameScriptInterface_Methods : CGameScriptInterface {
    void CloseHouseDoors(const CScriptThing& thing) const;
};

void CGameScriptInterface_Methods::CloseHouseDoors(const CScriptThing& thing) const
{
    if (thing.IsValid()) {
        CThingBuilding* building = thing.GetThing();
        if (building->m_typeFlags & 0x40)
            building->LockAllDoors(true, false);
    }
}
