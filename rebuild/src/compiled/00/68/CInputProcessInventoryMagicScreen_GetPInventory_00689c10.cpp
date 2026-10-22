int __fastcall CInputProcessInventoryMagicScreen_GetPInventory(const unsigned char* process)
{
    return *reinterpret_cast<const int*>(process + 52);
}