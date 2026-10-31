// NCreatureAction::CActionEventExplosion::SetDataFromString(const CCharString& data) -- parse "explosion def, attach type[, attachment object]" event data: field 0 names the explosion definition (its global index is stored), field 1's text after the first '_' picks HELPER (not a dummy), DUMMY or ACTION_HELPER; any other attach type fails, and unless the action helper is used field 2 is read into AttachmentObject
// Grade: DIFFER(408v408) -- one register: retail loads the type string's PStringData straight into esi (`mov esi,[esp+10h] ; test esi,esi`, then `mov ecx,esi` before the first operator== call); ours loads ecx, copies to esi and skips the later `mov ecx,esi`. Respelled this session to the PDB (NCreatureAction::CActionEventExplosion::SetDataFromString protected virtual; anonymous-namespace GetDataFromEventString(const CCharString&, long, CCharString&); CDefinitionManager::GetDefGlobalIndexFromName const; CCharString::GetLength() -> long) -- residue unchanged. Tried this session: natural if/else-if chain over CCharString::operator==(const char*) (re-reads PStringData per test, 395/403 bytes), a free inline compare over a named data pointer (371). Earlier: inline operator== chain; branch order swap; no-local PStringData.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00871290, 406 bytes (416 with padding), `ret 4`.
// Callees: (anonymous namespace) GetDataFromEventString(const CCharString&, long, CCharString&) @ 0x00870C90 (__fastcall, as the landed
//   CActionEventAddObject::SetDataFromString 0x00870D70), CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0,
//   CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410,
//   CCharString::FindCharPos(char) 0x0099E5A0, CCharString::GetSubString(unsigned long, unsigned long) const 0x0099EC70,
//   CBasicString<char>::operator==(const char*) 0x004115A0.
// Data: "" 0x0122D70E, "HELPER" 0x01277490, "DUMMY" 0x01277488, "ACTION_HELPER" 0x01277524.
// Layout: Ego_r.pdb CActionEventExplosion (32 bytes): AttachmentObject +0x14, ExplosionDefIndex +0x18, Dummy +0x1c,
//   UseActionHelper +0x1d. Every exit returns false (retail `xor al,al` on each path).
// Shape: the definition name is a local in its own block (its frame slot is reused by the tail substring); the
//   attach-type string is a named local homed in the dead `data` slot; the comparisons use the inline
//   CCharString::operator==(const char*) of the landed CCombatActionFallStunned ctor (null block: strcmp with "").
// Leaf: ?SetDataFromString@CActionEventExplosion@NCreatureAction@@MAE_NABVCCharString@@@Z
#include <string.h>
template <class T> class CBasicString { public:     // 13 bytes
    T*            PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const T* text) const;       // 0x004115A0 (CBasicString<char>)
};
struct CCharStringData {                        // 17 bytes
    CBasicString<char> Data;                    // +0x00
    long               NoRefs;                  // +0x0d
};
class CCharString { public:
    CCharStringData* PStringData;               // +0x00
    CCharString();                              // 0x0099E4B0
    ~CCharString();                             // 0x0099EAE0
    long FindCharPos(char c) const;             // 0x0099E5A0
    CCharString GetSubString(unsigned long start, unsigned long length) const;   // 0x0099EC70
    long GetLength() const { if (PStringData == 0) return 0; return PStringData->Data.StringLength; }
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

namespace {
bool __fastcall GetDataFromEventString(const CCharString& data, long index, CCharString& out);   // 0x00870C90
}

class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();                 // 0x0044C6B0
};

namespace NCreatureAction {
class CActionEventBase { public:
    virtual ~CActionEventBase();
    unsigned char _base[0x10];                  // +0x04 CActionEventBase words
protected:
    virtual bool SetDataFromString(const CCharString& data) = 0;
};
class CActionEventExplosion : public CActionEventBase { public:
    CCharString AttachmentObject;               // +0x14
    long        ExplosionDefIndex;              // +0x18
    bool        Dummy;                          // +0x1c
    bool        UseActionHelper;                // +0x1d
protected:
    virtual bool SetDataFromString(const CCharString& data);
};

bool CActionEventExplosion::SetDataFromString(const CCharString& data)
{
    {
        CCharString defName;
        if (!GetDataFromEventString(data, 0, defName))
            return false;
        ExplosionDefIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(defName);
    }
    {
        CCharString attachType;
        if (!GetDataFromEventString(data, 1, attachType))
            return false;
        long underscore = attachType.FindCharPos('_');
        CCharString prefix = attachType.GetSubString(0, underscore);
        CCharString type = attachType.GetSubString(underscore + 1, attachType.GetLength());
        CCharStringData* typeData = type.PStringData;
        if (typeData == 0) {
            if (strcmp("HELPER", "") == 0)
                goto helper;
            if (strcmp("DUMMY", "") == 0)
                goto dummy;
            if (strcmp("ACTION_HELPER", "") == 0)
                goto actionHelper;
        } else {
            if (typeData->Data == "HELPER")
                goto helper;
            if (typeData->Data == "DUMMY")
                goto dummy;
            if (typeData->Data == "ACTION_HELPER")
                goto actionHelper;
        }
        return false;
    helper:
        Dummy = false;
        goto done;
    dummy:
        Dummy = true;
        goto done;
    actionHelper:
        UseActionHelper = true;
    done:
        ;
    }
    if (UseActionHelper)
        return false;
    GetDataFromEventString(data, 2, AttachmentObject);
    return false;
}
}
