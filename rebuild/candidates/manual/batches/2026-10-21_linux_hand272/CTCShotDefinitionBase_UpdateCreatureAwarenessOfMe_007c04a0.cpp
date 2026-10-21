// CTCShotDefinitionBase::UpdateCreatureAwarenessOfMe() -- gather the things in the 10x10 square around the shot (excluding the living shooter); the first living creature that can hear it and has the AI TC plays a random ARROW_FLY_BY sound from the shot's sound def, and the shot remembers it made its sound
// Retail: Fable.exe @ 0x007C04A0, 496 bytes (plain `ret`).
// Callees: CThing::GetPosition 0x004C73D0, __ftol2 0x00BFEA70, CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CTCBase::GetThingSearchTools 0x00686D90, CThingSearchTools::GetAllThingsInRoughArea<IsNot> 0x00725F10 / <all> 0x007197C0,
//   LowerBound 0x0040F020, CDefClassBase::GetSubDef 0x0065F604, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0,
//   sound-group lookup 0x0065FBA4, CSoundInit() 0x0041C930, CTCBase::GetWorld 0x00686A70, CWorld::GetRandomSeed 0x0049D810,
//   GFRandom 0x004CDAE0 (__fastcall), CTCAI::PlaySound 0x0066ACE0, free 0x00BFEA14.
// Layout: Ego_r.pdb CTCShotDefinitionBase minus 4 after the 12-byte HitThingsList vector: PShooter +0x28, MadeSound +0x65.
//   CThing: TC bitset +0x20 (bit 0x10 = +0x20 bit 16), interface map +0x44, Dead +0x91 bit 0, def class +0x70.
// Shape: the empty filter has a user ctor (no zero store); the sound def is declared before the AI-TC GetInterface (its
//   zero store sits between the bit test and the jump); GetSubDef has a same-TU stand-in so the def stays in edi; the
//   sound name is a named CCharString in its own block (lea, not the ctor's eax); the def class comes from an inline
//   owner accessor (edx/ecx/eax order at the GetSubDef call).
struct C3DVector { float X, Y, Z; };
struct C2DBoxI {
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(long minX, long minY, long maxX, long maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) {}
};
struct CCharString {
    void* m_data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
extern "C" void __cdecl free(void* block);          // 0x00BFEA14
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0
struct CSoundInit {
    float Volume, Pitch; long a, b, c, d, e, f;
    CSoundInit();                                   // 0x0041C930
};
struct CSoundGroup { unsigned char* first; unsigned char* last; };
struct CSoundDef {
    virtual void v00();
    virtual void Destroy();
    long RefCount;
    unsigned char _pad08[0x20];
    struct CMap { CSoundGroup* Find(const CCharString& name); } Sounds;   // +0x28, 0x0065FBA4
};
struct CSoundDefPtr {
    CSoundDef* Object;
    CSoundDefPtr() : Object(0) {}
    ~CSoundDefPtr() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
};
struct CDefClassBase { bool GetSubDef(CSoundDefPtr* out); };   // 0x0065F604
CSoundDef* FindRef_CDefClassBase_GetSubDef(CDefClassBase* defClass);   // stand-in callee of the stand-in body below
// Stand-in body (the real 0x0065F604 sits in this TU out of line): it only writes the def through the out parameter, so
// VC7.1 knows the caller's def pointer does not escape the call (retail keeps the def in edi across the later calls).
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CSoundDefPtr* out)
{
    CSoundDef* def = FindRef_CDefClassBase_GetSubDef(this);
    out->Object = def;
    return def != 0;
}
#pragma auto_inline(on)
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last; SEntry* end;
    SEntry* LowerBound(const long& key);            // 0x0040F020
};
struct CTCAI { void PlaySound(const unsigned char& sound, const CSoundInit& init, long a, long b); };   // 0x0066ACE0
struct CThing {
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
    virtual bool CanHear();                         // slot 77 (+0x134)
    unsigned char _pad04[0x1c];
    unsigned long TCBits[4];                        // +0x20
    unsigned char _pad30[0x14];
    CInterfaceMap Interfaces;                       // +0x44
    unsigned char _pad50[0x20];
    CDefClassBase* DefClass;                        // +0x70
    unsigned char _pad74[0x1d];
    unsigned char Dead : 1;                         // +0x91 bit 0
    const C3DVector& GetPosition() const;           // 0x004C73D0
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (TCBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &Interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (T*)(it == map->last || type < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingVector {
    CThing** _M_start; CThing** _M_finish; CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingVector() { if (_M_start) free(_M_start); }
};
struct CBaseIntelligentPointer {
    void* __vftable; void* m_pItem; long m_link;
    CThing* GetPItem() const;                       // 0x00A01B50
};
struct CThingFilter_IsNot { const CThing* Thing; };
struct CThingFilter_All { CThingFilter_All() {} };
struct CThingSearchTools {
    bool GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_IsNot& filter, CThingVector& out);   // 0x00725F10
    bool GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_All& filter, CThingVector& out);     // 0x007197C0
};
struct CWorld { unsigned long* GetRandomSeed(); };  // 0x0049D810
struct CTCShotDefinitionBase {
    void*   __vftable;
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x20];
    CBaseIntelligentPointer PShooter;               // +0x28
    unsigned char _pad34[0x31];
    bool    MadeSound;                              // +0x65
    CThingSearchTools* GetThingSearchTools() const; // 0x00686D90
    CWorld* GetWorld();                             // 0x00686A70
    CDefClassBase* GetOwnerDefClass() const { return ParentThing->DefClass; }
    void UpdateCreatureAwarenessOfMe();
};

void CTCShotDefinitionBase::UpdateCreatureAwarenessOfMe()
{
    CThingVector things;
    const C3DVector& pos = ParentThing->GetPosition();
    long x = (long)pos.X;
    long y = (long)pos.Y;
    C2DBoxI area(x - 5, y - 5, x + 5, y + 5);
    if (PShooter.GetPItem() && !PShooter.GetPItem()->Dead)
    {
        CThingFilter_IsNot filter;
        filter.Thing = PShooter.GetPItem();
        GetThingSearchTools()->GetAllThingsInRoughArea(area, filter, things);
    }
    else
        GetThingSearchTools()->GetAllThingsInRoughArea(area, CThingFilter_All(), things);
    for (CThing** it = things._M_start; it != things._M_finish; ++it)
    {
        CThing* thing = *it;
        if (thing && !thing->Dead && !MadeSound && thing->CanHear())
        {
            CSoundDefPtr def;
            CTCAI* ai;
            if (thing->GetInterface(0x10, &ai) && GetOwnerDefClass()->GetSubDef(&def))
            {
                CSoundGroup* group;
                {
                    CCharString name("ARROW_FLY_BY");
                    group = def.Object->Sounds.Find(name);
                }
                if (group)
                {
                    ai->PlaySound(group->first[GFRandom(group->last - group->first, GetWorld()->GetRandomSeed())], CSoundInit(), 0, 0);
                    MadeSound = true;
                }
            }
        }
    }
}
