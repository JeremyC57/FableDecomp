// CTCTalk::GetPhonemeAnim(const CCharString& phoneme) const -- the animation index the talk definition maps a phoneme name to (0 when the phoneme is not in the map)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E3780, length 77 bytes (+ int3 padding).
// Callees: CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>&) @ 0x00485815 (manifest template argument
// "CAbilityDef" propagated), the phoneme map's find(const CCharString&) @ 0x0042B467 (manifest "StdMap_FindLE"; returns
// the node pointer), the def's virtual slot 1 (+4) when the count drops to zero.
// Identity: Ego_r.pdb has both `?GetPhonemeAnim@CThingCreatureBase@@MBEKABVCCharString@@@Z` and
// `?GetPhonemeAnim@CTCTalk@@ABEKABVCCharString@@@Z`; this body reads `this+4` (CTCBase::ParentThing) and is called
// from the CTCTalk speech code around 0x006E3CDA, so it is CTCTalk's (the manifest's CThingCreatureBase is propagated).
// Layout (retail, local model): CTCBase's parent thing at +4, the thing's PBaseDef at +0x70 (landed CTCLook::OnCreate
// 0x00753040); the talk definition's STLport map<CCharString, unsigned long> at +0x60 (header node pointer first);
// STLport tree nodes {colour, parent, left, right, key +0x10, value +0x14}; a counted definition's count at +4.
// Shape: the def pointer is zero-initialised in the frame and loaded ONCE into edi after GetSubDef; the map is a named
// pointer local (esi, re-used for `cmp eax,[esi]` after the find); the found value is a ternary into esi; the def is
// released through edi with NO null test (`dec [edi+4] ; jne`), so the release is written by hand on the local.
class CCharString;

class CDefPointeeBase { public:                        // a counted definition object
    virtual void v000();
    virtual void DestroyDef();                  // slot 1 (+4): called when the count drops to zero
    long m_refCount;                            // +4
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};

// agentNF15: the pointee is held non-const whatever T is (engine/CDefPointer.h stores CDefPointeeBase*), so the
//   CDefPointer<const T> out slots of the const def lookups keep the landed member code.
template <class T> struct CDefNonConst { typedef T Type; };
template <class T> struct CDefNonConst<const T> { typedef T Type; };
template<class T> class CDefPointer { public:          // CDefPointer<const T>: one counted pointer to a definition
    typename CDefNonConst<T>::Type* m_pObject;
    CDefPointer() : m_pObject(0) { }
};

struct SPhonemeNode {                           // STLport _Rb_tree_node<pair<const CCharString, unsigned long> >
    long          m_colour;                     // +0x00
    SPhonemeNode* m_parent;                     // +0x04
    SPhonemeNode* m_left;                       // +0x08
    SPhonemeNode* m_right;                      // +0x0c
    void*         m_key;                        // +0x10 CCharString
    unsigned long m_anim;                       // +0x14
};

struct CPhonemeMap {                            // map<CCharString, unsigned long>
    SPhonemeNode* m_header;                     // +0x00 end()
    unsigned long m_count;                      // +0x04
    SPhonemeNode* Find(const CCharString& key) const;       // 0x0042B467
};

class CCreatureDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x58];
    CPhonemeMap   Phonemes;                     // +0x60
};

class CDefClassBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x00485815 GetSubDef<CCreatureDef> (Ego_r.pdb member template)
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>* out) const
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char  _pad00[0x70];
    CDefClassBase* PBaseDef;                    // +0x70
};

class CTCTalk { public:
    void*   __vftable;                          // +0x00
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned long GetPhonemeAnim(const CCharString& phoneme) const;
};

unsigned long CTCTalk::GetPhonemeAnim(const CCharString& phoneme) const
{
    CDefPointer<const CCreatureDef> def;
    ParentThing->PBaseDef->GetSubDef(&def);
    CCreatureDef* talk = const_cast<CCreatureDef*>(def.m_pObject);
    const CPhonemeMap* phonemes = &talk->Phonemes;
    SPhonemeNode* it = phonemes->Find(phoneme);
    unsigned long anim = (it != phonemes->m_header) ? it->m_anim : 0;
    talk->ReleaseDef();
    return anim;
}