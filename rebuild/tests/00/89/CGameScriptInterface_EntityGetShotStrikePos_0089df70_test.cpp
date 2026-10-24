// Behaviour test for CGameScriptInterface::EntityGetShotStrikePos (0x0089DF70): only a live thing with the
// entity-events component (interface 0x42) and a recorded shot strike reports true and copies the StrikePos.
#include <stdio.h>
#include <string.h>
struct C3DVector { float X, Y, Z; };

struct CTCBase;
struct STCEntry {                                  // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long     m_kind;
    CTCBase* m_pTC;
};
struct CTCMap {                                    // the thing's component map at +0x44 (sorted vector of pairs)
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);   // 0x0040F020 (same-TU stand-in below)
    __forceinline CTCBase* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};

#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound (0x0040F020, STLport lower_bound over
// the 8-byte entries): the key is only read, so its address does not escape.
__declspec(noinline) STCEntry* CTCMap::LowerBound(const unsigned long& kind)
{
    STCEntry* it = m_pFirst;
    int len = int(m_pLast - m_pFirst);
    while (len > 0) {
        int half = len / 2;
        STCEntry* middle = it + half;
        if ((unsigned long)middle->m_kind < kind) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                          // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};

template <class T> __forceinline bool GetTCOut(CThing* thing, long kind, T** out)   // out-parameter lookup: the component stays in eax
{
    if (thing->HasTC(kind)) {
        *out = (T*)thing->m_tcs.Find(kind);
        return true;
    }
    return false;
}

struct CScriptThing {
    CThing* Thing;
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {}
    virtual void v006() {} virtual void v007() {} virtual void v008() {} virtual void v009() {} virtual void v010() {}
    virtual CThing* GetThing() const { return Thing; }   // slot 11 (+0x2c)
};

namespace NEntityEvents {
    struct CEventShotStruck {                      // Ego_r.pdb, 0x14 bytes
        void* _vftable;                            // +0x00
        long  _04;                                 // +0x04
        C3DVector StrikePos;                       // +0x08
    };
    struct CEventFilter_None { CEventFilter_None() {} };
}

struct CTCEntityEvents {                           // interface 0x42
    unsigned char _00[0x0c];
    NEntityEvents::CEventShotStruck* LastShotStruck;   // stand-in view of the event list
    template <class TEvent, class TFilter> bool GetEvent(TEvent** out, const TFilter&) const;
};

#pragma auto_inline(off)
// Same-TU stand-in for CTCEntityEvents::GetEvent<CEventShotStruck, CEventFilter_None> (0x008AC7A0): it writes only
// the out pointer and never reads the filter.
template <class TEvent, class TFilter>
__declspec(noinline) bool CTCEntityEvents::GetEvent(TEvent** out, const TFilter&) const
{
    if (LastShotStruck) {
        *out = LastShotStruck;
        return true;
    }
    return false;
}
#pragma auto_inline(on)

struct CGameScriptInterface {
    virtual bool EntityGetShotStrikePos(const CScriptThing& entity, C3DVector& position) const;
};

bool CGameScriptInterface::EntityGetShotStrikePos(const CScriptThing& entity, C3DVector& position) const
{
    CThing* thing = entity.GetThing();
    CTCEntityEvents* events;
    if (thing && !thing->m_dead && GetTCOut(thing, 0x42, &events))
    {
        NEntityEvents::CEventShotStruck* strike;
        if (events->GetEvent(&strike, NEntityEvents::CEventFilter_None()))
        {
            position = strike->StrikePos;
            return true;
        }
    }
    return false;
}

int main()
{
    CThing thing;
    STCEntry entries[3];
    CTCEntityEvents events;
    NEntityEvents::CEventShotStruck strike;
    CScriptThing entity;
    CGameScriptInterface gsi;
    C3DVector out;
    memset(&thing, 0, sizeof(thing));
    memset(&events, 0, sizeof(events));
    entries[0].m_kind = 0x07; entries[0].m_pTC = 0;
    entries[1].m_kind = 0x42; entries[1].m_pTC = (CTCBase*)&events;
    entries[2].m_kind = 0x50; entries[2].m_pTC = 0;
    thing.m_tcs.m_pFirst = entries; thing.m_tcs.m_pLast = entries + 3; thing.m_tcs.m_pEnd = entries + 3;
    strike.StrikePos.X = 1.5f; strike.StrikePos.Y = -2.0f; strike.StrikePos.Z = 8.25f;
    out.X = out.Y = out.Z = 0.0f;

    entity.Thing = 0;
    if (gsi.EntityGetShotStrikePos(entity, out)) { printf("FAIL null thing\n"); return 1; }
    entity.Thing = &thing;
    if (gsi.EntityGetShotStrikePos(entity, out)) { printf("FAIL no component bit\n"); return 1; }
    thing.m_tcMask[0x42 >> 5] |= 1UL << (0x42 & 31);
    if (gsi.EntityGetShotStrikePos(entity, out) || out.X != 0.0f) { printf("FAIL no event\n"); return 1; }
    events.LastShotStruck = &strike;
    thing.m_dead = 1;
    if (gsi.EntityGetShotStrikePos(entity, out)) { printf("FAIL dead\n"); return 1; }
    thing.m_dead = 0;
    if (!gsi.EntityGetShotStrikePos(entity, out)) { printf("FAIL strike\n"); return 1; }
    if (out.X != 1.5f || out.Y != -2.0f || out.Z != 8.25f) { printf("FAIL position\n"); return 1; }
    printf("FSE2_0089df70_TEST PASS\n");
    return 0;
}
