// CTCParticleAttacher::AttachParticles() -- fetch the attacher def from the owner's base def, then for every attachment create the particle at the owner and attach its emitter: to each matching dummy object / helper point of the owner's appearance, or to the owner itself when the attachment names no object; each particle is kept in ParticleList
// Leaf: AttachParticles
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F8890, 630 bytes, plain `ret`. Speed TU; the 8-byte intelligent-pointer
//   temporaries give the `and esp,-8` frame.
// Callees: CDefClassBase::GetSubDef<CParticleAttacherDef> @ 0x007F87A0 (pushes "CParticleAttacherDef" 0x01243F00; the
//   FableWin name GetSubDef<CAICreatureWillPowerIndicatorDef> is wrong), CDefStringTable::GetString @ 0x009D49B0 on
//   g_DefStringTable 0x013CA828, CTCGraphicAppearance::GetNoDummyObjects @ 0x004BE000 / GetNoHelperPoints @ 0x004BDE90,
//   CBasicString<char>::operator== @ 0x004115A0, CThing::GetPosition @ 0x004C73D0, CTCDParticleEmitter::Create (__fastcall,
//   particle index) @ 0x006E0880, CBaseIntelligentPointer ctor/SetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01C10, the
//   particle list push_back @ 0x004AD520, CTCDParticleEmitter::AttachToThing @ 0x006E0BE0, CCharString @ 0x0099EBF0 / dtor
//   @ 0x0099EAE0. Data: "" @ 0x0122D70E, CIntelligentPointer<CThing> vtable 0x01238C6C.
// Layout: retail CThing (PDB minus 0x10 from +0x48): PTCDriver +0x5c, PTCGraphicAppearance +0x64, ThingTypeFlags +0x6c,
//   PBaseDef +0x70, flags +0x91 (bit 0 Dead); component bitset +0x20 (interface 0x47 = bit 7 of +0x28, 0x4B = bit 11);
//   CTCParticleAttacher PDef +0x0c, ParticleList +0x10 (as the landed HandleMessage 0x007F8B60); CParticleAttacherDef
//   ParticlesToAttach +0x28; CParticleAttachmentInfo (Ego_r.pdb) 0x10 bytes.
// Shape: the oriented flag is two bitfield reads in if-form (`shr/and bl,1` then the second read on zero); the appearance
//   is a named local (loaded before the DummyObject test); attachType is `1; if (oriented) attachType = 9;`; the
//   intelligent-pointer temporary is a named block local; the helper loop is `if (count > 0) for (i = 0; i != count; ++i)`
//   (entry jle, loop jne).
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;    // 0x004115A0
};
class CCharStringData { public:
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
struct CDefStringTable {
    CCharString GetString(long index);                 // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;               // 0x013CA828

class C3DVector { public: float X, Y, Z; };
class CThing;
class CBaseObject;

class CTCDParticleEmitter {
public:
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
    static CThing* __fastcall Create(long particleIndex, const C3DVector& position, bool flag);   // 0x006E0880
};

class CTCGraphicAppearance {
public:
    long GetNoDummyObjects(const CCharString& name, float scale);   // 0x004BE000
    long GetNoHelperPoints(const CCharString& name, float scale);   // 0x004BDE90
};

struct CParticleAttachmentInfo {                       // NParticleAttachment::CParticleAttachmentInfo, 0x10
    long  ParticleIndex;                               // +0x00
    long  AttachmentObjectName;                        // +0x04 CDefString (string table index)
    float GenerationCutoffDistance;                    // +0x08
    bool  DummyObject;                                 // +0x0c
};
class CParticleAttacherDef { public:
    unsigned char _pad00[0x28];
    CParticleAttachmentInfo* ParticlesToAttachFirst;   // +0x28
    CParticleAttachmentInfo* ParticlesToAttachLast;    // +0x2c
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void DeleteThis();                         // slot 1
    long RefCount;                                     // +0x04
    bool GetSubDef(const CParticleAttacherDef** out);  // 0x007F87A0
};
struct CBaseDefPointer {
    CDefClassBase* P;
    CBaseDefPointer(CDefClassBase* p) : P(p) { if (P) ++P->RefCount; }
    ~CBaseDefPointer() { if (P) { if (--P->RefCount == 0) P->DeleteThis(); } }
};

class CThing {
public:
    unsigned char         _pad00[0x20];
    union {
        unsigned long     m_presentBits[9];            // +0x20
        struct { unsigned long _w0, _w1; unsigned long _b0 : 7; unsigned long HasTC47 : 1; } m_presentBitFields;
    };
    unsigned char         _pad44[0x5c - 0x44];
    CTCDParticleEmitter*  PTCDriver;                   // +0x5c
    unsigned char         _pad60[0x64 - 0x60];
    CTCGraphicAppearance* PTCGraphicAppearance;        // +0x64
    unsigned char         _pad68[0x6c - 0x68];
    unsigned long         _tt0 : 3;                    // +0x6c ThingTypeFlags
    unsigned long         ThingTypeBit3 : 1;
    unsigned long         _tt4 : 28;
    CDefClassBase*        PBaseDef;                    // +0x70
    unsigned char         _pad74[0x91 - 0x74];
    unsigned char         m_flags91;                   // +0x91 bit 0 dead
    const C3DVector& GetPosition() const;              // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                         // 0x00A01B10
    ~CBaseIntelligentPointer();                        // 0x00A01C10
    void SetPItem(CBaseObject* item);                  // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // CIntelligentPointer<CThing> vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
#pragma pack(pop)

struct CParticleList {
    CIntelligentPointer<CThing>* First;
    CIntelligentPointer<CThing>* Last;
    CIntelligentPointer<CThing>* End;
    void push_back(const CIntelligentPointer<CThing>& item);   // 0x004AD520
};

class CTCParticleAttacher {
public:
    void*                       m_vtable;              // +0x00
    CThing*                     ParentThing;           // +0x04
    unsigned char               _pad08[4];
    const CParticleAttacherDef* PDef;                  // +0x0c
    CParticleList               ParticleList;          // +0x10
    void AttachParticles();
};

void CTCParticleAttacher::AttachParticles()
{
    CBaseDefPointer baseDef(ParentThing->PBaseDef);
    if (baseDef.P)
        baseDef.P->GetSubDef(&PDef);
    for (const CParticleAttachmentInfo* info = PDef->ParticlesToAttachFirst; info != PDef->ParticlesToAttachLast; ++info) {
        CCharString name = g_DefStringTable.GetString(info->AttachmentObjectName);
        CThing* parent = ParentThing;
        bool oriented = parent->ThingTypeBit3;
        if (!oriented)
            oriented = parent->m_presentBitFields.HasTC47;
        CTCGraphicAppearance* appearance = parent->PTCGraphicAppearance;
        long count;
        if (info->DummyObject)
            count = appearance->GetNoDummyObjects(name, 1.0f);
        else
            count = appearance->GetNoHelperPoints(name, 1.0f);
        if (name == "") {
            CThing* particle = CTCDParticleEmitter::Create(info->ParticleIndex, ParentThing->GetPosition(), false);
            if (particle && !(particle->m_flags91 & 1)) {
                {
                    CIntelligentPointer<CThing> pointer(particle);
                    ParticleList.push_back(pointer);
                }
                CTCDParticleEmitter* emitter = 0;
                if (particle->HasTC(0x4b))
                    emitter = particle->PTCDriver;
                long attachType = 1;
                if (oriented)
                    attachType = 9;
                emitter->AttachToThing(ParentThing, attachType | 2, "", 0, 0.0f);
            }
        } else {
            if (count > 0)
            for (long i = 0; i != count; ++i) {
                CThing* particle = CTCDParticleEmitter::Create(info->ParticleIndex, ParentThing->GetPosition(), false);
                if (particle && !(particle->m_flags91 & 1)) {
                    {
                    CIntelligentPointer<CThing> pointer(particle);
                    ParticleList.push_back(pointer);
                }
                    CTCDParticleEmitter* emitter = 0;
                    if (particle->HasTC(0x4b))
                        emitter = particle->PTCDriver;
                    long attachType = 1;
                if (oriented)
                    attachType = 9;
                    emitter->AttachToThing(ParentThing, attachType | (info->DummyObject ? 2 : 4), name, i, 0.0f);
                }
            }
        }
    }
}