// NUISystem::CComponent::ChangePositionDelta(const C2DVector& delta, float time, bool immediate) -- move the component by a delta (unconverted from screen to relative units and scaled by the UI scale when it uses relative positions) from its current position
// Grade: DIFFER(181v181) 3 lines + 6 call relocs (chunk 605: every name respelled to the PDB, residue unchanged): the by-value argument copy loads d.Y after `sub esp,8` (retail loads both halves first) and the result copy stores d.X before the GetUIScale result-slot push (retail stores both after). Tried in chunk 605: copy ctor as `*this = other` (same), pack(1) C2DVector (same), + (float,float) ctor (same), a named converted local (184). Earlier: implicit copy, body-form/float-local copy ctors, memberwise operator=, pointer result, 2-float temporary, Set(), block/function-scope converted locals.
// Leaf: ?ChangePositionDelta@CComponent@NUISystem@@UAEXABVC2DVector@@M_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052EA30, 181 bytes, thiscall `ret 0Ch`; slot 32 of .?AVCComponent@NUISystem@@ (vtable 0x0124608C).
// Names (two sources each): IsUsingRelativePosition = slot 117 0x0052F3B0 (Ego_r.pdb ?IsUsingRelativePosition@CComponent@NUISystem@@UBE_NXZ,
//   landed 0x0052EEC0 uses the same slot); ChangePosition = slot 31 0x0052E9C0 (PDB ?ChangePosition@CComponent@NUISystem@@UAEXABVC2DVector@@M_N@Z,
//   the slot just before ChangePositionDelta, same signature); CManager::GetInstance 0x0041E5F2 (the singleton it allocates (0xD0) stores
//   vtable 0x01230134 = .?AVCManager@NUISystem@@; PDB ?GetInstance@CManager@NUISystem@@SIPAV12@XZ); CManager::GetUIScale 0x0041CF47 (PDB
//   ?GetUIScale@CManager@NUISystem@@QBE?AVC2DVector@@XZ, const); CUIScreenUnitConverter::UnConvertVector 0x0052E530 (FableWin.pdb
//   ?UnConvertVector@CUIScreenUnitConverter@NUISystem@@SI?AVC2DVector@@V3@@Z: static fastcall, C2DVector by value -> hidden result; it
//   computes x / backbuffer width (0x013B876C) * UI width, the exact inverse of the landed ConvertVector 0x0052E580). The manifest's
//   `ConvertX` and the parked `CFrontEndManager` spellings were wrong (NAME_FIXES.tsv).
// Layout: Position +0x34 / +0x38 (retail). Unused vtable slots are padding declarations (no CComponent engine header exists).
// Shape: the delta copied into a local (homed at [esp+4]); `d = UnConvertVector(d)` (copy-constructed argument slot, implicit
//   assignment from the result slot); each axis `GetInstance()->GetUIScale().X * d.X` (two GetUIScale calls, temporaries reuse the
//   result slot); then `d += Position` per axis and ChangePosition(d, time, immediate).
class C2DVector {
public:
    float X;
    float Y;
    C2DVector() { }
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};

namespace NUISystem {

class CManager {
public:
    static CManager* __fastcall GetInstance();           // 0x0041E5F2 ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
    C2DVector GetUIScale() const;                        // 0x0041CF47 ?GetUIScale@CManager@NUISystem@@QBE?AVC2DVector@@XZ
};

class CUIScreenUnitConverter {
public:
    static C2DVector __fastcall UnConvertVector(C2DVector vector);   // 0x0052E530 ?UnConvertVector@CUIScreenUnitConverter@NUISystem@@SI?AVC2DVector@@V3@@Z
};

class CComponent {
public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void ChangePosition(const C2DVector& position, float time, bool immediate);   // slot 31 (+0x7c) 0x0052E9C0
    virtual void ChangePositionDelta(const C2DVector& delta, float time, bool immediate);   // slot 32 (+0x80)
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual void v097();
    virtual void v098();
    virtual void v099();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual void v108();
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void v114();
    virtual void v115();
    virtual void v116();
    virtual bool IsUsingRelativePosition() const;        // slot 117 (+0x1d4) 0x0052F3B0
    unsigned char _pad04[0x34 - 4];
    C2DVector Position;                                  // +0x34
    C2DVector TargetPosition;                            // +0x3c
};

void CComponent::ChangePositionDelta(const C2DVector& delta, float time, bool immediate)
{
    C2DVector d = delta;
    if (IsUsingRelativePosition()) {
        d = CUIScreenUnitConverter::UnConvertVector(d);
        d.X = CManager::GetInstance()->GetUIScale().X * d.X;
        d.Y = CManager::GetInstance()->GetUIScale().Y * d.Y;
    }
    d.X = d.X + Position.X;
    d.Y = d.Y + Position.Y;
    ChangePosition(d, time, immediate);
}

} // namespace NUISystem
