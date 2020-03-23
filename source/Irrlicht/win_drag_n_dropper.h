/*
copyright 2020 edo9300 see LICENSE
*/
#ifndef WIN_DRAG_N_DROPPER_H
#define WIN_DRAG_N_DROPPER_H
#include <oleidl.h>
#include <vector2d.h>
#include <atomic>
#include <mutex>
#include <vector>
#include <string>
namespace irr {
class IrrlichtDevice;
}
class edoproDropper : public IDropTarget {
public:
	edoproDropper(HWND hwnd, bool(*callback)(irr::core::vector2di pos, bool isFile)/*, irr::IrrlichtDevice* dev*/) :window(hwnd), dragCheck(callback)/*, device(dev)*/{};
	~edoproDropper() = default;

	virtual HRESULT __stdcall QueryInterface(REFIID riid, void **ppv) override;
	virtual ULONG __stdcall AddRef() override { InterlockedIncrement((long *)&ref_count); return ref_count; }
	virtual ULONG __stdcall Release() override {
		if(InterlockedDecrement((long *)&ref_count) <= 0)
			delete this;
		return ref_count;
	}

	virtual HRESULT __stdcall DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) override;

	virtual HRESULT __stdcall DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) override;

	virtual HRESULT __stdcall DragLeave() override;

	virtual HRESULT __stdcall Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect) override;

	bool hasData();

	std::vector<std::wstring> getData(bool* isfile, irr::core::vector2di* dropPosition);
private:
	HWND window;
	ULONG ref_count{0UL};
	bool isDragging{false};
	bool isFile{false};
	bool(*dragCheck)(irr::core::vector2di pos, bool isFile){nullptr};
	irr::IrrlichtDevice* device{nullptr};
	std::atomic<bool> ready{false};
	std::vector<std::wstring> returns;
	std::mutex retrieving;
	irr::core::vector2di dropPosition{0,0};

};

#endif //WIN_DRAG_N_DROPPER_H