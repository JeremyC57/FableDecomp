// CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<..> >::operator= -- copy the pair vector out of line, then the Dirty flag at +0xd
// Leaf: ??4?$CVectorMap@KVCSubDefInfo@CDefClassBase@@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@@QAEAAV0@ABV0@@Z
// Name fix (NF382): was CTavernTableDef::operator= (its own body is 0x005F9BB2). The only retail caller is the landed
//   CCutsceneDef::operator= 0x00F2A000, which expands the CParentDefClassBase part of the copy and calls this for its SubDefs map;
//   FableWin CParentDefClassBase::operator= calls exactly this CVectorMap<unsigned long, CSubDefInfo>::operator= (definition_manager.obj).
//   The callee 0x00431F90 is the STLport
//   vector<pair<unsigned long, CSubDefInfo> >::operator= (reloc_bindings), the CVectorMap's base, and +0xd is CVectorMap::Dirty.
//   The base assignment keeps its landed stand-in spelling. Same bytes.
// Retail: Fable.exe (TLC, VC7.1) @ 0x006313A0, 27 bytes, `ret 4`.
// push esi/edi; edi=src; base::operator=(this,src); this[+0xd]=src[+0xd]; return this
struct BaseDef {
    char pad[0xd];       // 0x00..0x0c: the pair vector (+0) and the empty comparator (+0xc)
    // base operator= is a real out-of-line __fastcall; forward-declared, not inlined here
    BaseDef* AssignBase(const BaseDef* rhs);
};

class CDefClassBase { public:
    class CSubDefInfo;
};
template <class K, class V> class CKeyPairCompareLess { };

template <class K, class V, class CompareType> class CVectorMap : public BaseDef { public:
    char m_flag;         // +0x0d (Dirty)
    CVectorMap& operator=(const CVectorMap& rhs);
};

template <class K, class V, class CompareType>
CVectorMap<K, V, CompareType>& CVectorMap<K, V, CompareType>::operator=(const CVectorMap& rhs)
{
    this->AssignBase(&rhs);
    this->m_flag = rhs.m_flag;
    return *this;
}

template class CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >;
