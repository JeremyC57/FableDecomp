#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_VECTORKEY_PASS26
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
#include <cstdio>
#include <cstring>

static int errors;
#define CHECK(condition) do { if (!(condition)) { \
    if (errors < 20) std::printf("line %d: %s\n", __LINE__, #condition); ++errors; \
} } while (0)

static bool Same(const C3DVector& a, const C3DVector& b)
{
    return a.X == b.X && a.Y == b.Y && a.Z == b.Z;
}

unsigned long FABLE_FASTCALL CCRC::Calc(unsigned long seed, const void* bytes, unsigned long size)
{
    const unsigned char* cursor = (const unsigned char*)bytes;
    for (unsigned long i = 0; i != size; ++i) {
        seed ^= cursor[i];
        for (int bit = 0; bit != 8; ++bit)
            seed = (seed >> 1) ^ ((seed & 1) ? 0xedb88320UL : 0);
    }
    return seed;
}

static const char* const PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME = "primary_weapon";
static const char* const SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME = "secondary_weapon";
static const char* const MAIN_FOCAL_POSITION_NAME = "focus_eye_01";

namespace NParticleEngine {
    template<class K, class V> class FixtureParticleUpdateParameters {
    public:
        typedef std::pair<K, C3DVector> VectorParameter;
        struct VectorParameters {
            VectorParameter entries[2];
            unsigned count;

            VectorParameters() : count(0) {}
            void Insert(const VectorParameter& parameter)
            {
                CHECK(count < 2);
                if (count < 2)
                    entries[count++] = parameter;
            }
        } VectorParams;
        bool updated;

        FixtureParticleUpdateParameters() : updated(false) {}
        void Update()
        {
            CHECK(!updated);
            CHECK(VectorParams.count == 2);
            updated = true;
        }
    };
}

struct FixtureLightning;
struct FixtureParticleEmitter;
struct FixtureThing {
    bool dead;
    C3DVector position;
    FixtureLightning* lightning;
    FixtureParticleEmitter* emitter;
    unsigned lightningLookups, emitterLookups;

    FixtureThing() : dead(false), lightning(0), emitter(0), lightningLookups(0), emitterLookups(0) {}
    const C3DVector& GetPos() const { return position; }
    bool GetTC(FixtureLightning** result)
    {
        ++lightningLookups;
        if (!lightning)
            return false;
        *result = lightning;
        return true;
    }
    bool GetTC(FixtureParticleEmitter** result)
    {
        ++emitterLookups;
        if (!emitter)
            return false;
        *result = emitter;
        return true;
    }
};

template<class T> class FixtureIntelligentPointer {
public:
    FixtureIntelligentPointer(T* value = 0) : pointer(value) {}
    T* operator->() const { return pointer; }
    T& operator*() const { return *pointer; }
    T* pointer;
};

template<class T> bool IS_THING_ALIVE(const FixtureIntelligentPointer<T>& pointer)
{
    return pointer.pointer && !pointer->dead;
}

struct FixtureParticleEmitter {
    unsigned attachments, updates;
    const char* helperName;
    long attachmentType;
    FixtureThing* attachedTo;
    C3DVector position;

    FixtureParticleEmitter() : attachments(0), updates(0), helperName(0), attachmentType(0), attachedTo(0) {}
    void AttachToThing(FixtureThing& thing, long type, const char* helper, long index, float offset)
    {
        CHECK(index == 0 && offset == 0.0f && attachments == 0);
        attachmentType = type;
        helperName = helper;
        attachedTo = &thing;
        ++attachments;
    }
    C3DVector GetEmitterPosition() const { CHECK(attachments == 1); return position; }
    void SetEmitterUpdateParameters(const NParticleEngine::FixtureParticleUpdateParameters<unsigned long, unsigned long>& p)
    {
        CHECK(attachments == 1 && p.updated && p.VectorParams.count == 2);
        CHECK(p.VectorParams.entries[0].first == CCRC::Calc(0, "LIGHTNING_START", 15));
        CHECK(p.VectorParams.entries[1].first == CCRC::Calc(0, "LIGHTNING_END", 13));
        CHECK(Same(p.VectorParams.entries[0].second, position));
        CHECK(Same(p.VectorParams.entries[1].second, position));
        ++updates;
    }
};

