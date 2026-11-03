#ifndef FABLE_ENGINE_CBUSYRESOURCEMANAGER_H
#define FABLE_ENGINE_CBUSYRESOURCEMANAGER_H
struct IDirect3DDevice9;struct IDirect3DQuery9;
// Layout: original members with native four-byte STLport lists, size20.
class CBusyResourceManager {
 struct CPendingQuery {IDirect3DQuery9* Query;long Frame;};
public: CBusyResourceManager();~CBusyResourceManager();
private:
 std::list<CPendingQuery> FreeQueoryObjects;
 std::list<CPendingQuery> PendingQueories;
 long PendingFrame,FinishedFrame;
 IDirect3DDevice9* D3DDevice;
};
#endif
