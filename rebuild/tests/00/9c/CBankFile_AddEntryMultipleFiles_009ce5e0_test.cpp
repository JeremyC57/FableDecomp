// Behaviour test for CBankFile::AddEntryMultipleFiles 0x009CE5E0: a copy of the model's logic over std containers.
#include <cstdio>
#include <string>
#include <vector>
#include <map>

struct StateBlock { unsigned long crc; unsigned long CreateCRC() { return crc; } };
struct UpdateRec { unsigned long StateBlockCRC; std::vector<std::string> Filenames; bool RequiresUpdate; bool Exists;
    UpdateRec() : StateBlockCRC(0), RequiresUpdate(false), Exists(true) {}
    bool Equals(const std::vector<std::string>& f, StateBlock* sb) const { return StateBlockCRC == sb->CreateCRC() && Filenames == f; } };
struct RuntimeData { unsigned long DataOffset, DataSize; unsigned char DataType; bool Valid; };
struct DupException { std::string text; DupException(const std::string& t) : text(t) {} };

struct Bank {
    unsigned long Size;
    std::vector<std::string> Symbols; std::vector<unsigned long> Checksums; std::vector<RuntimeData> RT; std::vector<UpdateRec*> UD;
    std::map<std::string, unsigned long> OldSymbolIndices;
    std::vector<std::string> initSym, initFile; std::vector<unsigned long> initIdx;
    Bank() : Size(1) { RuntimeData b = {0,0,0,false}; RT.push_back(b); UD.push_back(0); Symbols.push_back(""); Checksums.push_back(0); }
    bool EntryExists(unsigned long i) const { return i != 0 && i < Size && RT[i].Valid; }
    void InitEntry(unsigned long i, const std::string& s, const std::string& f) { initIdx.push_back(i); initSym.push_back(s); initFile.push_back(f); }
    void Add(const std::string& symbol, unsigned long type, const std::vector<std::string>& filenames, StateBlock* sb)
    {
        unsigned long index;
        std::map<std::string, unsigned long>::iterator it = OldSymbolIndices.find(symbol);
        if (it != OldSymbolIndices.end()) {
            index = it->second;
            UpdateRec* u = UD[index];
            if (u->Exists) throw DupException("Duplicate symbol: " + symbol);
            u->Exists = true;
            RuntimeData& d = RT[index];
            if (d.DataType != type || !u->Equals(filenames, sb)) { d.DataType = (unsigned char)type; u->Filenames = filenames; u->StateBlockCRC = sb->CreateCRC(); u->RequiresUpdate = true; }
            InitEntry(index, symbol, filenames.empty() ? std::string("") : filenames[0]);
            return;
        }
        for (index = 1; index < Size; ++index) if (!EntryExists(index)) break;
        if (index >= Size) { RuntimeData b = {0,0,0,false}; RT.resize(index + 1, b); UD.resize(index + 1, 0); Symbols.resize(index + 1); Checksums.resize(index + 1, 0); Size = (unsigned long)RT.size(); }
        RuntimeData& d = RT[index];
        UpdateRec* u = new UpdateRec; UD[index] = u; Symbols[index] = symbol;
        d.Valid = true; d.DataType = (unsigned char)type; u->Filenames = filenames; u->StateBlockCRC = sb->CreateCRC(); u->Exists = true; u->RequiresUpdate = true;
        InitEntry(index, symbol, filenames.empty() ? std::string("") : filenames[0]);
    }
};

int main()
{
    Bank b; StateBlock sb = { 77 };
    std::vector<std::string> files; files.push_back("a.tga"); files.push_back("b.tga");
    std::vector<std::string> none;
    b.Add("TEX_A", 3, files, &sb);
    b.Add("TEX_B", 4, none, &sb);
    bool ok = b.Size == 3 && b.Symbols[1] == "TEX_A" && b.Symbols[2] == "TEX_B" && b.RT[1].Valid && b.RT[1].DataType == 3
        && b.UD[1]->StateBlockCRC == 77 && b.UD[1]->RequiresUpdate && b.initIdx.size() == 2 && b.initFile[0] == "a.tga" && b.initFile[1] == "";
    // known symbol from an older bank: refresh path
    b.OldSymbolIndices["TEX_A"] = 1; b.UD[1]->Exists = false; b.UD[1]->RequiresUpdate = false;
    b.Add("TEX_A", 3, files, &sb);
    ok = ok && !b.UD[1]->RequiresUpdate && b.UD[1]->Exists && b.initIdx.back() == 1 && b.Size == 3;
    // second add in the same pass throws
    bool threw = false;
    try { b.Add("TEX_A", 3, files, &sb); } catch (const DupException& e) { threw = e.text == "Duplicate symbol: TEX_A"; }
    ok = ok && threw;
    // changed CRC -> flagged for update
    b.UD[1]->Exists = false; sb.crc = 78; b.Add("TEX_A", 3, files, &sb);
    ok = ok && b.UD[1]->RequiresUpdate && b.UD[1]->StateBlockCRC == 78;
    // same filenames and CRC, only the type changed -> flagged for update, DataType rewritten
    b.UD[1]->Exists = false; b.UD[1]->RequiresUpdate = false; b.Add("TEX_A", 9, files, &sb);
    ok = ok && b.UD[1]->RequiresUpdate && b.RT[1].DataType == 9 && b.UD[1]->StateBlockCRC == 78;
    // nothing changed -> not flagged
    b.UD[1]->Exists = false; b.UD[1]->RequiresUpdate = false; b.Add("TEX_A", 9, files, &sb);
    ok = ok && !b.UD[1]->RequiresUpdate && b.RT[1].DataType == 9;
    // freed slot reused (entry 2 invalid)
    b.RT[2].Valid = false; b.Add("TEX_C", 5, none, &sb);
    ok = ok && b.Size == 3 && b.Symbols[2] == "TEX_C" && b.RT[2].DataType == 5;
    if (!ok) { std::printf("009ce5e0_TEST FAIL\n"); return 1; }
    std::printf("009ce5e0_TEST PASS\n");
    return 0;
}