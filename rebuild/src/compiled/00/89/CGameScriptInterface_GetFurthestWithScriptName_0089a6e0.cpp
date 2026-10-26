// CGameScriptInterface::GetFurthestWithScriptName(const CScriptThing& from, const CCharString& name) const -- script: of the things carrying a script name, the one whose XY position lies furthest from `from` (an empty script thing when none)
// Leaf: ?GetFurthestWithScriptName@CGameScriptInterface@@UBE?AVCScriptThing@@ABV2@ABVCCharString@@@Z
// Retail: Fable.exe @ 0x0089A6E0, 413 bytes, `ret 0Ch` (hidden return pointer); Ego_r.pdb same symbol.
// Body: vector<CScriptThing> things; GetAllThingsWithScriptName(name, things) (own vslot 75, +0x12c); empty -> return
// CScriptThing() (CBaseClass() 0x0099A2D0). Else best = begin, bestDistance (long, kept in the dead `name` slot) = 0; per
// element a by-value copy, squared XY distance between copy.GetPos() (CScriptThing vslot 6, devirtualised and inlined on
// the local copy: PImp ? PImp->GetPos() : the zero vector 0x0143E8E0) and from.GetPos() (virtual); distance > bestDistance
// -> bestDistance = (long)distance (_ftol), best = it. Returns a copy of *best; the vector is destroyed inline
// (virtual dtor per element with flag 0, then free 0x00BFEA14).
// Layout: the CScriptThing / CCountedPointer<CScriptThingImp> model of the matched 0x008A9C40 (vptr 0x01238C8C, PImp
// {Data, Info}), STLport vector over malloc. CScriptThingImp slots: 6 GetPos, 78 IsSameThing (used by the Nearest twin).
// Names: Ego_r.pdb method name; locals descriptive.
// Shape: the vector dtor must be __forceinline with a by-value _Destroy(first, last) helper (retail caches `last` in a
// register); CScriptThing::GetPos inline virtual so the local copy's call is devirtualised.
// Opt: /O2 (16-aligned entry, loop alignment padding).
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
class C3DVector { public: float X, Y, Z; };
extern const C3DVector g_ZeroVector;                    // 0x0143E8E0
struct CScriptThingImp {
    virtual void _i00();
    virtual void _i01();
    virtual void _i02();
    virtual void _i03();
    virtual void _i04();
    virtual void _i05();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18)
    virtual void _i07();
    virtual void _i08();
    virtual void _i09();
    virtual void _i10();
    virtual void _i11();
    virtual void _i12();
    virtual void _i13();
    virtual void _i14();
    virtual void _i15();
    virtual void _i16();
    virtual void _i17();
    virtual void _i18();
    virtual void _i19();
    virtual void _i20();
    virtual void _i21();
    virtual void _i22();
    virtual void _i23();
    virtual void _i24();
    virtual void _i25();
    virtual void _i26();
    virtual void _i27();
    virtual void _i28();
    virtual void _i29();
    virtual void _i30();
    virtual void _i31();
    virtual void _i32();
    virtual void _i33();
    virtual void _i34();
    virtual void _i35();
    virtual void _i36();
    virtual void _i37();
    virtual void _i38();
    virtual void _i39();
    virtual void _i40();
    virtual void _i41();
    virtual void _i42();
    virtual void _i43();
    virtual void _i44();
    virtual void _i45();
    virtual void _i46();
    virtual void _i47();
    virtual void _i48();
    virtual void _i49();
    virtual void _i50();
    virtual void _i51();
    virtual void _i52();
    virtual void _i53();
    virtual void _i54();
    virtual void _i55();
    virtual void _i56();
    virtual void _i57();
    virtual void _i58();
    virtual void _i59();
    virtual void _i60();
    virtual void _i61();
    virtual void _i62();
    virtual void _i63();
    virtual void _i64();
    virtual void _i65();
    virtual void _i66();
    virtual void _i67();
    virtual void _i68();
    virtual void _i69();
    virtual void _i70();
    virtual void _i71();
    virtual void _i72();
    virtual void _i73();
    virtual void _i74();
    virtual void _i75();
    virtual void _i76();
    virtual void _i77();
    virtual bool IsSameThing(const CScriptThingImp* other) const;   // slot 78 (+0x138)
};
class CBaseClass { public:                                     // the polymorphic root (vptr at +0)
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
class CCPPointerInfo { public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
struct CCountedScriptImp {                              // CCountedPointer<CScriptThingImp>
    CScriptThingImp* Data;
    CCPPointerInfo* Info;
    CCountedScriptImp() : Data(0), Info(0) {}
    CCountedScriptImp(const CCountedScriptImp& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedScriptImp()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CCharString { public: void* PStringData; };
class CScriptThing : public CBaseClass { public:                      // vtable 0x01238C8C
    CCountedScriptImp PImp;                             // +0x04
    CScriptThing() {}
    virtual ~CScriptThing() {}
    virtual bool IsDead() const;
    virtual const CCharString& GetName() const;
    virtual void _slot3();
    virtual void _slot4();
    virtual void _slot5();
    virtual const C3DVector& GetPos() const { return PImp.Data == 0 ? g_ZeroVector : PImp.Data->GetPos(); }
    bool operator==(const CScriptThing& other) const { return PImp.Data->IsSameThing(other.PImp.Data); }
};
__forceinline void _Destroy(CScriptThing* first, CScriptThing* last)
{
    for (; first != last; ++first)
        first->~CScriptThing();
}
struct CScriptThingVector {                             // STLport vector<CScriptThing> (malloc allocator)
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    CScriptThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    __forceinline ~CScriptThingVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start) free(_M_start);
    }
    bool empty() const { return _M_start == _M_finish; }
};
class CGameScriptInterface { public:
    virtual void _g00();
    virtual void _g01();
    virtual void _g02();
    virtual void _g03();
    virtual void _g04();
    virtual void _g05();
    virtual void _g06();
    virtual void _g07();
    virtual void _g08();
    virtual void _g09();
    virtual void _g10();
    virtual void _g11();
    virtual void _g12();
    virtual void _g13();
    virtual void _g14();
    virtual void _g15();
    virtual void _g16();
    virtual void _g17();
    virtual void _g18();
    virtual void _g19();
    virtual void _g20();
    virtual void _g21();
    virtual void _g22();
    virtual void _g23();
    virtual void _g24();
    virtual void _g25();
    virtual void _g26();
    virtual void _g27();
    virtual void _g28();
    virtual void _g29();
    virtual void _g30();
    virtual void _g31();
    virtual void _g32();
    virtual void _g33();
    virtual void _g34();
    virtual void _g35();
    virtual void _g36();
    virtual void _g37();
    virtual void _g38();
    virtual void _g39();
    virtual void _g40();
    virtual void _g41();
    virtual void _g42();
    virtual void _g43();
    virtual void _g44();
    virtual void _g45();
    virtual void _g46();
    virtual void _g47();
    virtual void _g48();
    virtual void _g49();
    virtual void _g50();
    virtual void _g51();
    virtual void _g52();
    virtual void _g53();
    virtual void _g54();
    virtual void _g55();
    virtual void _g56();
    virtual void _g57();
    virtual void _g58();
    virtual void _g59();
    virtual void _g60();
    virtual void _g61();
    virtual void _g62();
    virtual void _g63();
    virtual void _g64();
    virtual void _g65();
    virtual void _g66();
    virtual void _g67();
    virtual void _g68();
    virtual void _g69();
    virtual void _g70();
    virtual void _g71();
    virtual void _g72();
    virtual void _g73();
    virtual void _g74();
    virtual long GetAllThingsWithScriptName(const CCharString& name, CScriptThingVector& things) const;   // slot 75 (+0x12c)
    virtual CScriptThing GetFurthestWithScriptName(const CScriptThing& from, const CCharString& name) const;
};

CScriptThing CGameScriptInterface::GetFurthestWithScriptName(const CScriptThing& from, const CCharString& name) const
{
    CScriptThingVector things;
    GetAllThingsWithScriptName(name, things);
    if (things.empty())
        return CScriptThing();
    long bestDistance = 0;
    CScriptThing* best = things._M_start;
    for (CScriptThing* it = things._M_start; it != things._M_finish; ++it) {
        CScriptThing thing = *it;
        const C3DVector& pos = thing.GetPos();
        const C3DVector& fromPos = from.GetPos();
        float dx = pos.X - fromPos.X;
        float dy = pos.Y - fromPos.Y;
        float distance = dx * dx + dy * dy;
        if (distance > bestDistance) {
            bestDistance = (long)distance;
            best = it;
        }
    }
    return *best;
}