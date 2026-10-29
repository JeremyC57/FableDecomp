// NUISystem::CManager::GetCorrectComponentInterface(CComponent* component) -- return the component when its UI type is one that exposes the component interface, else null
// Leaf: ?GetCorrectComponentInterface@CManager@NUISystem@@QAEPAVCComponent@2@PAV32@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041CD93, 215 bytes (the 44-entry jump table included), `ret 4`; packed at an unaligned
//   address between the unaligned CObservable::AddExclusiveObserver 0x0041CD89 and the next body, i.e. a size-optimised TU.
// Identity: Ego_r.pdb `?GetCorrectComponentInterface@CManager@NUISystem@@QAEPAVCComponent@2@PAV32@@Z` (its donor 0x0042D6AD is also
//   unaligned). The catalog's bare `CManager` is NUISystem::CManager (pdb_class_keys: V, 5 + 5 names, all NUISystem); `this` is unused.
// Callees: the component's virtual GetType() const (slot 65, +0x104; landed 0x0052EFE0 `?GetType@CComponent@NUISystem@@UBE?AW4EType@2@XZ`).
// Shape: a switch over the type with one `result = component; break;` per accepted type (0..43 except 26, 28-31 and 37); the
//   unlisted types fall through to `return result` (null). Separate case bodies keep VC7.1's direct dword jump table (the bodies
//   are merged after lowering, so 38 entries point at one `mov edi,esi`); one shared body for all the labels makes a byte-index
//   table instead. `#pragma optimize("s", on)` drops the alignment `nop` before the table, as in retail.
//   CComponent's slots 1..64 are not needed by name here; they are positional placeholders (CUnprovenSlot_NNN), see NOTES.md.
// Layout: the body reads no CManager member. engine/CManager.h is a global flattened `struct CManager` (no namespace), so the
//   leaf's NUISystem::CManager is declared here (header debt).
#pragma optimize("s", on)
namespace NUISystem {

enum EType { };   // the UI component type (CComponent +0x12D widened); enumerator names unproven

class CComponent { public:
    virtual ~CComponent();                      // slot 0
    virtual void CUnprovenSlot_001();
    virtual void CUnprovenSlot_002();
    virtual void CUnprovenSlot_003();
    virtual void CUnprovenSlot_004();
    virtual void CUnprovenSlot_005();
    virtual void CUnprovenSlot_006();
    virtual void CUnprovenSlot_007();
    virtual void CUnprovenSlot_008();
    virtual void CUnprovenSlot_009();
    virtual void CUnprovenSlot_010();
    virtual void CUnprovenSlot_011();
    virtual void CUnprovenSlot_012();
    virtual void CUnprovenSlot_013();
    virtual void CUnprovenSlot_014();
    virtual void CUnprovenSlot_015();
    virtual void CUnprovenSlot_016();
    virtual void CUnprovenSlot_017();
    virtual void CUnprovenSlot_018();
    virtual void CUnprovenSlot_019();
    virtual void CUnprovenSlot_020();
    virtual void CUnprovenSlot_021();
    virtual void CUnprovenSlot_022();
    virtual void CUnprovenSlot_023();
    virtual void CUnprovenSlot_024();
    virtual void CUnprovenSlot_025();
    virtual void CUnprovenSlot_026();
    virtual void CUnprovenSlot_027();
    virtual void CUnprovenSlot_028();
    virtual void CUnprovenSlot_029();
    virtual void CUnprovenSlot_030();
    virtual void CUnprovenSlot_031();
    virtual void CUnprovenSlot_032();
    virtual void CUnprovenSlot_033();
    virtual void CUnprovenSlot_034();
    virtual void CUnprovenSlot_035();
    virtual void CUnprovenSlot_036();
    virtual void CUnprovenSlot_037();
    virtual void CUnprovenSlot_038();
    virtual void CUnprovenSlot_039();
    virtual void CUnprovenSlot_040();
    virtual void CUnprovenSlot_041();
    virtual void CUnprovenSlot_042();
    virtual void CUnprovenSlot_043();
    virtual void CUnprovenSlot_044();
    virtual void CUnprovenSlot_045();
    virtual void CUnprovenSlot_046();
    virtual void CUnprovenSlot_047();
    virtual void CUnprovenSlot_048();
    virtual void CUnprovenSlot_049();
    virtual void CUnprovenSlot_050();
    virtual void CUnprovenSlot_051();
    virtual void CUnprovenSlot_052();
    virtual void CUnprovenSlot_053();
    virtual void CUnprovenSlot_054();
    virtual void CUnprovenSlot_055();
    virtual void CUnprovenSlot_056();
    virtual void CUnprovenSlot_057();
    virtual void CUnprovenSlot_058();
    virtual void CUnprovenSlot_059();
    virtual void CUnprovenSlot_060();
    virtual void CUnprovenSlot_061();
    virtual void CUnprovenSlot_062();
    virtual void CUnprovenSlot_063();
    virtual void CUnprovenSlot_064();
    virtual EType GetType() const;              // slot 65 (+0x104) 0x0052EFE0
};

class CManager { public:
    CComponent* GetCorrectComponentInterface(CComponent* component);
};

CComponent* CManager::GetCorrectComponentInterface(CComponent* component)
{
    CComponent* result = 0;
    switch (component->GetType()) {
    case 0:
        result = component;
        break;
    case 1:
        result = component;
        break;
    case 2:
        result = component;
        break;
    case 3:
        result = component;
        break;
    case 4:
        result = component;
        break;
    case 5:
        result = component;
        break;
    case 6:
        result = component;
        break;
    case 7:
        result = component;
        break;
    case 8:
        result = component;
        break;
    case 9:
        result = component;
        break;
    case 10:
        result = component;
        break;
    case 11:
        result = component;
        break;
    case 12:
        result = component;
        break;
    case 13:
        result = component;
        break;
    case 14:
        result = component;
        break;
    case 15:
        result = component;
        break;
    case 16:
        result = component;
        break;
    case 17:
        result = component;
        break;
    case 18:
        result = component;
        break;
    case 19:
        result = component;
        break;
    case 20:
        result = component;
        break;
    case 21:
        result = component;
        break;
    case 22:
        result = component;
        break;
    case 23:
        result = component;
        break;
    case 24:
        result = component;
        break;
    case 25:
        result = component;
        break;
    case 27:
        result = component;
        break;
    case 32:
        result = component;
        break;
    case 33:
        result = component;
        break;
    case 34:
        result = component;
        break;
    case 35:
        result = component;
        break;
    case 36:
        result = component;
        break;
    case 38:
        result = component;
        break;
    case 39:
        result = component;
        break;
    case 40:
        result = component;
        break;
    case 41:
        result = component;
        break;
    case 42:
        result = component;
        break;
    case 43:
        result = component;
        break;
    }
    return result;
}

} // namespace NUISystem