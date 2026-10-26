// CQ_HangingTreeGoodScript::UpdateWayPoint() -- look up the "HTG_WayPoint<n>" marker for the current waypoint index, make it the active waypoint and report whether it is alive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D74E80, length 249 bytes, plain `ret`.
// Callees: GFIntToCharString(long) 0x0099F830 and operator+(const char*, const CCharString&) 0x0099F690 (both
// `__fastcall`, hidden result in ecx), ~CCharString 0x0099EAE0, CGameScriptInterface::GetThingWithScriptName (slot 72,
// +0x120, 0x008A7D60, through +0x40), operator delete 0x00BFE9BC, ~CBaseClass 0x0099A2E0, CScriptThing::IsAlive
// (virtual slot 75, +0x12c, 0x004AB130). Literal "HTG_WayPoint" 0x012D2AB8.
// Layout: Ego_r.pdb CQ_HangingTreeGoodScript - 0x14: CurrentWayPoint +0x60, ActiveWayPoint (CScriptThing) +0x8c.
// Shape: CScriptThing::operator= forwards to the counted pointer's inline operator= (the forwarded `&other.PImp`
// leaves retail's dead `add eax,4`), which loads Info before Data, skips equal blocks and re-reads the member after
// the `dec`; the returned temporary's inline destructor runs before the IsAlive call; the bool survives the name's
// destructor in bl.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
CCharString __fastcall GFIntToCharString(long value);                        // 0x0099F830 (hidden result in ecx)
CCharString __fastcall operator+(const char* left, const CCharString& right); // 0x0099F690 (hidden result in ecx)
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)

class CCPPointerInfo { public:                                // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);         // destroys the pointee
    void* Data;
};

template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        Release();
        Data = 0;
        Info = 0;
    }
};

class CBaseClass { public:                                    // polymorphic root of the script handles
    CBaseClass();                                      // 0x0099A2D0
    virtual ~CBaseClass();                             // 0x0099A2E0
};

class CScriptThing : public CBaseClass { public:                     // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                // +0x04 Data, +0x08 Info
    ~CScriptThing() {}
    CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const; virtual void v06() const; virtual void v07() const; virtual void v08() const;
    virtual void v09() const; virtual void v10() const; virtual void v11() const; virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const;
    virtual void v17() const; virtual void v18() const; virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const;
    virtual void v25() const; virtual void v26() const; virtual void v27() const; virtual void v28() const; virtual void v29() const; virtual void v30() const; virtual void v31() const; virtual void v32() const;
    virtual void v33() const; virtual void v34() const; virtual void v35() const; virtual void v36() const; virtual void v37() const; virtual void v38() const; virtual void v39() const; virtual void v40() const;
    virtual void v41() const; virtual void v42() const; virtual void v43() const; virtual void v44() const; virtual void v45() const; virtual void v46() const; virtual void v47() const; virtual void v48() const;
    virtual void v49() const; virtual void v50() const; virtual void v51() const; virtual void v52() const; virtual void v53() const; virtual void v54() const; virtual void v55() const; virtual void v56() const;
    virtual void v57() const; virtual void v58() const; virtual void v59() const; virtual void v60() const; virtual void v61() const; virtual void v62() const; virtual void v63() const; virtual void v64() const;
    virtual void v65() const; virtual void v66() const; virtual void v67() const; virtual void v68() const; virtual void v69() const; virtual void v70() const; virtual void v71() const; virtual void v72() const;
    virtual void v73() const; virtual void v74() const;
    virtual bool IsAlive() const;                      // slot 75 (+0x12c), 0x004AB130
};

class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name); // slot 72 (+0x120)
};

class CScriptBase { public:
    virtual void v00();
    unsigned char _body[0x3c];                       // +0x04 .. +0x40
};

namespace NScript {
class CQ_HangingTreeGoodScript : public CScriptBase {
public:
    CGameScriptInterface* m_pInterface;              // +0x40
    void*                 m_pOwner;                  // +0x44
    unsigned char         _pad48[0x18];              // the state bools +0x48..+0x59, HTG_State +0x5c
    long                  CurrentWayPoint;           // +0x60
    long                  CurrentGuards;             // +0x64
    CScriptThing          Leader;                    // +0x68
    CScriptThing          Prisoner;                  // +0x74
    CScriptThing          PartySpawnPoint;           // +0x80
    CScriptThing          ActiveWayPoint;            // +0x8c
    bool UpdateWayPoint();
};
} // namespace NScript
using NScript::CQ_HangingTreeGoodScript;

bool NScript::CQ_HangingTreeGoodScript::UpdateWayPoint()
{
    CCharString name = "HTG_WayPoint" + GFIntToCharString(CurrentWayPoint);
    ActiveWayPoint = m_pInterface->GetThingWithScriptName(name);
    return ActiveWayPoint.IsAlive();
}