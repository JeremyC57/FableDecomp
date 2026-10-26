// CTCWeapon::SetAiming @ 0x005da4d0
// if (this->m_aiming == -1) this->m_aiming = 0;
class CTCWeapon { public:
    char pad[0x14];
    int m_aiming; // +0x14
    void __fastcall SetAiming();
};

void __fastcall CTCWeapon::SetAiming() {
    if (m_aiming == -1)
        m_aiming = 0;
}