struct FixtureSpecialAbilities {
    static FixtureThing* particle;
    static const FixtureThing* expectedHome;
    static unsigned creations;
    static long effect;
    static FixtureThing* CreateParticleEffect(long requestedEffect, const C3DVector& position)
    {
        CHECK(expectedHome && &position == &expectedHome->position);
        effect = requestedEffect;
        ++creations;
        return particle;
    }
};
FixtureThing* FixtureSpecialAbilities::particle;
const FixtureThing* FixtureSpecialAbilities::expectedHome;
unsigned FixtureSpecialAbilities::creations;
long FixtureSpecialAbilities::effect;

struct FixtureLightning {
    enum ELightningState { INACTIVE, DELAYING, ACTIVATING, ACTIVE, DEACTIVATINGCHILDREN, DEACTIVATING, DEAD };
    struct CLightningBeam {
        ELightningState m_lsState;
        float m_fDelay, m_fTime, m_fTimeBase;
        long m_nFrameCount;
        bool m_bMainBeam, m_bRightHand;
        FixtureIntelligentPointer<FixtureThing> m_tcpHome, m_tpParticleHandle, m_tcpTarget;
        C3DVector m_vStartPos, m_vEndPos;
        static long g_nLightningEffect, g_nLargeLightningEffect;
        void Activate();
        void ChangeState(ELightningState state)
        {
            m_fTimeBase = m_fTime;
            m_lsState = state;
            if (state == ACTIVE)
                m_nFrameCount = -1;
        }
    };

    unsigned loops;
    FixtureLightning() : loops(0) {}
    void StartLoopingSound() { ++loops; }
};
long FixtureLightning::CLightningBeam::g_nLightningEffect = 101;
long FixtureLightning::CLightningBeam::g_nLargeLightningEffect = 102;

#define CTCWillLightning FixtureLightning
#define CThing FixtureThing
#define CTCDParticleEmitter FixtureParticleEmitter
#define CIntelligentPointer FixtureIntelligentPointer
#define CTCSpecialAbilities FixtureSpecialAbilities
#define CParticleUpdateParameters FixtureParticleUpdateParameters

void CTCWillLightning::CLightningBeam::Activate()
{
    if (m_lsState == INACTIVE || m_lsState == DEACTIVATING) {
        if (m_fDelay == 0.0f) {
            ChangeState(ACTIVATING);
            if (IS_THING_ALIVE(m_tcpHome)) {
                CTCWillLightning* pTCLightning;
                if (m_tcpHome->GetTC(&pTCLightning))
                    pTCLightning->StartLoopingSound();
            }
        }
        else {
            ChangeState(DELAYING);
        }

        if (IS_THING_ALIVE(m_tcpHome) && !IS_THING_ALIVE(m_tpParticleHandle)) {
            if (m_bMainBeam)
                m_tpParticleHandle = CIntelligentPointer<CThing>(
                    CTCSpecialAbilities::CreateParticleEffect(g_nLargeLightningEffect, m_tcpHome->GetPos()));
            else
                m_tpParticleHandle = CIntelligentPointer<CThing>(
                    CTCSpecialAbilities::CreateParticleEffect(g_nLightningEffect, m_tcpHome->GetPos()));

            if (IS_THING_ALIVE(m_tpParticleHandle)) {
                CTCDParticleEmitter* tcParticleEmitter = 0;
                m_tpParticleHandle->GetTC(&tcParticleEmitter);

                if (m_bMainBeam) {
                    if (m_bRightHand)
                        tcParticleEmitter->AttachToThing(*m_tcpHome, 10, PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME, 0, 0.0f);
                    else
                        tcParticleEmitter->AttachToThing(*m_tcpHome, 10, SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME, 0, 0.0f);
                }
                else {
                    tcParticleEmitter->AttachToThing(*m_tcpHome, 12, MAIN_FOCAL_POSITION_NAME, 0, 0.0f);
                }

                m_vStartPos = tcParticleEmitter->GetEmitterPosition();
                if (IS_THING_ALIVE(m_tcpTarget))
                    m_vEndPos = m_tcpTarget->GetPos();

                std::pair<unsigned long, C3DVector> vtStartPos(NParticleEngine::MAKE_VECTOR_PARAM_FROM_BASE(0, "LIGHTNING_START"), m_vStartPos);
                std::pair<unsigned long, C3DVector> vtEndPos(NParticleEngine::MAKE_VECTOR_PARAM_FROM_BASE(0, "LIGHTNING_END"), m_vStartPos);
                NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> upParams;
                upParams.VectorParams.Insert(vtStartPos);
                upParams.VectorParams.Insert(vtEndPos);
                upParams.Update();
                tcParticleEmitter->SetEmitterUpdateParameters(upParams);
            }
        }
    }
}

