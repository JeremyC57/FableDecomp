// NUISystem::CMorphingSprite::ProcessChangeState() -- the CChangingStateComponent state change (0x0052C920), then, when the info of the current state (+0x148, virtual slot 135) has flag 8, the morph entry for that state (the map at +0x18c, operator[] @ 0x00553E60) is applied with the sprite's own word at +0x150 (virtual slot 151)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005546F0, length 72 bytes.
// Callees: CChangingStateComponent::ProcessChangeState @ 0x0052C920, this object's virtual slots 135 (+0x21c, the state by value) and 151 (+0x25c, (entry, word)), the state map's operator[] @ 0x00553E60 (labelled std::map::insert; thiscall with the key by const reference, returning the mapped word's address). Vtable slot 58 of NUISystem::CMorphingSprite. Shape: `this` in esi, the key's address in edi, `ret`.
struct SStateInfo_005546f0 {
    unsigned long m_flags;                         // +0x00 (bit 8)
};
struct CStateMorphMap_005546f0 {                   // map<long, long> at +0x18c
    long& operator[](const long& state);           // 0x00553E60
};

struct CChangingStateComponent {                   // the polymorphic root: the sprite's vtable (slots 135 / 151 below) sits at +0, so the base call takes `this` unchanged
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
    virtual void v031();
    virtual void v032();
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
    virtual void v117();
    virtual void v118();
    virtual void v119();
    virtual void v120();
    virtual void v121();
    virtual void v122();
    virtual void v123();
    virtual void v124();
    virtual void v125();
    virtual void v126();
    virtual void v127();
    virtual void v128();
    virtual void v129();
    virtual void v130();
    virtual void v131();
    virtual void v132();
    virtual void v133();
    virtual void v134();
    virtual const SStateInfo_005546f0* GetStateInfo(long state);   // slot 135 (+0x21c)
    virtual void v136();
    virtual void v137();
    virtual void v138();
    virtual void v139();
    virtual void v140();
    virtual void v141();
    virtual void v142();
    virtual void v143();
    virtual void v144();
    virtual void v145();
    virtual void v146();
    virtual void v147();
    virtual void v148();
    virtual void v149();
    virtual void v150();
    virtual void ApplyMorph(long entry, long word);   // slot 151 (+0x25c)
    void ProcessChangeState();                     // 0x0052C920
};

namespace NUISystem {
class CMorphingSprite : public CChangingStateComponent { public:
    unsigned char          _pad04[0x144];          // +0x04 .. +0x148
    long                   m_currentState;         // +0x148
    long                   m_14c;
    long                   m_morphWord;            // +0x150
    unsigned char          _pad154[0x38];          // +0x154 .. +0x18c
    CStateMorphMap_005546f0 m_stateMorphs;         // +0x18c
    void ProcessChangeState();
};

void CMorphingSprite::ProcessChangeState()
{
    CChangingStateComponent::ProcessChangeState();
    if (GetStateInfo(m_currentState)->m_flags & 8)
        ApplyMorph(m_stateMorphs[m_currentState], m_morphWord);
}
}