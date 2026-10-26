// CTCInventoryTrade::OpenActualBuyInventoryPC() -- rebuilds the PC trade screen: the buy inventory when the top-level list component (+0x220) is named PC_TOP_LEVEL_TRADE_LIST, else the special-items inventory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005AE972, length 108 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CTCInventoryTrade::DeleteComponents() @ 0x0059C8AE, the open menu component's virtual slot 84 (+0x150, its name as a
// CCharString by hidden pointer), CBasicString<char>::operator==(const char*) @ 0x004115A0 (inside the inline
// CCharString::operator==, whose null-string arm is the strcmp intrinsic against "" @ 0x0122D70E), ~CCharString @ 0x0099EAE0,
// ConstructActualBuyInventoryPC() @ 0x005AC10E, ConstructActualSpecialInventoryPC() @ 0x005A8C66.
// String: "PC_TOP_LEVEL_TRADE_LIST" @ 0x012536C8.
// Shape: the CCharString operator== is `__forceinline` (a size TU leaves a plain inline out of line); the result is a named bool ([ebp-1]) so the name temporary dies before the branch; both arms are calls
// sharing the `mov ecx,ebx`.
// Layout: retail CTCInventoryTrade PTop_Level_List +0x220 (engine/CTCInventoryTrade.h at the retail STLport 4.6 offsets,
//   as the landed constructor 0x0059EB0C: PMouse_Pointer +0x21c, PTop_Level_List +0x220, PCurrentOpenTradeMenu +0x224).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const;    // 0x004115A0
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    ~CCharString();                             // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
};
namespace NUISystem {
class CComponent { public:
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
    virtual void v78();
    virtual void v79();
    virtual void v80();
    virtual void v81();
    virtual void v82();
    virtual void v83();
    virtual CCharString GetName() const;        // slot 84 (+0x150)
};
}

class CTCInventoryTrade { public:
    unsigned char _pad_0x00[0x220];
    NUISystem::CComponent* PTop_Level_List;   // +0x220
    void DeleteComponents();                    // 0x0059C8AE
    void ConstructActualBuyInventoryPC();       // 0x005AC10E
    void ConstructActualSpecialInventoryPC();   // 0x005A8C66
    void OpenActualBuyInventoryPC();
};

void CTCInventoryTrade::OpenActualBuyInventoryPC()
{
    DeleteComponents();
    bool topLevel = PTop_Level_List->GetName() == "PC_TOP_LEVEL_TRADE_LIST";
    if (topLevel)
        ConstructActualBuyInventoryPC();
    else
        ConstructActualSpecialInventoryPC();
}