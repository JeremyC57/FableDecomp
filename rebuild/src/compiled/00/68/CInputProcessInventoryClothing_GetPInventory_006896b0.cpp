int __fastcall CInputProcessInventoryClothing_GetPInventory(const unsigned char* process)
{
    return *reinterpret_cast<const int*>(process + 52);
}