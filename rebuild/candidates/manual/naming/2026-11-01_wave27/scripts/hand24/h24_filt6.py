# h24_filt6.py: naming wave 24, family filt6 (DECISIONS_W23_RESOLVED item 5) -- the six CThingSearchTools rows wave 23 held out because
# their namespace stand-ins hid filter classes that do not match the filter operator they call. Each is re-identified from the callee
# address (the operator's label and body) and the Ego_r / FableWin instantiation of that exact template, then respelled by fam_filt24.py
# (the wave 23 filt transform: filter(*p), Bind by pointer F1/F2/F3, PDB `const T&` operators):
#   0x006156E0 GetPFirstLeafThing<HasTC, CThingMarker>   calls 0x006A80A0 (HasTC: bitset<274> range check 0x112); type list 0xB is
#              CThingMarker's (the landed GetAllLeafThings rows 0x00617780.. / 0x00964EF0); Ego_r+FableWin instantiate it (was
#              <AICreatureHasProperty, CThingAICreature>; CThingAICreature's list is 0x1).
#   0x006D3FB0 GetPFirstThing<HasTC, CThing>               calls 0x006A80A0; FableWin; its caller 0x006CFAA0 binds that spelling (was HasDefGlobalIndex).
#   0x007EE660 GetPFirstThing<HasTC, const CThingVillage>  calls 0x006A80A0; Ego_r+FableWin (was HasScriptName).
#   0x0093F930 GetPBestThing<In2DRange, Nearest, CThing>   calls 0x006A76B0 (In2DRange); Ego_r+FableWin (was HasDefGlobalIndex).
#   0x00777000 GetPFirstThing<Bind2<IsNot, In3DRange>, CThing>  calls 0x006A76A0 (IsNot: one pointer compare) and 0x006A7780 (In3DRange);
#              Ego_r+FableWin (was Bind2<HasTC, InMap>).
#   0x009223A0 GetPBestThing<Bind3<IsNot, IsNot, HasScriptName>, Nearest, CThing>  calls 0x006A76A0 twice and 0x006A7E30 (HasScriptName:
#              the +0x74 script-name compare); Ego_r+FableWin (was Bind3<HasTC, InRegion, IsRegionEntranceActive>, another instantiation).
import os, sys, subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
W = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
R = os.path.join(W, 'runs')
he.edit('006156e0', 'src', '''class CThingGameObject : public CThing { public:
    enum { ThingTypeMask = 0x7 };
};
class CThingCreatureBase : public CThingGameObject { public:
    enum { ThingTypeMask = 0xf };
    long GetCreatureCategory() const;               // 0x00662000: def (+0xe0)->+0xc4
};
class CThingAICreature : public CThingCreatureBase { public:
    enum { ThingTypeMask = 0x1f };
    enum { ThingListType = 0xb };                // PeekTypeList index
};''', '''class CThingMarker : public CThing { public:
    enum { ThingListType = 0xb };                // PeekTypeList index (CThingMarker's list, as the landed GetAllLeafThings rows)
};''')
he.edit('006156e0', 'src', '    long Property;\n    bool operator()(const CThing* thing) const;      // 0x006A80A0 (the HasTC bitset test)',
        '    long Type;                                    // ETCInterfaceType\n    bool operator()(const CThing* thing) const;      // 0x006A80A0 (the HasTC bitset test)')
he.edit('009223a0', 'src', '''class CThingFilter_InRegion { public:
    void* Region;
    bool operator()(const CThing* thing) const;      // 0x006A76A0 (manifest: CThingFilter_IsNot)
};
''', '')
he.edit('009223a0', 'src', '''namespace N_006a76a0 { struct CThingFilter_HasTC {
    long Type;                                    // ETCInterfaceType
''', '''namespace N_006a76a0 { struct CThingFilter_HasTC {
    const void* Thing;                            // the excluded thing
''')
he.edit('009223a0', 'src', '''    void* RegionEntrance;
    bool operator()(const CThing* thing) const;      // 0x006A7E30''', '''    const void* ScriptName;
    bool operator()(const CThing* thing) const;      // 0x006A7E30''')
he.edit('00777000', 'src', '''namespace N_006a76a0 { struct CThingFilter_HasTC {
    long Type;''', '''namespace N_006a76a0 { struct CThingFilter_HasTC {
    const void* Thing;                            // the excluded thing''')
