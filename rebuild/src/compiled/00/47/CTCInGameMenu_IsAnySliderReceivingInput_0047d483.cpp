// CTCInGameMenu::IsAnySliderReceivingInput() const -- true when any UI component's video or audio options list is in input state 5 or 3
// Leaf: IsAnySliderReceivingInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047D483, 180 bytes; size TU without a frame pointer
// (`#pragma optimize("s", on)`), the twin of the landed IsQuitMenuReceivingInput (0x0047D537) with two lookups.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (built in the argument area, x2),
//          NUISystem::CComponent vtable slot 3 (+0x0C, find child by name, CCharString by value) and the child's slot
//          87 (+0x15C, the input state, called twice per child).
// Shape: one index loop over Components re-counted each pass; per component "UI_OPTIONS_LIST" then
// "UI_AUDIO_OPTIONS_LIST" are looked up with the element re-read and its vtable taken before the by-value string is
// built; `child && (state == 5 || state == 3)` for each, every hit sharing the trailing `mov al,1` exit.
// Layout: retail CTCInGameMenu Components vector +0x58 (PDB +0x68; the engine header carries PDB offsets).
#pragma optimize("s", on)


class CCharStringData;

class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    CCharString(const CCharString& other);           // (declared: by-value arguments are built in place)
    ~CCharString();
};

struct CComponent {                                  // NUISystem::CComponent
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CComponent* FindChild(CCharString name); // slot 3 (+0x0C)
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
    virtual void v84();
    virtual void v85();
    virtual void v86();
    virtual long GetInputState();                    // slot 87 (+0x15C)
};

template <class T> struct CRetailVector {            // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};

class CTCInGameMenu { public:
    void*         __vftable;                         // +0x00
    void*         ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x50];
    CRetailVector<CComponent*> Components;           // +0x58
    bool IsAnySliderReceivingInput() const;
};

bool CTCInGameMenu::IsAnySliderReceivingInput() const
{
    for (unsigned long i = 0; i < Components.size(); ++i) {
        CComponent* videoList = Components[i]->FindChild(CCharString("UI_OPTIONS_LIST", -1));
        if (videoList && (videoList->GetInputState() == 5 || videoList->GetInputState() == 3))
            return true;
        CComponent* audioList = Components[i]->FindChild(CCharString("UI_AUDIO_OPTIONS_LIST", -1));
        if (audioList && (audioList->GetInputState() == 5 || audioList->GetInputState() == 3))
            return true;
    }
    return false;
}