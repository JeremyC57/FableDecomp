# mk.py: write measurement TUs (NOT candidates) for 0x0045C5F6 ??1CThingNoiseDef (NF355)
import sys
E='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF355/work/exp/'
HEAD = '''// NF355 measurement (NOT a candidate): %s
// Leaf: ??1CThingNoiseDef@@UAE@XZ
%s#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCNOISEDEF_IMPLICIT_DTOR
#define FABLE_RETAIL_CTCNOISEDEF_CTOR_INLINE
#include "fable_string.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CTCNoiseDef.h"
class CThingComponentSet {
public:
    ~CThingComponentSet();                                   // 0x00450BA0
    unsigned char Entries[8];
};
#pragma pack(push, 1)
class CThingBaseDef : public CParentDefClassBase {
public:
    CThingBaseDef();                                         // 0x006772D0
%s    %s                                                      // +0x39
    unsigned char _rest[0x37 - 8];
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CThingBaseDef) == 0x70);
class CPersistContext;
class CThingNoiseDef : public CThingBaseDef {
public:
    CThingNoiseDef() { }
    static CDefClassBase* FABLE_FASTCALL Alloc();
    virtual void Transfer(CPersistContext&);
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    CTCNoiseDef Noise;                                       // +0x70
};
FABLE_STATIC_ASSERT(sizeof(CThingNoiseDef) == 0xa4);
%s
'''
ALLOC = 'CDefClassBase* CThingNoiseDef::Alloc() { return new CThingNoiseDef; }\n'
CTOR_OOL = None
variants = {
 'a1': ('Alloc form, implicit ~CThingBaseDef, typed Components, no pragma', '', '', 'CThingComponentSet Components;', ALLOC),
 'a2': ('Alloc form, implicit ~CThingBaseDef, typed Components, optimize s', '#pragma optimize("s", on)\n', '', 'CThingComponentSet Components;', ALLOC),
 'a3': ('Alloc form, declared virtual ~CThingBaseDef, opaque Components, no pragma', '', '    virtual ~CThingBaseDef();\n', 'unsigned char Components[8];', ALLOC),
 'a4': ('Alloc form, declared virtual ~CThingBaseDef, opaque Components, optimize s', '#pragma optimize("s", on)\n', '    virtual ~CThingBaseDef();\n', 'unsigned char Components[8];', ALLOC),
}
for k, (d, prag, dt, comp, body) in variants.items():
    open(E + k + '.cpp', 'w').write(HEAD % (d, prag, dt, comp, body))