r = subprocess.run([sys.executable, os.path.join(W, 'scripts', 'fam_filt24.py'), os.path.join(R, 'base_filt6.rows.txt'), os.path.join(R, 'filt6_rename.tsv')], capture_output=True, text=True, env=os.environ)
for l in r.stdout.splitlines():
    print(l.replace('edited', 'ok'))
sys.stderr.write(r.stderr)
# ---- callers that bound the re-identified instantiations under other spellings ----
# 0x007EE5A0 CTCVillageMember::OnInitialActivate: 0x007EE660 is GetPFirstThing<HasTC, const CThingVillage> (Ego_r + FableWin; the
#   non-const CThingVillage instantiation is in neither PDB): the out pointer is a const CThingVillage*.
he.edit('007ee5a0', 'src', '        CThingVillage* village;\n', '        const CThingVillage* village;           // Ego_r / FableWin GetPFirstThing<HasTC, const CThingVillage> (naming wave 24)\n')
# 0x00774B40 CTCShop::GetFreeSaleItemPlaces: 0x00777000 is the const member template GetPFirstThing<Bind2<IsNot, In3DRange>, CThing>
#   (its second filter 0x006A7780 is In3DRange; Ego_r + FableWin; FableWin's In3DRange(const C3DVector&, float) / IsNot(const CThing&)
#   constructors); the binder holds its filters by pointer (Ego_r F1 / F2).
A = '00774b40'
he.edit(A, 'src', '''class CThingFilter_IsNot { public:
    CThing* Thing;
    CThingFilter_IsNot(CThing* thing) : Thing(thing) {}
};
class CThingFilter_In2DRange { public:
    C3DVector Position;
    float     Range;
    CThingFilter_In2DRange(const C3DVector& position, float range) : Position(position), Range(range) {}
};
template <class A, class B> class CFilter_Bind2 { public:
    const A* PA;
    const B* PB;
    CFilter_Bind2(const A& a, const B& b) : PA(&a), PB(&b) {}
};
class CThingSearchTools { public:
    bool GetPFirstThing(const CFilter_Bind2<CThingFilter_IsNot, CThingFilter_In2DRange>& filter, CThing** out);   // 0x00777000
};''', '''// Naming (naming wave 24): 0x00777000 is the const member template CThingSearchTools::GetPFirstThing<CFilter_Bind2<CThingFilter_IsNot,
//   CThingFilter_In3DRange>, CThing> (its filter callees are IsNot 0x006A76A0 and In3DRange 0x006A7780; Ego_r + FableWin instantiate it);
//   it was declared as a plain member over Bind2<IsNot, In2DRange>.
class CThingFilter_IsNot { public:
    const CThing* Thing;
    CThingFilter_IsNot(const CThing& thing) : Thing(&thing) {}
};
class CThingFilter_In3DRange { public:
    C3DVector Position;
    float     Range;
    CThingFilter_In3DRange(const C3DVector& position, float range) : Position(position), Range(range) {}
};
template <class A, class B> class CFilter_Bind2 { public:     // Ego_r.pdb: both filters held by pointer
    const A* F1;
    const B* F2;
    CFilter_Bind2(const A& f1, const B& f2) : F1(&f1), F2(&f2) {}
};
class CThingSearchTools { public:
    template <class TFilter, class TThing> bool GetPFirstThing(const TFilter& filter, TThing** out) const;   // 0x00777000 (<Bind2<IsNot, In3DRange>, CThing>)
};''')
he.edit(A, 'src', '''            if (PeekWorld().GetThingSearchTools()->GetPFirstThing(CFilter_Bind2<CThingFilter_IsNot, CThingFilter_In2DRange>(
                    CThingFilter_IsNot(ParentThing), CThingFilter_In2DRange(GoodsPlaces[i].Position, 0.2f)), &found))''', '''            if (PeekWorld().GetThingSearchTools()->GetPFirstThing(CFilter_Bind2<CThingFilter_IsNot, CThingFilter_In3DRange>(
                    CThingFilter_IsNot(*ParentThing), CThingFilter_In3DRange(GoodsPlaces[i].Position, 0.2f)), &found))''')
he.edit(A, 'src', '//   CThingSearchTools::GetPFirstThing<CFilter_Bind2<CThingFilter_IsNot, CThingFilter_In2DRange>, CThing> @ 0x00777000',
        '//   CThingSearchTools::GetPFirstThing<CFilter_Bind2<CThingFilter_IsNot, CThingFilter_In3DRange>, CThing> @ 0x00777000')
