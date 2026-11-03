F = "rebuild/src/compiled/00/59/CTCInventory_OnSerialise_00591a66.cpp"
EDITS = [
 (F, "    void Transfer(const char* name, std::vector<CCharString, std::allocator<CCharString> >* value);   // 0x0049B8D0",
     "    template <class T> void TransferVector(const char* name, std::vector<T, std::allocator<T> >& value);   // 0x0049B8D0 <CCharString>"),
 (F, 'context->Transfer("QuickAccessItemNames", &names);', 'context->TransferVector("QuickAccessItemNames", names);', 2),
 (F, "    void reserve(unsigned long count);                 // 0x004BA7A0", "    void reserve(unsigned int count);                  // 0x004BA7A0"),
 (F, "CPersistContext::Transfer(const char*, vector<CCharString>*) 0x0049B8D0", "CPersistContext::TransferVector<CCharString>(const char*, vector<CCharString>&) 0x0049B8D0"),
]
