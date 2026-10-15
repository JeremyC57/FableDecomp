// CScriptThing::operator==(const CScriptThing& other) const -- compare the two counted pointers' implementations through slot 78
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AAE20, length 22 bytes (32 with padding). 8 callers (0x00CBEF0D,
// 0x00E6B4F6, 0x00E8623E ...). The catalogue row 008aae20 (96 bytes) held this body, operator!= at 0x008AAE40 and two
// getters at 0x008AAE60 / 0x008AAE70 (skipped ledger, chunk 901).
// engine/CScriptThing.h (PDB): +0x04 PImp_Data (CCountedPointer<CScriptThing>::Data), the pointee's vtable slot 78
// (+0x138) takes the other implementation and returns the bool straight through the tail jump.
// Both operands go through the counted pointer's inline accessor (a bare member read allocates the registers the other way round).
// Shape: `mov ecx,[ecx+4] ; mov eax,[esp+4] ; mov eax,[eax+4] ; mov edx,[ecx] ; mov [esp+4],eax ; jmp [edx+138h]`.
#include "engine/CScriptThing.h"

struct CScriptThingImp_Retail {                 // the implementation PImp_Data points at
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
    virtual void v75();
    virtual void v76();
    virtual void v77();
    virtual bool IsSameThing(CScriptThingImp_Retail* other);   // slot 78 (+0x138)
};

struct CScriptThing_Methods : CScriptThing {
    CScriptThingImp_Retail* GetPImp() const { return reinterpret_cast<CScriptThingImp_Retail*>(PImp_Data); }   // the counted pointer's accessor (inlined)
    bool operator==(const CScriptThing_Methods& other) const;
};

bool CScriptThing_Methods::operator==(const CScriptThing_Methods& other) const
{
    return GetPImp()->IsSameThing(other.GetPImp());
}
