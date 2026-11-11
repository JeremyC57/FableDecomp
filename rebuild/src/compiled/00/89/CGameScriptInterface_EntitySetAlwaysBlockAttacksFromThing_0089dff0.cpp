// Leaf: ?CGameScriptInterface_EntitySetAlwaysBlockAttacksFromThing@@YIXPAX0PAUCEntityWrap@@1E@Z
typedef unsigned char  u8;
typedef unsigned long  u32;
struct BlockSet { void* _node; void* _end; };
class CThing { public: char _p0[0x24]; u32 flags; char _p1[0x44-0x28]; BlockSet blockSet; char _p2[0x91-0x4C]; u8 guardFlag; };
struct CEntityWrap;
typedef CThing* (__fastcall *ResolveFn)(CEntityWrap*);
// agentNF17: the BlockSet2 stand-ins are the explicit specialisation CVectorMap<ETCInterfaceType, CTCBase*> (the landed 0x0074EE90
//   idiom) and its std::pair value_type; 0x0040F020 is its LowerBound (NF14 / established spelling).
class CTCBase;
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                   // +0x00 (the STLport vector)
    value_type* _M_finish;                                  // +0x04
    value_type* LowerBound(const ETCInterfaceType& key);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct Mapped { void Add(CThing* thing); void Remove(CThing* thing); };
// Member modeled as __fastcall free fn: this in ecx (dead), unused edx, then 3 stack args (ret 0xc).
void __fastcall CGameScriptInterface_EntitySetAlwaysBlockAttacksFromThing(void* self, void* edx, CEntityWrap* entityA, CEntityWrap* entityB, u8 alwaysBlock)
{
    CThing* a = (*(ResolveFn**)entityA)[0x2c/4](entityA);
    CThing* b = (*(ResolveFn**)entityB)[0x2c/4](entityB);
    // Single short-circuit chain shifts liveness so VC7.1 parks the 2nd vtable in edx
    // (mov edx,[ecx]; mov esi,eax; call [edx+2c]) -- matches retail's 0x000f..0x0013.
    if (!a || (1 & a->guardFlag) || !b || (b->guardFlag & 1) || !(a->flags & 0x10000)) return;
    ETCInterfaceType key = (ETCInterfaceType)0x30;
    CInterfaceMap* bs = (CInterfaceMap*)((char*)a + 0x44);   // add esi,0x44 ; mov ecx,esi
    CInterfaceMap::value_type* entry = bs->LowerBound(key);  // thiscall member
    CInterfaceMap::value_type* end   = bs->_M_finish;        // mov esi,[esi+4]
    CInterfaceMap::value_type* sel;
    if (entry == end) sel = end;
    else if (entry->first <= 0x30) sel = entry;
    else sel = end;
    Mapped* mapped = (Mapped*)sel->second;                   // mov ecx,[eax+4]
    if (alwaysBlock) mapped->Add(b); else mapped->Remove(b);   // __fastcall members
}
// Stand-in (owner rule 2026-11-05): CGameScriptInterface_EntitySetAlwaysBlockAttacksFromThing for CGameScriptInterface::EntitySetAlwaysBlockAttacksFromThing; RTTI vtable slot ?EntitySetAlwaysBlockAttacksFromThing@CGameScriptInterface@@UBEXABVCScriptThing@@0_N@Z + PDB name set (egor/fablewin).
