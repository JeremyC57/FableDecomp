// Byte-exact reconstruction of CCreatureAction_FireMissileWeapon::FrameUpdate @ 0x00c3c5d0
// Leaf: ?CCreatureAction_FireMissileWeapon_FrameUpdate@@YI_NPAVCCreatureAction_FireMissileWeapon@@@Z
// __fastcall (this in ecx). Returns bool (al).

// Global error sink referenced by mov ecx, 0x143bb58; call 0xc3d090 is a thiscall
// member taking one stack int arg. Model as a real (thiscall) member so codegen emits
// push <arg>; mov ecx, self; call  -- with NO edx setup.
class CCodaContext { public:
    void SetError(int a0);   // 0x00C3D090 (Ego_r.pdb ?SetError@CCodaContext@@QAEXH@Z)
};
struct ErrSink {
};

extern ErrSink g_errSink;

class CCreatureAction_FireMissileWeapon { public:
    char pad[0xc];
    unsigned int counter; // +0xc
};

bool __fastcall CCreatureAction_FireMissileWeapon_FrameUpdate(CCreatureAction_FireMissileWeapon* self)
{
    unsigned int v = self->counter;
    if (v < 0xFFFFFFFFu) {
        self->counter = v + 1;
        return true;
    }
    ((CCodaContext*)&g_errSink)->SetError(-6);
    return false;
}