#undef CTCWillLightning
#undef CThing
#undef CTCDParticleEmitter
#undef CIntelligentPointer
#undef CTCSpecialAbilities
#undef CParticleUpdateParameters

static FixtureThing* MakeThing(FixtureThing& thing, int state)
{
    thing.dead = state == 2;
    return state == 0 ? 0 : &thing;
}

int main()
{
    for (unsigned scenario = 0; scenario != 4536; ++scenario) {
        unsigned digits = scenario;
        const int state = digits % 7; digits /= 7;
        const int delay = digits % 3; digits /= 3;
        const int homeState = digits % 3; digits /= 3;
        const int priorParticleState = digits % 3; digits /= 3;
        const int createdParticleState = digits % 3; digits /= 3;
        const bool liveTarget = (digits & 1) != 0;
        const bool lightningTC = (digits & 2) != 0;
        const bool mainBeam = (digits & 4) != 0;
        for (int hand = 0; hand != 2; ++hand) {
            FixtureLightning lightning;
            FixtureLightning::CLightningBeam beam;
            FixtureThing home, existing, created, target;
            FixtureParticleEmitter emitter;
            home.position = C3DVector(7, 8, 9);
            home.lightning = lightningTC ? &lightning : 0;
            target.position = C3DVector(10, 11, 12);
            target.dead = !liveTarget;
            emitter.position = C3DVector(2, 4, 6);
            created.emitter = &emitter;
            beam.m_lsState = (FixtureLightning::ELightningState)state;
            beam.m_fDelay = delay == 0 ? 0.0f : delay == 1 ? -0.5f : 0.5f;
            beam.m_fTime = 9.0f;
            beam.m_fTimeBase = 1.0f;
            beam.m_nFrameCount = 23;
            beam.m_bMainBeam = mainBeam;
            beam.m_bRightHand = hand != 0;
            beam.m_tcpHome.pointer = MakeThing(home, homeState);
            beam.m_tpParticleHandle.pointer = MakeThing(existing, priorParticleState);
            beam.m_tcpTarget.pointer = scenario & 1 ? &target : liveTarget ? &target : 0;
            beam.m_vStartPos = C3DVector(-1, -2, -3);
            beam.m_vEndPos = C3DVector(-4, -5, -6);
            FixtureThing* initialParticle = beam.m_tpParticleHandle.pointer;
            FixtureSpecialAbilities::particle = MakeThing(created, createdParticleState);
            FixtureSpecialAbilities::expectedHome = &home;
            FixtureSpecialAbilities::creations = 0;
            FixtureSpecialAbilities::effect = 0;

            beam.Activate();

            const bool activate = state == FixtureLightning::INACTIVE || state == FixtureLightning::DEACTIVATING;
            const bool soundLookup = activate && delay == 0 && homeState == 1;
            const bool create = activate && homeState == 1 && priorParticleState != 1;
            const bool attach = create && createdParticleState == 1;
            CHECK(beam.m_lsState == (activate ? (delay == 0 ? FixtureLightning::ACTIVATING : FixtureLightning::DELAYING) : state));
            CHECK(beam.m_fTimeBase == (activate ? 9.0f : 1.0f) && beam.m_nFrameCount == 23);
            CHECK(home.lightningLookups == (soundLookup ? 1U : 0U));
            CHECK(lightning.loops == (soundLookup && lightningTC ? 1U : 0U));
            CHECK(FixtureSpecialAbilities::creations == (create ? 1U : 0U));
            CHECK(FixtureSpecialAbilities::effect == (create ? (mainBeam ? 102 : 101) : 0));
            CHECK(beam.m_tpParticleHandle.pointer == (create ? FixtureSpecialAbilities::particle : initialParticle));
            CHECK(created.emitterLookups == (attach ? 1U : 0U));
            CHECK(emitter.attachments == (attach ? 1U : 0U) && emitter.updates == emitter.attachments);
            if (attach) {
                const char* name = mainBeam ? (hand ? PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME :
                    SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME) : MAIN_FOCAL_POSITION_NAME;
                CHECK(emitter.helperName == name && emitter.attachedTo == &home);
                CHECK(emitter.attachmentType == (mainBeam ? 10 : 12));
            }
            CHECK(Same(beam.m_vStartPos, attach ? emitter.position : C3DVector(-1, -2, -3)));
            CHECK(Same(beam.m_vEndPos, attach && liveTarget ? target.position : C3DVector(-4, -5, -6)));
        }
    }
    std::puts(errors ? "007a1380_TEST FAIL" : "007a1380_TEST PASS");
    return errors ? 1 : 0;
}