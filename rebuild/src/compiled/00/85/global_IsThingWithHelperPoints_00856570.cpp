// IsThingWithHelperPoints(const CThing&, vector<CCharString>&) -- anonymous-namespace helper of the combat-action TU: true when the thing's graphic appearance carries every named helper point (walks the names until the first one missing)
// Leaf: ?IsThingWithHelperPoints@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00856570, 50 bytes, __fastcall (ecx = thing, edx = names), plain `ret`.
// Identity: Ego_r.pdb ?IsThingWithHelperPoints@?A0xad05d24c@@YI_NABVCThing@@AAV?$vector@VCCharString@@...@std@@@Z,
//   right after ProcessWeaponTrailPointsToIncludeCylinderCloseToCreatureAxis in the CCombatActionBase TU (TLC 0x00856310
//   then 0x00856570); callers 0x0085919D / 0x008591AC in the same TU.
// Callee: NGraphicAppearance::CTCGraphicAppearance::HasHelperPoint(const CCharString&) const 0x004BE170 (landed).
// Layout: CThing PTCGraphicAppearance +0x64 (Ego_r.pdb +0x74 less the 12-byte Dinkumware list; landed 0x007C25A0 reads
//   the same slot); STLport vector<CCharString> {start, finish, end_of_storage}, CCharString one pointer.
// Shape: the appearance pointer is a local (ebx, loaded once); no empty-range guard ahead of the first HasHelperPoint
//   call = a do-while; `it == end ? true : false` gives the bare `cmp ; sete al` (`return it == end;` adds xor eax,eax).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PString;
};

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    bool HasHelperPoint(const CCharString& name) const;   // 0x004BE170
};
}

class CThing {
public:
    unsigned char _pad_0x00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
};

namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {   // STLport layout: three pointers
public:
    typedef T* iterator;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}

namespace {
bool __fastcall IsThingWithHelperPoints(const CThing& thing, std::vector<CCharString>& helperNames)
{
    const NGraphicAppearance::CTCGraphicAppearance* appearance = thing.PTCGraphicAppearance;
    std::vector<CCharString>::iterator it = helperNames.begin();
    do {
        if (!appearance->HasHelperPoint(*it))
            break;
        ++it;
    } while (it != helperNames.end());
    return it == helperNames.end() ? true : false;
}
}

bool (__fastcall *g_keepIsThingWithHelperPoints)(const CThing&, std::vector<CCharString>&) =
    &IsThingWithHelperPoints;   // keeps the internal-linkage body emitted; not part of the retail function